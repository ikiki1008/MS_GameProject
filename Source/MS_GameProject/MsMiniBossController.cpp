#include "MsMiniBossController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Sight.h"
#include "GameFramework/Character.h"
#include "MsPlayer.h"
#include "EnemyMiniBoss.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY(LogMiniBossController);

AMsMiniBossController::AMsMiniBossController() {
    MsPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("MsPerceptionComponent"));
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

    // �þ� ���� ����
    SightConfig->SightRadius = 2000.0f;  // �þ� �ݰ�
    SightConfig->LoseSightRadius = SightConfig->SightRadius + 500.0f;  // �þ߿��� ����� �ݰ�
    SightConfig->PeripheralVisionAngleDegrees = 360.0f;  // 360�� ����
    SightConfig->SetMaxAge(5.0f);  // ���� ���� �ð�

    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

    MsPerceptionComponent->ConfigureSense(*SightConfig);
    MsPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

    MsPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AMsMiniBossController::OnTargetDetected);
}

void AMsMiniBossController::OnPossess(APawn* InPawn) {
    Super::OnPossess(InPawn);

    if (InPawn) {
        UE_LOG(LogMiniBossController, Warning, TEXT("Pawn Possessed: %s"), *InPawn->GetName());
        OriginalPosition = InPawn->GetActorLocation();
    }

    PlayerDetect = false;
    PlayerAttack = false;  
    IsPlayerNear = false;
}

void AMsMiniBossController::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    if (PlayerDetect) {
        FVector EnemyLocation = GetPawn()->GetActorLocation();  // �̴� ���� ��ġ
        FVector PlayerLocation = TargetPawn->GetActorLocation();  // �÷��̾� ��ġ
        float DistanceToPlayer = FVector::Dist(EnemyLocation, PlayerLocation);
        AEnemyMiniBoss* Enemy = Cast<AEnemyMiniBoss>(GetPawn());

        if (Enemy && Enemy->Life <= 0) {
            UE_LOG(LogMiniBossController, Warning, TEXT("Enemy is dead. Stopping attack."));
            StopMovement();
            ClearFocus(EAIFocusPriority::Gameplay);
            if (GetWorld()->GetTimerManager().IsTimerActive(TimerHandle)) {
                GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
            }
            PlayerAttack = false;
            PlayerDetect = false;
            return;
        }

        if (DistanceToPlayer <= 100.0f) {
            IsPlayerNear = true;
        }
        else {
            IsPlayerNear = false;
        }
    }
    else {
        OnLoseSightOfPlayer();
    }
}

void AMsMiniBossController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus) {
    // �÷��̾ �����Ǿ��� ���� ó��
    if (Stimulus.WasSuccessfullySensed()) {
        UE_LOG(LogMiniBossController, Log, TEXT("Player detected: %s"), *Actor->GetName());
        PlayerDetect = true;
        TargetPawn = Cast<APawn>(Actor);
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMsMiniBossController::CheckSituation, 2.0f, true);
    }
    else {
        PlayerDetect = false;
    }
}

void AMsMiniBossController::CheckSituation() {
    SetFocus(TargetPawn);
    MoveToActor(TargetPawn);

    if (IsPlayerNear) {
        PlayerAttack = true;
        UGameplayStatics::ApplyDamage(TargetPawn, 100.0f, this, GetPawn(), UDamageType::StaticClass());
    }
    else {
        MoveToActor(TargetPawn);
    }
}

void AMsMiniBossController::OnLoseSightOfPlayer() {
    PlayerDetect = false;
    PlayerAttack = false;
    ClearFocus(EAIFocusPriority::Gameplay);
}

bool AMsMiniBossController::IsPlayerDead(AActor* Player) {
    AMsPlayer* Crow = Cast<AMsPlayer>(Player);
    if (Crow) {
        return Crow->Life <= 0;
    }
    return false;
}
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

    // 시야 감지 설정
    SightConfig->SightRadius = 2000.0f;  // 시야 반경
    SightConfig->LoseSightRadius = SightConfig->SightRadius + 500.0f;  // 시야에서 벗어나는 반경
    SightConfig->PeripheralVisionAngleDegrees = 360.0f;  // 360도 감지
    SightConfig->SetMaxAge(5.0f);  // 감지 지속 시간

    // 감지할 대상의 타입 설정 (플레이어만 감지하도록 설정)
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
    PlayerAttack = false;  // 시작 시 false 설정
}

void AMsMiniBossController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus) {
    // 플레이어가 감지되었을 때의 처리
    if (Stimulus.WasSuccessfullySensed()) {
        UE_LOG(LogMiniBossController, Log, TEXT("Player detected: %s"), *Actor->GetName());
        PlayerDetect = true;
        TargetPawn = Cast<APawn>(Actor);
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMsMiniBossController::CheckSituation, 2.0f, true);
    }
    else {
        OnLoseSightOfPlayer();
    }
}

void AMsMiniBossController::CheckSituation() {
    if (TargetPawn && !IsPlayerDead(TargetPawn)) {
        SetFocus(TargetPawn);
        MoveToActor(TargetPawn);

        FVector EnemyLocation = GetPawn()->GetActorLocation();  // 미니 보스 위치
        FVector PlayerLocation = TargetPawn->GetActorLocation();  // 플레이어 위치
        float DistanceToPlayer = FVector::Dist(EnemyLocation, PlayerLocation);
        AEnemyMiniBoss* Enemy = Cast<AEnemyMiniBoss>(GetPawn());

        if (DistanceToPlayer <= 200.0f) {
            UE_LOG(LogMiniBossController, Warning, TEXT("enemy is less than 200cm."));
            if (Enemy && Enemy->Life <= 0) {
                UE_LOG(LogMiniBossController, Warning, TEXT("Enemy is dead. Stopping attack."));
                PlayerAttack = false;
                PlayerDetect = false;
            }
            else {
                PlayerAttack = true;
                UGameplayStatics::ApplyDamage(TargetPawn, 100.0f, this, GetPawn(), UDamageType::StaticClass());
                UE_LOG(LogMiniBossController, Warning, TEXT("attacked!!!"));
            }
        }
        else {
            UE_LOG(LogMiniBossController, Warning, TEXT("enemy is further than 200cm."));
            MoveToActor(TargetPawn);
            PlayerAttack = false;
        }
    }
    else {
        //player dead
        OnLoseSightOfPlayer();
    }
}

void AMsMiniBossController::OnLoseSightOfPlayer() {
    UE_LOG(LogMiniBossController, Log, TEXT("Lost sight of the player."));
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

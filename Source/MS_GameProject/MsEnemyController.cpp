#include "MsEnemyController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Sight.h"
#include "GameFramework/Character.h"
#include "MsPlayer.h"
#include "EnemyRat.h"
#include "EnemyFish.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY(LogMsEnemyController);

AMsEnemyController::AMsEnemyController(){
    PrimaryActorTick.bCanEverTick = true;

    PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("MsPawnSensingComponent"));
    PawnSensingComponent->SightRadius = 500.0f;
    PawnSensingComponent->SetPeripheralVisionAngle(180.0f);

    PawnSensingComponent->OnSeePawn.AddDynamic(this, &AMsEnemyController::OnSeePawn);
}

void AMsEnemyController::OnPossess(APawn* InPawn) {
    Super::OnPossess(InPawn);

    if (InPawn) {
        UE_LOG(LogMsEnemyController, Warning, TEXT("Pawn Possessed: %s"), *InPawn->GetName());
        OriginalPosition = InPawn->GetActorLocation();
    }
    PlayerDetect = false;
    AttackPlayer = false; // ���� �� false ����
}

void AMsEnemyController::OnSeePawn(APawn* DetectedPawn) {
    if (DetectedPawn && !IsPlayerDead(DetectedPawn)) {
        UE_LOG(LogMsEnemyController, Log, TEXT("Pawn seen: %s"), *DetectedPawn->GetName());
        PlayerDetect = true; // �÷��̾� �߰� �� true

        SetFocus(DetectedPawn);
        MoveToActor(DetectedPawn);
        
        FVector EnemyLocation = GetPawn()->GetActorLocation(); //�� ��ġ
        FVector PlayerLocation = DetectedPawn->GetActorLocation(); // �÷��̾� ��ġ
        float DistanceToPlayer = FVector::Dist(EnemyLocation, PlayerLocation);

        if (DistanceToPlayer <= 80.0f) { //�� �Ÿ������� 80cm �̳��� ���
            AttackPlayer = true;
            UGameplayStatics::ApplyDamage(DetectedPawn, 50.0f, this, GetPawn(), UDamageType::StaticClass());
            UE_LOG(LogMsEnemyController, Warning, TEXT(" %%%% attack to player %%%%"));
        }
        else {
            AttackPlayer = false;
            SetFocus(DetectedPawn);
            MoveToActor(DetectedPawn);
        }
    }
    else {
        OnLoseSightOfPlayer();
    }
}

void AMsEnemyController::OnLoseSightOfPlayer() {
    UE_LOG(LogMsEnemyController, Log, TEXT("Lost sight of the player."));
    PlayerDetect = false;
    AttackPlayer = false;
    ClearFocus(EAIFocusPriority::Gameplay);
    ReturnToOriginalPosition();
}

void AMsEnemyController::ReturnToOriginalPosition() {
    PlayerDetect = false;
    AttackPlayer = false;
    ClearFocus(EAIFocusPriority::Gameplay);
}

bool AMsEnemyController::IsPlayerDead(AActor* PlayerActor) {
    AMsPlayer* Player = Cast<AMsPlayer>(PlayerActor);
    if (Player) {
        return Player->Life <= 0;
    }
    return false;
}

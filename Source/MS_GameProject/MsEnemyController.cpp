#include "MsEnemyController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Sight.h"
#include "GameFramework/Character.h"
#include "MsPlayer.h"
#include "EnemyRat.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY(LogMsEnemyController);

AMsEnemyController::AMsEnemyController(){
    PrimaryActorTick.bCanEverTick = true;

    PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("MsPawnSensingComponent"));
    PawnSensingComponent->SetPeripheralVisionAngle(180.0f);
    PawnSensingComponent->SightRadius = 500.0f;
    PawnSensingComponent->OnSeePawn.AddDynamic(this, &AMsEnemyController::OnSeePawn);
}

void AMsEnemyController::OnPossess(APawn* InPawn) {
    Super::OnPossess(InPawn);

    if (InPawn) {
        UE_LOG(LogMsEnemyController, Warning, TEXT("Pawn Possessed: %s"), *InPawn->GetName());
        OriginalPosition = InPawn->GetActorLocation();
    }

    AEnemyRat* Enemy = Cast<AEnemyRat>(GetPawn());
    if (Enemy && Enemy->Special) { //아이템을 주는 스페셜 몬스터인지 확인
        SpecialMonster = Enemy->Special;
    }

    PlayerDetect = false;
    AttackPlayer = false; // 시작 시 false 설정
}

void AMsEnemyController::OnSeePawn(APawn* DetectedPawn) { 
    if (DetectedPawn && !IsPlayerDead(DetectedPawn)) {
        UE_LOG(LogMsEnemyController, Log, TEXT("Pawn seen: %s"), *DetectedPawn->GetName());
        PlayerDetect = true; // 플레이어 발견 시 true
        TargetPawn = DetectedPawn;

        SetFocus(DetectedPawn);
        MoveToActor(DetectedPawn);

        FVector EnemyLocation = GetPawn()->GetActorLocation(); // 폰 위치
        FVector PlayerLocation = DetectedPawn->GetActorLocation(); // 플레이어 위치
        float DistanceToPlayer = FVector::Dist(EnemyLocation, PlayerLocation);
        AEnemyRat* Enemy = Cast<AEnemyRat>(GetPawn());

        if (SpecialMonster) {
            Damage = 200.0f;
            if (DistanceToPlayer <= 150.0f) {
                if (Enemy && Enemy->Life <= 0) {
                    UE_LOG(LogMsEnemyController, Warning, TEXT("Enemy is dead. Stopping attack."));
                    AttackPlayer = false;
                    PlayerDetect = false;
                }
                else {
                    AttackPlayer = true;
                    UGameplayStatics::ApplyDamage(TargetPawn, Damage, this, GetPawn(), UDamageType::StaticClass());
                    UE_LOG(LogMsEnemyController, Warning, TEXT(" %%%% special attack to player %%%%"));
                }
            }
            else {
                MoveToActor(DetectedPawn);
            }
        }
        else {
            Damage = 50.0f;
            if (DistanceToPlayer <= 80.0f) { // 두 거리간격이 80cm 이내일 경우
                if (Enemy && Enemy->Life <= 0) {
                    UE_LOG(LogMsEnemyController, Warning, TEXT("Enemy is dead. Stopping attack."));
                    AttackPlayer = false;
                    PlayerDetect = false;
                }
                else {
                    AttackPlayer = true;
                    UGameplayStatics::ApplyDamage(TargetPawn, Damage, this, GetPawn(), UDamageType::StaticClass());
                    UE_LOG(LogMsEnemyController, Warning, TEXT(" %%%% attack to player %%%%"));
                }
            }
            else {
                UE_LOG(LogMsEnemyController, Warning, TEXT(" %%%%  is it here???? %%%%"));
                MoveToActor(DetectedPawn);
            }
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

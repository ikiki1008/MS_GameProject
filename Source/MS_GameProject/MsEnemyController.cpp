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
    if (Enemy && Enemy->Special) { //�������� �ִ� ����� �������� Ȯ��
        SpecialMonster = Enemy->Special;
    }

    PlayerDetect = false;
    AttackPlayer = false; // ���� �� false ����
}

void AMsEnemyController::OnSeePawn(APawn* DetectedPawn) { 
    if (DetectedPawn && !IsPlayerDead(DetectedPawn)) {
        UE_LOG(LogMsEnemyController, Log, TEXT("Pawn seen: %s"), *DetectedPawn->GetName());
        PlayerDetect = true; // �÷��̾� �߰� �� true
        TargetPawn = DetectedPawn;

        SetFocus(DetectedPawn);
        MoveToActor(DetectedPawn);

        FVector EnemyLocation = GetPawn()->GetActorLocation(); // �� ��ġ
        FVector PlayerLocation = DetectedPawn->GetActorLocation(); // �÷��̾� ��ġ
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
            if (DistanceToPlayer <= 80.0f) { // �� �Ÿ������� 80cm �̳��� ���
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

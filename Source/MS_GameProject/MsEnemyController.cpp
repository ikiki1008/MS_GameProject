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

AMsEnemyController::AMsEnemyController() {
    PrimaryActorTick.bCanEverTick = true;

    AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("EnemyPerception"));
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("EnemySightConfig"));
    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("EnemyCapsule"));

    SightConfig->SightRadius = 50.0f;
    SightConfig->LoseSightRadius = 60.0f;
    SightConfig->PeripheralVisionAngleDegrees = 180.0f;
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

    AIPerceptionComponent->ConfigureSense(*SightConfig);
    AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
    AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AMsEnemyController::OnSensed);

    CapsuleComponent->InitCapsuleSize(5.0f, 10.0f);
    CapsuleComponent->SetCollisionProfileName(TEXT("MsEnemy"));
    CapsuleComponent->SetupAttachment(RootComponent);

}

void AMsEnemyController::BeginPlay() {
    Super::BeginPlay();
    //CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AMsEnemyController::OnOverlapBegin);

    // Set the timer to call CheckPerception every second
    GetWorld()->GetTimerManager().SetTimer(PerceptionTimerHandle, this, &AMsEnemyController::CheckPerception, 1.0f, true);
}

void AMsEnemyController::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    // Check if player is dead and update state
    TArray<AActor*> PerceivedActors;
    AIPerceptionComponent->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);

    for (AActor* Actor : PerceivedActors) {
        AMsPlayer* Player = Cast<AMsPlayer>(Actor);
        if (Player) {
            if (IsPlayerDead(Player)) {
                AEnemyRat* EnemyRat = Cast<AEnemyRat>(GetPawn());
                if (EnemyRat) {
                    EnemyRat->RandomWalk(true);
                }
                UE_LOG(LogMsEnemyController, Warning, TEXT(" ***** Player dead *****"));
                return;  
            }
        }
    }
}

void AMsEnemyController::OnPossess(APawn* InPawn) {
    Super::OnPossess(InPawn);
}

void AMsEnemyController::OnSensed(const TArray<AActor*>& UpdatedActors) {
    bool bPlayerDetected = false;

    for (AActor* Actor : UpdatedActors) {
        FActorPerceptionBlueprintInfo Information;
        AIPerceptionComponent->GetActorsPerception(Actor, Information);

        if (Information.LastSensedStimuli.Num() > 0 && Information.LastSensedStimuli[0].WasSuccessfullySensed()) {
            AMsPlayer* Player = Cast<AMsPlayer>(Actor);

            if (Player) {
                if (IsPlayerDead(Player)) {
                    // Stop attacking and start random walk
                    AEnemyRat* EnemyRat = Cast<AEnemyRat>(GetPawn());
                    if (EnemyRat) {
                        EnemyRat->RandomWalk(true);
                    }
                    UE_LOG(LogMsEnemyController, Warning, TEXT(" ***** Player dead *****"));
                    bPlayerDetected = false;
                    break;
                }

                AEnemyRat* EnemyRat = Cast<AEnemyRat>(GetPawn());
                if (EnemyRat) {
                    EnemyRat->RandomWalk(true);
                }

                bPlayerDetected = true;
                SetFocus(Actor);
                FVector PlayerLocation = Actor->GetActorLocation();
                MoveToLocation(PlayerLocation);

                UE_LOG(LogMsEnemyController, Warning, TEXT(" ***** Player detected and moving towards! *****"));
                UGameplayStatics::ApplyDamage(Actor, 50.0f, this, GetPawn(), UDamageType::StaticClass());
                UE_LOG(LogMsEnemyController, Warning, TEXT(" %%%% Player overlapped and damaged from the front! %%%%"));
                break;
            }
        }
    }

    if (!bPlayerDetected) {
        AEnemyRat* EnemyRat = Cast<AEnemyRat>(GetPawn());
        if (EnemyRat) {
            EnemyRat->RandomWalk(false);
        }
    }
}

//void AMsEnemyController::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
//    if (OtherActor && (OtherActor != GetPawn())) {
//        AMsPlayer* MainPlayer = Cast<AMsPlayer>(OtherActor);
//        if (MainPlayer) {
//
//            UGameplayStatics::ApplyDamage(MainPlayer, 50.0f, this, GetPawn(), UDamageType::StaticClass());
//            UE_LOG(LogMsEnemyController, Warning, TEXT(" %%%% Player overlapped and damaged from the front! %%%%"));
//        }
//    }
//}

void AMsEnemyController::CheckPerception() {
    TArray<AActor*> UpdatedActors;
    AIPerceptionComponent->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), UpdatedActors);
    OnSensed(UpdatedActors);
}

bool AMsEnemyController::IsPlayerDead(AActor* PlayerActor) {
    AMsPlayer* Player = Cast<AMsPlayer>(PlayerActor);
    if (Player) {
        return Player->Life <= 0;
    }
    return false;
}

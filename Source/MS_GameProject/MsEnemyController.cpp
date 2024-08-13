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

AMsEnemyController::AMsEnemyController()
{
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

    bHasFoundMeshes = false;
}

void AMsEnemyController::BeginPlay()
{
    Super::BeginPlay();

    FindAndLogAllAIActorMeshes();
    IsAttacking = false;
    IsPlayerDetected = false;

    // Set the timer to call CheckPerception every second
    GetWorld()->GetTimerManager().SetTimer(PerceptionTimerHandle, this, &AMsEnemyController::CheckPerception, 1.0f, true);
}

void AMsEnemyController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMsEnemyController::OnPossess(APawn* InPawn){
    Super::OnPossess(InPawn);
}

void AMsEnemyController::OnSensed(const TArray<AActor*>& UpdatedActors) {
    bool bPlayerDetectedThisFrame = false;

    for (AActor* Actor : UpdatedActors) {
        FActorPerceptionBlueprintInfo Information;
        AIPerceptionComponent->GetActorsPerception(Actor, Information);

        if (Information.LastSensedStimuli.Num() > 0 && Information.LastSensedStimuli[0].WasSuccessfullySensed()) {
            AMsPlayer* Player = Cast<AMsPlayer>(Actor);

            if (Player) {
                bPlayerDetectedThisFrame = true;

                if (IsPlayerDead(Player)) {
                    IsAttacking = false;
                    IsPlayerDetected = false;
                    UE_LOG(LogMsEnemyController, Warning, TEXT(" ***** Player dead *****"));
                    break;
                }

                IsAttacking = true;
                IsPlayerDetected = true;
                //SetFocus(Actor);
                FVector PlayerLocation = Actor->GetActorLocation();
                MoveToLocation(PlayerLocation);

                UE_LOG(LogMsEnemyController, Warning, TEXT(" ***** Player detected and moving towards! *****"));
                UGameplayStatics::ApplyDamage(Actor, 50.0f, this, GetPawn(), UDamageType::StaticClass());
                UE_LOG(LogMsEnemyController, Warning, TEXT(" %%%% Player overlapped and damaged from the front! %%%%"));
                break;
            }
        }
    }

    if (!bPlayerDetectedThisFrame) {
        // If no player was detected, start random movement
        IsPlayerDetected = false;
        IsAttacking = false;
        MoveRandomly();
    }
}


void AMsEnemyController::FindAndLogAllAIActorMeshes(){
    if (bHasFoundMeshes){
        return;
    }

    UWorld* World = GetWorld();
    if (!World){
        UE_LOG(LogMsEnemyController, Warning, TEXT("World not found!"));
        return;
    }

    TArray<AActor*> AllActors;
    UGameplayStatics::GetAllActorsOfClass(World, AActor::StaticClass(), AllActors);

    for (AActor* Actor : AllActors){
        if (!Actor){
            continue;
        }
    }

    bHasFoundMeshes = true;
}

void AMsEnemyController::MoveRandomly(){
    APawn* ControlledPawn = K2_GetPawn();
    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
    FVector RandomLocation;
    FVector CurrentLocation = ControlledPawn->GetActorLocation();

    if (NavSystem) {
        NavSystem->K2_GetRandomReachablePointInRadius(GetWorld(), CurrentLocation, RandomLocation, 500.0f);
        UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, RandomLocation);   
    }
}

void AMsEnemyController::CheckPerception(){
    UE_LOG(LogMsEnemyController, Warning, TEXT(" ***** check perception *****"));
    TArray<AActor*> UpdatedActors;
    AIPerceptionComponent->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), UpdatedActors);
    OnSensed(UpdatedActors);
}

bool AMsEnemyController::IsPlayerDead(AActor* PlayerActor){
    AMsPlayer* Player = Cast<AMsPlayer>(PlayerActor);
    if (Player){
        return Player->Life <= 0;
    }
    return false;
}

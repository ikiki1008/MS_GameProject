#include "MsEnemyController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Sight.h"
#include "GameFramework/Character.h"
#include "MsPlayer.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "EnemyRat.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"

DEFINE_LOG_CATEGORY(LogMsEnemyController);

AMsEnemyController::AMsEnemyController() {
    PrimaryActorTick.bCanEverTick = true;

    AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    SightConfig->SightRadius = 30.0f; // 감지 범위를 5미터(500cm)로 설정
    SightConfig->LoseSightRadius = 40.0f; // LoseSightRadius를 SightRadius보다 조금 더 크게 설정
    SightConfig->PeripheralVisionAngleDegrees = 360.0f; // 360도 시야
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

    AIPerceptionComponent->ConfigureSense(*SightConfig);
    AIPerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());
    AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AMsEnemyController::OnPerceptionUpdated);

    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
    CapsuleComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AMsEnemyController::OnOverlapBegin);

}

void AMsEnemyController::BeginPlay() {
    Super::BeginPlay();
   // APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
   // SetFocus(PlayerPawn);
}

void AMsEnemyController::OnPossess(APawn* InPawn) {
    Super::OnPossess(InPawn);
}

void AMsEnemyController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus) {
    if (Stimulus.WasSuccessfullySensed()) {
        ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn());

        if (ControlledCharacter) {
            AMsPlayer* PlayerCharacter = Cast<AMsPlayer>(Actor);
            if (PlayerCharacter) {
                UGameplayStatics::ApplyDamage(PlayerCharacter, 100.0f, this, ControlledCharacter, UDamageType::StaticClass());
                UE_LOG(LogMsEnemyController, Warning, TEXT(" #### Player got Damaged!!! ##### "));
            }
        }
    }
}

void AMsEnemyController::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
    AMsPlayer* MainPlayer = Cast<AMsPlayer>(OtherActor);
    if (MainPlayer) {
        UGameplayStatics::ApplyDamage(MainPlayer, 100.0f, this,  GetPawn(), UDamageType::StaticClass());
    }
}

void AMsEnemyController::MoveToPlayer(AActor* PlayerActor) {
    if (PlayerActor) {
        UAIBlueprintHelperLibrary::SimpleMoveToActor(this, PlayerActor);
    }
}

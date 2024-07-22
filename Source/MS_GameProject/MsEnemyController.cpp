#include "MsEnemyController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Sight.h"
#include "GameFramework/Character.h"
#include "MsPlayer.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"

AMsEnemyController::AMsEnemyController() {
    PrimaryActorTick.bCanEverTick = true;

    AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    SightConfig->SightRadius = 500.0f; // SightRadius를 500으로 변경
    SightConfig->LoseSightRadius = 600.0f; // LoseSightRadius를 600으로 변경
    SightConfig->PeripheralVisionAngleDegrees = 90.0f;
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

    AIPerceptionComponent->ConfigureSense(*SightConfig);
    AIPerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());
    AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AMsEnemyController::OnPerceptionUpdated);
}

void AMsEnemyController::BeginPlay() {
    Super::BeginPlay();
}

void AMsEnemyController::OnPossess(APawn* InPawn) {
    Super::OnPossess(InPawn);
}

void AMsEnemyController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus) {
    if (Stimulus.WasSuccessfullySensed()) {
        ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn());
        if (ControlledCharacter) {
            ControlledCharacter->Jump(); // 플레이어를 인식하면 점프

            AMsPlayer* PlayerCharacter = Cast<AMsPlayer>(Actor);
            if (PlayerCharacter) {
                // 현재 컨트롤러를 사용하여 데미지 적용
                UGameplayStatics::ApplyDamage(PlayerCharacter, 100.0f, this, ControlledCharacter, UDamageType::StaticClass());
            }
        }
    }
}

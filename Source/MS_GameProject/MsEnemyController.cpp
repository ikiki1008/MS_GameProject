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
    SightConfig->SightRadius = 500.0f; // SightRadius�� 500���� ����
    SightConfig->LoseSightRadius = 600.0f; // LoseSightRadius�� 600���� ����
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
            ControlledCharacter->Jump(); // �÷��̾ �ν��ϸ� ����

            AMsPlayer* PlayerCharacter = Cast<AMsPlayer>(Actor);
            if (PlayerCharacter) {
                // ���� ��Ʈ�ѷ��� ����Ͽ� ������ ����
                UGameplayStatics::ApplyDamage(PlayerCharacter, 100.0f, this, ControlledCharacter, UDamageType::StaticClass());
            }
        }
    }
}

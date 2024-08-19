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

    // �⺻ ���� ����
    SightConfig->SightRadius = 50.0f;
    SightConfig->LoseSightRadius = 100.0f;
    SightConfig->PeripheralVisionAngleDegrees = 180.0f;
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

    // ĸ�� ������Ʈ �⺻ ����
    CapsuleComponent->SetCollisionProfileName(TEXT("MsEnemy"));
    CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // Enable collision
    CapsuleComponent->SetCollisionObjectType(ECollisionChannel::ECC_Pawn); // Set the collision channel
    CapsuleComponent->SetNotifyRigidBodyCollision(true); // Enable hit events
    CapsuleComponent->SetupAttachment(RootComponent);

    AIPerceptionComponent->ConfigureSense(*SightConfig);
    AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
    AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AMsEnemyController::OnSensed);
}

void AMsEnemyController::BeginPlay()
{
    Super::BeginPlay();

    if (CapsuleComponent) {
        CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AMsEnemyController::OnBeginOverlap);
        CapsuleComponent->OnComponentEndOverlap.AddDynamic(this, &AMsEnemyController::OnEndOverlap);
    }

    // ĳ������ ũ�⿡ �°� ĸ�� ũ�� ����
    AdjustCapsuleSize();

    IsEnemyDead = false;
    IsAttacking = false;
    IsPlayerDetected = false;

    // Set the timer to call CheckPerception every second
    GetWorld()->GetTimerManager().SetTimer(PerceptionTimerHandle, this, &AMsEnemyController::CheckPerception, 1.0f, true);
}

void AMsEnemyController::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this) {
        AMsPlayer* Player = Cast<AMsPlayer>(OtherActor);
        if (Player && IsPlayerDetected) {
            IsPlayerDetected = true;

            if (IsPlayerDetected) {
                IsAttacking = true;
                // �÷��̾�� ���ظ� ����
                UGameplayStatics::ApplyDamage(Player, 50.0f, this, GetPawn(), UDamageType::StaticClass());
                UE_LOG(LogMsEnemyController, Warning, TEXT(" **** fish is attacking ****"));
            }
        }
    }
}

void AMsEnemyController::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor != this) {
        AMsPlayer* Player = Cast<AMsPlayer>(OtherActor);
        if (Player && IsPlayerDetected) {
            // �÷��̾���� ��ħ�� ������ ���� ���� ó��
            IsAttacking = false;
            IsPlayerDetected = false;
            UE_LOG(LogMsEnemyController, Warning, TEXT(" **** Player overlap ended ****"));

            // �ʿ信 ���� �߰� �ൿ ��: ������ �̵� ����
            MoveRandomly();
        }
    }
}

void AMsEnemyController::AdjustCapsuleSize()
{
    if (CapsuleComponent)
    {
        // �⺻ ĳ���� ũ�� (��: ���� 180cm, �ݰ� 50cm)
        const float DefaultCharacterHeight = 180.0f;  // cm ����
        const float DefaultCharacterRadius = 50.0f;   // cm ����

        // ���� ĳ������ ������ ��������
        APawn* ControlledPawn = K2_GetPawn();
        if (ControlledPawn)
        {
            FVector CharacterScale = ControlledPawn->GetActorScale3D();
            float CharacterHeight = DefaultCharacterHeight * CharacterScale.Z; // ���̿� �������� ����
            float CharacterRadius = DefaultCharacterRadius * CharacterScale.X; // �ݰ濡 �������� ����

            // ĸ�� ������Ʈ�� ũ�� ����, 2�� ������ 
            CapsuleComponent->SetCapsuleSize(CharacterRadius, CharacterHeight / 2.0f);
        }
    }
}

void AMsEnemyController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMsEnemyController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (InPawn){
        UE_LOG(LogMsEnemyController, Warning, TEXT("Pawn Possessed: %s"), *InPawn->GetName());
    }
}

void AMsEnemyController::OnSensed(const TArray<AActor*>& UpdatedActors) {
    bool bPlayerDetectedThisFrame = false;

    if (!bPlayerDetectedThisFrame) {
        IsPlayerDetected = false;
        IsAttacking = false;
        MoveRandomly();
    }

    for (AActor* Actor : UpdatedActors) {
        FActorPerceptionBlueprintInfo Information;
        AIPerceptionComponent->GetActorsPerception(Actor, Information);
        UE_LOG(LogMsEnemyController, Warning, TEXT("Actor: %s, Sensed: %d"), *Actor->GetName(), Information.LastSensedStimuli.Num());

        if (Information.LastSensedStimuli.Num() > 0 && Information.LastSensedStimuli[0].WasSuccessfullySensed()) {
            AMsPlayer* Player = Cast<AMsPlayer>(Actor);

            if (Player) {
                bPlayerDetectedThisFrame = true;
                IsPlayerDetected = true;

                if (IsPlayerDead(Player)) {
                    IsAttacking = false;
                    IsPlayerDetected = false;
                    UE_LOG(LogMsEnemyController, Warning, TEXT(" ***** Player dead *****"));
                    break;
                }

                MoveToLocation(Player->GetActorLocation());
                UE_LOG(LogMsEnemyController, Warning, TEXT(" ***** Player detected and moving towards! *****"));

                IsAttacking = true;
                UGameplayStatics::ApplyDamage(Player, 50.0f, this, GetPawn(), UDamageType::StaticClass());
                UE_LOG(LogMsEnemyController, Warning, TEXT(" %%%% Player overlapped and damaged from the front! %%%%"));
                break;
            }
        }
    }
}

void AMsEnemyController::MoveRandomly()
{
    APawn* ControlledPawn = K2_GetPawn();
    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
    FVector RandomLocation;
    FVector CurrentLocation = ControlledPawn->GetActorLocation();

    if (NavSystem) {
        NavSystem->K2_GetRandomReachablePointInRadius(GetWorld(), CurrentLocation, RandomLocation, 500.0f);
        UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, RandomLocation);
    }
}

void AMsEnemyController::CheckPerception() {
    UE_LOG(LogMsEnemyController, Warning, TEXT(" ***** Checking perception *****"));
    TArray<AActor*> UpdatedActors;
    AIPerceptionComponent->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), UpdatedActors);
    for (AActor* Actor : UpdatedActors) {
        if (Actor) {
            UE_LOG(LogMsEnemyController, Warning, TEXT("Detected Actor: %s"), *Actor->GetName());
        }
    }
    OnSensed(UpdatedActors);
}

bool AMsEnemyController::IsPlayerDead(AActor* PlayerActor) {
    AMsPlayer* Player = Cast<AMsPlayer>(PlayerActor);
    if (Player) {
        return Player->Life <= 0;
    }
    return false;
}

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

    // 기본 감지 설정
    SightConfig->SightRadius = 50.0f;
    SightConfig->LoseSightRadius = 100.0f;
    SightConfig->PeripheralVisionAngleDegrees = 180.0f;
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

    // 캡슐 컴포넌트 기본 설정
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

    // 캐릭터의 크기에 맞게 캡슐 크기 설정
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
                // 플레이어에게 피해를 입힘
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
            // 플레이어와의 겹침이 끝났을 때의 로직 처리
            IsAttacking = false;
            IsPlayerDetected = false;
            UE_LOG(LogMsEnemyController, Warning, TEXT(" **** Player overlap ended ****"));

            // 필요에 따라 추가 행동 예: 무작위 이동 시작
            MoveRandomly();
        }
    }
}

void AMsEnemyController::AdjustCapsuleSize()
{
    if (CapsuleComponent)
    {
        // 기본 캐릭터 크기 (예: 높이 180cm, 반경 50cm)
        const float DefaultCharacterHeight = 180.0f;  // cm 단위
        const float DefaultCharacterRadius = 50.0f;   // cm 단위

        // 현재 캐릭터의 스케일 가져오기
        APawn* ControlledPawn = K2_GetPawn();
        if (ControlledPawn)
        {
            FVector CharacterScale = ControlledPawn->GetActorScale3D();
            float CharacterHeight = DefaultCharacterHeight * CharacterScale.Z; // 높이에 스케일을 적용
            float CharacterRadius = DefaultCharacterRadius * CharacterScale.X; // 반경에 스케일을 적용

            // 캡슐 컴포넌트의 크기 설정, 2로 나눈다 
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

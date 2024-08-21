#include "EnemyFish.h"
#include "MsEnemyController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"

DEFINE_LOG_CATEGORY(LogEnemyFish);

AEnemyFish::AEnemyFish()
{
    PrimaryActorTick.bCanEverTick = true;

    // 사운드 컴포넌트 추가
    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
    AudioComponent->SetupAttachment(RootComponent);

    // 사운드 큐 로드
    static ConstructorHelpers::FObjectFinder<USoundBase> SoundCueFinder(TEXT("/Script/Engine.SoundWave'/Game/MsSounds/Rat.Rat'"));
    if (SoundCueFinder.Succeeded()){
        Sound = SoundCueFinder.Object;
    }

    Life = 150.0f;
    Speed = 100.0f;
    PlayerFound = false;
    AttackToPlayer = false;
}

void AEnemyFish::BeginPlay()
{
    Super::BeginPlay();

    if (Sound) {
        AudioComponent->SetSound(Sound);
        AudioComponent->Play();
    }

    // 1초마다 CheckPerception 함수를 호출하는 타이머 설정
    GetWorld()->GetTimerManager().SetTimer(PerceptionTimerHandle, this, &AEnemyFish::CheckPerception, 1.0f, true);
}

void AEnemyFish::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateSoundVolume();
}

void AEnemyFish::CheckPerception() {
    AMsEnemyController* EnemyController = Cast<AMsEnemyController>(GetController());
    if (EnemyController) {
        PlayerFound = EnemyController->PlayerDetect;
        AttackToPlayer = EnemyController->AttackPlayer;

        if (AttackToPlayer) {
                UE_LOG(LogEnemyFish, Warning, TEXT(" ***** fish attacking to player *****"));
                CallFishAttack();
            } else {
            UE_LOG(LogEnemyFish, Warning, TEXT(" ***** fish lost sight of player... just ragging around again *****"));
            CallFishIdle();
        }
    }
}

void AEnemyFish::CallFishIdle() {
    UFunction* IdleMotion = FindFunction(TEXT("Idle"));

    if (IdleMotion) {
        UE_LOG(LogEnemyFish, Warning, TEXT(" ***** CallFishIdle *****"));
        ProcessEvent(IdleMotion, nullptr);
    }

    Speed = 100.0f;  // 스피드를 초기화
    GetCharacterMovement()->MaxWalkSpeed = Speed;
}

void AEnemyFish::CallFishRun() {
    UFunction* RunMotion = FindFunction(TEXT("Run"));

    if (RunMotion) {
        UE_LOG(LogEnemyFish, Warning, TEXT(" ***** CallFishRun *****"));
        ProcessEvent(RunMotion, nullptr);
    }

    Speed = 220.0f;  // 스피드를 220으로 설정
    GetCharacterMovement()->MaxWalkSpeed = Speed;
}

void AEnemyFish::CallFishAttack() {
    UFunction* AttackMotion = FindFunction(TEXT("Attack"));

    if (AttackMotion) {
        UE_LOG(LogEnemyFish, Warning, TEXT(" ***** CallFishAttack *****"));
        ProcessEvent(AttackMotion, nullptr);
    }

    Speed = 180.0f;  
    GetCharacterMovement()->MaxWalkSpeed = Speed;
}

void AEnemyFish::CallFishDie() {
    UFunction* DieMotion = FindFunction(TEXT("Dead"));

    if (DieMotion) {
        ProcessEvent(DieMotion, nullptr);
        if (GetCapsuleComponent()) {
            GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            GetCapsuleComponent()->SetSimulatePhysics(false);
        }
        GetWorld()->GetTimerManager().SetTimer(DieTimerHandle, this, &AEnemyFish::DestroyActor, 3.0f, false);
        UE_LOG(LogEnemyFish, Warning, TEXT(" ***** fish  is dead... *****"));
    }

    Speed = 0.0f;  // 죽을 시 스피드를 0으로 설정
    GetCharacterMovement()->MaxWalkSpeed = Speed;
}

float AEnemyFish::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    // Update health or state here
    Life -= ActualDamage;

    if (Life <= 0) {
        UE_LOG(LogEnemyFish, Warning, TEXT(" ***** enemy dead *****"));
        CallFishDie();
    }

    return ActualDamage;
}

void AEnemyFish::DestroyActor() {
    K2_DestroyActor();
}

void AEnemyFish::UpdateSoundVolume()
{
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (PlayerPawn)
    {
        float Distance = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());
        float VolumeMultiplier = FMath::Clamp(1.0f - (Distance / 1000.0f), 0.0f, 1.0f);

        if (VolumeMultiplier > 0.0f)
        {
            if (!AudioComponent->IsPlaying())
            {
                AudioComponent->Play();
            }
            AudioComponent->SetVolumeMultiplier(VolumeMultiplier);
        }
        else
        {
            if (AudioComponent->IsPlaying())
            {
                AudioComponent->Stop();
            }
        }
    }
}

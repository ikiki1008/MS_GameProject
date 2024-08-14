#include "EnemyRat.h"
#include "MsEnemyController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"

DEFINE_LOG_CATEGORY(LogEnemyRat);

AEnemyRat::AEnemyRat(){
    PrimaryActorTick.bCanEverTick = true;

    // 사운드 컴포넌트 추가
    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
    AudioComponent->SetupAttachment(RootComponent);

    // 사운드 큐 로드
    static ConstructorHelpers::FObjectFinder<USoundBase> SoundCueFinder(TEXT("/Script/Engine.SoundWave'/Game/MsSounds/Rat.Rat'"));
    if (SoundCueFinder.Succeeded()){
        Sound = SoundCueFinder.Object;
    }

    Speed = 50.0f;
    Life = 100.0f;
    PlayerFound = false;
    AttackToPlayer = false;
}

void AEnemyRat::BeginPlay(){
    Super::BeginPlay();

    if (Sound){
        AudioComponent->SetSound(Sound);
        AudioComponent->Play();
    }

    // 1초마다 CheckPerception 함수를 호출하는 타이머 설정
    GetWorld()->GetTimerManager().SetTimer(PerceptionTimerHandle, this, &AEnemyRat::CheckPerception, 1.0f, true);
}

void AEnemyRat::Tick(float DeltaTime){
    Super::Tick(DeltaTime);
    UpdateSoundVolume();
}

void AEnemyRat::UpdateSoundVolume(){
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (PlayerPawn){
        float Distance = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());
        float VolumeMultiplier = FMath::Clamp(1.0f - (Distance / 1000.0f), 0.0f, 1.0f);

        if (VolumeMultiplier > 0.0f){
            if (!AudioComponent->IsPlaying()){
                AudioComponent->Play();
            }
            AudioComponent->SetVolumeMultiplier(VolumeMultiplier);
        }
        else {
            if (AudioComponent->IsPlaying()) {
                AudioComponent->Stop();
            }
        }
    }
}

void AEnemyRat::CallRatAttack(bool Attack) {
    UFunction* AttackMotion = FindFunction(TEXT("RatAttack"));
    UFunction* IdleMotion = FindFunction(TEXT("RatIdle"));
    if (AttackMotion) {
        ProcessEvent(AttackMotion, nullptr);
        UE_LOG(LogEnemyRat, Warning, TEXT(" ***** rat  start attack! *****"));
    }
    else {
        ProcessEvent(IdleMotion, nullptr);
    }
}

void AEnemyRat::CheckPerception(){
    AMsEnemyController* EnemyController = Cast<AMsEnemyController>(GetController());
    if (EnemyController){
        PlayerFound = EnemyController->IsPlayerDetected;
        AttackToPlayer = EnemyController->IsAttacking;

        if (AttackToPlayer){
            UE_LOG(LogEnemyRat, Warning, TEXT(" ***** Player detected and attacking! *****"));
            Speed = 100.0f;
            CallRatAttack(true);
        }
        else {
            Speed = 50.0f;
            CallRatAttack(false);
        }
    }
}
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

    PlayerFound = false;
    AttackToPlayer = false;
    IsRatDead = false;
}

void AEnemyRat::BeginPlay(){
    Super::BeginPlay();

    if (Sound){
        AudioComponent->SetSound(Sound);
        AudioComponent->Play();
    }

    CallRatAttack(false);
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
    if (Attack) {
        ProcessEvent(AttackMotion, nullptr);
        UE_LOG(LogEnemyRat, Warning, TEXT(" ***** rat  start attack! *****"));
    }
    else {
        ProcessEvent(IdleMotion, nullptr);
    }
}

void AEnemyRat::CallRatDie() {
    UFunction* DeadMotion = FindFunction(TEXT("RatDead"));
   
    if (DeadMotion) {
        ProcessEvent(DeadMotion, nullptr);
        UE_LOG(LogEnemyRat, Warning, TEXT(" ***** rat  is dead... *****"));
    }
}

void AEnemyRat::CheckPerception(){
    AMsEnemyController* EnemyController = Cast<AMsEnemyController>(GetController());
    if (EnemyController){
        PlayerFound = EnemyController->IsPlayerDetected;
        AttackToPlayer = EnemyController->IsAttacking;
        IsRatDead = EnemyController->IsEnemyDead;

        if (IsRatDead) {
            //MsEnemyController 를 통하여 ai 몬스터의 피가 0으로 깎였는지 확인.
            CallRatDie();
        }
        else {
            if (AttackToPlayer){
                UE_LOG(LogEnemyRat, Warning, TEXT(" ***** Player detected and attacking! *****"));
                CallRatAttack(true);
            }
            else {
                UE_LOG(LogEnemyRat, Warning, TEXT(" ***** rat is just walking around ...  *****"));
                CallRatAttack(false);
            }
        }
    }
}
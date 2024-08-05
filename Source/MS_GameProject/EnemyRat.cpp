#include "EnemyRat.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"

DEFINE_LOG_CATEGORY(LogEnemyRat);

AEnemyRat::AEnemyRat()
{
    PrimaryActorTick.bCanEverTick = true;

    // 사운드 컴포넌트 추가
    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
    AudioComponent->SetupAttachment(RootComponent);

    // 사운드 큐 로드
    static ConstructorHelpers::FObjectFinder<USoundBase> SoundCueFinder(TEXT("/Script/Engine.SoundWave'/Game/MsSounds/Rat.Rat'"));
    if (SoundCueFinder.Succeeded())
    {
        Sound = SoundCueFinder.Object;
    }
}

void AEnemyRat::BeginPlay()
{
    Super::BeginPlay();
    RandomWalk(false);

    if (Sound)
    {
        AudioComponent->SetSound(Sound);
        AudioComponent->Play();
    }
}

void AEnemyRat::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateSoundVolume();
}

void AEnemyRat::RandomWalk(bool found)
{
    if (!GetController()) return;

    if (!found) {
        UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
        if (NavSystem){
            FVector RandomLocation;
            NavSystem->K2_GetRandomReachablePointInRadius(GetWorld(), GetActorLocation(), RandomLocation, 500.0f);
            UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), RandomLocation);
            UE_LOG(LogEnemyRat, Warning, TEXT(" ***** the mouse is randomly walking.... *****"));
        }
    }
    else {
        UE_LOG(LogEnemyRat, Warning, TEXT(" ***** the mouse is gonna go to the player.... *****"));
    }
}

void AEnemyRat::UpdateSoundVolume()
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

//void AEnemyRat::MoveToPlayer()
//{
//    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0); //Player character == number zero
//    if (PlayerCharacter)
//    {
//        FVector PlayerLocation = PlayerCharacter->GetActorLocation();
//        UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), PlayerLocation);
//        UE_LOG(LogEnemyRat, Warning, TEXT(" ***** this mouse has found the Player *****"));
//    }
//}

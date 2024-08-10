#include "EnemyFish.h"
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
	if (SoundCueFinder.Succeeded())
	{
		Sound = SoundCueFinder.Object;
	}
}


void AEnemyFish::BeginPlay()
{
	Super::BeginPlay();
	RandomWalk(true);
	
	if (Sound) {
		AudioComponent->SetSound(Sound);
		AudioComponent->Play();
	}
}

void AEnemyFish::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateSoundVolume();
}

void AEnemyFish::RandomWalk(bool found) {
	if (!found) {
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		if (NavSystem) {
			FVector RandomLocation;
			NavSystem->K2_GetRandomReachablePointInRadius(GetWorld(), GetActorLocation(), RandomLocation, 500.0f);
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), RandomLocation);
			UE_LOG(LogEnemyFish, Warning, TEXT(" ***** the fish  is randomly walking.... *****"));
		}
	}
	else {
		UE_LOG(LogEnemyFish, Warning, TEXT(" ***** the fish is gonna go to the player.... *****"));
	}
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



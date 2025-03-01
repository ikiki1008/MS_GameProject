#include "MsCoins.h"
#include "MsPlayer.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Texture.h"
#include "Materials/MaterialInstanceDynamic.h"

AMsCoins::AMsCoins()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMsCoins::BeginPlay()
{
	Super::BeginPlay();

	USphereComponent* SphereComponent = GetSphereComponent();

	if (SphereComponent) {
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AMsCoins::OnOverlapBegin);
	}
}

void AMsCoins::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && (OtherActor != this)) {
        AMsPlayer* MainPlayer = Cast<AMsPlayer>(OtherActor);
        if (MainPlayer) {
            //MainPlayer->AddHP(100); // Adjust health by 100
            Destroy(); // Destroy the heal food actor
        }
    }
}

USphereComponent* AMsCoins::GetSphereComponent() const {
    return FindComponentByClass<USphereComponent>();
}


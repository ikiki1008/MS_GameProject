#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MsCoins.generated.h"

UCLASS()
class MS_GAMEPROJECT_API AMsCoins : public AActor
{
	GENERATED_BODY()
	
public:	
	AMsCoins();

	UFUNCTION(BlueprintCallable, Category = "Components")
	class USphereComponent* GetSphereComponent() const;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MsHealFoods.generated.h"

UCLASS()
class MS_GAMEPROJECT_API AMsHealFoods : public AActor
{
    GENERATED_BODY()

public:
    AMsHealFoods();

    UFUNCTION(BlueprintCallable, Category = "Components")
    class USphereComponent* GetSphereComponent() const;

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};

DECLARE_LOG_CATEGORY_EXTERN(LogHealFood, Log, All);

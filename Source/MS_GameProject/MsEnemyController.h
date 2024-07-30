#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Components/CapsuleComponent.h"
#include "MsEnemyController.generated.h"

UCLASS()
class MS_GAMEPROJECT_API AMsEnemyController : public AAIController
{
    GENERATED_BODY()

public:
    AMsEnemyController();

    UPROPERTY(EditAnywhere)
    UCapsuleComponent* CapsuleComponent;

    UPROPERTY(EditAnywhere)
    UAIPerceptionComponent* AIPerceptionComponent;

    UPROPERTY(EditAnywhere)
    UAISenseConfig_Sight* SightConfig;

protected:
    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;

    UFUNCTION()
    void OnSensed(const TArray<AActor*>& UpdatedActors);

    //UFUNCTION()
    //void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    void CheckPerception();

    FTimerHandle PerceptionTimerHandle;

private:
    void AttackPlayer(AActor* Player);
};

DECLARE_LOG_CATEGORY_EXTERN(LogMsEnemyController, Log, All);

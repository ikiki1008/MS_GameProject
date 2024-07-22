
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MsEnemyController.generated.h"

UCLASS()
class MS_GAMEPROJECT_API AMsEnemyController : public AAIController
{
	GENERATED_BODY()

public:
    AMsEnemyController();

protected:
    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn);

public:
    UFUNCTION(BlueprintCallable)
    void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

private:
    UPROPERTY(VisibleAnywhere, Category = "AI")
    class UAIPerceptionComponent* AIPerceptionComponent;

    UPROPERTY(VisibleAnywhere, Category = "AI")
    class UAISenseConfig_Sight* SightConfig;
	
};

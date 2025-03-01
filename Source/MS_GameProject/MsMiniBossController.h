#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "MsMiniBossController.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMiniBossController, Log, All);

UCLASS()
class MS_GAMEPROJECT_API AMsMiniBossController : public AAIController
{
	GENERATED_BODY()

public:
	AMsMiniBossController();

	// Tick 함수 추가
	virtual void Tick(float DeltaTime) override;

	virtual void OnPossess(APawn* InPawn) override;

	bool IsPlayerDead(AActor* Player);

	// 플레이어 감지 및 공격 상태
	bool PlayerDetect;
	bool PlayerAttack;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Perception")
	UAIPerceptionComponent* MsPerceptionComponent;  // AI Perception Component

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Perception")
	UAISenseConfig_Sight* SightConfig;  // Sight Configuration

private:
	bool IsPlayerNear;

	FTimerHandle TimerHandle;
	
	FTimerHandle AttackTimerHandle;

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus Stimulus);  // 감지 이벤트 처리

	UFUNCTION()
	void OnLoseSightOfPlayer();  

	void CheckSituation();

	void PerformAttack();

	APawn* TargetPawn; 

	FVector OriginalPosition; 
};

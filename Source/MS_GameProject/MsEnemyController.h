#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/CapsuleComponent.h"
#include "MsEnemyController.generated.h"

UCLASS()
class MS_GAMEPROJECT_API AMsEnemyController : public AAIController
{
    GENERATED_BODY()

public:
    AMsEnemyController();

    virtual void OnPossess(APawn* InPawn) override;

    // 플레이어가 사망했는지 여부를 체크하는 함수
    bool IsPlayerDead(AActor* Player);
    bool PlayerDetect;
    bool AttackPlayer;
    bool RandomMove;

    //UFUNCTION(BlueprintCallable, Category = "MSEnemy")
    //void MoveRandomly();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MsEnemy")
    UPawnSensingComponent* PawnSensingComponent;

private:

    UFUNCTION()
    void OnSeePawn(APawn* DetectedPawn);

    UFUNCTION()
    void OnLoseSightOfPlayer();

    void ReturnToOriginalPosition();

    FVector OriginalPosition;
};

// 로그 카테고리 선언
DECLARE_LOG_CATEGORY_EXTERN(LogMsEnemyController, Log, All);

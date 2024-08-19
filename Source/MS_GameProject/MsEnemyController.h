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

    // 캡슐 컴포넌트
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MSEnemy")
    UCapsuleComponent* CapsuleComponent;

    // AI 인식 컴포넌트
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MSEnemy")
    UAIPerceptionComponent* AIPerceptionComponent;

    // 시야 설정
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MSEnemy")
    UAISenseConfig_Sight* SightConfig;

    // 플레이어를 발견했는지 여부를 나타내는 변수
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MSEnemy")
    bool IsPlayerDetected;

    // 공격 상태 여부를 나타내는 변수
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MSEnemy")
    bool IsAttacking;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MSEnemy")
    bool IsEnemyDead;

    // 적이 랜덤으로 이동하도록 하는 함수
    UFUNCTION(BlueprintCallable, Category = "MSEnemy")
    void MoveRandomly();

    // 캡슐 크기 조정 함수
    UFUNCTION(BlueprintCallable, Category = "MSEnemy")
    void AdjustCapsuleSize();

protected:
    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;
    virtual void Tick(float DeltaTime) override;
    FTimerHandle AttackTimerHandle;

    // AI 인식 업데이트에 대한 콜백 함수
    UFUNCTION()
    void OnSensed(const TArray<AActor*>& UpdatedActors);

    // 인식 상태를 주기적으로 체크하는 함수
    void CheckPerception();

private:
    // 플레이어가 사망했는지 여부를 체크하는 함수
    bool IsPlayerDead(AActor* Player);

    // 인식 타이머 핸들
    FTimerHandle PerceptionTimerHandle;
};

// 로그 카테고리 선언
DECLARE_LOG_CATEGORY_EXTERN(LogMsEnemyController, Log, All);

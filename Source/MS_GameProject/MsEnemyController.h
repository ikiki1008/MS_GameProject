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

    // ĸ�� ������Ʈ
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MSEnemy")
    UCapsuleComponent* CapsuleComponent;

    // AI �ν� ������Ʈ
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MSEnemy")
    UAIPerceptionComponent* AIPerceptionComponent;

    // �þ� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MSEnemy")
    UAISenseConfig_Sight* SightConfig;

    // �÷��̾ �߰��ߴ��� ���θ� ��Ÿ���� ����
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MSEnemy")
    bool IsPlayerDetected;

    // ���� ���� ���θ� ��Ÿ���� ����
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MSEnemy")
    bool IsAttacking;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MSEnemy")
    bool IsEnemyDead;

    // ���� �������� �̵��ϵ��� �ϴ� �Լ�
    UFUNCTION(BlueprintCallable, Category = "MSEnemy")
    void MoveRandomly();

    // ĸ�� ũ�� ���� �Լ�
    UFUNCTION(BlueprintCallable, Category = "MSEnemy")
    void AdjustCapsuleSize();

protected:
    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;
    virtual void Tick(float DeltaTime) override;
    FTimerHandle AttackTimerHandle;

    // AI �ν� ������Ʈ�� ���� �ݹ� �Լ�
    UFUNCTION()
    void OnSensed(const TArray<AActor*>& UpdatedActors);

    // �ν� ���¸� �ֱ������� üũ�ϴ� �Լ�
    void CheckPerception();

private:
    // �÷��̾ ����ߴ��� ���θ� üũ�ϴ� �Լ�
    bool IsPlayerDead(AActor* Player);

    // �ν� Ÿ�̸� �ڵ�
    FTimerHandle PerceptionTimerHandle;
};

// �α� ī�װ� ����
DECLARE_LOG_CATEGORY_EXTERN(LogMsEnemyController, Log, All);

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

    // �÷��̾ ����ߴ��� ���θ� üũ�ϴ� �Լ�
    bool IsPlayerDead(AActor* Player);

    //�÷��̾� �߰߰� ���� ����
    bool PlayerDetect;
    bool AttackPlayer;

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

    APawn* TargetPawn;

    FVector OriginalPosition; //���� ��ġ�� ���ư����� ��ġ�� �����ϴ� ����

    bool SpecialMonster; //�������� ��ȯ�ϴ� Ư���� �����ΰ�?

    float Radius; //sightRadius �ݰ�

    float Damage;

};

DECLARE_LOG_CATEGORY_EXTERN(LogMsEnemyController, Log, All);

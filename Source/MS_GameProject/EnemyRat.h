#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyRat.generated.h"

UCLASS()
class MS_GAMEPROJECT_API AEnemyRat : public ACharacter
{
    GENERATED_BODY()

public:
    AEnemyRat();

    // MsEnemyController�� ���¸� �����ϴ� ����
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "RatAnime")
    bool PlayerFound;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "RatAnime")
    bool AttackToPlayer;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "RatAnime")
    float Speed;


   // void RandomWalk(bool found);

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(BlueprintReadWrite, Category = "Rat Life")
    float Life;

    UFUNCTION(BlueprintCallable, Category = "Attack")
    void CallRatAttack(bool Attack);

private:
    void UpdateSoundVolume();

    // 1�ʸ��� �÷��̾� Ž�� ���¸� üũ�ϴ� �Լ�
    void CheckPerception();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
    class UAudioComponent* AudioComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
    class USoundBase* Sound;

    // Ÿ�̸� �ڵ鷯
    FTimerHandle PerceptionTimerHandle;
};

// log ����
DECLARE_LOG_CATEGORY_EXTERN(LogEnemyRat, Log, All);
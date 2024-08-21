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

    UPROPERTY(BlueprintReadWrite, Category = "Life")
    float Life;
    // MsEnemyController�� ���¸� �����ϴ� ����
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Anime")
    bool PlayerFound;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Anime")
    bool AttackToPlayer;
    virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

    //UFUNCTION(BlueprintCallable, Category = "Enemy")
    //void MoveRandomly();

protected:

    // Ÿ�̸� �ڵ鷯
    FTimerHandle PerceptionTimerHandle;
    FTimerHandle DieTimerHandle;
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:
    void UpdateSoundVolume();

    // 1�ʸ��� �÷��̾� Ž�� ���¸� üũ�ϴ� �Լ�
    void CheckPerception();

    //enemy arrack motion
    UFUNCTION(BlueprintCallable, Category = "Attack")
    void CallAttack(bool Attack);

    //enemy die motion
    UFUNCTION(BlueprintCallable, Category = "Dead")
    void CallDie();

    //enemy Hit motion
    UFUNCTION(BlueprintCallable, Category = "Hit")
    void CallHit();

    //enemy run motion
    UFUNCTION(BlueprintCallable, Category = "Run")
    void CallRun();

    UFUNCTION(BlueprintCallable, Category = "Idle")
    void CallIdle();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
    class UAudioComponent* AudioComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
    class USoundBase* Sound;

    void DestroyActor();
};

// log ����
DECLARE_LOG_CATEGORY_EXTERN(LogEnemyRat, Log, All);
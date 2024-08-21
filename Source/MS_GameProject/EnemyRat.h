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
    // MsEnemyController의 상태를 참조하는 변수
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Anime")
    bool PlayerFound;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Anime")
    bool AttackToPlayer;
    virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

    //UFUNCTION(BlueprintCallable, Category = "Enemy")
    //void MoveRandomly();

protected:

    // 타이머 핸들러
    FTimerHandle PerceptionTimerHandle;
    FTimerHandle DieTimerHandle;
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:
    void UpdateSoundVolume();

    // 1초마다 플레이어 탐지 상태를 체크하는 함수
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

// log 선언
DECLARE_LOG_CATEGORY_EXTERN(LogEnemyRat, Log, All);
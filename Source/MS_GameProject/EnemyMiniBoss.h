#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyMiniBoss.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMiniBoss, Log, All);

UCLASS()
class MS_GAMEPROJECT_API AEnemyMiniBoss : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyMiniBoss();

    UPROPERTY(EditAnywhere, Category = "Life")
    float Life;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Anime")
    bool PlayerFound;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Anime")
    bool AttackToPlayer;

    virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

    //enemy die motion
    UFUNCTION(BlueprintCallable, Category = "Dead")
    bool CallDie();

protected:
	virtual void BeginPlay() override;
    FTimerHandle PerceptionTimerHandle;
    FTimerHandle DieTimerHandle;
    FTimerHandle WaitTimerHandle;

public:	
	virtual void Tick(float DeltaTime) override;

private:
    FTimerHandle TimerHandle;

    bool IsNoticed;

    void CheckPerception();

    void SetMode();

    //enemy arrack motion
    UFUNCTION(BlueprintCallable, Category = "Attack")
    void CallMiniBossAttack();

    //enemy Hit motion
    UFUNCTION(BlueprintCallable, Category = "Hit")
    void CallHit();

    //enemy run motion
    UFUNCTION(BlueprintCallable, Category = "Run")
    void CallRun();

    UFUNCTION(BlueprintCallable, Category = "Idle")
    void CallIdle();

    void DestroyActor();

    void NoticedMotion();
};
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MsPlayer.generated.h"

UCLASS()
class MS_GAMEPROJECT_API AMsPlayer : public ACharacter
{
    GENERATED_BODY()

public:
    AMsPlayer();

protected:
    virtual void BeginPlay() override;

public:
    FTimerHandle TimerHandle;
    bool bFlyForward;
    UPROPERTY(EditAnywhere)
    float MovementSpeed;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> PlayerHPProgress;

    UPROPERTY()
    class UPlayerHPBar* PlayerHPBar;

    UPROPERTY(BlueprintReadWrite, Category = "Player Life")
    float Life;

    UFUNCTION(BlueprintCallable, Category = "Dead")
    bool CallDie(bool dead);

    UFUNCTION(BlueprintCallable, Category = "Attack")
    void CallAttack();

    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
    void AddHP(float Amount);

protected:
    UPROPERTY(EditAnywhere)
    class UCameraComponent* Camera;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Speed;

    void MoveForward(float InputValue);
    void LookUp(float InputValue);
    void MoveRight(float InputValue);
    void TurnCamera(float InputValue);
    void UpdateHPBar(float CurrentHP);

    float TurnSensitivity;
    float LookUpSensitivity;

    float MinPitch;  // 최대 아래쪽 회전 각도
    float MaxPitch;  // 최대 위쪽 회전 각도
    float MinYaw;    // 최대 왼쪽 회전 각도
    float MaxYaw;    // 최대 오른쪽 회전 각도
};

DECLARE_LOG_CATEGORY_EXTERN(LogMsPlayer, Log, All);

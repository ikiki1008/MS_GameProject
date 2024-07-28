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
    float MovementSpeed;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> PlayerHPProgress;
    UPROPERTY()
    class UPlayerHPBar* PlayerHPBar;
    UPROPERTY(BlueprintReadWrite, Category = "Player Life")
    float Life;

    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
    UPROPERTY(EditAnywhere)
    class UCameraComponent* Camera;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Speed;

    void MoveForward(float InputValue);
    void MoveRight(float InputValue);
    void TurnCamera(float InputValue);
    void UpdateHPBar(float CurrentHP);
};

DECLARE_LOG_CATEGORY_EXTERN(LogMsPlayer, Log, All);

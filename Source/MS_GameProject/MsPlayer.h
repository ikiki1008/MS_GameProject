
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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Stats")
	float MaxHP;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Stats")
	float CurrentHP;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> PlayerHPProgress;
	UPROPERTY()
	class UPlayerHPBar* PlayerHPBar;

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

	//UPROPERTY(EditAnywhere)
	//class USpringArmComponent* SpringArm;

	void MoveForward(float InputValue);
	void MoveRight(float InputValue);
	void TurnCamera(float InputValue);
	void UpdateHPBar();
	//void LookUp(float InputValue);

};

DECLARE_LOG_CATEGORY_EXTERN(LogMsPlayer, Log, All);

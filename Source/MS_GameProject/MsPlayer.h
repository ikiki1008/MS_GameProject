
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

	// Boolean to check if the player should start flying
	bool bFlyForward;

	// Movement speed
	float MovementSpeed;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Life;

	//UPROPERTY(EditAnywhere)
	//class USpringArmComponent* SpringArm;

	void MoveForward(float InputValue);
	void MoveRight(float InputValue);
	void TurnCamera(float InputValue);
	//void LookUp(float InputValue);

};

DECLARE_LOG_CATEGORY_EXTERN(LogMsPlayer, Log, All);

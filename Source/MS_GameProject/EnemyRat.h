#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyRat.generated.h"

UCLASS()
class MS_GAMEPROJECT_API AEnemyRat : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyRat();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintPure)
	bool IsPlayerNear() const;
	UFUNCTION(BlueprintPure)
	bool IsPlayerFound() const;

	void SetPlayerFound(bool bFound);
	void SetPlayerNear(bool bNear);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Variables for animation blueprint
	UPROPERTY(BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	bool bIsPlayerFound;

	UPROPERTY(BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	bool bIsPlayerNear;
};

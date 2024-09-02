#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyPeter.generated.h"

UCLASS()
class MS_GAMEPROJECT_API AEnemyPeter : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyPeter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MsEnemyMaster.generated.h"

UCLASS()
class MS_GAMEPROJECT_API AMsEnemyMaster : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMsEnemyMaster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float Health;

	UFUNCTION(BlueprintCallable, Category="Enemy")
	virtual void Attack();

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	virtual void TakeDamage(float DamageAmount);

};

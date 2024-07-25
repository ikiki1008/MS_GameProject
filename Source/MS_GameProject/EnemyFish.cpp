// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyFish.h"

// Sets default values
AEnemyFish::AEnemyFish()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyFish::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyFish::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyFish::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


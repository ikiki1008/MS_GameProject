// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPeter.h"

// Sets default values
AEnemyPeter::AEnemyPeter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyPeter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyPeter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyPeter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


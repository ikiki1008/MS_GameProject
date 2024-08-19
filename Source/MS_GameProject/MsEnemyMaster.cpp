// Fill out your copyright notice in the Description page of Project Settings.


#include "MsEnemyMaster.h"

// Sets default values
AMsEnemyMaster::AMsEnemyMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Health = 100.0f;

}

// Called when the game starts or when spawned
void AMsEnemyMaster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMsEnemyMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMsEnemyMaster::Attack() {

}

void AMsEnemyMaster::TakeDamage(float DamageAmount) {
	Health -= DamageAmount;
	if (Health <= 0) {
		Destroy();
	}
}


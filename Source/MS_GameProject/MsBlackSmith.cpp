// Fill out your copyright notice in the Description page of Project Settings.


#include "MsBlackSmith.h"

// Sets default values
AMsBlackSmith::AMsBlackSmith()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMsBlackSmith::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMsBlackSmith::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMsBlackSmith::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


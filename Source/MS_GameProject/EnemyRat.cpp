#include "EnemyRat.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemyRat::AEnemyRat()
{
	// Set this character to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;
	bIsPlayerFound = false;
	bIsPlayerNear = false;
}

// Called when the game starts or when spawned
void AEnemyRat::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemyRat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemyRat::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyRat::SetPlayerFound(bool bFound) {
	bIsPlayerFound = bFound;
}

void AEnemyRat::SetPlayerNear(bool bNear){
	bIsPlayerNear = bNear;
}

bool AEnemyRat::IsPlayerFound() const{
	return bIsPlayerFound;
}

bool AEnemyRat::IsPlayerNear() const {
	return bIsPlayerNear;
}

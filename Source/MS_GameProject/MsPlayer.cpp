
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"

#include "MsPlayer.h"
#include "Engine/World.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY(LogMsPlayer); // Define the log category

AMsPlayer::AMsPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	//SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	//SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;


	MovementSpeed = 400.0f;
}

void AMsPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void AMsPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMsPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMsPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMsPlayer::MoveRight);
	PlayerInputComponent->BindAxis("TurnCamera", this, &AMsPlayer::TurnCamera);
	//PlayerInputComponent->BindAxis("LookUp", this, &AMsPlayer::LookUp);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMsPlayer::Jump);

	Speed = 300.0f;
	Life = 1000.0f;
}

void AMsPlayer::MoveForward(float InputValue) {
	FVector ForwardDirection = GetActorForwardVector();
	AddMovementInput(ForwardDirection, InputValue);
}

void AMsPlayer::MoveRight(float InputValue) {
	FVector RightDirection = GetActorRightVector();
	AddMovementInput(RightDirection, InputValue);
}

void AMsPlayer::TurnCamera(float InputValue) {
	AddControllerYawInput(InputValue);
}

float AMsPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Life -= ActualDamage;
	UE_LOG(LogMsPlayer, Warning, TEXT(" #### Current Life: %f"), Life);

	if (Life <= 0) {
		//player dead
	}

	return ActualDamage;
}







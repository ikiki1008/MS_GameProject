#include "MsPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "PlayerHPBar.h"

DEFINE_LOG_CATEGORY(LogMsPlayer); // Define the log category

AMsPlayer::AMsPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;

	MovementSpeed = 200.0f;
	MaxHP = 1000.0f;
	CurrentHP = 1000.0f;
}

void AMsPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerHPProgress) {
		PlayerHPBar = CreateWidget<UPlayerHPBar>(GetWorld(), PlayerHPProgress);
		if (PlayerHPBar) {
			PlayerHPBar->AddToViewport();
			UpdateHPBar();
		}
	}
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
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMsPlayer::Jump);
}

void AMsPlayer::MoveForward(float InputValue) {
	FVector ForwardDirection = GetActorForwardVector();
	AddMovementInput(ForwardDirection, InputValue);
	UE_LOG(LogMsPlayer, Warning, TEXT(" #### Current Life: %f"), CurrentHP);
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

	CurrentHP -= ActualDamage;
	UE_LOG(LogMsPlayer, Warning, TEXT(" #### Current Life: %f"), CurrentHP);

	if (CurrentHP <= 0) {
		UE_LOG(LogMsPlayer, Warning, TEXT(" #### Player Dead $$$$$ "));
	}

	UpdateHPBar();
	return ActualDamage;
}

void AMsPlayer::UpdateHPBar() {
	if (PlayerHPBar) {
		PlayerHPBar->UpdateHP(CurrentHP, MaxHP);
	}
}







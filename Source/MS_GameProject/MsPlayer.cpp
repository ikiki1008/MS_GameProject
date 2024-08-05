#include "MsPlayer.h"
#include "Camera/CameraComponent.h"
#include "MsEnemyController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "PlayerHPBar.h"
#include "GameFramework/DamageType.h"
#include "Engine/EngineTypes.h"

DEFINE_LOG_CATEGORY(LogMsPlayer);

AMsPlayer::AMsPlayer()
{
    PrimaryActorTick.bCanEverTick = true;
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
    Camera->SetupAttachment(RootComponent);
    Camera->bUsePawnControlRotation = true;

    MovementSpeed = 200.0f;
    Life = 1000.0f;

    GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMsPlayer::OnOverlapBegin);
    GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AMsPlayer::OnOverlapEnd);
}

void AMsPlayer::BeginPlay()
{
    Super::BeginPlay();
    if (PlayerHPProgress) {
        PlayerHPBar = CreateWidget<UPlayerHPBar>(GetWorld(), PlayerHPProgress);
        if (PlayerHPBar) {
            PlayerHPBar->AddToViewport();
            UpdateHPBar(Life);
        }
    }
}

void AMsPlayer::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
    UE_LOG(LogMsPlayer, Warning, TEXT("OnOverlap begin ...."));

    if (OtherActor && (OtherActor != this)) {
        APawn* OtherPawn = Cast<APawn>(OtherActor);
        if (OtherPawn && OtherPawn->GetController() && OtherPawn->GetController()->IsA(AMsEnemyController::StaticClass())) {
            UE_LOG(LogMsPlayer, Warning, TEXT("OnOverlap begin with enemy ...."));
        }
    }
}

void AMsPlayer::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
    UE_LOG(LogMsPlayer, Warning, TEXT("OnOverlap End"));
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
        UE_LOG(LogMsPlayer, Warning, TEXT(" #### Player Dead $$$$$ "));
        CallDie(); // 블루프린트에서 정의된 Die 함수 호출
    }

    UpdateHPBar(Life);
    return ActualDamage;
}

void AMsPlayer::AddHP(float Amount) {
    Life += Amount;
    if (Life > 1000.0f) {
        Life = 1000.0f;
    }

    UE_LOG(LogMsPlayer, Warning, TEXT(" #### Health Modified: %f"), Life)
    UpdateHPBar(Life);
}

void AMsPlayer::UpdateHPBar(float CurrentHP) {
    if (PlayerHPBar) {
        PlayerHPBar->UpdateHP(CurrentHP);
    }
}

void AMsPlayer::CallDie() {

    UFunction* DieFunction = FindFunction(TEXT("Die")); // "Die"는 블루프린트에서 정의한 함수의 이름입니다.
    if (DieFunction) {
        UE_LOG(LogMsPlayer, Warning, TEXT(" is reached here?? "));
        ProcessEvent(DieFunction, nullptr);
    }
    UE_LOG(LogMsPlayer, Warning, TEXT(" Call Die function end"));
}

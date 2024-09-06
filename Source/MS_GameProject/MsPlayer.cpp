
#include "MsPlayer.h"
#include "Camera/CameraComponent.h"
#include "MsEnemyController.h"
#include "MsMiniBossController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
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

    IsWidgetActive = false;
    ChooseWidget = nullptr;

    MovementSpeed = 200.0f;
    Life = 1000.0f;

    TurnSensitivity = 0.5f;
    LookUpSensitivity = 0.5f;

    MinPitch = -30.0f;   // 아래로 최대 0도
    MaxPitch = 90.0f;    // 위로 최대 90도
    MinYaw = -360.0f;     // 왼쪽으로 최대 180도
    MaxYaw = 360.0f;      // 오른쪽으로 최대 180도

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
    PlayerInputComponent->BindAxis("LookUp", this, &AMsPlayer::LookUp);
    PlayerInputComponent->BindAxis("MoveRight", this, &AMsPlayer::MoveRight);
    PlayerInputComponent->BindAxis("SelectLeft", this, &AMsPlayer::ChooseLeft);
    PlayerInputComponent->BindAxis("SelectRight", this, &AMsPlayer::ChooseRight);
    PlayerInputComponent->BindAxis("TurnCamera", this, &AMsPlayer::TurnCamera);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMsPlayer::Jump);
    PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMsPlayer::CallAttack); 
}

void AMsPlayer::MoveForward(float InputValue) {
    FVector ForwardDirection = GetActorForwardVector();
    AddMovementInput(ForwardDirection, InputValue);
}

void AMsPlayer::LookUp(float InputValue) {
    FRotator ControlRotation = GetControlRotation();
    float NewPitch = ControlRotation.Pitch + InputValue * LookUpSensitivity;

    // Pitch 값 제한
    NewPitch = FMath::Clamp(NewPitch, MinPitch, MaxPitch);

    ControlRotation.Pitch = NewPitch;
    GetController()->SetControlRotation(ControlRotation);
}

void AMsPlayer::MoveRight(float InputValue) {
    FVector RightDirection = GetActorRightVector();
    AddMovementInput(RightDirection, InputValue);
}

void AMsPlayer::TurnCamera(float InputValue) {
    FRotator ControlRotation = GetControlRotation();
    float NewYaw = ControlRotation.Yaw + InputValue * TurnSensitivity;

    // Yaw 값 제한
    if (NewYaw < MinYaw) {
        NewYaw = MinYaw;
    }
    else if (NewYaw > MaxYaw) {
        NewYaw = MaxYaw;
    }

    ControlRotation.Yaw = NewYaw;
    GetController()->SetControlRotation(ControlRotation);
}

void AMsPlayer::ChooseLeft(float Value){ //버튼을 선택하는 위젯이 활성화 될때 잠시 방향키 (A,D) 를 선택키로 쓸수있게 한다
    if (IsWidgetActive && ChooseWidget)
    {
        ChooseWidget->NavigateLeft();
    }
}

void AMsPlayer::ChooseRight(float Value) {
    if (IsWidgetActive && ChooseWidget)
    {
        ChooseWidget->NavigateRight();
    }
}

void  AMsPlayer::SetWidgetActive(bool Active) { //현재 버튼 선택 위젯이 켜졌는지 확인한다
    IsWidgetActive = Active;
}

bool AMsPlayer::IsWidgetActivated() const { //위젯 활성여부의 값을 return
    return IsWidgetActive;
}

float AMsPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    Life -= ActualDamage;
    UE_LOG(LogMsPlayer, Warning, TEXT(" #### Current Life: %f"), Life);

    if (Life <= 0) {
        UE_LOG(LogMsPlayer, Warning, TEXT(" #### Player Dead $$$$$ "));
        CallDie(true); // 블루프린트에서 정의된 Die 함수 호출
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

bool AMsPlayer::CallDie(bool dead) {
    if (dead) {
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

        UFunction* DieFunction = FindFunction(TEXT("Die")); // "Die"는 블루프린트에서 정의한 함수의 이름
        if (DieFunction) {
            ProcessEvent(DieFunction, nullptr);
        }
        return true;
    }
    return false;
}

void AMsPlayer::CallAttack() {
    //카메라를 이용하여 시야에 들어온 몬스터 위치 찾기
    FVector Start = Camera->GetComponentLocation();
    FVector Forward = Camera->GetForwardVector();
    FVector End = Start + (Forward * 1000.0f);

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this); //시야에 플레이어 본인이 담길 경우 무시하기

    TArray<FHitResult> HitResults;
    GetWorld()->LineTraceMultiByChannel(HitResults, Start, End, ECC_Pawn, CollisionParams);

    for (FHitResult Hit : HitResults) {
        AActor* Monster = Hit.GetActor();
        if (Monster) {
            APawn* Target = Cast<APawn>(Monster);
            if (Target && Target->GetController()->IsA(AMsEnemyController::StaticClass())) {
                UGameplayStatics::ApplyDamage(Target, 100.0f, GetController(), this, UDamageType::StaticClass());
                UE_LOG(LogMsPlayer, Warning, TEXT("player attacked!"));
            }
            else if (Target && Target->GetController()->IsA(AMsMiniBossController::StaticClass())) {
                UGameplayStatics::ApplyDamage(Target, 100.0f, GetController(), this, UDamageType::StaticClass());
                UE_LOG(LogMsPlayer, Warning, TEXT("player attacked to mini boss!"));
            }
        }
    }

    //do the attack motion in blueprint
    UFunction* AttackFunction = FindFunction(TEXT("Attack"));
    if (AttackFunction) {
        ProcessEvent(AttackFunction, nullptr);
    }
}

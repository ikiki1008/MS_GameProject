
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

    MinPitch = -30.0f;   // �Ʒ��� �ִ� 0��
    MaxPitch = 90.0f;    // ���� �ִ� 90��
    MinYaw = -360.0f;     // �������� �ִ� 180��
    MaxYaw = 360.0f;      // ���������� �ִ� 180��

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

    // Pitch �� ����
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

    // Yaw �� ����
    if (NewYaw < MinYaw) {
        NewYaw = MinYaw;
    }
    else if (NewYaw > MaxYaw) {
        NewYaw = MaxYaw;
    }

    ControlRotation.Yaw = NewYaw;
    GetController()->SetControlRotation(ControlRotation);
}

void AMsPlayer::ChooseLeft(float Value){ //��ư�� �����ϴ� ������ Ȱ��ȭ �ɶ� ��� ����Ű (A,D) �� ����Ű�� �����ְ� �Ѵ�
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

void  AMsPlayer::SetWidgetActive(bool Active) { //���� ��ư ���� ������ �������� Ȯ���Ѵ�
    IsWidgetActive = Active;
}

bool AMsPlayer::IsWidgetActivated() const { //���� Ȱ�������� ���� return
    return IsWidgetActive;
}

float AMsPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    Life -= ActualDamage;
    UE_LOG(LogMsPlayer, Warning, TEXT(" #### Current Life: %f"), Life);

    if (Life <= 0) {
        UE_LOG(LogMsPlayer, Warning, TEXT(" #### Player Dead $$$$$ "));
        CallDie(true); // �������Ʈ���� ���ǵ� Die �Լ� ȣ��
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

        UFunction* DieFunction = FindFunction(TEXT("Die")); // "Die"�� �������Ʈ���� ������ �Լ��� �̸�
        if (DieFunction) {
            ProcessEvent(DieFunction, nullptr);
        }
        return true;
    }
    return false;
}

void AMsPlayer::CallAttack() {
    //ī�޶� �̿��Ͽ� �þ߿� ���� ���� ��ġ ã��
    FVector Start = Camera->GetComponentLocation();
    FVector Forward = Camera->GetForwardVector();
    FVector End = Start + (Forward * 1000.0f);

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this); //�þ߿� �÷��̾� ������ ��� ��� �����ϱ�

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

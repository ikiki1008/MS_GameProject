#include "EnemyMiniBoss.h"
#include "MsMiniBossController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

DEFINE_LOG_CATEGORY(LogMiniBoss);

AEnemyMiniBoss::AEnemyMiniBoss(){
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyMiniBoss::BeginPlay(){
	Super::BeginPlay();
	
	Life = 1500.0f;
	IsNoticed = false;
	PlayerFound = false;
	AttackToPlayer = false;

	CheckPerception();
	GetWorld()->GetTimerManager().SetTimer(PerceptionTimerHandle, this, &AEnemyMiniBoss::CheckPerception, 2.0f, true);
}

void AEnemyMiniBoss::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}

void AEnemyMiniBoss::CheckPerception() {
	AMsMiniBossController* MiniBossController = Cast<AMsMiniBossController>(GetController());
	if (MiniBossController) {
		PlayerFound = MiniBossController->PlayerDetect;
		AttackToPlayer = MiniBossController->PlayerAttack;

		if (PlayerFound) {
			if (!IsNoticed) {
				NoticedMotion();
			}
			else {
				CallRun();
				if (AttackToPlayer) {
					CallMiniBossAttack();
				}
				else {
					CallRun();
				}
			}
		}
	}
	else {
		CallIdle();
	}
}

void AEnemyMiniBoss::NoticedMotion() {
	UFunction* StartFight = FindFunction(TEXT("RunToPlayer"));
	ProcessEvent(StartFight, nullptr);
	IsNoticed = true;
	UE_LOG(LogMiniBoss, Warning, TEXT(" ***** enemy noticed player! *****"));
}

void AEnemyMiniBoss::CallMiniBossAttack() {
	// life 값에 따른 공격 처리
	if (Life >= 600) {
		ProcessEvent(FindFunction(TEXT("FirstAttack")), nullptr);
	}
	else if (Life >= 400) {
		ProcessEvent(FindFunction(TEXT("SecondAttack")), nullptr);
	}
	else if (Life >= 200) {
		ProcessEvent(FindFunction(TEXT("ThirdAttack")), nullptr);
	}
	else {
		ProcessEvent(FindFunction(TEXT("FinalAttack")), nullptr);
	}
}

void AEnemyMiniBoss::CallRun() {
	UFunction* RunMotion = FindFunction(TEXT("CatchPlayer"));
	if (RunMotion) {
		ProcessEvent(RunMotion, nullptr);
		UE_LOG(LogMiniBoss, Warning, TEXT(" ***** start run! *****"));
	}
}

void AEnemyMiniBoss::CallIdle() {
	UFunction* IdleMotion = FindFunction(TEXT("AttackIdle"));
	if (IdleMotion) {
		ProcessEvent(IdleMotion, nullptr);
		UE_LOG(LogMiniBoss, Warning, TEXT(" ***** start idle *****"));
	}
}

void AEnemyMiniBoss::CallHit() {
	UFunction* HitMotion = FindFunction(TEXT("Damaged"));
	if (HitMotion) {
		ProcessEvent(HitMotion, nullptr);
		UE_LOG(LogMiniBoss, Warning, TEXT(" ***** hit by player... get damaged! *****"));
	}
}

bool AEnemyMiniBoss::CallDie() {
	UFunction* DeadMotion = FindFunction(TEXT("Death"));
	if (DeadMotion) {
		ProcessEvent(DeadMotion, nullptr);

		GetWorld()->GetTimerManager().ClearTimer(PerceptionTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(DieTimerHandle, this, &AEnemyMiniBoss::DestroyActor, 6.0f, false);

		return true;
	}
	return false;
}

float AEnemyMiniBoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UE_LOG(LogMiniBoss, Warning, TEXT(" ***** Hit!!! *****"));
	// Update health or state here
	Life -= ActualDamage;
	CallHit();
	if (Life <= 0) {
		UE_LOG(LogMiniBoss, Warning, TEXT(" ***** enemy dead *****"));
		CallDie();
	}

	return ActualDamage;
}

void AEnemyMiniBoss::DestroyActor() {
	K2_DestroyActor();
}

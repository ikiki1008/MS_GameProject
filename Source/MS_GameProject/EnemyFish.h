#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyFish.generated.h"

UCLASS()
class MS_GAMEPROJECT_API AEnemyFish : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyFish();
	
	UPROPERTY(BlueprintReadWrite, Category = "Fish Life")
	float Life;

	UPROPERTY(BlueprintReadWrite, Category = "Fish Speed")
	float Speed;

	// MsEnemyController의 상태를 참조하는 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "FishAnime")
	bool PlayerFound;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "FishAnime")
	bool AttackToPlayer;


	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	FTimerHandle PerceptionTimerHandle;
	FTimerHandle DieTimerHandle;

private:
	void UpdateSoundVolume();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	class UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	class USoundBase* Sound;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void CallFishAttack();

	UFUNCTION(BlueprintCallable, Category = "Idle")
	void CallFishIdle();

	UFUNCTION(BlueprintCallable, Category = "Run")
	void CallFishRun();

	UFUNCTION(BlueprintCallable, Category = "Die")
	void CallFishDie();

	// 1초마다 플레이어 탐지 상태를 체크하는 함수
	void CheckPerception();

	void DestroyActor(); //죽을 시 캐릭터가 사라지도록
};

//log 선언
DECLARE_LOG_CATEGORY_EXTERN(LogEnemyFish, Log, All);
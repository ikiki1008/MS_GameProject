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
	void RandomWalk(bool found);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	void UpdateSoundVolume();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	class UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	class USoundBase* Sound;

};

//log ¼±¾ð
DECLARE_LOG_CATEGORY_EXTERN(LogEnemyFish, Log, All);
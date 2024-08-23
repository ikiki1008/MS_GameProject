#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MsBlackSmith.generated.h"

UCLASS()
class MS_GAMEPROJECT_API AMsBlackSmith : public ACharacter
{
	GENERATED_BODY()

public:
	AMsBlackSmith();

protected:
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool IsMeetBlackSmith;
	void CreateWidget();

	//UFUNCTION(BlueprintCallable, Category = "Beg")
	//void CallBegging();

	//UFUNCTION(BlueprintCallable, Category = "Appreciate")
	//void CallAppreciate();

	//UFUNCTION(BlueprintCallable, Category = "Ignore")
	//void CallIgnore();

	//UFUNCTION(BlueprintCallable, Category = "Mission")
	//void CallMission();

	//UFUNCTION(BlueprintCallable, Category = "Wait")
	//void CallWait();

	//UFUNCTION(BlueprintCallable, Category = "MissonSuccess")
	//void CallSuccess();
};

DECLARE_LOG_CATEGORY_EXTERN(LogBlackSmith, Log, All);

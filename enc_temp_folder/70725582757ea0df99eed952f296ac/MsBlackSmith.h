#pragma once
#include "AccpOrIgnoreWidget.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h" 
#include "MsBlackSmith.generated.h"

UCLASS()
class MS_GAMEPROJECT_API AMsBlackSmith : public ACharacter
{
	GENERATED_BODY()

public:
	AMsBlackSmith();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, Category = "ChooseResult")
	FString getResult; // 버튼 클릭 시 결과 string 을 담을 버튼

	UAccpOrIgnoreWidget* AccpOrIgnoreWidget;
	bool WidgetOpened;

	void IsChooseWidgetActive(bool Active);
	bool SetWidgetActive() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackSmithText")
	FText BlackSmithText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackSmithText")
	bool IsWidgetOpen;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackSmithText")
	bool IsWidgetClosed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackSmithText")
	bool IsFinishedWidget;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	bool IsMeetBlackSmith;
	bool IsFinishedIntro;
	bool IsAskedForMoney;
	bool IsPrayed;
	bool IsAppreciated;
	bool IsGaveMission;

	FTimerHandle TimerHandle;

	APlayerController* PlayerController;

	void CreateWidgetBox();
	void	EnableFunctions();
	void MeetingFirst();
	bool Introduce();
	void AskingBigMoney();
	void Ignore();
	void Mission();

	UPROPERTY()
	UUserWidget* BlackSmithWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* CollisionComponent;
};

DECLARE_LOG_CATEGORY_EXTERN(LogBlackSmith, Log, All);
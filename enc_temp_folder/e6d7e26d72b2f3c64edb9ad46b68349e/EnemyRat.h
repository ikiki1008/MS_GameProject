#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyRat.generated.h"

UCLASS()
class MS_GAMEPROJECT_API AEnemyRat : public ACharacter
{
    GENERATED_BODY()

public:
    AEnemyRat();

    // MsEnemyController의 상태를 참조하는 변수
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "RatAnime")
    bool PlayerFound;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "RatAnime")
    bool AttackToPlayer;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Dead")
    bool IsRatDead;


protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    //enemy rat arrack motion
    UFUNCTION(BlueprintCallable, Category = "Attack")
    void CallRatAttack(bool Attack);

    //enemy rat die motion
    UFUNCTION(BlueprintCallable, Category = "Dead")
    void CallRatDie();

private:
    void UpdateSoundVolume();

    // 1초마다 플레이어 탐지 상태를 체크하는 함수
    void CheckPerception();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
    class UAudioComponent* AudioComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
    class USoundBase* Sound;

    // 타이머 핸들러
    FTimerHandle PerceptionTimerHandle;
};

// log 선언
DECLARE_LOG_CATEGORY_EXTERN(LogEnemyRat, Log, All);
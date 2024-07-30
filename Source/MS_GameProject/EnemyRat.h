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
    void RandomWalk(bool found);

protected:
    virtual void BeginPlay() override;
};

DECLARE_LOG_CATEGORY_EXTERN(LogEnemyRat, Log, All);
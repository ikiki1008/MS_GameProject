#include "EnemyRat.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

DEFINE_LOG_CATEGORY(LogEnemyRat);

AEnemyRat::AEnemyRat()
{

    PrimaryActorTick.bCanEverTick = true;
}

void AEnemyRat::BeginPlay()
{
    Super::BeginPlay();
    RandomWalk(false);
}

void AEnemyRat::RandomWalk(bool found)
{
    if (!GetController()) return;

    if (!found) {
        UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
        if (NavSystem){
            FVector RandomLocation;
            NavSystem->K2_GetRandomReachablePointInRadius(GetWorld(), GetActorLocation(), RandomLocation, 500.0f);
            UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), RandomLocation);
            UE_LOG(LogEnemyRat, Warning, TEXT(" ***** the mouse is randomly walking.... *****"));
        }
    }
    else {
        UE_LOG(LogEnemyRat, Warning, TEXT(" ***** the mouse is gonna go to the player.... *****"));
    }
}

//void AEnemyRat::MoveToPlayer()
//{
//    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0); //Player character == number zero
//    if (PlayerCharacter)
//    {
//        FVector PlayerLocation = PlayerCharacter->GetActorLocation();
//        UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), PlayerLocation);
//        UE_LOG(LogEnemyRat, Warning, TEXT(" ***** this mouse has found the Player *****"));
//    }
//}

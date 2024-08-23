#include "MsBlackSmith.h"
#include "MsPlayer.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogBlackSmith);

AMsBlackSmith::AMsBlackSmith()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AMsBlackSmith::BeginPlay()
{
	Super::BeginPlay();
    IsMeetBlackSmith = false;
}

void AMsBlackSmith::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 플레이어와의 거리를 구하는 코드
    AMsPlayer* Player = Cast<AMsPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));  // 첫 번째 플레이어의 폰 가져오기
    if (Player) {
        FVector BlackSmithLocation = GetActorLocation();  // 블랙스미스의 위치
        FVector PlayerLocation = Player->GetActorLocation();  // 플레이어의 위치

        float DistanceToPlayer = FVector::Dist(BlackSmithLocation, PlayerLocation);  // 거리 계산

        // 50cm(= 0.5m) 이내인지 확인
        if (DistanceToPlayer <= 200.0f && !IsMeetBlackSmith){
            UE_LOG(LogBlackSmith, Warning, TEXT("Player is within 200cm of the Blacksmith!"));
            CreateWidget();
        } 
    }
}

void AMsBlackSmith::CreateWidget() {
    IsMeetBlackSmith = true;
    UFunction* Widget = FindFunction(TEXT("Widget"));
    if (Widget) {
        UE_LOG(LogBlackSmith, Warning, TEXT("start creating widget"));
        ProcessEvent(Widget, nullptr);
    }
    //여기서 위젯 박스를 생성하고 플레이어에게 선택지가 주어진다
}

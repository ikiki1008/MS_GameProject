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

    // �÷��̾���� �Ÿ��� ���ϴ� �ڵ�
    AMsPlayer* Player = Cast<AMsPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));  // ù ��° �÷��̾��� �� ��������
    if (Player) {
        FVector BlackSmithLocation = GetActorLocation();  // �����̽��� ��ġ
        FVector PlayerLocation = Player->GetActorLocation();  // �÷��̾��� ��ġ

        float DistanceToPlayer = FVector::Dist(BlackSmithLocation, PlayerLocation);  // �Ÿ� ���

        // 50cm(= 0.5m) �̳����� Ȯ��
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
    //���⼭ ���� �ڽ��� �����ϰ� �÷��̾�� �������� �־�����
}

#include "MsBlackSmith.h"
#include "BlackSmithWidget.h"
#include "MsPlayer.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogBlackSmith);

AMsBlackSmith::AMsBlackSmith()
{
    PrimaryActorTick.bCanEverTick = true;

    // 오버랩 콜리전 컴포넌트 초기화
    CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
    CollisionComponent->InitBoxExtent(FVector(50.0f, 50.0f, 50.0f));
    CollisionComponent->SetCollisionProfileName(TEXT("Trigger"));
    CollisionComponent->SetupAttachment(RootComponent);

    // 오버랩 이벤트 바인딩
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMsBlackSmith::OnOverlapBegin);
}

void AMsBlackSmith::BeginPlay() {
    UE_LOG(LogBlackSmith, Warning, TEXT(" begin ...."));
    Super::BeginPlay();
    IsMeetBlackSmith = false;
    IsFinishedIntro = false;
    IsAskedForMoney = false;

    // 충돌 상자 설정 확인
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void AMsBlackSmith::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
    UE_LOG(LogBlackSmith, Warning, TEXT("OnOverlap begin ...."));

    if (OtherActor && (OtherActor != this)) {
        AMsPlayer* Player = Cast<AMsPlayer>(OtherActor);
        if (Player) {
            CreateWidgetBox();

            // 블랙스미스와 플레이어 간의 거리 계산
            //FVector BlackSmithLocation = GetActorLocation();
            //FVector PlayerLocation = Player->GetActorLocation();
            //float DistanceToPlayer = FVector::Dist(BlackSmithLocation, PlayerLocation);
        }
    }
}

void AMsBlackSmith::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
    if (IsWidgetOpen) {
        UE_LOG(LogBlackSmith, Warning, TEXT("true == widget opening"));
    }
    else {
        UE_LOG(LogBlackSmith, Warning, TEXT("false == widget closed"));
    }
}

void AMsBlackSmith::CreateWidgetBox() {
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController) {
        PlayerController->DisableInput(PlayerController);
    }

    if (!IsWidgetOpen) {
        if (!IsMeetBlackSmith) {
            MeetingFirst();
        }
        else if (IsMeetBlackSmith && !IsFinishedIntro) {
            Introduce();
        }
        else if (IsFinishedIntro && !IsAskedForMoney) {
            AskingBigMoney();
        }
    }
    GetWorldTimerManager().SetTimer(TimerHandle, this, &AMsBlackSmith::EnableCollision, 12.0f, false);
}

void AMsBlackSmith::MeetingFirst() {
    BlackSmithText = FText::FromString(TEXT("\nThere is a guy who looks weird but has a dirty sexy vibe..\nHe's trying to introduce himself.\nNow let's listen to what he says."));
    UFunction* Widget = FindFunction(TEXT("Widget"));

    if (Widget) {
        UE_LOG(LogBlackSmith, Warning, TEXT("start creating widget"));
        ProcessEvent(Widget, nullptr);
        //GetWorldTimerManager().SetTimer(TimerHandle, this, &AMsBlackSmith::CheckWidgetOpen, 1.0f, true);
        IsMeetBlackSmith = true;
    }
}

bool AMsBlackSmith::Introduce() {
    BlackSmithText = FText::FromString(TEXT("\nHe's introducing himself as a 'BETIFOL' Blacksmith\n...He seems little bit shy but he can help you to upgrade your weapon\nLet's ask him out for a help."));
    UFunction* Widget = FindFunction(TEXT("Introduce"));
    if (Widget) {
        UE_LOG(LogBlackSmith, Warning, TEXT("start creating introducing widget"));
        
        ProcessEvent(Widget, nullptr);
        IsFinishedIntro = true;
        return true;
        //GetWorldTimerManager().SetTimer(TimerHandle, this, &AMsBlackSmith::CheckWidgetOpen, 1.0f, true);
    }
    return false;
}

void AMsBlackSmith::AskingBigMoney() {

    BlackSmithText = FText::FromString(TEXT("\nHe's asking for 500 bucks at least\nBut you ain't got stacks...\nWhat would you do? You can just ignore and keep adventuring."));
    UFunction* Widget = FindFunction(TEXT("Money"));
    if (Widget) {
        UE_LOG(LogBlackSmith, Warning, TEXT("start creating Money widget"));

        ProcessEvent(Widget, nullptr);
        IsAskedForMoney = true;
        //GetWorldTimerManager().SetTimer(TimerHandle, this, &AMsBlackSmith::CheckWidgetOpen, 1.0f, true);
    }
}

void AMsBlackSmith::Ignore() {
    BlackSmithText = FText::FromString(TEXT("\nNow he's begging you so bad\nHe said he just got pranked you and actually he's so poor can't even go shopping...\nLet's listen what he suggest."));
    UFunction* Widget = FindFunction(TEXT("Beg"));
    if (Widget) {
        UE_LOG(LogBlackSmith, Warning, TEXT("start creating ignore widget"));
        ProcessEvent(Widget, nullptr);
        IsFinishedIntro = true;
    }
}

void AMsBlackSmith::EnableCollision() {
    if (PlayerController) {
        PlayerController->EnableInput(PlayerController);
    }
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}


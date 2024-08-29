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
    IsPrayed = false;
    IsAppreciated = false;
    IsGaveMission = false;

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
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); //몽타주와 위젯이 실행되는동안 다른 이벤트가 발생하지 않도록 disable 처리
    PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController) {
        PlayerController->DisableInput(PlayerController); //몽타주와 위젯이 실행되는동안 다른 이벤트가 발생하지 않도록 disable 처리
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
        //else if (IsAskedForMoney && !IsPrayed) {
        //    Begging();
        //}
    }
    GetWorldTimerManager().SetTimer(TimerHandle, this, &AMsBlackSmith::EnableFunctions, 12.0f, false); //12초 후 EnableFunctions 함수를 호출하여 콜리전과 이벤트클릭을 허용한다
}

void AMsBlackSmith::MeetingFirst() {
    BlackSmithText = FText::FromString(TEXT("\nThere is a guy who looks weird but has a dirty sexy vibe..\nHe's trying to introduce himself.\nNow let's listen to what he says."));
    UFunction* Widget = FindFunction(TEXT("Widget"));

    if (Widget) {
        UE_LOG(LogBlackSmith, Warning, TEXT("start creating widget"));
        ProcessEvent(Widget, nullptr);
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
    }
    return false;
}

void AMsBlackSmith::AskingBigMoney() {

    //BlackSmithText = FText::FromString(TEXT("\nHe's asking for 500 bucks at least\nBut you ain't got stacks...\nWhat would you do? You can just ignore and keep adventuring."));
    UFunction* Widget = FindFunction(TEXT("Money"));
    if (Widget) {
        UE_LOG(LogBlackSmith, Warning, TEXT("start creating Money widget"));

        ProcessEvent(Widget, nullptr);
        IsAskedForMoney = true;
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

void AMsBlackSmith::EnableFunctions() {
    if (PlayerController) {
        PlayerController->EnableInput(PlayerController);
    }
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}


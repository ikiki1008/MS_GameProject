﻿#include "MsBlackSmith.h"
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

    WidgetOpened = false;
    AccpOrIgnoreWidget = nullptr;

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

void AMsBlackSmith::IsChooseWidgetActive(bool Active) {
    WidgetOpened = Active;
}

bool AMsBlackSmith::SetWidgetActive() const {
    return WidgetOpened;
}

void AMsBlackSmith::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
    //UE_LOG(LogBlackSmith, Log, TEXT("IsWidgetOpen: %s"), IsWidgetOpen ? TEXT("True") : TEXT("False"));

    if (AccpOrIgnoreWidget && WidgetOpened) {
        FString Result = AccpOrIgnoreWidget->Result;
        UE_LOG(LogBlackSmith, Warning, TEXT("Current Result: %s"), *Result);  // 현재 Result 값을 로그로 출력

        if (Result.Equals("")) {
            UE_LOG(LogBlackSmith, Warning, TEXT("result empty...."));
        }
        else if (Result.Equals("Begging", ESearchCase::IgnoreCase)) {
            UE_LOG(LogBlackSmith, Warning, TEXT("Result is Begging"));
            Mission();
            AccpOrIgnoreWidget = nullptr;
            WidgetOpened = false;
        }
        else if (Result.Equals("Ignore", ESearchCase::IgnoreCase)) {
            UE_LOG(LogBlackSmith, Warning, TEXT("Result is Ignore"));
            Ignore();
            AccpOrIgnoreWidget = nullptr;
            WidgetOpened = false;
        }
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
        else if (IsPrayed && !IsAppreciated) {
            Appreciating();
        }
        else if (IsAppreciated && !IsGaveMission) {
            Mission();
        }
        else if (IsGaveMission) {
            Waiting();
        }
    }

    GetWorldTimerManager().SetTimer(TimerHandle, this, &AMsBlackSmith::EnableFunctions, 10.0f, false); //10초 후 EnableFunctions 함수를 호출하여 콜리전과 이벤트클릭을 허용한다
}

void AMsBlackSmith::MeetingFirst() {
    BlackSmithText = FText::FromString(TEXT("\n이상하지만 묘하게 섹시한 기운을 지닌 남자가 있다..\n그는 당신에게 무언가 말을 하려는 듯 하다.\n가까이 다가가 그의 얘기를 들어보자."));
    UFunction* Widget = FindFunction(TEXT("Widget"));

    if (Widget) {
        UE_LOG(LogBlackSmith, Warning, TEXT("start creating widget"));
        ProcessEvent(Widget, nullptr);
        IsMeetBlackSmith = true;
    }
}

bool AMsBlackSmith::Introduce() {
    BlackSmithText = FText::FromString(TEXT("\n그는 자신을 이곳 지하 던전의 대장장이라고 소개 한다..\n...묘하게 쑥쓰러워 하는 몸짓이 이상하지만 그는 당신의 무기를 업그레이드 시킬 수 있다.\n그에게 도움을 요청해 보자."));
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
    UFunction* Widget = FindFunction(TEXT("Money"));
    if (Widget) {
        UE_LOG(LogBlackSmith, Warning, TEXT("start creating Money widget"));

        ProcessEvent(Widget, nullptr);
        IsAskedForMoney = true;
    }
}

void AMsBlackSmith::Ignore() {
    BlackSmithText = FText::FromString(TEXT("\n갑자기 그가 최선을 다해 빌기 시작한다.\n그는 장난을 쳐본거라며 사실 자기도 돈이 없어 쇼핑도 못 가고 있다고 한다...\n그가 제안하는 부탁을 들어보자."));
    UFunction* Widget = FindFunction(TEXT("Beg"));
    if (Widget) {
        UE_LOG(LogBlackSmith, Warning, TEXT("start creating ignore widget"));
        ProcessEvent(Widget, nullptr);
        IsPrayed = true;
    }
}

void AMsBlackSmith::Appreciating() {
    BlackSmithText = FText::FromString(TEXT("\n그는 자신의 제안을 들어주는 것에 대해 감사를 표한다....\n...하지만 묘하게 사과가 불충분하다.\n그는 이 지하 던전에서 어떻게 돈을 얻을 수 있는 지 알려 주려 한다."));
    UFunction* Widget = FindFunction(TEXT("Appreciate"));
    if (Widget) {
        UE_LOG(LogBlackSmith, Warning, TEXT("start creating Appreciate widget"));
        ProcessEvent(Widget, nullptr);
        IsAppreciated = true;
    }
}

void AMsBlackSmith::Mission() {
    BlackSmithText = FText::FromString(TEXT("\n지하 던전 아주 깊숙한 곳에 대장장이를 귀찮게 하는 몬스터가 있다고 한다.\n몬스터는 호시탐탐 대장장이를 지하던전에서 내쫒으려고 기회를 엿본다고 한다..\n 대장장이는 해당 몬스터를 무찌르면 돈을 얻을 수 있다고 귀뜸해준다.\n이제 던전의 깊은 곳 으로 향하여 모험을 떠나 보자."));
    UFunction* Widget = FindFunction(TEXT("Mission"));
    if (Widget) {
        UE_LOG(LogBlackSmith, Warning, TEXT("start creating mission widget"));
        ProcessEvent(Widget, nullptr);
        IsGaveMission = true;
    }
}

void AMsBlackSmith::Waiting() {
    BlackSmithText = FText::FromString(TEXT("\n그가 잘 다녀라오라고 마중을 해준다.... 하지만 기분이 미묘하다."));
    UFunction* Widget = FindFunction(TEXT("Wait"));
    if (Widget) {
        UE_LOG(LogBlackSmith, Warning, TEXT("start creating mission widget"));
        ProcessEvent(Widget, nullptr);
    }
}

void AMsBlackSmith::EnableFunctions() {
    if (PlayerController) {
        PlayerController->EnableInput(PlayerController);
    }
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}
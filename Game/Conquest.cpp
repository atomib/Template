#include "FPSTemplate.h"
#include "Conquest.h"
#include "Game/TicketGameState.h"
#include "World/ConquestFlag.h"
#include "Game/TicketGameState.h"
#include "BaseGameInstance.h"

AConquest::AConquest(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameStateClass = ATicketGameState::StaticClass();

	static ConstructorHelpers::FObjectFinder<UObject> NewScoreWidgetClass(TEXT("WidgetBlueprint'/Game/FPSTemplate/HUD/Widgets/GameState/WBP_TDMScore.WBP_TDMScore_C'"));
	ScoreWidgetClass = (UClass*)NewScoreWidgetClass.Object;
}

void AConquest::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimerHandle_TickFlags;
	GetWorldTimerManager().SetTimer(TimerHandle_TickFlags, this, &AConquest::TickFlags, 5.f, true);
}

void AConquest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//
}

void AConquest::InitGameState()
{
	Super::InitGameState();

	ATicketGameState * TicketGameState = Cast<ATicketGameState>(GameState);
	UBaseGameInstance * GameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
	if (TicketGameState && GameInstance)
	{
		TicketGameState->SetStartTickets(GameInstance->NumTickets);
	}
}

void AConquest::OnPlayerDied(ABasePlayerController * PC)
{
	Super::OnPlayerDied(PC);

	ATicketGameState * TicketGameState = Cast<ATicketGameState>(GameState);
	if (TicketGameState)
	{
		if (PC->GetTeam() == ETeamEnum::TeamB)
		{
			if (TicketGameState->AddTeamBTickets(-1))
			{
				EndMatch(ETeamEnum::TeamA);
			}
		}
		else
		{
			if (TicketGameState->AddTeamATickets(-1))
			{
				EndMatch(ETeamEnum::TeamB);
			}
		}
	}
}

void AConquest::TickFlags()
{
	if (IsMatchInProgress())
	{
		ATicketGameState * TicketGameState = Cast<ATicketGameState>(GameState);
		if (!TicketGameState) return;

		for (TActorIterator<AConquestFlag> FlagItr(GetWorld()); FlagItr; ++FlagItr)
		{
			const ETeamEnum ControllingTeam = FlagItr->GetControllingTeam();
			switch (ControllingTeam)
			{
			case (ETeamEnum::TeamA) :
				if (TicketGameState->AddTeamBTickets(-1))
					EndMatch(ETeamEnum::TeamA);
				break;
			case (ETeamEnum::TeamB) :
				if (TicketGameState->AddTeamATickets(-1))
					EndMatch(ETeamEnum::TeamB);
				break;
			}
		}
	}
}
#include "FPSTemplate.h"
#include "TeamDeathmatch.h"
#include "Game/TicketGameState.h"
#include "BaseGameInstance.h"

ATeamDeathmatch::ATeamDeathmatch(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameStateClass = ATicketGameState::StaticClass();

	static ConstructorHelpers::FObjectFinder<UObject> NewScoreWidgetClass(TEXT("WidgetBlueprint'/Game/FPSTemplate/HUD/Widgets/GameState/WBP_TDMScore.WBP_TDMScore_C'"));
	ScoreWidgetClass = (UClass*)NewScoreWidgetClass.Object;
}

void ATeamDeathmatch::InitGameState()
{
	Super::InitGameState();

	ATicketGameState * TicketGameState = Cast<ATicketGameState>(GameState);
	UBaseGameInstance * GameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
	if (TicketGameState && GameInstance)
	{
		// Number of kills needed to win the game
		TicketGameState->SetMaxTickets(GameInstance->NumTickets);
	}
}

void ATeamDeathmatch::OnPlayerDied(ABasePlayerController * PC)
{
	Super::OnPlayerDied(PC);

	ATicketGameState * TicketGameState = Cast<ATicketGameState>(GameState);
	if (TicketGameState)
	{
		// Check if a team has enough kills to win the match
		if (PC->GetTeam() == ETeamEnum::TeamB)
		{
			if (TicketGameState->AddTeamATickets(1))
			{
				EndMatch(ETeamEnum::TeamA);
			}
		}
		else
		{
			if (TicketGameState->AddTeamBTickets(1))
			{
				EndMatch(ETeamEnum::TeamB);
			}
		}
	}
}
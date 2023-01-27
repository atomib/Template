#include "FPSTemplate.h"
#include "FreeForAll.h"
#include "Game/FreeForAllGameState.h"
#include "BaseGameInstance.h"

AFreeForAll::AFreeForAll(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameStateClass = AFreeForAllGameState::StaticClass();
	TeamMode = ETeamModeEnum::FreeForAll;

	static ConstructorHelpers::FObjectFinder<UObject> NewScoreWidgetClass(TEXT("WidgetBlueprint'/Game/FPSTemplate/HUD/Widgets/GameState/WBP_FFAScore.WBP_FFAScore_C'"));
	ScoreWidgetClass = (UClass*)NewScoreWidgetClass.Object;
}

void AFreeForAll::InitGameState()
{
	Super::InitGameState();

	AFreeForAllGameState * FFAGameState = Cast<AFreeForAllGameState>(GameState);
	UBaseGameInstance * GameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
	if (FFAGameState && GameInstance)
	{
		FFAGameState->SetMaxKills(GameInstance->NumTickets);
	}
}

void AFreeForAll::OnPlayerKilledEnemy(const FDamageEvent & DamageEvent, ABasePlayerController * PC, ABasePlayerController * OtherPC)
{
	Super::OnPlayerKilledEnemy(DamageEvent, PC, OtherPC);

	AFreeForAllGameState * GS = Cast<AFreeForAllGameState>(GameState);
	if (GS)
	{
		ABasePlayerState * PS = Cast<ABasePlayerState>(PC->PlayerState);
		if (PS && PS->GetKills() >= GS->GetMaxKills())
		{
			// Player wins when he gets enough kills
			EndMatch(PS);
		}
	}
}
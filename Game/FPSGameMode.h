#pragma once

#include "Game/LoadoutBuilderComponent.h"
#include "Soldier/Soldier.h"
#include "Player/BasePlayerController.h"
#include "GameFramework/GameMode.h"
#include "FPSGameMode.generated.h"

UENUM(BlueprintType)
enum class ETeamModeEnum : uint8
{
	MultipleTeams,
	OneTeam,
	FreeForAll,
};

UCLASS()
class FPSTEMPLATE_API AFPSGameMode : public AGameMode
{
	GENERATED_BODY()

	// Loadout builder
	UPROPERTY()
		ULoadoutBuilderComponent * LoadoutBuilderComponent;

	TSubclassOf<ASoldier> SoldierClass;

public:
	// Widget class used for game mode specific score display
	UPROPERTY(BlueprintReadOnly, Category = GameMode)
		TSubclassOf<UUserWidget> ScoreWidgetClass;

	// Whether players can cause damage to their teammates
	UPROPERTY(BlueprintReadOnly, Category = GameMode)
		bool bFriendlyFire;

	// Whether players may spawn with their custom loadout, if not DefaultLoadout will be used
	UPROPERTY(BlueprintReadOnly, Category = GameMode)
		bool bUseCustomLoadout;

	// Loadout to use for every player if custom loadouts are not allowed
	UPROPERTY(BlueprintReadOnly, Category = GameMode)
		FLoadoutStruct DefaultLoadout;

	// Set whether player can select team, all players play against each other or all players play in one team
	UPROPERTY(BlueprintReadOnly, Category = GameMode)
		ETeamModeEnum TeamMode;
	
	// Constructor
	AFPSGameMode(const FObjectInitializer & ObjectInitializer);

	virtual void InitGameState() override;

	virtual void PostLogin(APlayerController * NewPlayer) override;

	virtual void Logout(AController * Exiting) override;

	virtual void Reset() override;

	virtual AActor * FindPlayerStart(AController * Player);

	//virtual AActor * ChoosePlayerStart_Implementation(AController * Player) override;

	// Will move a player to the correct state after login
	virtual void MovePlayerToEntryState(ABasePlayerController * PC);

	// Called when match time expired
	virtual void OnTimeExpired();

	// Ends this match, no team wins
	virtual void EndMatch() override;

	// End the match with specified winner team
	void EndMatch(ETeamEnum WinnerTeam);

	// End match with specified player as winner
	void EndMatch(ABasePlayerState * WinnerPS);

	// Move all player controllers to NewState
	void MoveAllPlayersToState(EPlayerStateEnum NewState);

	// Calcuate damage amount for player, by default it only disable friendly fire if bFriendlyFire == false
	virtual void CalcDamage(float & OutDamageAmount, ABasePlayerController * PC, ABasePlayerController * OtherPC);

	// Called when a soldier dies from any damage
	void OnSoldierDied(ASoldier * Soldier, const FDamageEvent & DamageEvent, ABasePlayerController * EventInstigator, AActor * DamageCauser);

	// Overrideable event for when any player died
	virtual void OnPlayerDied(ABasePlayerController * PC);
	
protected:
	// Overrideable event for when a player killed an enemy player
	virtual void OnPlayerKilledEnemy(const FDamageEvent & DamageEvent, ABasePlayerController * PC, ABasePlayerController * OtherPC);

	// Overrideable event for when a player killed a friendly player
	virtual void OnPlayerKilledTeammate(const FDamageEvent & DamageEvent, ABasePlayerController * PC, ABasePlayerController * OtherPC);

public:
	int32 GetNumPlayersInTeam(ETeamEnum Team) const;

	// Player wants to join a team
	void PlayerJoinTeam(ABasePlayerController * PC, ETeamEnum InTeam);

	// Automatically join team, based on player count
	void PlayerAutoJoinTeam(ABasePlayerController * PC);

protected:
	// Overrideable, returns whether specified player may join a team
	virtual bool CanPlayerJoinTeam(ABasePlayerController * PC, ETeamEnum InTeam) const;

public:
	// Player wants to spawn
	void PlayerSpawn(ABasePlayerController * PC);
	
	// Overrideable, return whether player may spawn
	virtual bool CanPlayerSpawn(ABasePlayerController * PC) const;

	// Spawn a new item for the player if possible
	void PlayerBuyItem(ASoldier * Soldier, TSubclassOf<AItem> ItemClass);

	bool CanPlayerBuyItem(ASoldier * Soldier, TSubclassOf<AItem> ItemClass) const;

	// Return the loadout to use for player
	virtual const FLoadoutStruct & GetPlayerLoadout(ABasePlayerController * PC) const;

	// Evaluate loadout and set it on player controller
	void PlayerSetLoadout(ABasePlayerController * PC, const FLoadoutStruct & Loadout, uint8 NewLoadoutSlot);

	// Kick player
	UFUNCTION(BlueprintCallable, Category = GameMode)
		virtual bool KickPlayer(ABasePlayerController * KickedPlayer, const FText & KickReason);

	// Travel to new map
	UFUNCTION(BlueprintCallable, Category = GameMode)
		virtual void ServerTravel(const FString & MapName);

	// Broadcast chat message to all clients
	void BroadcastChatMessage(ABasePlayerController * FromPC, const FString & Message, bool bTeamOnly);

protected:
	// Whether one player is allowed to receive the chat message from another player
	virtual bool CanPlayerReceiveChatMessage(ABasePlayerController * FromPC, ABasePlayerController * ToPC, bool bTeamOnly);

};

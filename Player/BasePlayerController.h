#pragma once

#include "Soldier/Soldier.h"
#include "Game/LoadoutBuilderComponent.h"
#include "Damage/BaseDamageType.h"
#include "HUD/GameOverlayWidget.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

UENUM()
enum class EPlayerStateEnum : uint8
{
	None,
	TeamSelection,
	Spawn,
	Playing,
	Death,
	Spectating,
	MatchEnd,
};

UENUM(BlueprintType)
enum class EMessageTypeEnum : uint8
{
	None,
	Team,
	Global,
};

UCLASS()
class FPSTEMPLATE_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

	// Current state of this player controller, replicated
	UPROPERTY(ReplicatedUsing = OnRep_State)
		EPlayerStateEnum State;

	UFUNCTION()
		void OnRep_State();

	FTimerHandle TimerHandle_ChangeState;

	// Loadout to equip on next spawn
	FLoadoutStruct Loadout;

	// Slot of the loadout, used to get soldier mesh
	uint8 LoadoutSlot;

	// Whether loadout was changed and has to be updated on server
	bool bLoadoutChanged;

	// Whether customization menu is open
	bool bCustomizing;

	// Template class for GameOverlayWidget
	TSubclassOf<UGameOverlayWidget> GameOverlayWidgetClass;

	// Widget that exists in every state and shows chat, kill feed, points etc.
	UPROPERTY()
		UGameOverlayWidget * GameOverlayWidget;

	// Whether menu is currently open
	bool bMenuOpen;

	// Whether mini map if enlarged
	bool bMiniMapEnlarged;

public:
	// Type of message which the player is currently typing
	UPROPERTY(BlueprintReadOnly, Category = PlayerController)
		EMessageTypeEnum CurrentMessageType;

	// Player state that last killed us, used within DeathHUD widget
	UPROPERTY(BlueprintReadOnly, Category = PlayerController)
		ABasePlayerState * LastKillerPlayer;

	ABasePlayerController();

	void BeginPlay() override;

	void SetupInputComponent() override;

	// Overriden because original implementation did things we don't want
	void Reset() override;

	// Same here
	void ClientReset_Implementation() override;

	// Reset scores on player state
	void ResetScore();

	void UnFreeze() override;

	void OnPossess(APawn * InPawn) override;

	// Overriden to avoid original implementation
	void FailedToSpawnPawn() override;

	// Set team on player state
	void SetTeam(ETeamEnum InTeam);

	// Add score on PlayerState
	void AddScore(int32 Val);

	// Add kill on PlayerState
	void AddKill();

	// Add death on PlayerState
	void AddDeath();

	// Called for all PlayerControllers when a player got killed, executes on client
	UFUNCTION(Client, Reliable, WithValidation)
		void ClientOnPlayerDied(TSubclassOf<UBaseDamageType> DamageTypeClass, ABasePlayerState * SoldierPlayerState, ABasePlayerState * KillerPlayerState);

	// Called when assisted a kill
	UFUNCTION(Client, Reliable, WithValidation)
		void ClientOnAssistedKill(int32 DamageAmount);

	// Called when player damages a soldier, will notify HUD to draw hitmarker if an enemy was hit
	UFUNCTION(Client, Reliable, WithValidation)
		void ClientOnDamagedSoldier(ASoldier * Soldier, float DamageAmount);

	// Called when owning soldier was damaged, will draw blood on screen
	void OnTookDamage(float DamageAmount, AActor * Causer);

	// Called when a bullet hits a networked actor on client
	void OnClientHitActor(AActor * DamagedActor, const FPointDamageEvent & DamageEvent, AActor * DamageCauser);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerOnClientHitActor(AActor * DamagedActor, const FPointDamageEvent & DamageEvent, AActor * DamageCauser);

	UFUNCTION(BlueprintPure, Category = PlayerController)
		bool GetIsMiniMapEnlarged() const { return bMiniMapEnlarged; };

	UFUNCTION(Client, Reliable)
		void ClientOnRoundOver(ETeamEnum WinnerTeam);

	// Team
	UFUNCTION(BlueprintPure, Category = PlayerController)
		ETeamEnum GetTeam() const;

	UFUNCTION(BlueprintPure, Category = PlayerController)
		bool IsEnemyFor(ABasePlayerController * PC) const;

	// Buy
	void OpenBuyMenu();

	UFUNCTION(BlueprintCallable, Category = Player)
		void CloseBuyMenu();

	UFUNCTION(BlueprintCallable, Category = Player)
		void BuyItem(TSubclassOf<AItem> ItemClass);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerBuyItem(TSubclassOf<AItem> ItemClass);

	// Return state
	EPlayerStateEnum GetState() const { return State; };

	// Change states
	void EnterTeamSelectionState();

	void EnterSpawnState();

	void EnterPlayingState();

	void EnterDeathState();

	void EnterSpectatingState();

	void EnterMatchEndState();

private:
	// State change notification events, executed on owning client
	void OnEnterTeamSelectionState();

	void OnEnterSpawnState();

	void OnEnterPlayingState();

	void OnEnterDeathState();

	void OnEnterSpectatingState();

	void OnEnterMatchEndState();

	void OnEnterState(TSubclassOf<AHUD> HUDClass, bool bMouseEnabled);

protected:
	// Set view target to camera actor selected in world settings
	void SetViewTargetToMenuCamera();

public:
	UFUNCTION(BlueprintCallable, Category = PlayerController)
		void SwitchToNextCamera();

	void FadeIn();

	// Input
	// Kill soldier
	UFUNCTION(BlueprintCallable, Category = PlayerController)
		void Suicide();

	UFUNCTION(BlueprintCallable, Category = PlayerController)
		void OpenCustomization();

	UFUNCTION(BlueprintCallable, Category = PlayerController)
		void CloseCustomization();

	void OpenScoreboard();

	void CloseScoreboard();

	void EnlargeMiniMap();

	void MinimizeMiniMap();

	// Open in game menu
	void OpenMenu();

	// Close in game menu
	UFUNCTION(BlueprintCallable, Category = PlayerController)
		void CloseMenu();

	// Try to join team
	UFUNCTION(BlueprintCallable, Category = PlayerController)
		void JoinTeam(ETeamEnum InTeam);

private:
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerJoinTeam(ETeamEnum InTeam);

public:
	// Join the team with less players
	UFUNCTION(BlueprintCallable, Category = PlayerController)
		void AutoJoinTeam();

private:
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAutoJoinTeam();

public:
	// Try to spawn
	UFUNCTION(BlueprintCallable, Category = PlayerController)
		void Spawn();

private:
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSpawn();

	// Begin writing a chat message
	void BeginGlobalChatMessage();

	void BeginTeamChatMessage();

	void BeginChatMessage(EMessageTypeEnum MessageType);

public:
	// Cancel chat message
	UFUNCTION(BlueprintCallable, Category = PlayerController)
		void CancelChatMessage();

	// Chat
	UFUNCTION(BlueprintCallable, Category = PlayerController)
		void SendChatMessage(const FString & Message, bool bTeamOnly);

private:
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSendChatMessage(const FString & Message, bool bTeamOnly);

public:
	UFUNCTION(Client, Reliable, WithValidation)
		void ClientReceiveChatMessage(const FString & Message, ABasePlayerState * FromPlayer);

	// Function callable from customization HUD
	UFUNCTION(BlueprintCallable, Category = PlayerController)
		void SetLoadout(const FLoadoutStruct & NewLoadout, uint8 NewLoadoutSlot);

	// Called from game mode when custom loadout was evaluated
	void ApplyLoadout(const FLoadoutStruct & NewLoadout, uint8 NewLoadoutSlot);

	const FLoadoutStruct & GetLoadout() const { return Loadout; };

	uint8 GetLoadoutSlot() const { return LoadoutSlot; }

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetLoadout(const FLoadoutStruct & NewLoadout, uint8 NewLoadoutSlot);
	
};

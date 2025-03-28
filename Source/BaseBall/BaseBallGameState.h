#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BaseBallLib.h"
#include "Net/UnrealNetwork.h"
#include "BaseBallGameState.generated.h"

UCLASS()
class BASEBALL_API ABaseBallGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ABaseBallGameState();

	virtual void BeginPlay() override;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "GameState")
	TArray<int32> PlayerNumbers;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "GameState")
	TArray<int32> PlayerAttempts;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "GameState")
	int32 CurrentPlayerIndex;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "GameState")
	EGameResult GameResult;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "GameState")
	TArray<int32> CurrentStrikes;

	UFUNCTION(NetMultiCast, Reliable)
	void Multi_UpdateMessage(const FString& Message);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseBallGameState.h"
#include "BaseBallLib.h"
#include "BaseBallGameMode.generated.h"


UCLASS()
class BASEBALL_API ABaseBallGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABaseBallGameMode();

	virtual void PostLogin(APlayerController* Players) override;

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void ProcessChatMessage(const FString& Message, APlayerController* Sender);
	
};

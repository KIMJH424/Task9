#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BaseBallPlayerController.generated.h"

UCLASS()
class BASEBALL_API ABaseBallPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Chat")
	void SendMessage(const FString& Message);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendMessage(const FString& Message);

	UFUNCTION(Client, Reliable)
	void Client_CheckValidError(const FString& ErrorMessage);

	UPROPERTY(BlueprintReadWrite, Category = "Player")
	int32 PlayerIndex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Chat")
	TSubclassOf<class UUserWidget> ChatWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "Chat")
	class UUserWidget* ChatWidget;
};

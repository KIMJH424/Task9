#include "BaseBallPlayerController.h"
#include "BaseBallGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/UserWidget.h"
#include "ChatWidget.h"
#include "Engine/Engine.h"



void ABaseBallPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		PlayerIndex = 0;
	}
	else
	{
		PlayerIndex = 1;
	}

	if (IsLocalController() && ChatWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: ChatWidget created successfully."), *GetName());

		ChatWidget = CreateWidget<UUserWidget>(this, ChatWidgetClass);
		if (ChatWidget)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: ChatWidget created successfully."), *GetName());
			ChatWidget->AddToViewport();

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: ChatWidget creation failed!"), *GetName());
		}
	}
}

void ABaseBallPlayerController::SendMessage(const FString& Message)
{
	if (HasAuthority())
	{
		ABaseBallGameMode* GameMode = Cast<ABaseBallGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->ProcessChatMessage(Message, this);
		}
	}
	else
	{
		Server_SendMessage(Message);
	}
}

void ABaseBallPlayerController::Server_SendMessage_Implementation(const FString& Message)
{
	ABaseBallGameMode* GameMode = Cast<ABaseBallGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->ProcessChatMessage(Message, this);
	}
}

bool ABaseBallPlayerController::Server_SendMessage_Validate(const FString& Message)
{
	if (!Message.StartsWith(TEXT("/")))
	{
		return false;
	}

	FString CommandPart = Message.RightChop(1).TrimStartAndEnd();
	if (CommandPart.Len() != 3)
	{
		return false;
	}
	return true;
}

void ABaseBallPlayerController::Client_CheckValidError_Implementation(const FString& ErrorMessage)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, ErrorMessage);
	}
}



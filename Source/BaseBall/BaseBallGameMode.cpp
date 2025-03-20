#include "BaseBallGameMode.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "BaseBallPlayerController.h"
#include "ChatWidget.h"

ABaseBallGameMode::ABaseBallGameMode()
{
	// GameStateClass = ABaseBallGameState::StaticClass();
}

void ABaseBallGameMode::PostLogin(APlayerController* Players)
{
	Super::PostLogin(Players);

	ABaseBallPlayerController* PC = Cast<ABaseBallPlayerController>(Players);
	if (PC)
	{
		PC->PlayerIndex = GetNumPlayers() - 1;

		UE_LOG(LogTemp, Warning, TEXT("Assigned PlayerIndex %d to %s"), PC->PlayerIndex, *PC->GetName());
	}
}

void ABaseBallGameMode::ProcessChatMessage(const FString& Message, APlayerController* Sender)
{
	UE_LOG(LogTemp, Log, TEXT("Chat Message from %s: %s"), *Sender->GetName(), *Message);

	ABaseBallPlayerController* SenderPC = Cast<ABaseBallPlayerController>(Sender);
	if (!SenderPC)
	{
		UE_LOG(LogTemp, Warning, TEXT("Sender is not a valid"));
		return;
	}
	ABaseBallGameState* GS = GetGameState<ABaseBallGameState>();

	if (GS)
	{
		UE_LOG(LogTemp, Log, TEXT("Sender Index: %d, Current Turn: %d"), SenderPC->PlayerIndex, GS->CurrentPlayerIndex);

		if (SenderPC->PlayerIndex != GS->CurrentPlayerIndex)
		{
			UE_LOG(LogTemp, Warning, TEXT("Not Your Turn."));
			SenderPC->Client_CheckValidError(TEXT("순서가 아닙니다"));
			return;
		}
	}

	if (!Message.StartsWith(TEXT("/")))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid chat format"));
		return;
	}

	FString GuessStr = Message.RightChop(1).TrimStartAndEnd();
	TArray<int32> PlayerGuess;
	for (int32 i = 0; i < GuessStr.Len(); i++)
	{
		FString DigitStr = GuessStr.Mid(i, 1);
		int32 Digit = FCString::Atoi(*DigitStr);
		PlayerGuess.Add(Digit);
	}

	TArray<int32> TargetNumbers;
	if (GS && GS->PlayerNumbers.Num() >= 3)
	{
		TargetNumbers = GS->PlayerNumbers;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Target numbers not properly set in GameState."));
		return;
	}

	int32 Strike = 0;
	int32 Ball = 0;

	int32 HostAttempts = GS ? GS->PlayerAttempts[0] : 3;
	int32 GuestAttempts = GS ? GS->PlayerAttempts[1] : 3;
	EGameResult RoundResult = EGameResult::Playing;

	EPlayerTurn CurrentTurn = (GS->CurrentPlayerIndex == 0) ? EPlayerTurn::Host : EPlayerTurn::Guest;

	UBaseBallLib::Referee(TargetNumbers, PlayerGuess, Strike, Ball, HostAttempts, GuestAttempts, CurrentTurn, RoundResult);

	if (GS)
	{
		GS->GameResult = RoundResult;

		if (CurrentTurn == EPlayerTurn::Guest)
		{
			GS->PlayerAttempts[1] = GuestAttempts;
			GS->CurrentStrikes[1] += Strike;
		}
		else
		{
			GS->PlayerAttempts[0] = HostAttempts;
			GS->CurrentStrikes[0] += Strike;
		}

		GS->CurrentPlayerIndex = (GS->CurrentPlayerIndex + 1) % 2;
	}

	FString ResultStr = FString::Printf(TEXT("Strike: %d, Ball: %d"), Strike, Ball);
	UE_LOG(LogTemp, Log, TEXT("%s"), *ResultStr);

	if (GS && ((GS->PlayerAttempts[0] <= 0 && GS->PlayerAttempts[1] <= 0) || (RoundResult != EGameResult::Playing)))
	{
		if (RoundResult == EGameResult::Playing && GS->PlayerAttempts[0] <= 0 && GS->PlayerAttempts[1] <= 0)
		{
			if (GS->CurrentStrikes[0] > GS->CurrentStrikes[1])
			{
				GS->GameResult = EGameResult::HostWin;
			}
			else if (GS->CurrentStrikes[1] > GS->CurrentStrikes[0])
			{
				GS->GameResult = EGameResult::GuestWin;
			}
			else
			{
				GS->GameResult = EGameResult::Draw;
			}
		}

		FString FinalResult;
		switch (GS->GameResult)
		{
		case EGameResult::HostWin:
			FinalResult = TEXT("Host wins!");
			break;
		case EGameResult::GuestWin:
			FinalResult = TEXT("Guest wins!");
			break;
		case EGameResult::Draw:
			FinalResult = TEXT("Draw!");
			break;
		default:
			FinalResult = TEXT("Game Over!");
			break;
		}
		UE_LOG(LogTemp, Log, TEXT("Game Over: %s"), *FinalResult);
		Multi_UpdateMessage(FinalResult);
	}
	else
	{
		Multi_UpdateMessage(ResultStr);
	}
}


void ABaseBallGameMode::Multi_UpdateMessage_Implementation(const FString& Message)
{
	UE_LOG(LogTemp, Log, TEXT("Multi_UpdateMessage called with: %s"), *Message);

	for (FConstPlayerControllerIterator I = GetWorld()->GetPlayerControllerIterator(); I; ++I)
	{
		ABaseBallPlayerController* PC = Cast<ABaseBallPlayerController>(I->Get());
		if (PC && PC->ChatWidget)
		{
			UE_LOG(LogTemp, Warning, TEXT("Updating chat for player: %s"), *PC->GetName());
			UChatWidget* ChatW = Cast<UChatWidget>(PC->ChatWidget);
			if (ChatW)
			{
				ChatW->AddChatMessage(Message);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("ChatWidget is null for player: %s"), *PC->GetName());
			}
		}
	}
}
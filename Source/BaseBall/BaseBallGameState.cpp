#include "BaseBallGameState.h"
#include "BaseBallPlayerController.h"
#include "ChatWidget.h"

ABaseBallGameState::ABaseBallGameState()
{
	CurrentPlayerIndex = 0;
	GameResult = EGameResult::Playing;

	PlayerAttempts.SetNum(2);
	PlayerAttempts[0] = 3;
	PlayerAttempts[1] = 3;

	PlayerNumbers.Empty();

	CurrentStrikes.SetNum(2);
	CurrentStrikes[0] = 0;
	CurrentStrikes[1] = 0;
}

void ABaseBallGameState::BeginPlay()
{
	Super::BeginPlay();

	UBaseBallLib::RandomNumbers(PlayerNumbers);

	FString TargetStr;
	for (int32 Num : PlayerNumbers)
	{
		TargetStr += FString::Printf(TEXT("%d"), Num);
	}
	UE_LOG(LogTemp, Log, TEXT("Target Num : %s"), *TargetStr);
}

void ABaseBallGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseBallGameState, PlayerNumbers);
	DOREPLIFETIME(ABaseBallGameState, PlayerAttempts);
	DOREPLIFETIME(ABaseBallGameState, CurrentPlayerIndex);
	DOREPLIFETIME(ABaseBallGameState, GameResult);
	DOREPLIFETIME(ABaseBallGameState, CurrentStrikes);
}

void ABaseBallGameState::Multi_UpdateMessage_Implementation(const FString& Message)
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

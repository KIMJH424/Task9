#include "BaseBallLib.h"
#include "Math/UnrealMathUtility.h"

void UBaseBallLib::RandomNumbers(TArray<int32>& OutNumbers)
{
	OutNumbers.Empty();
	TArray<int32> AvailableNumbers = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	for (int32 i = 0; i < 3; i++)
	{
		int32 Index = FMath::RandRange(0, AvailableNumbers.Num() - 1);
		OutNumbers.Add(AvailableNumbers[Index]);
		AvailableNumbers.RemoveAt(Index);
	}
}

void UBaseBallLib::Referee(const TArray<int32>& ServerNum,
	const TArray<int32>& PlayerNum,
	int32& Strike, int32& Ball,
	int32& HostAttempts, int32& GuestAttempts,
	EPlayerTurn CurrentTurn, EGameResult& OutResult)
{
	Strike = 0;
	Ball = 0;

	if (PlayerNum.Num() < 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerNum array size is less than 3. Received %d digits."), PlayerNum.Num());
		OutResult = EGameResult::Playing;
		return;
	}

	for (int32 i = 0; i < 3; i++)
	{ 
		if (PlayerNum[i] == ServerNum[i])
		{
			Strike++;
		}
		else if (ServerNum.Contains(PlayerNum[i]))
		{
			Ball++;
		}
	}

	switch (CurrentTurn)
	{
		case EPlayerTurn::Guest:
			GuestAttempts--;
			break;

		case EPlayerTurn::Host:
			HostAttempts--;
			break;
	}

	if (Strike == 3)
	{
		OutResult = (CurrentTurn == EPlayerTurn::Guest) ? EGameResult::GuestWin : EGameResult::HostWin;
	}
	else if (HostAttempts == 0 && GuestAttempts == 0)
	{
		OutResult = EGameResult::Draw;
	}
	else
	{
		OutResult = EGameResult::Playing;
	}
}

bool UBaseBallLib::IsGameOver(int32 HostAttempts, int32 GuestAttempts, EGameResult Result)
{
	if (Result != EGameResult::Playing)
	{
		return true;
	}
	if (HostAttempts <= 0 && GuestAttempts <= 0)
	{
		return true;
	}
	return false;
}


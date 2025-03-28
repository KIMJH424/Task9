#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BaseBallLib.generated.h"

UENUM(BlueprintType)
enum class EGameResult : uint8
{
	Playing UMETA(DisplayName = "Playing"),
	HostWin UMETA(DisplayName = "Host Win"),
	GuestWin UMETA(DisplayName = "Guest Win"),
	Draw UMETA(DisplayName = "Draw")
};

UENUM(BlueprintType)
enum class EPlayerTurn : uint8
{
	Host UMETA(DisplayName = "Host"),
	Guest UMETA(DisplayName= "Guest")
};

UCLASS()
class BASEBALL_API UBaseBallLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Number BaseBall")
	static void RandomNumbers(TArray<int32>& OutNumbers);

	UFUNCTION(BlueprintCallable, Category = "Number BaseBall")
	static void Referee(const TArray<int32>& ServerNum, 
		const TArray<int32>& PlayerNum, 
		int32& Strike, int32& Ball, 
		int32& HostAttempts, int32& GuestAttempts, 
		EPlayerTurn CurrentTurn, EGameResult& OutResult);

	UFUNCTION(BlueprintCallable, Category = "Number BaseBall")
	static bool IsGameOver(int32 HostAttempts, int32 GuestAttempts, EGameResult Result);
};

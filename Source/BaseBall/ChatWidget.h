#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "ChatWidget.generated.h"

UCLASS()
class BASEBALL_API UChatWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "Chat")
	UEditableTextBox* ChatInputBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Chat")
	FString CommittedText;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "Chat")
	class UVerticalBox* ChatLogBox;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION(BlueprintCallable, Category = "Chat")
	FString GetInputText() const;

	UFUNCTION(BlueprintCallable, Category = "Chat")
	void AddChatMessage(const FString& Message);
};

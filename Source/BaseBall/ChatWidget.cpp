#include "ChatWidget.h"
#include "BaseBallPlayerController.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"

void UChatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ChatInputBox)
	{
		ChatInputBox->OnTextCommitted.AddDynamic(this, &UChatWidget::OnTextCommitted);
	}

	if (ChatLogBox)
	{
		UE_LOG(LogTemp, Log, TEXT("%s: ChatLogBox is successfully bound."), *GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: ChatLogBox is NOT bound!"), *GetName());
	}
}

void UChatWidget::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		CommittedText = Text.ToString();

		if (ChatInputBox)
		{
			ChatInputBox->SetText(FText::GetEmpty());
		}

		if (APlayerController* PC = GetOwningPlayer())
		{
			if (ABaseBallPlayerController* ChatPC = Cast<ABaseBallPlayerController>(PC))
			{
				ChatPC->SendMessage(CommittedText);
			}
		}
	}
}

FString UChatWidget::GetInputText() const
{
	return CommittedText;
}

void UChatWidget::AddChatMessage(const FString& Message)
{
	if (ChatLogBox)
	{
		UTextBlock* NewText = NewObject<UTextBlock>(this);
		if (NewText)
		{
			NewText->SetText(FText::FromString(Message));
			ChatLogBox->AddChild(NewText);
		}
	}
}

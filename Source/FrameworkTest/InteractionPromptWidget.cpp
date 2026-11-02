// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionPromptWidget.h"
#include "Components/TextBlock.h"

void UInteractionPromptWidget::ShowPrompt(const FText& KeyName, const FText& ActionText)
{
	const FText NewText = FText::FormatNamed(PromptFormat, TEXT("Key"), KeyName, TEXT("Action"), ActionText);

	// Called every frame while aiming: only touch the UI when something actually changes
	if (!PromptText->GetText().EqualTo(NewText))
	{
		PromptText->SetText(NewText);
	}
	if (GetVisibility() != ESlateVisibility::HitTestInvisible)
	{
		SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UInteractionPromptWidget::HidePrompt()
{
	if (GetVisibility() != ESlateVisibility::Collapsed)
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionPromptWidget.generated.h"

class UTextBlock;

/**
 * "[E] Open" prompt. Logic in C++, layout in a child Widget Blueprint.
 * The Widget Blueprint MUST contain a TextBlock named exactly "PromptText".
 */
UCLASS(Abstract)
class FRAMEWORKTEST_API UInteractionPromptWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void ShowPrompt(const FText& KeyName, const FText& ActionText);
	void HidePrompt();

protected:
	// Linked by name to the TextBlock of the Widget Blueprint
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> PromptText;

	// {Key} and {Action} are replaced at runtime
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Prompt")
	FText PromptFormat = NSLOCTEXT("Interaction", "PromptFormat", "[{Key}] {Action}");
};

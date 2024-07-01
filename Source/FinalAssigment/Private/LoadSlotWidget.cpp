// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadSlotWidget.h"

#include "ArchVizPlayerController.h"
#include "SlotNamesSaveGame.h"
#include "Kismet/GameplayStatics.h"

void ULoadSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SlotListComboBox)
	{
		PopulateSaveSlots();
	}

	if (LoadSlotButton)
	{
		LoadSlotButton->OnClicked.AddDynamic(this, &ULoadSlotWidget::OnLoadSlotButtonClicked);
	}
}

void ULoadSlotWidget::PopulateSaveSlots()
{
	USlotNamesSaveGame* SlotNamesSaveGame = Cast<USlotNamesSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("SlotNamesSaveSlot"), 0));
	if (SlotNamesSaveGame)
	{
		for (const FString& SlotName : SlotNamesSaveGame->SlotNames)
		{
			SlotListComboBox->AddOption(SlotName);
		}
	}
}

void ULoadSlotWidget::OnLoadSlotButtonClicked()
{
	FString SelectedSlot = SlotListComboBox->GetSelectedOption();

	if (!SelectedSlot.IsEmpty())
	{
		if (AArchVizPlayerController* PlayerController = Cast<AArchVizPlayerController>(GetOwningPlayer()))
		{
			PlayerController->LoadGame(SelectedSlot);
			RemoveFromParent();  // Close the widget after loading
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No slot selected. Please select a slot to load."));
	}
}
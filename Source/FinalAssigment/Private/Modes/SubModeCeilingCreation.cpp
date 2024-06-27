// Fill out your copyright notice in the Description page of Project Settings.


#include "Modes/SubModeCeilingCreation.h"

#include "EnhancedInputSubsystems.h"

USubModeCeilingCreation::USubModeCeilingCreation()
{
}

void USubModeCeilingCreation::Setup()
{
	SelectedActor = nullptr;
}

void USubModeCeilingCreation::Cleanup()
{

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Cleanup Happen"));
}

void USubModeCeilingCreation::SetupInputMapping()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Ceiling Setup Input Mapping"));
	
}

void USubModeCeilingCreation::EnterSubMode(UWallConstructionWidget* WallConstructionWidget)
{
	if (PlayerController) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(InputMappingContext, 0);

			Setup();
		}
		if (WallConstructionWidget)
		{
			WallConstructionWidget->LengthInput->GetParent()->SetVisibility(ESlateVisibility::Visible);
			WallConstructionWidget->WidthInput->GetParent()->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void USubModeCeilingCreation::ExitSubMode(UWallConstructionWidget* WallConstructionWidget)
{
	if (PlayerController) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->RemoveMappingContext(InputMappingContext);

			//Cleanup();
		}
		if (WallConstructionWidget)
		{
			WallConstructionWidget->LengthInput->GetParent()->SetVisibility(ESlateVisibility::Hidden);
			WallConstructionWidget->WidthInput->GetParent()->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

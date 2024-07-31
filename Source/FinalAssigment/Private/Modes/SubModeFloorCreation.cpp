// Fill out your copyright notice in the Description page of Project Settings.


#include "Modes/SubModeFloorCreation.h"
#include "ArchVizPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FloorActor.h"

USubModeFloorCreation::USubModeFloorCreation()
{
}

void USubModeFloorCreation::Setup()
{
	SelectedActor = nullptr;
}

void USubModeFloorCreation::Cleanup()
{
	if (auto* ActorToDestroy = Cast<AFloorActor>(SelectedActor))
	{
		if (ActorToDestroy->WallState == EBuildingSubModeState::Moving)
		{
			SelectedActor = nullptr; // Reset the selected actor
			ActorToDestroy->Destroy(); // Destroy the actor
		}
		ActorToDestroy->UnhighlightDeselectedActor();

	}
	else
	{
		SelectedActor = nullptr;
	}

}

void USubModeFloorCreation::ToggleMovementSelectedActor()
{
	if (SelectedActor)
	{
		bIsNewWall = false;
		Cast<AFloorActor>(SelectedActor)->WallState = EBuildingSubModeState::Moving;
		Cast<AArchVizPlayerController>(PlayerController)->BroadcastToast("Actor movement started");
	}
}
void USubModeFloorCreation::DeleteSelectedActor()
{
	if (SelectedActor)
	{
		// Disable custom depth rendering for the selected actor
		Cast<AFloorActor>(SelectedActor)->UnhighlightDeselectedActor();

		// Collect all attached actors
		TArray<AActor*> AttachedActors;
		SelectedActor->GetAttachedActors(AttachedActors);

		// Destroy all attached actors
		for (AActor* AttachedActor : AttachedActors)
		{
			if (AttachedActor)
			{
				AttachedActor->Destroy();
			}
		}

		// Destroy the selected actor
		SelectedActor->Destroy();
		SelectedActor = nullptr;
		bIsNewWall = false;
		bFirstClickDone = false;
		Cast<AArchVizPlayerController>(PlayerController)->BroadcastToast("Selected actor and attached actors destroyed");

	}
}


void USubModeFloorCreation::SetupInputMapping()
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
	InputMappingContext = NewObject<UInputMappingContext>(this);

	OnWallLeftClick = NewObject<UInputAction>(this);
	OnWallLeftClick->ValueType = EInputActionValueType::Boolean;

	OnWallRightClick = NewObject<UInputAction>(this);
	OnWallRightClick->ValueType = EInputActionValueType::Boolean;

	OnWallDelete = NewObject<UInputAction>(this);
	OnWallDelete->ValueType = EInputActionValueType::Boolean;

	OnShowInstruction = NewObject<UInputAction>(this);
	OnShowInstruction->ValueType = EInputActionValueType::Boolean;

	StartMovement = NewObject<UInputAction>(this);
	StartMovement->ValueType = EInputActionValueType::Boolean;

	OnActorRotate = NewObject<UInputAction>(this);
	OnActorRotate->ValueType = EInputActionValueType::Boolean;


	if (InputMappingContext)
	{

		InputMappingContext->MapKey(OnWallLeftClick, EKeys::LeftMouseButton);
		InputMappingContext->MapKey(OnWallRightClick, EKeys::RightMouseButton);
		InputMappingContext->MapKey(OnActorRotate, EKeys::R);
		InputMappingContext->MapKey(OnWallDelete, EKeys::Delete);
		InputMappingContext->MapKey(OnShowInstruction, EKeys::I);
		InputMappingContext->MapKey(StartMovement, EKeys::M);

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("InputMappingContext Not Populated"));
	}

	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(OnWallLeftClick, ETriggerEvent::Started, this, &USubModeFloorCreation::WallLeftClickProcess);
		EnhancedInputComponent->BindAction(OnWallRightClick, ETriggerEvent::Started, this, &USubModeFloorCreation::WallRightClickProcess);
		EnhancedInputComponent->BindAction(OnWallDelete, ETriggerEvent::Started, this, &USubModeFloorCreation::DeleteSelectedActor);
		EnhancedInputComponent->BindAction(OnShowInstruction, ETriggerEvent::Started, this, &USubModeFloorCreation::ShowInstructionTab);
		EnhancedInputComponent->BindAction(OnShowInstruction, ETriggerEvent::Completed, this, &USubModeFloorCreation::HideInstructionTab);
		EnhancedInputComponent->BindAction(StartMovement, ETriggerEvent::Completed, this, &USubModeFloorCreation::ToggleMovementSelectedActor);
		EnhancedInputComponent->BindAction(OnActorRotate, ETriggerEvent::Started, this, &USubModeFloorCreation::RotateSelectedActor);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("EnhancedInputComponent Not Populated"));
	}
}

void USubModeFloorCreation::RotateSelectedActor()
{
	if (SelectedActor)
	{
		SelectedActor->AddActorLocalRotation(FRotator(0.f, 90.f, 0.f));
		bIsNewWall = false;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Actor rotated by 80 degrees"));
	}
}
void USubModeFloorCreation::EnterSubMode(UWallConstructionWidget* Widget)
{
	if (PlayerController) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(InputMappingContext, 0);

			Setup();
		}
		if(Widget)
		{
			CurrentWidget = Widget;
			Widget->LengthInput->GetParent()->SetVisibility(ESlateVisibility::Visible);
			Widget->WidthInput->GetParent()->SetVisibility(ESlateVisibility::Visible);
			Widget->Floor->SetBackgroundColor(FColor::Black);
			
		}
	}
}

void USubModeFloorCreation::ExitSubMode(UWallConstructionWidget* Widget)
{
	if (PlayerController) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->RemoveMappingContext(InputMappingContext);
			Cleanup();
		}
		if (Widget)
		{
			Widget->Floor->SetBackgroundColor(FColor::White);
			Widget->LengthInput->GetParent()->SetVisibility(ESlateVisibility::Hidden);
			Widget->WidthInput->GetParent()->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void USubModeFloorCreation::ShowInstructionTab()
{

	if (CurrentWidget)
	{
		CurrentWidget->InstructionBtn->SetVisibility(ESlateVisibility::Hidden);
		CurrentWidget->Allkeys->SetVisibility(ESlateVisibility::Visible);
	}
}

void USubModeFloorCreation::HideInstructionTab()
{
	if (CurrentWidget)
	{
		CurrentWidget->InstructionBtn->SetVisibility(ESlateVisibility::Visible);
		CurrentWidget->Allkeys->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USubModeFloorCreation::WallLeftClickProcess()
{
	FHitResult HitResult;
	PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		const FVector ClickLocation = HitResult.Location;

		if (SelectedActor && Cast<AFloorActor>(SelectedActor)->WallState == EBuildingSubModeState::Constructing && bFirstClickDone)
		{
			bFirstClickDone = false;
			Cast<AFloorActor>(SelectedActor)->WallState = EBuildingSubModeState::Placed;
			Cast<AFloorActor>(SelectedActor)->SetEndLocation(ClickLocation);
			Cast<AArchVizPlayerController>(PlayerController)->BroadcastToast("Actor placed");
		}
		else if(SelectedActor && Cast<AFloorActor>(SelectedActor)->WallState == EBuildingSubModeState::Moving && !bFirstClickDone)
		{
			



			if (bIsNewWall)
			{
				bFirstClickDone = true;
				Cast<AFloorActor>(SelectedActor)->SetStartLocation(ClickLocation);
				Cast<AFloorActor>(SelectedActor)->SetActorLocation(Utility::SnapToGrid(ClickLocation, FVector(10, 10, 0)));
				Cast<AFloorActor>(SelectedActor)->WallState = EBuildingSubModeState::Constructing;
			}
			else
			{
				bFirstClickDone = false;
				Cast<AFloorActor>(SelectedActor)->WallState = EBuildingSubModeState::Placed;
				Cast<AFloorActor>(SelectedActor)->SetStartLocation(Utility::SnapToGrid(ClickLocation, FVector(20)));
			}

			/*FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;*/


			if (AFloorActor* SpawnedActor = Cast<AFloorActor>(HitResult.GetActor()))
			{
				if (SelectedActor)
				{
					Cast<AFloorActor>(SelectedActor)->UnhighlightDeselectedActor();
				}
				
				SelectedActor = SpawnedActor;

				if (SelectedActor)
				{
					Cast<AFloorActor>(SelectedActor)->HighlightSelectedActor();
					
				}
				
				Cast<AArchVizPlayerController>(PlayerController)->BroadcastToast("Actor selected for moving");
			}
			
		}
		
	}
}


void USubModeFloorCreation::WallRightClickProcess()
{
	FHitResult HitResult;
	PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		if (SelectedActor && Cast<AFloorActor>(SelectedActor)->WallState == EBuildingSubModeState::Moving)
		{
			
			SelectedActor->Destroy();
			Cast<AArchVizPlayerController>(PlayerController)->BroadcastToast("Actor destroyed");
			SelectedActor = nullptr;
		}
		
		if (SelectedActor)
		{
			Cast<AFloorActor>(SelectedActor)->UnhighlightDeselectedActor();
		}
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (AFloorActor* SpawnedActor = GetWorld()->SpawnActor<AFloorActor>(AFloorActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams))
		{
			bIsNewWall = true;
			SelectedActor = SpawnedActor;
			if (DynamicMaterial)
			{
				Cast<AFloorActor>(SelectedActor)->SetMaterial(DynamicMaterial);
			}
			if (SelectedActor)
			{
				Cast<AFloorActor>(SelectedActor)->HighlightSelectedActor();
			}
			Cast<AArchVizPlayerController>(PlayerController)->BroadcastToast("Actor spawned and selected for moving");

		}
	}
	
	
}
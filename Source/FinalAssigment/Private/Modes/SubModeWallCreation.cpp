// Fill out your copyright notice in the Description page of Project Settings.


#include "Modes/SubModeWallCreation.h"

#include "ArchVizPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InteriorDesignActor.h"
#include "Components/TextBlock.h"

USubModeWallCreation::USubModeWallCreation()
{
	
}

void USubModeWallCreation::Cleanup()
{
	if (auto* ActorToDestroy = Cast<AWallActor>(SelectedActor))
	{
		if (ActorToDestroy->WallState == EBuildingSubModeState::Moving)
		{
			SelectedActor = nullptr;
			ActorToDestroy->Destroy(); 
		}
		// ActorToDestroy->GetProceduralMeshComponent()->SetRenderCustomDepth(false);
	}
	else
	{
		SelectedActor = nullptr;
	}
}

void USubModeWallCreation::ShowInstructionTab()
{
	if (CurrentWidget)
	{
		CurrentWidget->InstructionBtn->SetVisibility(ESlateVisibility::Hidden);
		CurrentWidget->Allkeys->SetVisibility(ESlateVisibility::Visible);
	}
}

void USubModeWallCreation::HideInstructionTab()
{
	if (CurrentWidget)
	{
		CurrentWidget->InstructionBtn->SetVisibility(ESlateVisibility::Visible);
		CurrentWidget->Allkeys->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USubModeWallCreation::ToggleMovementSelectedActor()
{
	if (SelectedActor)
	{
		bIsNewWall = false;
		Cast<AWallActor>(SelectedActor)->WallState = EBuildingSubModeState::Moving;
		Cast<AArchVizPlayerController>(PlayerController)->BroadcastToast("Actor movement started");

	}
}

void USubModeWallCreation::SetupInputMapping()
{
	 UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
	InputMappingContext = NewObject<UInputMappingContext>(this);

	OnWallLeftClick = NewObject<UInputAction>(this);
	OnWallLeftClick->ValueType = EInputActionValueType::Boolean;

	OnWallRightClick = NewObject<UInputAction>(this);
	OnWallRightClick->ValueType = EInputActionValueType::Boolean;

	OnWallRotate = NewObject<UInputAction>(this);
	OnWallRotate->ValueType = EInputActionValueType::Boolean;

	OnWallDelete = NewObject<UInputAction>(this);
	OnWallDelete->ValueType = EInputActionValueType::Boolean;

	OnShowInstruction = NewObject<UInputAction>(this);
	OnShowInstruction->ValueType = EInputActionValueType::Boolean;


	StartMovement = NewObject<UInputAction>(this);
	StartMovement->ValueType = EInputActionValueType::Boolean;

	if (InputMappingContext)
	{

		InputMappingContext->MapKey(OnWallLeftClick, EKeys::LeftMouseButton);
		InputMappingContext->MapKey(OnWallRightClick, EKeys::RightMouseButton);
		InputMappingContext->MapKey(OnWallRotate, EKeys::R);
		InputMappingContext->MapKey(OnWallDelete, EKeys::Delete);
		InputMappingContext->MapKey(OnShowInstruction, EKeys::I);
		InputMappingContext->MapKey(StartMovement, EKeys::M);
		//InputMappingContext->MapKey(OnDeSelectWall, EKeys::Tab);

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("InputMappingContext Not Populated"));
	}

	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(OnWallLeftClick, ETriggerEvent::Started, this, &USubModeWallCreation::WallLeftClickProcess);
		EnhancedInputComponent->BindAction(OnWallRightClick, ETriggerEvent::Started, this, &USubModeWallCreation::WallRightClickProcess);
		EnhancedInputComponent->BindAction(OnWallRotate, ETriggerEvent::Started, this, &USubModeWallCreation::RotateSelectedActor);
		EnhancedInputComponent->BindAction(OnWallDelete, ETriggerEvent::Started, this, &USubModeWallCreation::DeleteSelectedWallActor);
		EnhancedInputComponent->BindAction(OnShowInstruction, ETriggerEvent::Started, this, &USubModeWallCreation::ShowInstructionTab);
		EnhancedInputComponent->BindAction(OnShowInstruction, ETriggerEvent::Completed, this, &USubModeWallCreation::HideInstructionTab);
		EnhancedInputComponent->BindAction(StartMovement, ETriggerEvent::Completed, this, &USubModeWallCreation::ToggleMovementSelectedActor);
		//EnhancedInputComponent->BindAction(OnDeSelectWall, ETriggerEvent::Started, this, &UBuildingCreationMode::DeSelectedSelectedActor);

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("EnhancedInputComponent Not Populated"));
	}
}

void USubModeWallCreation::EnterSubMode(UWallConstructionWidget* Widget)
{
	if (PlayerController) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(InputMappingContext, 0);

			Setup();
		}
		if (Widget)
		{

			CurrentWidget = Widget;
			Widget->LengthInput->GetParent()->SetVisibility(ESlateVisibility::Visible);
			Widget->WidthInput->GetParent()->SetVisibility(ESlateVisibility::Hidden);
			Widget->Wall->SetBackgroundColor(FColor::Black);
			Widget->ToggleDoorButton->SetVisibility(ESlateVisibility::Visible);

			if (SelectedActor)
			{
				//Widget->LengthInput->SetValue(SelectedActor->GetLength());
			}
		}
	}
}

void USubModeWallCreation::ExitSubMode(UWallConstructionWidget* Widget)
{
	if (PlayerController) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->RemoveMappingContext(InputMappingContext);

		}
		if (Widget)
		{
			Widget->LengthInput->GetParent()->SetVisibility(ESlateVisibility::Hidden);
			Widget->WidthInput->GetParent()->SetVisibility(ESlateVisibility::Hidden);
			Widget->Wall->SetBackgroundColor(FColor::White);
			Widget->ToggleDoorButton->SetVisibility(ESlateVisibility::Hidden);
			
		}
		Cleanup();
	}
}

void USubModeWallCreation::Setup()
{
	SelectedActor = nullptr;
}

void USubModeWallCreation::WallLeftClickProcess()
{
	FHitResult HitResult;
	PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		const FVector ClickLocation = HitResult.Location;


		if (SelectedActor && Cast<AWallActor>(SelectedActor)->WallState == EBuildingSubModeState::Constructing && !bIsDoorAdding && bFirstClickDone)
		{
			bFirstClickDone = false;
			Cast<AWallActor>(SelectedActor)->WallState = EBuildingSubModeState::Placed;
			Cast<AWallActor>(SelectedActor)->SetWallEndLocation(ClickLocation);

			
		}
		else if(SelectedActor && Cast<AWallActor>(SelectedActor)->WallState == EBuildingSubModeState::Moving)
		{
			if (bIsNewWall)
			{
				bFirstClickDone = true;
				Cast<AWallActor>(SelectedActor)->SetWallStartLocation(ClickLocation);
				Cast<AWallActor>(SelectedActor)->WallState = EBuildingSubModeState::Constructing;
				Cast<AWallActor>(SelectedActor)->CreateWallSegment();
			}
			else
			{
				bFirstClickDone = false;
				Cast<AWallActor>(SelectedActor)->WallState = EBuildingSubModeState::Placed;
				Cast<AWallActor>(SelectedActor)->SetWallEndLocation(ClickLocation);
			}
			
		}
		else if (AActor* HitActor = HitResult.GetActor())
		{
			if (ACubeActor* CubeActor = Cast<ACubeActor>(HitActor))
			{
				
				if (AWallActor* WallActor = Cast<AWallActor>(CubeActor->GetParentActor()))
				{
					FVector LocalClickLocation = WallActor->GetTransform().InverseTransformPosition(HitResult.Location);

					if (SelectedActor)
					{
						//SelectedActor->GetProceduralMeshComponent()->SetRenderCustomDepth(false);
					}

					SelectedActor = WallActor;

					if (bIsDoorAdding && WallActor->WallState == EBuildingSubModeState::Placed)
					{
						WallActor->SetIsDoorAdded(true);
						WallActor->AddDoor(LocalClickLocation);
						Cast<AArchVizPlayerController>(PlayerController)->BroadcastToast("Door added to the wall");
					}
					else if (bIsDoorAdding && WallActor->WallState == EBuildingSubModeState::Moving)
					{
						WallActor->WallState = EBuildingSubModeState::Placed;
						Cast<AArchVizPlayerController>(PlayerController)->BroadcastToast("Wall placed while adding door");
					}
					else
					{
						// Select the wall actor for movement
						//WallActor->WallState = EBuildingSubModeState::Moving;
					}
				}
				
			}
		}

		// Highlight the selected actor
		if (SelectedActor)
		{
			// SelectedActor->GetProceduralMeshComponent()->SetRenderCustomDepth(true);
			Cast<AWallActor>(SelectedActor)->HighlightSelectedActor();
			if (SelectedActor)
			{
				//CurrentWidget->LengthInput->SetValue(SelectedActor->GetLength());
			}
			
		}
	}
}




void USubModeWallCreation::WallRightClickProcess()
{
	// Deselect the current actor if it is in moving state
	if (SelectedActor && Cast<AWallActor>(SelectedActor)->WallState == EBuildingSubModeState::Moving)
	{
		SelectedActor->Destroy();
		SelectedActor = nullptr;
		Cast<AArchVizPlayerController>(PlayerController)->BroadcastToast("Actor destroyed");
	}
	
	if (SelectedActor)
	{
		//SelectedActor->GetProceduralMeshComponent()->SetRenderCustomDepth(false);
	}
	// If no actor is being moved, create a new wall
	FHitResult HitResult;
	PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		FVector spawnLocation = HitResult.Location;
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (AWallActor* SpwanedActor = GetWorld()->SpawnActor<AWallActor>(AWallActor::StaticClass(), spawnLocation, FRotator::ZeroRotator, SpawnParams))
		{
			SpwanedActor->SetIsDoorAdded(bIsDoorAdding);
			SelectedActor = SpwanedActor;
			bIsNewWall = true;

			if (SelectedActor)
			{
				//SelectedActor->GetProceduralMeshComponent()->SetRenderCustomDepth(true);
				//SelectedActor->GetProceduralMeshComponent()->CustomDepthStencilValue = 2.0;
			}

			if (DynamicMaterial)
			{
				Cast<AWallActor>(SelectedActor)->SetMaterial(DynamicMaterial);
			}
			Cast<AArchVizPlayerController>(PlayerController)->BroadcastToast("Actor spawned and selected for moving");
		}
	}
	
}

void USubModeWallCreation::RotateSelectedActor()
{
	if (SelectedActor)
	{
		SelectedActor->AddActorLocalRotation(FRotator(0.f, 90.f, 0.f));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Actor rotated by 80 degrees"));
	}
}

void USubModeWallCreation::DeleteSelectedWallActor()
{

	
	if (SelectedActor)
	{
		//SelectedActor->GetProceduralMeshComponent()->SetRenderCustomDepth(false);

		TArray<AActor*> AttachedActors;
		SelectedActor->GetAttachedActors(AttachedActors);
		for (AActor* AttachedActor : AttachedActors)
		{
			if (AttachedActor)
			{
				AttachedActor->Destroy();
			}
		}

		SelectedActor->Destroy();
		SelectedActor = nullptr;
		Cast<AArchVizPlayerController>(PlayerController)->BroadcastToast("Selected actor and its attached actors destroyed");


	}
}

void USubModeWallCreation::SetIsDoorAddingFlag(bool flag)
{
	bIsDoorAdding = flag;
}
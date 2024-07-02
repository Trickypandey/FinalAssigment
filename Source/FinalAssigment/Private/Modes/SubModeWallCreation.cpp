// Fill out your copyright notice in the Description page of Project Settings.


#include "Modes/SubModeWallCreation.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
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
		ActorToDestroy->GetProceduralMeshComponent()->SetRenderCustomDepth(false);
	}
	else
	{
		SelectedActor = nullptr;
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

	if (InputMappingContext)
	{

		InputMappingContext->MapKey(OnWallLeftClick, EKeys::LeftMouseButton);
		InputMappingContext->MapKey(OnWallRightClick, EKeys::RightMouseButton);
		InputMappingContext->MapKey(OnWallRotate, EKeys::R);
		InputMappingContext->MapKey(OnWallDelete, EKeys::Delete);
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
			//Widget->WidthInput->SetVisibility(ESlateVisibility::Visible);
			/*Widget->Wall->SetBackgroundColor(FLinearColor(184.0f / 255.0f, 184.0f / 255.0f, 184.0f / 255.0f, 255.0f / 255.0f));*/

			if (SelectedActor)
			{
				Widget->LengthInput->SetValue(SelectedActor->GetLength());
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
			/*Widget->Wall->SetBackgroundColor(FLinearColor(126.0f / 255.0f, 126.0f / 255.0f, 126.0f / 255.0f, 1.0f));*/

			//Widget->WidthInput->SetVisibility(ESlateVisibility::Visible);
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


		if (SelectedActor && Cast<AWallActor>(SelectedActor)->WallState == EBuildingSubModeState::Moving && !bIsDoorAdding)
		{
			Cast<AWallActor>(SelectedActor)->WallState = EBuildingSubModeState::Placed;
			/*FVector SnappedLocation = Utility::SnapToGrid(ClickLocation, FVector(10));*/
			//SelectedActor->SetActorLocation(SnappedLocation);
		}
		// Check if the clicked actor is a wall
		else if (AWallActor* WallActor = Cast<AWallActor>(HitResult.GetActor()))
		{
			FVector LocalClickLocation = WallActor->GetTransform().InverseTransformPosition(ClickLocation);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("World Clicked Location: X=%f, Y=%f, Z=%f"), ClickLocation.X, ClickLocation.Y, ClickLocation.Z));

			// Deselect the previously selected actor if there is one
			if (SelectedActor)
			{
				SelectedActor->GetProceduralMeshComponent()->SetRenderCustomDepth(false);
			}

			SelectedActor = WallActor;

			if (bIsDoorAdding && WallActor->WallState == EBuildingSubModeState::Placed)
			{
				// If adding a door is requested, update the wall actor
				WallActor->SetIsDoorAdded(true);
				WallActor->SetDoorLocation(LocalClickLocation.X);
				WallActor->CreateMesh();
			}
			else if(bIsDoorAdding && WallActor->WallState == EBuildingSubModeState::Moving)
			{
				WallActor->WallState = EBuildingSubModeState::Placed;
			}
			else
			{
				// Select the wall actor for movement
				WallActor->WallState = EBuildingSubModeState::Moving;
			}
		}

		// Highlight the selected actor
		if (SelectedActor)
		{
			SelectedActor->GetProceduralMeshComponent()->SetRenderCustomDepth(true);
			if (SelectedActor)
			{
				CurrentWidget->LengthInput->SetValue(SelectedActor->GetLength());
			}
			if (DynamicMaterial)
			{
				SelectedActor->GetProceduralMeshComponent()->SetMaterial(0, DynamicMaterial);
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
	}
	
		if (SelectedActor)
		{
			SelectedActor->GetProceduralMeshComponent()->SetRenderCustomDepth(false);
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
				SpwanedActor->WallState = EBuildingSubModeState::Moving;
				SelectedActor = SpwanedActor;

				if (SelectedActor)
				{
					SelectedActor->GetProceduralMeshComponent()->SetRenderCustomDepth(true);
					SelectedActor->GetProceduralMeshComponent()->CustomDepthStencilValue = 2.0;
				}

				if (DynamicMaterial)
				{
					SelectedActor->GetProceduralMeshComponent()->SetMaterial(0, DynamicMaterial);
				}
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
		SelectedActor->GetProceduralMeshComponent()->SetRenderCustomDepth(false);
		SelectedActor->Destroy();
		SelectedActor = nullptr;

	}
}

void USubModeWallCreation::SetIsDoorAddingFlag(bool flag)
{
	bIsDoorAdding = flag;
}


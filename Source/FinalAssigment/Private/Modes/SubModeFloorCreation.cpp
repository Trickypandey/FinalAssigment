// Fill out your copyright notice in the Description page of Project Settings.


#include "Modes/SubModeFloorCreation.h"

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
			ActorToDestroy->WallState = EBuildingSubModeState::Placed;
			SelectedActor = nullptr; // Reset the selected actor
			ActorToDestroy->Destroy(); // Destroy the actor
		}
	}
	else
	{
		SelectedActor = nullptr;
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


	if (InputMappingContext)
	{

		InputMappingContext->MapKey(OnWallLeftClick, EKeys::LeftMouseButton);
		InputMappingContext->MapKey(OnWallRightClick, EKeys::RightMouseButton);

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("InputMappingContext Not Populated"));
	}

	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(OnWallLeftClick, ETriggerEvent::Started, this, &USubModeFloorCreation::WallLeftClickProcess);
		EnhancedInputComponent->BindAction(OnWallRightClick, ETriggerEvent::Started, this, &USubModeFloorCreation::WallRightClickProcess);
		
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("EnhancedInputComponent Not Populated"));
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
			Widget->LengthInput->GetParent()->SetVisibility(ESlateVisibility::Visible);
			Widget->WidthInput->GetParent()->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void USubModeFloorCreation::ExitSubMode(UWallConstructionWidget* Widget)
{
	if (PlayerController) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->RemoveMappingContext(InputMappingContext);

			//Cleanup();
		}
		if (Widget)
		{
			Widget->LengthInput->GetParent()->SetVisibility(ESlateVisibility::Hidden);
			Widget->WidthInput->GetParent()->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void USubModeFloorCreation::WallLeftClickProcess()
{
	FHitResult HitResult;
	PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		const FVector ClickLocation = HitResult.Location;

		if (SelectedActor && Cast<AFloorActor>(SelectedActor)->WallState == EBuildingSubModeState::Moving)
		{
			Cast<AFloorActor>(SelectedActor)->WallState = EBuildingSubModeState::Placed;
			FVector SnappedLocation = Utility::SnapToGrid(ClickLocation,FVector(20));
			SelectedActor->SetActorLocation(SnappedLocation);
			
		}
		else
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			if (AFloorActor* SpawnedActor = Cast<AFloorActor>(HitResult.GetActor()))
			{
				SpawnedActor->WallState = EBuildingSubModeState::Moving;
				SelectedActor = SpawnedActor;
				if (DynamicMaterial)
				{
					SelectedActor->GetProceduralMeshComponent()->SetMaterial(0, DynamicMaterial);
				}
				SpawnedActor->WallState = EBuildingSubModeState::Moving;
			}
			
		}
		
	}
}


void USubModeFloorCreation::WallRightClickProcess()
{
	if (SelectedActor)
	{
		Cast<AFloorActor>(SelectedActor)->WallState = EBuildingSubModeState::Placed;
		SelectedActor = nullptr;
	}
	FHitResult HitResult;
	PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		FVector SpawnLocation = Utility::SnapToGrid(HitResult.Location, FVector(20.0f, 20.0f, 0.0f));
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (AFloorActor* SpawnedActor = GetWorld()->SpawnActor<AFloorActor>(AFloorActor::StaticClass(), SpawnLocation, FRotator::ZeroRotator, SpawnParams))
		{
			SelectedActor = SpawnedActor;
			if (DynamicMaterial)
			{
				SelectedActor->GetProceduralMeshComponent()->SetMaterial(0, DynamicMaterial);
			}
		}
	}
	
}

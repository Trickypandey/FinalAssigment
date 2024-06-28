// Fill out your copyright notice in the Description page of Project Settings.


#include "Modes/InteriorDesignMode.h"

#include "CeilingActor.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FloorActor.h"
#include "InputMappingContext.h"
#include "InteriorDesignWidget.h"
#include "WallActor.h"

UInteriorDesignMode::UInteriorDesignMode()
{
}

void UInteriorDesignMode::Setup()
{
	/*if (IsValid(InteriorDesignActorRef) && !IsValid(InteriorDesignActor)) {
		InteriorDesignActor = GetWorld()->SpawnActor<AInteriorDesignActor>(InteriorDesignActorRef, FTransform{});
	}*/

	InteriorDesignActor = nullptr;
	if (IsValid(WidgetRef) && !IsValid(Widget)) {
		Widget = CreateWidget<UInteriorDesignWidget>(PlayerController, WidgetRef, "Interior Mode Widget");
	}
}

void UInteriorDesignMode::SetupInputMapping()
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent)) {
		InputMappingContext = NewObject<UInputMappingContext>(this);

		OnLeftClickAction = NewObject<UInputAction>(this);
		OnLeftClickAction->ValueType = EInputActionValueType::Boolean;

		OnRightClickAction = NewObject<UInputAction>(this);
		OnRightClickAction->ValueType = EInputActionValueType::Boolean;


		OnRotateAction = NewObject<UInputAction>(this);
		OnRotateAction->ValueType = EInputActionValueType::Boolean;



		if (InputMappingContext)
		{
			InputMappingContext->MapKey(OnLeftClickAction, EKeys::LeftMouseButton);
			InputMappingContext->MapKey(OnRightClickAction, EKeys::RightMouseButton);
			InputMappingContext->MapKey(OnRotateAction, EKeys::R);

		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("InputMappingContext Not Populated"));
		}

		if (EnhancedInputComponent)
		{
			EnhancedInputComponent->BindAction(OnLeftClickAction, ETriggerEvent::Completed, this, &UInteriorDesignMode::HandleLeftClickAction);
			EnhancedInputComponent->BindAction(OnRightClickAction, ETriggerEvent::Completed, this, &UInteriorDesignMode::HandleRightClickAction);
			EnhancedInputComponent->BindAction(OnRotateAction, ETriggerEvent::Completed, this, &UInteriorDesignMode::RotateSelectedActor);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("EnhancedInputComponent Not Populated"));
		}


	}
}

void UInteriorDesignMode::EnterMode()
{

	if (IsValid(PlayerController)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}

		ShowWidget();
	}
}

void UInteriorDesignMode::ExitMode()
{
	if (IsValid(PlayerController)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->RemoveMappingContext(InputMappingContext);
		}

		HideWidget();
		CleanUp();
	}
}

void UInteriorDesignMode::CleanUp()
{
	if (InteriorDesignActor && InteriorDesignActor->InteriorState == EBuildingSubModeState::Moving)
	{
		InteriorDesignActor->Destroy();
	}

	InteriorDesignActor = nullptr;
}

void UInteriorDesignMode::SetMeshData(const FFurnitureData& FurnitureData)
{
	InteriorCurrentData = FurnitureData;
	//if (!InteriorDesignActor)
	//{
	//	InteriorDesignActor = GetWorld()->SpawnActor<AInteriorDesignActor>(InteriorDesignActorRef, FTransform{});
	//}
	//InteriorDesignActor->SetStaticMesh(FurnitureData.Type);
	//InteriorDesignActor->SetActorAttachebalType(FurnitureData.Attachable);


}

void UInteriorDesignMode::HandleLeftClickAction()
{
	if (!PlayerController) return;

	// Check if the InteriorDesignActor is in the Moving state
	if (InteriorDesignActor && InteriorDesignActor->InteriorState == EBuildingSubModeState::Moving)
	{
		FCollisionQueryParams TraceParams;
		TraceParams.bTraceComplex = true;
		TraceParams.AddIgnoredActor(PlayerController);
		TraceParams.AddIgnoredActor(InteriorDesignActor);

		FHitResult HitResult;
		FVector CursorWorldLocation;
		FVector CursorWorldDirection;
		PlayerController->DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);

		// Line trace to find a surface to attach to
		if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams))
		{
			AActor* HitActor = HitResult.GetActor();

			// Check if the HitActor is of the attachable type for InteriorDesignActor
			switch (InteriorDesignActor->AttachebleTo)
			{
				case EBuildingAttachable::FloorAttachable:
				{
					AFloorActor* FloorActor = Cast<AFloorActor>(HitActor);
					if (FloorActor)
					{
						InteriorDesignActor->SetActorLocation(HitResult.Location);
						InteriorDesignActor->AttachToActor(FloorActor, FAttachmentTransformRules::KeepWorldTransform);
						InteriorDesignActor->InteriorState = EBuildingSubModeState::Placed;
						InteriorDesignActor = nullptr; // Reset the current InteriorDesignActor
					}
				}
				break;

				case EBuildingAttachable::WallAttachable:
				{
					AWallActor* WallActor = Cast<AWallActor>(HitActor);
					if (WallActor)
					{
						InteriorDesignActor->SetActorLocation(HitResult.Location);
						InteriorDesignActor->AttachToActor(WallActor, FAttachmentTransformRules::KeepWorldTransform);
						InteriorDesignActor->InteriorState = EBuildingSubModeState::Placed;
						InteriorDesignActor = nullptr; // Reset the current InteriorDesignActor
					}
				}
				break;

				case EBuildingAttachable::CeilingAttachable:
				{
					ACeilingActor* CeilingActor = Cast<ACeilingActor>(HitActor);
					if (CeilingActor)
					{
						InteriorDesignActor->SetActorLocation(HitResult.Location);
						InteriorDesignActor->AttachToActor(CeilingActor, FAttachmentTransformRules::KeepWorldTransform);
						InteriorDesignActor->InteriorState = EBuildingSubModeState::Placed;
						InteriorDesignActor = nullptr; // Reset the current InteriorDesignActor
					}
				}
				break;

				default:
					break;
			}
		}
	}
	else if (!InteriorDesignActor || (InteriorDesignActor && InteriorDesignActor->InteriorState == EBuildingSubModeState::Placed))
	{
		FCollisionQueryParams TraceParams;
		TraceParams.bTraceComplex = true;
		TraceParams.AddIgnoredActor(PlayerController);

		FHitResult HitResult;
		FVector CursorWorldLocation;
		FVector CursorWorldDirection;
		PlayerController->DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);

		// Line trace to find a clicked InteriorDesignActor
		if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams))
		{
			AInteriorDesignActor* ClickedActor = Cast<AInteriorDesignActor>(HitResult.GetActor());
			if (ClickedActor && ClickedActor->InteriorState == EBuildingSubModeState::Placed)
			{
				// Set the clicked actor to moving state
				ClickedActor->InteriorState = EBuildingSubModeState::Moving;
				InteriorDesignActor = ClickedActor; // Assign the clicked actor to be the current InteriorDesignActor
			}
		}
	}
}



void UInteriorDesignMode::HandleRightClickAction()
{
	if (!PlayerController) return;

	if (InteriorDesignActor && InteriorDesignActor->InteriorState == EBuildingSubModeState::Moving)
	{
		// Destroy the currently moving InteriorDesignActor
		InteriorDesignActor->Destroy();
		InteriorDesignActor = nullptr;
	}

	if (!InteriorDesignActor)
	{
		// Spawn a new InteriorDesignActor at the cursor location
		FCollisionQueryParams TraceParams;
		TraceParams.bTraceComplex = true;
		TraceParams.AddIgnoredActor(PlayerController);

		FHitResult HitResult;
		FVector CursorWorldLocation;
		FVector CursorWorldDirection;
		PlayerController->DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);

		if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams))
		{
			const FVector SpawnLocation = HitResult.Location;
			const FRotator SpawnRotation = FRotator::ZeroRotator;

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			if (AInteriorDesignActor* SpawnedActor = GetWorld()->SpawnActor<AInteriorDesignActor>(InteriorDesignActorRef, SpawnLocation, SpawnRotation, SpawnParams))
			{
				SpawnedActor->InteriorState = EBuildingSubModeState::Moving;
				if (SpawnedActor)
				{
					SpawnedActor->SetStaticMesh(InteriorCurrentData.Type);
					SpawnedActor->SetActorAttachebalType(InteriorCurrentData.Attachable);
				}
				InteriorDesignActor = SpawnedActor;
			}
		}
	}
}


void UInteriorDesignMode::RotateSelectedActor()
{
	if (InteriorDesignActor)
	{
		InteriorDesignActor->AddActorLocalRotation(FRotator(0.f, 90.f, 0.f));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Actor rotated by 90 degrees"));
	}
}
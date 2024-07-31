// Fill out your copyright notice in the Description page of Project Settings.


#include "Modes/SubModeCeilingCreation.h"

#include "ArchVizPlayerController.h"
#include "CeilingActor.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "WallActor.h"

USubModeCeilingCreation::USubModeCeilingCreation()
{
}

void USubModeCeilingCreation::Setup()
{
	SelectedActor = nullptr;
}

void USubModeCeilingCreation::Cleanup()
{

	if (auto* ActorToDestroy = Cast<ACeilingActor>(SelectedActor))
	{
		if (ActorToDestroy->WallState == EBuildingSubModeState::Moving)
		{
			SelectedActor = nullptr;
			ActorToDestroy->Destroy(); 
		}
		ActorToDestroy->UnhighlightDeselectedActor();

	}
	else
	{
		SelectedActor = nullptr;
	}
}

void USubModeCeilingCreation::SetupInputMapping()
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
		InputMappingContext->MapKey(OnWallDelete, EKeys::Delete);
		InputMappingContext->MapKey(OnShowInstruction, EKeys::I);
		InputMappingContext->MapKey(OnActorRotate, EKeys::R);
		InputMappingContext->MapKey(StartMovement, EKeys::M);



	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("InputMappingContext Not Populated"));
	}

	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(OnWallLeftClick, ETriggerEvent::Started, this, &USubModeCeilingCreation::WallLeftClickProcess);
		EnhancedInputComponent->BindAction(OnWallRightClick, ETriggerEvent::Started, this, &USubModeCeilingCreation::WallRightClickProcess);
		EnhancedInputComponent->BindAction(OnWallDelete, ETriggerEvent::Started, this, &USubModeCeilingCreation::DeleteSelectedWallActor);
		EnhancedInputComponent->BindAction(OnShowInstruction, ETriggerEvent::Started, this, &USubModeCeilingCreation::ShowInstructionTab);
		EnhancedInputComponent->BindAction(OnShowInstruction, ETriggerEvent::Completed, this, &USubModeCeilingCreation::HideInstructionTab);
		EnhancedInputComponent->BindAction(StartMovement, ETriggerEvent::Completed, this, &USubModeCeilingCreation::ToggleMovementSelectedActor);
		EnhancedInputComponent->BindAction(OnActorRotate, ETriggerEvent::Started, this, &USubModeCeilingCreation::RotateSelectedActor);





	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("EnhancedInputComponent Not Populated"));
	}
	
}
void USubModeCeilingCreation::RotateSelectedActor()
{
	if (SelectedActor)
	{
		SelectedActor->AddActorLocalRotation(FRotator(0.f, 90.f, 0.f));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Actor rotated by 80 degrees"));
	}
}
void USubModeCeilingCreation::EnterSubMode(UWallConstructionWidget* CeilingConstructionWidget)
{
	if (PlayerController) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(InputMappingContext, 0);

			Setup();
		}
		if (CeilingConstructionWidget)
		{
			CurrentWidget = CeilingConstructionWidget;
			CeilingConstructionWidget->LengthInput->GetParent()->SetVisibility(ESlateVisibility::Visible);
			CeilingConstructionWidget->WidthInput->GetParent()->SetVisibility(ESlateVisibility::Visible);
			CeilingConstructionWidget->Ceiling->SetBackgroundColor(FColor::Black);
			if (auto* actor = Cast<ACeilingActor>(SelectedActor) )
			{
				
				/*CeilingConstructionWidget->LengthInput->SetValue(actor->GetLength());
				CeilingConstructionWidget->WidthInput->SetValue(actor->GetWidth());*/
			}
		}
	}
}

void USubModeCeilingCreation::ExitSubMode(UWallConstructionWidget* CeilingConstructionWidget)
{
	if (PlayerController) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->RemoveMappingContext(InputMappingContext);

			Cleanup();
		}
		if (CeilingConstructionWidget)
		{
			CeilingConstructionWidget->Ceiling->SetBackgroundColor(FColor::White);

			CeilingConstructionWidget->LengthInput->GetParent()->SetVisibility(ESlateVisibility::Hidden);
			CeilingConstructionWidget->WidthInput->GetParent()->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void USubModeCeilingCreation::ShowInstructionTab()
{

	if (CurrentWidget)
	{
		CurrentWidget->InstructionBtn->SetVisibility(ESlateVisibility::Hidden);
		CurrentWidget->Allkeys->SetVisibility(ESlateVisibility::Visible);
	}
}

void USubModeCeilingCreation::ToggleMovementSelectedActor()
{
	if (SelectedActor)
	{
		bIsNewWall = false;
		Cast<ACeilingActor>(SelectedActor)->WallState = EBuildingSubModeState::Moving;
		Cast<AArchVizPlayerController>(PlayerController)->BroadcastToast("Actor movement started");
	}
}

void USubModeCeilingCreation::HideInstructionTab()
{
	if (CurrentWidget)
	{
		CurrentWidget->InstructionBtn->SetVisibility(ESlateVisibility::Visible);
		CurrentWidget->Allkeys->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USubModeCeilingCreation::WallLeftClickProcess()
{
	FHitResult HitResult;
	PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		const FVector ClickLocation = HitResult.Location;

		if (SelectedActor && Cast<ACeilingActor>(SelectedActor)->WallState == EBuildingSubModeState::Constructing && bFirstClickDone)
		{
			AActor* HitActor = HitResult.GetActor();
			ACubeActor* CubeActor = Cast<ACubeActor>(HitActor);
			AAWallDoorActor* WallDoorActor = Cast<AAWallDoorActor>(HitActor);

			if (CubeActor || WallDoorActor)
			{
				AActor* ParentActor = nullptr;
				if (CubeActor)
				{
					ParentActor = CubeActor->GetDefaultAttachComponent()->GetAttachParentActor();
				}
				else if (WallDoorActor)
				{
					ParentActor = WallDoorActor->GetDefaultAttachComponent()->GetAttachParentActor();
				}

				if (AWallActor* WallActor = Cast<AWallActor>(ParentActor))
				{
					Cast<AArchVizPlayerController>(PlayerController)->BroadcastToast("Actor Placed");
					bFirstClickDone = false;

					// Ensure SelectedActor is a valid ACeilingActor before casting and accessing it
					if (ACeilingActor* CeilingActor = Cast<ACeilingActor>(SelectedActor))
					{
						CeilingActor->WallState = EBuildingSubModeState::Placed;
						CeilingActor->SetEndLocation(ClickLocation);
					}
				}
			}
		}
		else if(SelectedActor && Cast<ACeilingActor>(SelectedActor)->WallState == EBuildingSubModeState::Moving && !bFirstClickDone)
		{
			AActor* HitActor = HitResult.GetActor();
			ACubeActor* CubeActor = Cast<ACubeActor>(HitActor);
			AAWallDoorActor* WallDoorActor = Cast<AAWallDoorActor>(HitActor);

			if (CubeActor || WallDoorActor)
			{
				AActor* ParentActor = nullptr;
				if (CubeActor)
				{
					ParentActor = CubeActor->GetDefaultAttachComponent()->GetAttachParentActor();
				}
				else if (WallDoorActor)
				{
					ParentActor = WallDoorActor->GetDefaultAttachComponent()->GetAttachParentActor();
				}

				if (AWallActor* WallActor = Cast<AWallActor>(ParentActor))
				{
					if (bIsNewWall)
					{
						bFirstClickDone = true;
						Cast<ACeilingActor>(SelectedActor)->SetStartLocation(ClickLocation);
						Cast<ACeilingActor>(SelectedActor)->SetActorLocation(Utility::SnapToGrid(ClickLocation, FVector(10, 10, 0)));
						Cast<ACeilingActor>(SelectedActor)->WallState = EBuildingSubModeState::Constructing;
					}
					else
					{
						bFirstClickDone = false;
						Cast<ACeilingActor>(SelectedActor)->WallState = EBuildingSubModeState::Placed;
						Cast<ACeilingActor>(SelectedActor)->SetStartLocation(Utility::SnapToGrid(ClickLocation, FVector(20)));
					}

				}
			}

		}
		else if (ACeilingActor* SpawnedActor = Cast<ACeilingActor>(HitResult.GetActor()))
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			if (auto* actor = Cast<ACeilingActor>(SelectedActor))
			{
				actor->UnhighlightDeselectedActor();
			}
			
			SelectedActor = SpawnedActor;
		
			Cast<AArchVizPlayerController>(PlayerController)->BroadcastToast("Actor selected for moving");

			
			
			if (SelectedActor)
			{
				Cast<ACeilingActor>(SelectedActor)->HighlightSelectedActor();
			}
			if (DynamicMaterial)
			{
				Cast<ACeilingActor>(SelectedActor)->SetMaterial(DynamicMaterial);
			}
		}

	}
}


void USubModeCeilingCreation::WallRightClickProcess()
{
	FHitResult HitResult;
	FVector spawnLocation = HitResult.Location;
	PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		if (SelectedActor  && Cast<ACeilingActor>(SelectedActor)->WallState == EBuildingSubModeState::Moving)
		{
			SelectedActor->Destroy();
			SelectedActor = nullptr;
			Cast<AArchVizPlayerController>(PlayerController)->BroadcastToast("Actor destroyed");

		}
		if (SelectedActor)
		{
			Cast<ACeilingActor>(SelectedActor)->UnhighlightDeselectedActor();
		}

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (ACeilingActor* SpawnedActor = GetWorld()->SpawnActor<ACeilingActor>(ACeilingActor::StaticClass(), spawnLocation, FRotator::ZeroRotator, SpawnParams))
		{
			
			SelectedActor = SpawnedActor;
			bIsNewWall = true;
			Cast<ACeilingActor>(SelectedActor)->HighlightSelectedActor();
			if (DynamicMaterial)
			{
				Cast<ACeilingActor>(SelectedActor)->SetMaterial( DynamicMaterial);
			}

			if (SelectedActor)
			{
				Cast<ACeilingActor>(SelectedActor)->HighlightSelectedActor();
			}
		}
		Cast<AArchVizPlayerController>(PlayerController)->BroadcastToast("Actor spawned and selected for moving");

			
	}

}

void USubModeCeilingCreation::DeleteSelectedWallActor()
{
	if (SelectedActor)
	{
		Cast<ACeilingActor>(SelectedActor)->UnhighlightDeselectedActor();
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
		SelectedActor->Destroy();
		SelectedActor = nullptr;
		bIsNewWall = false;
		bFirstClickDone = false;
		Cast<AArchVizPlayerController>(PlayerController)->BroadcastToast("Selected actor and attached actors destroyed");


	}
}

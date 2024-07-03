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
		ActorToDestroy->GetProceduralMeshComponent()->SetRenderCustomDepth(false);

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


	if (InputMappingContext)
	{

		InputMappingContext->MapKey(OnWallLeftClick, EKeys::LeftMouseButton);
		InputMappingContext->MapKey(OnWallRightClick, EKeys::RightMouseButton);
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
		EnhancedInputComponent->BindAction(OnWallLeftClick, ETriggerEvent::Started, this, &USubModeCeilingCreation::WallLeftClickProcess);
		EnhancedInputComponent->BindAction(OnWallRightClick, ETriggerEvent::Started, this, &USubModeCeilingCreation::WallRightClickProcess);
		EnhancedInputComponent->BindAction(OnWallDelete, ETriggerEvent::Started, this, &USubModeCeilingCreation::DeleteSelectedWallActor);
		EnhancedInputComponent->BindAction(OnShowInstruction, ETriggerEvent::Started, this, &USubModeCeilingCreation::ShowInstructionTab);
		EnhancedInputComponent->BindAction(OnShowInstruction, ETriggerEvent::Completed, this, &USubModeCeilingCreation::HideInstructionTab);
		EnhancedInputComponent->BindAction(StartMovement, ETriggerEvent::Completed, this, &USubModeCeilingCreation::ToggleMovementSelectedActor);




	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("EnhancedInputComponent Not Populated"));
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
			if (SelectedActor)
			{
				CeilingConstructionWidget->LengthInput->SetValue(SelectedActor->GetLength());
				CeilingConstructionWidget->WidthInput->SetValue(SelectedActor->GetWidth());
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

		if (SelectedActor && Cast<ACeilingActor>(SelectedActor)->WallState == EBuildingSubModeState::Moving)
		{
			if (Cast<AWallActor>(HitResult.GetActor()))
			{
				Cast<AArchVizPlayerController>(PlayerController)->BroadcastToast("Actor movement started");
				Cast<ACeilingActor>(SelectedActor)->WallState = EBuildingSubModeState::Placed;
			}
		}
		else
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			if (ACeilingActor* SpawnedActor = Cast<ACeilingActor>(HitResult.GetActor()))
			{
				if (SelectedActor)
				{
					SelectedActor->GetProceduralMeshComponent()->SetRenderCustomDepth(false);
				}
				SpawnedActor->WallState = EBuildingSubModeState::Moving;
				SelectedActor = SpawnedActor;
				if (SelectedActor && CurrentWidget)
				{
					SelectedActor->GetProceduralMeshComponent()->SetRenderCustomDepth(true);
					CurrentWidget->LengthInput->SetValue(SelectedActor->GetLength());
					CurrentWidget->WidthInput->SetValue(SelectedActor->GetWidth());
				}
				if (DynamicMaterial)
				{
					SelectedActor->GetProceduralMeshComponent()->SetMaterial(0, DynamicMaterial);
				}
				SpawnedActor->WallState = EBuildingSubModeState::Moving;
				Cast<AArchVizPlayerController>(PlayerController)->BroadcastToast("Actor selected for moving");

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
				SelectedActor->GetProceduralMeshComponent()->SetRenderCustomDepth(false);
			}
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			if (ACeilingActor* SpawnedActor = GetWorld()->SpawnActor<ACeilingActor>(ACeilingActor::StaticClass(), spawnLocation, FRotator::ZeroRotator, SpawnParams))
			{
				SpawnedActor->WallState = EBuildingSubModeState::Moving;
				SelectedActor = SpawnedActor;
				SelectedActor->GetProceduralMeshComponent()->SetRenderCustomDepth(true);
				if (DynamicMaterial)
				{
					SelectedActor->GetProceduralMeshComponent()->SetMaterial(0, DynamicMaterial);
				}

				if (SelectedActor)
				{
					SelectedActor->GetProceduralMeshComponent()->SetRenderCustomDepth(true);
					SelectedActor->GetProceduralMeshComponent()->CustomDepthStencilValue = 2.0;
				}
			}
			Cast<AArchVizPlayerController>(PlayerController)->BroadcastToast("Actor spawned and selected for moving");

			
	}

}

void USubModeCeilingCreation::DeleteSelectedWallActor()
{
	if (SelectedActor)
	{
		SelectedActor->GetProceduralMeshComponent()->SetRenderCustomDepth(false);
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

		Cast<AArchVizPlayerController>(PlayerController)->BroadcastToast("Selected actor and attached actors destroyed");


	}
}

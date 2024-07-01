// Fill out your copyright notice in the Description page of Project Settings.


#include "Modes/SubModeCeilingCreation.h"

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


	if (InputMappingContext)
	{

		InputMappingContext->MapKey(OnWallLeftClick, EKeys::LeftMouseButton);
		InputMappingContext->MapKey(OnWallRightClick, EKeys::RightMouseButton);
		InputMappingContext->MapKey(OnWallDelete, EKeys::Delete);


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
			if (SelectedActor)
			{
				CeilingConstructionWidget->LengthInput->SetValue(SelectedActor->GetLength());
				CeilingConstructionWidget->WidthInput->SetValue(SelectedActor->GetWidth());
			}
		}
	}
}

void USubModeCeilingCreation::ExitSubMode(UWallConstructionWidget* WidgetCeiling)
{
	if (PlayerController) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->RemoveMappingContext(InputMappingContext);

			Cleanup();
		}
		if (WidgetCeiling)
		{
			WidgetCeiling->LengthInput->GetParent()->SetVisibility(ESlateVisibility::Hidden);
			WidgetCeiling->WidthInput->GetParent()->SetVisibility(ESlateVisibility::Hidden);
		}
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
				Cast<ACeilingActor>(SelectedActor)->WallState = EBuildingSubModeState::Placed;
				FVector SnappedLocation = Utility::SnapToGrid(ClickLocation, FVector(20));
				SelectedActor->SetActorLocation(SnappedLocation);
			}
		}
		else
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			if (ACeilingActor* SpawnedActor = Cast<ACeilingActor>(HitResult.GetActor()))
			{
				SpawnedActor->WallState = EBuildingSubModeState::Moving;
				SelectedActor = SpawnedActor;
				if (CurrentWidget)
				{
					CurrentWidget->LengthInput->SetValue(SelectedActor->GetLength());
					CurrentWidget->WidthInput->SetValue(SelectedActor->GetWidth());
				}
				if (DynamicMaterial)
				{
					SelectedActor->GetProceduralMeshComponent()->SetMaterial(0, DynamicMaterial);
				}
				SpawnedActor->WallState = EBuildingSubModeState::Moving;
			}

		}

	}
}


void USubModeCeilingCreation::WallRightClickProcess()
{
	if (SelectedActor)
	{
		Cast<ACeilingActor>(SelectedActor)->WallState = EBuildingSubModeState::Placed;
		SelectedActor = nullptr;
	}
	FHitResult HitResult;
	PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		FVector SpawnLocation = Utility::SnapToGrid(HitResult.Location, FVector(20.0f, 20.0f, 0.0f));
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (ACeilingActor* SpawnedActor = GetWorld()->SpawnActor<ACeilingActor>(ACeilingActor::StaticClass(), SpawnLocation, FRotator::ZeroRotator, SpawnParams))
		{
			SpawnedActor->WallState = EBuildingSubModeState::Moving;
			SelectedActor = SpawnedActor;
			if (DynamicMaterial)
			{
				SelectedActor->GetProceduralMeshComponent()->SetMaterial(0, DynamicMaterial);
			}
		}
	}

}

void USubModeCeilingCreation::DeleteSelectedWallActor()
{
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Modes/SubModeWallCreation.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

USubModeWallCreation::USubModeWallCreation()
{
	
}

void USubModeWallCreation::Cleanup()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Cleanup Happen"));
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

void USubModeWallCreation::EnterSubMode()
{
	if (PlayerController) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(InputMappingContext, 0);

			Setup();
		}
	}
}

void USubModeWallCreation::ExitSubMode()
{
	if (PlayerController) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->RemoveMappingContext(InputMappingContext);

			//Cleanup();
		}
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

		if (auto* WallActor = Cast<AWallActor>(HitResult.GetActor()))
		{
			FVector LocalClickLocation = WallActor->GetTransform().InverseTransformPosition(ClickLocation);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("World Clicked Location: X=%f, Y=%f, Z=%f"), ClickLocation.X, ClickLocation.Y, ClickLocation.Z));
			SelectedActor = WallActor;
		}
		else
		{
			if (SelectedActor)
			{
				
				SelectedActor->SetActorLocation(ClickLocation);
			}
		}
	}
}

void USubModeWallCreation::WallRightClickProcess()
{
	if (SelectedActor && Cast<AWallActor>(SelectedActor)->WallState == EBuildingSubModeState::Moving)
	{
		SelectedActor->Destroy();
	}
	FHitResult HitResult;
	PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		FVector spawnLocation = HitResult.Location;
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (AWallActor* SpwanedActor = GetWorld()->SpawnActor<AWallActor>(AWallActor::StaticClass(), spawnLocation, FRotator::ZeroRotator, SpawnParams))
		{
			SpwanedActor->WallState = EBuildingSubModeState::Placed;
			SelectedActor = SpwanedActor;

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

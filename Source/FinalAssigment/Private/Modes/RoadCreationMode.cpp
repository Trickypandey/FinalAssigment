// Fill out your copyright notice in the Description page of Project Settings.


#include "Modes/RoadCreationMode.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "RoadCreationWidget.h"
#include "UArchVizSaveGame.h"
#include "Kismet/GameplayStatics.h"

URoadCreationMode::URoadCreationMode() : CurrentRoadActor{nullptr}, OnLeftClickAction(nullptr),
                                         OnRoadRightClick(nullptr), DynamicMaterial(nullptr)
{
}

void URoadCreationMode::Setup()
{
	/*if (IsValid(RoadActorRef) && !IsValid(CurrentRoadActor)) {
		CurrentRoadActor = GetWorld()->SpawnActor<ARoadActor>(RoadActorRef, FTransform{});
	}*/

	if (IsValid(WidgetRef) && !IsValid(Widget)) {
		Widget = CreateWidget<URoadCreationWidget>(GetWorld(), WidgetRef, "Road Mode Widget");
	}
}



void URoadCreationMode::SetupInputMapping()
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent)) {
		InputMappingContext = NewObject<UInputMappingContext>(this);

		OnLeftClickAction = NewObject<UInputAction>(this);
		OnLeftClickAction->ValueType = EInputActionValueType::Boolean;

		OnRoadRightClick = NewObject<UInputAction>(this);
		OnRoadRightClick->ValueType = EInputActionValueType::Boolean;

		ShowInstruction = NewObject<UInputAction>(this);
		ShowInstruction->ValueType = EInputActionValueType::Boolean;

		OnDelete = NewObject<UInputAction>(this);
		OnDelete->ValueType = EInputActionValueType::Boolean;


		if (InputMappingContext)
		{
			InputMappingContext = NewObject<UInputMappingContext>(this);
			InputMappingContext->MapKey(OnLeftClickAction, EKeys::LeftMouseButton);
			InputMappingContext->MapKey(OnRoadRightClick, EKeys::RightMouseButton);
			InputMappingContext->MapKey(ShowInstruction, EKeys::I);
			InputMappingContext->MapKey(OnDelete, EKeys::Delete);
			
		}
		else
		{			
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("InputMappingContext Not Populated"));
		}

		if (EnhancedInputComponent)
		{
			EnhancedInputComponent->BindAction(OnLeftClickAction, ETriggerEvent::Completed, this, &URoadCreationMode::HandleLeftClickAction);
			EnhancedInputComponent->BindAction(OnRoadRightClick, ETriggerEvent::Completed, this, &URoadCreationMode::HandleRightClickAction);
			EnhancedInputComponent->BindAction(ShowInstruction, ETriggerEvent::Started, this, &URoadCreationMode::ShowInstructionTab);
			EnhancedInputComponent->BindAction(ShowInstruction, ETriggerEvent::Completed, this, &URoadCreationMode::HideInstructionTab);
			EnhancedInputComponent->BindAction(OnDelete, ETriggerEvent::Completed, this, &URoadCreationMode::DeleteSelectedRoadActor);
		}
		else
		{			
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("EnhancedInputComponent Not Populated"));
		}


	}
}

void URoadCreationMode::EnterMode()
{
	if (IsValid(PlayerController)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}

		ShowWidget();
	}
}

void URoadCreationMode::ExitMode()
{
	if (IsValid(PlayerController)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->RemoveMappingContext(InputMappingContext);
		}

		HideWidget();
		CleanUp();
	}
}

void URoadCreationMode::CleanUp()
{
	if (CurrentRoadActor)
	{
		CurrentRoadActor->GetProceduralMeshComponent()->SetRenderCustomDepth(false);
		CurrentRoadActor = nullptr;
	}
}

void URoadCreationMode::HandleLeftClickAction()
{
	FHitResult HitResult;
	PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

	if (HitResult.bBlockingHit) {
		AActor* HitActor = HitResult.GetActor();
		if (HitActor && HitActor->IsA(ARoadActor::StaticClass())) {
			// Reset custom depth for all road actors
			ResetCustomDepthForAllRoadActors();

			// Set the current road actor and enable custom depth
			CurrentRoadActor = Cast<ARoadActor>(HitActor);
			CurrentRoadActor->GetProceduralMeshComponent()->SetRenderCustomDepth(true);

			return;
		}
		else if (CurrentRoadActor) {
			const FVector ClickLocation = HitResult.Location;
			CurrentRoadActor->AddSplinePoint(ClickLocation);

			if (DynamicMaterial) {
				CurrentRoadActor->GetProceduralMeshComponent()->SetMaterial(0, DynamicMaterial);
			}

			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Added spline point at location: %s"), *ClickLocation.ToString()));
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("No road actor selected or no hit detected"));
		}
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("No hit detected"));
	}
}


void URoadCreationMode::HandleRightClickAction()
{
	FHitResult HitResult;
	PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

	if (HitResult.bBlockingHit)
	{
		FVector ClickLocation = HitResult.Location;
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ResetCustomDepthForAllRoadActors();

		CurrentRoadActor = GetWorld()->SpawnActor<ARoadActor>(ARoadActor::StaticClass(), ClickLocation, FRotator::ZeroRotator, SpawnParams);
		if (CurrentRoadActor)
		{
			CurrentRoadActor->GetProceduralMeshComponent()->SetRenderCustomDepth(true);
			CurrentRoadActor->GetProceduralMeshComponent()->CustomDepthStencilValue = 2.0;
			RoadActors.Add(CurrentRoadActor);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Road actor spawned at cursor location"));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("No hit detected"));
	}
}


void URoadCreationMode::AddMaterialToRoad(const FMaterialData& MeshData)
{
	UMaterialInterface* BaseMaterial = MeshData.Type;
	if (!BaseMaterial)
	{
		UE_LOG(LogTemp, Error, TEXT("BaseMaterial is nullptr in AArchVizPlayerController"));
		return;
	}

	DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
	if (BaseMaterial && CurrentRoadActor)
	{
		
		CurrentRoadActor->SetMaterialForSection(0, BaseMaterial);
	}
}

void URoadCreationMode::SaveRoads(UUArchVizSaveGame*& SaveGameInstance)
{
	// Create the save game object
	

	// Check if the save game object was created successfully
	if (!SaveGameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create save game object"));
		return;
	}


	// Loop through all road actors and save their data
	for (ARoadActor* RoadActor : RoadActors)
	{
		if (IsValid(RoadActor))
		{
			FRoadActorData RoadData = RoadActor->SaveRoadActorData();
			SaveGameInstance->RoadActorArray.Add(RoadData);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid road actor found in RoadActors array"));
		}
	}
}

void URoadCreationMode::LoadRoads(UUArchVizSaveGame*& LoadGameInstance)
{
	if (!LoadGameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load game from slot"));
		return;
	}

	for (const FRoadActorData& RoadData : LoadGameInstance->RoadActorArray)
	{
		ARoadActor* NewRoadActor = GetWorld()->SpawnActor<ARoadActor>(RoadActorRef, RoadData.ActorTransform);
		if (NewRoadActor)
		{
			NewRoadActor->LoadRoadActorData(RoadData);
			RoadActors.Add(NewRoadActor);
		}
	}
	
}

void URoadCreationMode::SetWidth(float Invalue)
{
	if(CurrentRoadActor)
	{
		CurrentRoadActor->SetRoadWidth(Invalue);
	}
}

void URoadCreationMode::ShowInstructionTab()
{
	auto RoadWidget = Cast<URoadCreationWidget>(Widget);
	if (Widget && RoadWidget)
	{
		RoadWidget->InstructionBtn->SetVisibility(ESlateVisibility::Hidden);
		RoadWidget->Allkeys->SetVisibility(ESlateVisibility::Visible);
	}
}

void URoadCreationMode::HideInstructionTab()
{
	auto RoadWidget = Cast<URoadCreationWidget>(Widget);
	if (Widget && RoadWidget)
	{
		RoadWidget->InstructionBtn->SetVisibility(ESlateVisibility::Visible);
		RoadWidget->Allkeys->SetVisibility(ESlateVisibility::Hidden);
	}
}

void URoadCreationMode::DeleteSelectedRoadActor()
{
	if(CurrentRoadActor)
	{
		CurrentRoadActor->GetProceduralMeshComponent()->SetRenderCustomDepth(false);
		CurrentRoadActor->Destroy();
		CurrentRoadActor = nullptr;

	}
}

void URoadCreationMode::ResetCustomDepthForAllRoadActors()
{
	for (ARoadActor* RoadActor : RoadActors)
	{
		if (IsValid(RoadActor))
		{
			RoadActor->GetProceduralMeshComponent()->SetRenderCustomDepth(false);
		}
	}
}
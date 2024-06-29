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


		if (InputMappingContext)
		{
			InputMappingContext = NewObject<UInputMappingContext>(this);
			InputMappingContext->MapKey(OnLeftClickAction, EKeys::LeftMouseButton);
			InputMappingContext->MapKey(OnRoadRightClick, EKeys::RightMouseButton);
			
		}
		else
		{			
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("InputMappingContext Not Populated"));
		}

		if (EnhancedInputComponent)
		{
			EnhancedInputComponent->BindAction(OnLeftClickAction, ETriggerEvent::Completed, this, &URoadCreationMode::HandleLeftClickAction);
			EnhancedInputComponent->BindAction(OnRoadRightClick, ETriggerEvent::Completed, this, &URoadCreationMode::HandleRightClickAction);
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
	CurrentRoadActor = nullptr;
}

void URoadCreationMode::HandleLeftClickAction() {
	if (IsValid(CurrentRoadActor)) {
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

		if (HitResult.bBlockingHit) {
			AActor* HitActor = HitResult.GetActor();
			if (HitActor && HitActor->IsA(ARoadActor::StaticClass())) {
				CurrentRoadActor = Cast<ARoadActor>(HitActor);
				return;
			}

			if (CurrentRoadActor) {
				const FVector ClickLocation = HitResult.Location;
				CurrentRoadActor->AddSplinePoint(ClickLocation);

				if (DynamicMaterial) {
					CurrentRoadActor->ProceduralMeshComponent->SetMaterial(0, DynamicMaterial);
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

		CurrentRoadActor = GetWorld()->SpawnActor<ARoadActor>(ARoadActor::StaticClass(), ClickLocation, FRotator::ZeroRotator, SpawnParams);
		if (CurrentRoadActor)
		{
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
	if (DynamicMaterial && CurrentRoadActor)
	{
		CurrentRoadActor->ProceduralMeshComponent->SetMaterial(0, DynamicMaterial);
	}
}



void URoadCreationMode::SaveRoads()
{
	// Create the save game object
	UUArchVizSaveGame* SaveGameInstance = Cast<UUArchVizSaveGame>(UGameplayStatics::CreateSaveGameObject(UUArchVizSaveGame::StaticClass()));

	// Check if the save game object was created successfully
	if (!SaveGameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create save game object"));
		return;
	}

	// Attempt to save the game to the slot and check for success
	bool bSaveSuccessful = UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("RoadSaveSlot"), 0);
	if (bSaveSuccessful)
	{
		UE_LOG(LogTemp, Log, TEXT("Empty game successfully saved to slot"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to save empty game to slot"));
		return; // Exit early if empty save fails
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

	// Attempt to save the game to the slot again with data
	bSaveSuccessful = UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("RoadSaveSlot"), 0);
	if (bSaveSuccessful)
	{
		UE_LOG(LogTemp, Log, TEXT("Game with road data successfully saved to slot"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to save game with road data to slot"));
	}
}



void URoadCreationMode::LoadRoads()
{
	UUArchVizSaveGame* LoadGameInstance = Cast<UUArchVizSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("RoadSaveSlot"), 0));

	if (LoadGameInstance)
	{
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
}
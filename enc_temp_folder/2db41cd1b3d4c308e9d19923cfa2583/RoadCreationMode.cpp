// Fill out your copyright notice in the Description page of Project Settings.


#include "Modes/RoadCreationMode.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "RoadCreationWidget.h"

URoadCreationMode::URoadCreationMode() : CurrentRoadActor{nullptr}, OnLeftClickAction(nullptr),
                                         OnRoadRightClick(nullptr), DynamicMaterial(nullptr)
{
}

void URoadCreationMode::Setup()
{
	if (IsValid(RoadActorRef) && !IsValid(CurrentRoadActor)) {
		CurrentRoadActor = GetWorld()->SpawnActor<ARoadActor>(RoadActorRef, FTransform{});
	}

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
	}
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
			/*RoadArray.Add(CurrentRoadActor);*/
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

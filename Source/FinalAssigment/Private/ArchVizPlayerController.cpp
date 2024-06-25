#include "ArchVizPlayerController.h"

#include "CeilingActor.h"
#include "WallActor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FloorActor.h"
#include "SlabActor.h"
#include "UiWidget.h"

AArchVizPlayerController::AArchVizPlayerController()
{
    PrimaryActorTick.bCanEverTick = true;
    bShowMouseCursor = true;
    bIsWallCreationMode = false;
    bIsRoadConstructionMode = true;
    bIsActorSpawning = false;
    bIsAddingDoor = false;
    SelectedActor = nullptr;
    SelectedActorType = EObjectType::Wall;
    CurrentMode = EModes::RoadConstruction;
}

ACubeActor* AArchVizPlayerController::GetSelectedActor()
{
    return SelectedActor;
}

void AArchVizPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    SetupEnhancedInputBindings();
}

void AArchVizPlayerController::SetupEnhancedInputBindings()
{
    UEnhancedInputComponent* Eic = Cast<UEnhancedInputComponent>(InputComponent);
    WallMappingContext = NewObject<UInputMappingContext>(this);
    RoadMappingContext = NewObject<UInputMappingContext>(this);
    InteriorMappingContext = NewObject<UInputMappingContext>(this);

    OnWallLeftClick = NewObject<UInputAction>(this);
    OnWallLeftClick->ValueType = EInputActionValueType::Boolean;

    OnWallRightClick = NewObject<UInputAction>(this);
    OnWallRightClick->ValueType = EInputActionValueType::Boolean;

    OnWallRotate = NewObject<UInputAction>(this);
    OnWallRotate->ValueType = EInputActionValueType::Boolean;

    OnWallDelete = NewObject<UInputAction>(this);
    OnWallDelete->ValueType = EInputActionValueType::Boolean;

    OnRoadAddPoint = NewObject<UInputAction>(this);
    OnRoadAddPoint->ValueType = EInputActionValueType::Boolean;

    OnDeSelectWall = NewObject<UInputAction>(this);
    OnDeSelectWall->ValueType = EInputActionValueType::Boolean;

    OnRoadRightClick = NewObject<UInputAction>(this);
    OnRoadRightClick->ValueType = EInputActionValueType::Boolean;


    check(Eic);

    Eic->BindAction(OnWallLeftClick, ETriggerEvent::Started, this, &AArchVizPlayerController::WallLeftClickProcess);
    Eic->BindAction(OnWallRightClick, ETriggerEvent::Started, this, &AArchVizPlayerController::WallRightClickProcess);
    Eic->BindAction(OnWallRotate, ETriggerEvent::Started, this, &AArchVizPlayerController::RotateSelectedActor);
    Eic->BindAction(OnWallDelete, ETriggerEvent::Started, this, &AArchVizPlayerController::DeleteSelectedActor);
    Eic->BindAction(OnDeSelectWall, ETriggerEvent::Started, this, &AArchVizPlayerController::DeSelectedSelectedActor);


    Eic->BindAction(OnRoadAddPoint, ETriggerEvent::Started, this, &AArchVizPlayerController::RoadLeftClick);
    Eic->BindAction(OnRoadRightClick, ETriggerEvent::Started, this, &AArchVizPlayerController::RoadRightClick);

    if (WallMappingContext)
    {
        WallMappingContext->MapKey(OnWallLeftClick, EKeys::LeftMouseButton);
        WallMappingContext->MapKey(OnWallRightClick, EKeys::RightMouseButton);
        WallMappingContext->MapKey(OnWallRotate, EKeys::R);
        WallMappingContext->MapKey(OnWallDelete, EKeys::Delete);
        WallMappingContext->MapKey(OnDeSelectWall, EKeys::Tab);
    }

    if (RoadMappingContext)
    {
        RoadMappingContext->MapKey(OnRoadAddPoint, EKeys::LeftMouseButton);
        RoadMappingContext->MapKey(OnRoadRightClick, EKeys::RightMouseButton);
    }

    AddCurrentModeMappingContext();
}

void AArchVizPlayerController::AddCurrentModeMappingContext() const
{
    auto* SubSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (!SubSystem)
    {
        UE_LOG(LogTemp, Warning, TEXT("EnhancedInputLocalPlayerSubsystem is not available."));
        return;
    }

    SubSystem->ClearAllMappings();
    UE_LOG(LogTemp, Log, TEXT("Cleared all input mappings."));

    UInputMappingContext* ContextToRemove1 = nullptr;
    UInputMappingContext* ContextToRemove2 = nullptr;
    UInputMappingContext* ContextToAdd = nullptr;


    switch (CurrentMode) {
	    case EModes::WallCreation:
		    {
		        ContextToAdd = WallMappingContext;
		        ContextToRemove1 = RoadMappingContext;
		        ContextToRemove2 = InteriorMappingContext;
		        UE_LOG(LogTemp, Log, TEXT("Wall Creation Mode activated."));
		    }
		    break;
	    case EModes::RoadConstruction:
		    {
		        ContextToAdd = RoadMappingContext;
		        ContextToRemove1 = WallMappingContext;
		        ContextToRemove2 = InteriorMappingContext;
		        UE_LOG(LogTemp, Log, TEXT("Road Construction Mode activated."));
		    }
		    break;
	    case EModes::InteriorDesign:
		    {
		        ContextToAdd = InteriorMappingContext;
		        ContextToRemove1 = RoadMappingContext;
		        ContextToRemove2 = WallMappingContext;
		        UE_LOG(LogTemp, Log, TEXT("Interior Design Mode activated."));
		    }
		    break;
	    default:
		    {
	            ContextToAdd = WallMappingContext;
	            ContextToRemove1 = RoadMappingContext;
	            ContextToRemove2 = InteriorMappingContext;
	            UE_LOG(LogTemp, Log, TEXT("Wall Creation Mode activated."));
		    };
    }

    if (ContextToRemove1)
    {
        SubSystem->RemoveMappingContext(ContextToRemove1);
        UE_LOG(LogTemp, Log, TEXT("Removed input mapping context: %s"), *ContextToRemove1->GetName());
    }
	if (ContextToRemove2)
    {
        SubSystem->RemoveMappingContext(ContextToRemove2);
        UE_LOG(LogTemp, Log, TEXT("Removed input mapping context: %s"), *ContextToRemove2->GetName());
    }

    if (ContextToAdd)
    {
        SubSystem->AddMappingContext(ContextToAdd, 0);
        UE_LOG(LogTemp, Log, TEXT("Added input mapping context: %s"), *ContextToAdd->GetName());
    }
}

void AArchVizPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (SelectedActor && bIsActorSpawning && bIsWallCreationMode)
    {
        FHitResult HitResult;
        FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, SelectedActor);

        FVector CursorWorldLocation;
        FVector CursorWorldDirection;
        DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);

        if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams))
        {
            FVector NewLocation = HitResult.Location;
          
            SelectedActor->SetActorLocation(NewLocation);
            SnapWall();
        }
    }
}

void AArchVizPlayerController::BeginPlay()
{
	Super::BeginPlay();

    if (!UiWidgetInstance && UiWidgetClass)
    {
        UiWidgetInstance = CreateWidget<UUiWidget>(this, UiWidgetClass);
        if (UiWidgetInstance)
        {
            UiWidgetInstance->AddToViewport();
        }
    }

    if (RoadWidgetClass && !RoadWidgetInstance)
    {
        RoadWidgetInstance = CreateWidget<URoadCreationWidget>(this, RoadWidgetClass);
        if (RoadWidgetInstance)
        {
            
            RoadWidgetInstance->AddToViewport();
        }
    }

    if (WallWidgetClass && !WallWidgetInstance)
    {
        WallWidgetInstance = CreateWidget<UWallConstructionWidget>(this, WallWidgetClass);
       
    }
}

void AArchVizPlayerController::AddMaterialToRoad(const FMaterialData& MeshData)
{
    UMaterialInterface* BaseMaterial = MeshData.Type;
    if (!BaseMaterial)
    {
        UE_LOG(LogTemp, Error, TEXT("BaseMaterial is nullptr in AArchVizPlayerController"));
        return;
    }

    DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
    if (DynamicMaterial && SelectedActor)
    {
        CurrentRoadActor->ProceduralMeshComponent->SetMaterial(0, DynamicMaterial);
    }
}

void AArchVizPlayerController::RoadLeftClick()
{
    FHitResult HitResult;
    GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

    if (HitResult.bBlockingHit)
    {
        AActor* HitActor = HitResult.GetActor();
        if (HitActor && HitActor->IsA(ARoadActor::StaticClass()))
        {
        	CurrentRoadActor = Cast<ARoadActor>(HitActor);
            return;
        }

        if (CurrentRoadActor)
        {
	        const FVector ClickLocation = HitResult.Location;
            CurrentRoadActor->AddSplinePoint(ClickLocation);

            if (DynamicMaterial)
            {
                CurrentRoadActor->ProceduralMeshComponent->SetMaterial(0, DynamicMaterial);
            }

            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Added spline point at location: %s"), *ClickLocation.ToString()));
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("No road actor selected or no hit detected"));
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("No hit detected"));
    }
}

void AArchVizPlayerController::RoadRightClick()
{
    FHitResult HitResult;
    GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

    if (HitResult.bBlockingHit)
    {
        FVector ClickLocation = HitResult.Location;
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        CurrentRoadActor = GetWorld()->SpawnActor<ARoadActor>(ARoadActor::StaticClass(), ClickLocation, FRotator::ZeroRotator, SpawnParams);
        if (CurrentRoadActor)
        {
            RoadArray.Add(CurrentRoadActor);
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Road actor spawned at cursor location"));
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("No hit detected"));
    }
}

void AArchVizPlayerController::DeSelectedSelectedActor()
{
    if (SelectedActor)
    {
	    if (UProceduralMeshComponent* MeshComponent = SelectedActor->GetProceduralMeshComponent())
        {
            MeshComponent->SetRenderCustomDepth(false);
            MeshComponent->CustomDepthStencilValue = 0; // Resetting to default
        }

        if (Cast<AWallActor>(SelectedActor))
        {
            WallWidgetInstance->LengthInput->GetParent()->SetVisibility(ESlateVisibility::Hidden);
           
        }
        else if (Cast<ASlabActor>(SelectedActor))
        {
            WallWidgetInstance->LengthInput->GetParent()->SetVisibility(ESlateVisibility::Hidden);
            WallWidgetInstance->WidthInput->GetParent()->SetVisibility(ESlateVisibility::Hidden);
        }
        SelectedActor = nullptr;
    }
}

void AArchVizPlayerController::WallLeftClickProcess()
{
    FHitResult HitResult;
    GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

    // Reset actor spawning state since we are interacting with existing actors
    bIsActorSpawning = false;

    if (auto [WallActor, LocalClickLocation] = IsWallActor(HitResult); WallActor)
    {
        // Clicked on a wall actor, update the selected actor
        SelectedActor = WallActor;

        // Highlight the selected actor
        if (SelectedActor)
        {
            SelectedActor->GetProceduralMeshComponent()->SetRenderCustomDepth(true);
            SelectedActor->GetProceduralMeshComponent()->CustomDepthStencilValue = 2.0;
        }

        // Unhighlight previously selected actor if exists
        if (PreviousSelectedActor && PreviousSelectedActor != SelectedActor)
        {
            PreviousSelectedActor->GetProceduralMeshComponent()->SetRenderCustomDepth(false);
        }

        // Update the previously selected actor to the current one
        PreviousSelectedActor = SelectedActor;

        // Additional logic as needed
        if (AWallActor* tempWallActor = Cast<AWallActor>(WallActor); bIsAddingDoor && tempWallActor)
        {
            // If adding a door is requested, update the wall actor
            tempWallActor->SetIsDoorAdded(bIsAddingDoor);
            tempWallActor->SetDoorLocation(LocalClickLocation.X);
            tempWallActor->CreateMesh();
        }
        if (ASlabActor* tempSlabActor = Cast<ASlabActor>(WallActor); tempSlabActor)
        {
            tempSlabActor->CreateMesh();
        }
    }
    else
    {
        // Clicked on empty space, spawn the selected actor at the hit location
        if (HitResult.bBlockingHit)
        {
            SpawnLocation = HitResult.Location;
            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
            if (SelectedActor)
            {
				SelectedActor->SetActorLocation(SpawnLocation);
            }
            bIsActorSpawning = true;
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Actor spawned at cursor location"));
        }
    }

    // Additional logic based on selected actor type
    if (Cast<AWallActor>(SelectedActor))
    {
        WallWidgetInstance->LengthInput->SetValue(SelectedActor->GetLength());
        WallWidgetInstance->LengthInput->GetParent()->SetVisibility(ESlateVisibility::Visible);
        WallWidgetInstance->WidthInput->GetParent()->SetVisibility(ESlateVisibility::Hidden);
    }
    else if (Cast<ASlabActor>(SelectedActor))
    {
        WallWidgetInstance->LengthInput->GetParent()->SetVisibility(ESlateVisibility::Visible);
        WallWidgetInstance->WidthInput->GetParent()->SetVisibility(ESlateVisibility::Visible);
        WallWidgetInstance->LengthInput->SetValue(SelectedActor->GetLength());
        WallWidgetInstance->WidthInput->SetValue(SelectedActor->GetWidth());
    }
}

void AArchVizPlayerController::WallRightClickProcess()
{
    FHitResult HitResult;
    GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
    bIsActorSpawning = true;
    if (HitResult.bBlockingHit)
    {
        FVector spawnLocation = HitResult.Location;
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        ACubeActor* SpwanedActor = nullptr;


        switch (SelectedActorType) {
	        case EObjectType::Wall: {
	            
	            SpwanedActor = GetWorld()->SpawnActor<AWallActor>(AWallActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	            break;
	        }
	        case EObjectType::Floor: {
	           
	            SpwanedActor = GetWorld()->SpawnActor<AFloorActor>(AFloorActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	            break;
	        }

	        case EObjectType::Ceiling: {
	           
	            SpwanedActor = GetWorld()->SpawnActor<ACeilingActor>(ACeilingActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	            break;
	        }
	        default: {
	            
	            SpwanedActor = GetWorld()->SpawnActor<AWallActor>(AWallActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	            break;
	        }
        }

        if (SpwanedActor)
        {
            if(DynamicMaterial)
            {
				SpwanedActor->GetProceduralMeshComponent()->SetMaterial(0, DynamicMaterial);
            }
            SelectedActor = SpwanedActor;
        }
        
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Actor spawned at cursor location"));
    }
}

void AArchVizPlayerController::RotateSelectedActor()
{
    if (SelectedActor)
    {
        SelectedActor->AddActorLocalRotation(FRotator(0.f, 90.f, 0.f));
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Actor rotated by 80 degrees"));
    }
}

void AArchVizPlayerController::SetIsAddingDoor(bool DoorFlag)
{

    bIsAddingDoor = DoorFlag;
}

void AArchVizPlayerController::ModeChangeHandle(EModes Mode)
{
	switch (Mode) {
		case EModes::WallCreation:
			{
	            if (WallWidgetInstance)
	            {
	                WallWidgetInstance->AddToViewport();
	            }

	          
	            if (RoadWidgetInstance)
	            {
	                RoadWidgetInstance->RemoveFromViewport();
	            }

	            bIsWallCreationMode = true;
	            bIsRoadConstructionMode = false;
			}
			break;
		case EModes::RoadConstruction:
			{
	            if (RoadWidgetInstance)
	            {
	               
	                RoadWidgetInstance->AddToViewport();
	            }

	            
	            if (WallWidgetInstance)
	            {
	                WallWidgetInstance->RemoveFromViewport();
	            }

	            bIsWallCreationMode = false;
	            bIsRoadConstructionMode = true;
			}
			break;
		case EModes::InteriorDesign:
			{
				
			}
			break;
      
	}

		CurrentMode = Mode;
		DynamicMaterial = nullptr;
        AddCurrentModeMappingContext();
}

void AArchVizPlayerController::DeleteSelectedActor()
{
    if (SelectedActor)
    {
        SelectedActor->GetProceduralMeshComponent()->SetRenderCustomDepth(false);
        SelectedActor->Destroy();
        SelectedActor = nullptr;

    }
}

void AArchVizPlayerController::SnapWall()
{
    if (SelectedActor && bIsWallCreationMode) {
        auto CurrentLocation = SelectedActor->GetActorLocation();

        CurrentLocation.X = FMath::RoundToFloat(CurrentLocation.X / WallSnapValue) * WallSnapValue;

        CurrentLocation.Y = FMath::RoundToFloat(CurrentLocation.Y / WallSnapValue) * WallSnapValue;

        SelectedActor->SetActorLocation(CurrentLocation);
    }
}

void AArchVizPlayerController::SpawnSelectedActor(EObjectType Type)
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    // Destroy previous selected actor if exists
    if (SelectedActor && bIsActorSpawning)
    {
        SelectedActor->Destroy();
    }
    else
    {
        SelectedActor = nullptr;
    }

    switch (Type) {
	    case EObjectType::Wall: {
	        SelectedActorType = EObjectType::Wall;
	        SelectedActor = GetWorld()->SpawnActor<AWallActor>(AWallActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	        break;
	    }
	    case EObjectType::Floor: {
	        SelectedActorType = EObjectType::Floor;
	        SelectedActor = GetWorld()->SpawnActor<AFloorActor>(AFloorActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	        break;
	    }

    	case EObjectType::Ceiling: {
	        SelectedActorType = EObjectType::Floor;
	        SelectedActor = GetWorld()->SpawnActor<ACeilingActor>(ACeilingActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	        break;
	    }
	    default: {
	        SelectedActorType = EObjectType::Wall;
	        SelectedActor = GetWorld()->SpawnActor<AWallActor>(AWallActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	        break;
	    }
    }


}

void AArchVizPlayerController::ApplyMaterialWallProceduralMesh(const FMaterialData& MeshData)
{
    UMaterialInterface* BaseMaterial = MeshData.Type;
    if (!BaseMaterial)
    {
        UE_LOG(LogTemp, Error, TEXT("BaseMaterial is nullptr in AArchVizPlayerController"));
        return;
    }

    DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
    if (DynamicMaterial && SelectedActor)
    {
        SelectedActor->GetProceduralMeshComponent()->SetMaterial(0, DynamicMaterial);
    }
}

TPair<ACubeActor*, FVector> AArchVizPlayerController::IsWallActor(const FHitResult& HitResult)
{
    if (HitResult.bBlockingHit)
    {
        const FVector ClickLocation = HitResult.Location;

        if (auto* WallActor = Cast<ACubeActor>(HitResult.GetActor()))
        {
            FVector LocalClickLocation = WallActor->GetTransform().InverseTransformPosition(ClickLocation);
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("World Clicked Location: X=%f, Y=%f, Z=%f"), ClickLocation.X, ClickLocation.Y, ClickLocation.Z));
            return TPair<ACubeActor*, FVector>(WallActor, LocalClickLocation);
        }
    }
    return TPair<ACubeActor*, FVector>(nullptr, FVector::ZeroVector);
}
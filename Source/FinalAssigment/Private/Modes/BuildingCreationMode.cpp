// Fill out your copyright notice in the Description page of Project Settings.


#include "Modes/BuildingCreationMode.h"
#include "AssertManager.h"
#include "SlabActor.h"
#include "WallConstructionWidget.h"

UBuildingCreationMode::UBuildingCreationMode()
{
}

void UBuildingCreationMode::Setup()
{
    if (IsValid(WallCreationModeRef)) {
        WallCreationMode = NewObject<USubModeWallCreation>(this, WallCreationModeRef);
        WallCreationMode->InitParams(PlayerController);
        WallCreationMode->Setup();
    }
 
    /*if (IsValid(FloorCreationModeRef)) {
        FloorCreationMode = NewObject<USubModeFloorCreation>(this, FloorCreationModeRef);
        FloorCreationMode->InitParams(PlayerController);
        FloorCreationMode->Setup();
    }
    if (IsValid(CeilingModeRef)) {
        CeilingCreationMode = NewObject<USubModeCeilingCreation>(this, CeilingModeRef);
        CeilingCreationMode->InitParams(PlayerController);
        CeilingCreationMode->Setup();
    }*/
    if (IsValid(WidgetRef) && !IsValid(Widget)) {
        Widget = CreateWidget<UWallConstructionWidget>(PlayerController, WidgetRef);
    }
}

void UBuildingCreationMode::SetSubMode(UUBuildingCreationSubMode* NewSubMode) {
    if (CurrentBuildingCreationSubMode) {
        CurrentBuildingCreationSubMode->ExitSubMode();
    }

    CurrentBuildingCreationSubMode = NewSubMode;

    if (CurrentBuildingCreationSubMode) {
        CurrentBuildingCreationSubMode->EnterSubMode();
    }
}

void UBuildingCreationMode::SetupInputMapping()
{
   /* UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
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
		InputMappingContext->MapKey(OnDeSelectWall, EKeys::Tab);
		
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("InputMappingContext Not Populated"));
	}

	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(OnWallLeftClick, ETriggerEvent::Started, this, &UBuildingCreationMode::WallLeftClickProcess);
		EnhancedInputComponent->BindAction(OnWallRightClick, ETriggerEvent::Started, this, &UBuildingCreationMode::WallRightClickProcess);
		EnhancedInputComponent->BindAction(OnWallRotate, ETriggerEvent::Started, this, &UBuildingCreationMode::RotateSelectedActor);
		EnhancedInputComponent->BindAction(OnWallDelete, ETriggerEvent::Started, this, &UBuildingCreationMode::DeleteSelectedActor);
		EnhancedInputComponent->BindAction(OnDeSelectWall, ETriggerEvent::Started, this, &UBuildingCreationMode::DeSelectedSelectedActor);
		
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("EnhancedInputComponent Not Populated"));
	}*/

    if (IsValid(WallCreationMode)) {
        WallCreationMode->SetupInputMapping();
    }
  
    if (IsValid(CeilingCreationMode)) {
        CeilingCreationMode->SetupInputMapping();
    }
    if (IsValid(FloorCreationMode)) {
        FloorCreationMode->SetupInputMapping();
    }
}

void UBuildingCreationMode::EnterMode()
{
	Super::ShowWidget();
}

void UBuildingCreationMode::ExitMode()
{
	Super::HideWidget();
}

ACubeActor*& UBuildingCreationMode::GetSelectedActor()
{
    return CurrentBuildingCreationSubMode->GetSelectedActor();
}

//ACubeActor* UBuildingCreationMode::GetSelectedActor()
//{
//	return SelectedActor;
//}

//void UBuildingCreationMode::WallLeftClickProcess()
//{
//    FHitResult HitResult;
//    PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
//
//    // Reset actor spawning state since we are interacting with existing actors
//    bIsActorSpawning = false;
//
//    if (auto [WallActor, LocalClickLocation] = IsWallActor(HitResult); WallActor)
//    {
//        // Clicked on a wall actor, update the selected actor
//        SelectedActor = WallActor;
//
//        // Highlight the selected actor
//        if (SelectedActor)
//        {
//            SelectedActor->GetProceduralMeshComponent()->SetRenderCustomDepth(true);
//            SelectedActor->GetProceduralMeshComponent()->CustomDepthStencilValue = 2.0;
//        }
//
//        // Unhighlight previously selected actor if exists
//        if (PreviousSelectedActor && PreviousSelectedActor != SelectedActor)
//        {
//            PreviousSelectedActor->GetProceduralMeshComponent()->SetRenderCustomDepth(false);
//        }
//
//        // Update the previously selected actor to the current one
//        PreviousSelectedActor = SelectedActor;
//
//        // Additional logic as needed
//        if (AWallActor* tempWallActor = Cast<AWallActor>(WallActor); bIsAddingDoor && tempWallActor)
//        {
//            // If adding a door is requested, update the wall actor
//            tempWallActor->SetIsDoorAdded(bIsAddingDoor);
//            tempWallActor->SetDoorLocation(LocalClickLocation.X);
//            tempWallActor->CreateMesh();
//        }
//        if (ASlabActor* tempSlabActor = Cast<ASlabActor>(WallActor); tempSlabActor)
//        {
//            tempSlabActor->CreateMesh();
//        }
//    }
//    else
//    {
//        // Clicked on empty space, spawn the selected actor at the hit location
//        if (HitResult.bBlockingHit)
//        {
//        	
//            FActorSpawnParameters SpawnParams;
//            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//            if (SelectedActor)
//            {
//                SelectedActor->SetActorLocation(HitResult.Location);
//            }
//            bIsActorSpawning = true;
//            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Actor spawned at cursor location"));
//        }
//    }
//
//    // Additional logic based on selected actor type
//    /*if (Cast<AWallActor>(SelectedActor))
//    {
//        WallWidgetInstance->LengthInput->SetValue(SelectedActor->GetLength());
//        WallWidgetInstance->LengthInput->GetParent()->SetVisibility(ESlateVisibility::Visible);
//        WallWidgetInstance->WidthInput->GetParent()->SetVisibility(ESlateVisibility::Hidden);
//    }
//    else if (Cast<ASlabActor>(SelectedActor))
//    {
//        WallWidgetInstance->LengthInput->GetParent()->SetVisibility(ESlateVisibility::Visible);
//        WallWidgetInstance->WidthInput->GetParent()->SetVisibility(ESlateVisibility::Visible);
//        WallWidgetInstance->LengthInput->SetValue(SelectedActor->GetLength());
//        WallWidgetInstance->WidthInput->SetValue(SelectedActor->GetWidth());
//    }*/
//}

//TPair<ACubeActor*, FVector> UBuildingCreationMode::IsWallActor(const FHitResult& HitResult)
//{
//    if (HitResult.bBlockingHit)
//    {
//        const FVector ClickLocation = HitResult.Location;
//
//        if (auto* WallActor = Cast<ACubeActor>(HitResult.GetActor()))
//        {
//            FVector LocalClickLocation = WallActor->GetTransform().InverseTransformPosition(ClickLocation);
//            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("World Clicked Location: X=%f, Y=%f, Z=%f"), ClickLocation.X, ClickLocation.Y, ClickLocation.Z));
//            return TPair<ACubeActor*, FVector>(WallActor, LocalClickLocation);
//        }
//    }
//    return TPair<ACubeActor*, FVector>(nullptr, FVector::ZeroVector);
//}


//void UBuildingCreationMode::DeSelectedSelectedActor()
//{
//    if (SelectedActor)
//    {
//        if (UProceduralMeshComponent* MeshComponent = SelectedActor->GetProceduralMeshComponent())
//        {
//            MeshComponent->SetRenderCustomDepth(false);
//            MeshComponent->CustomDepthStencilValue = 0; // Resetting to default
//        }
//
//       /* if (Cast<AWallActor>(SelectedActor))
//        {
//            WallWidgetInstance->LengthInput->GetParent()->SetVisibility(ESlateVisibility::Hidden);
//
//        }
//        else if (Cast<ASlabActor>(SelectedActor))
//        {
//            WallWidgetInstance->LengthInput->GetParent()->SetVisibility(ESlateVisibility::Hidden);
//            WallWidgetInstance->WidthInput->GetParent()->SetVisibility(ESlateVisibility::Hidden);
//        }*/
//        SelectedActor = nullptr;
//    }
//}

void UBuildingCreationMode::SpawnSelectedActor(EBuildingCreationType Type)
{
    //FActorSpawnParameters SpawnParams;
    //SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    //// Destroy previous selected actor if exists
    //if (SelectedActor && bIsActorSpawning)
    //{
    //    SelectedActor->Destroy();
    //}
    //else
    //{
    //    SelectedActor = nullptr;
    //}

    //switch (Type) {
	   // case EBuildingCreationType::Wall: {
	   //     SelectedActorType = EBuildingCreationType::Wall;
	   //     SelectedActor = GetWorld()->SpawnActor<AWallActor>(AWallActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	   //     break;
	   // }
	   // case EBuildingCreationType::Floor: {
	   //     SelectedActorType = EBuildingCreationType::Floor;
	   //     SelectedActor = GetWorld()->SpawnActor<AFloorActor>(AFloorActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	   //     break;
	   // }

	   // case EBuildingCreationType::Ceiling: {
	   //     SelectedActorType = EBuildingCreationType::Ceiling;
	   //     SelectedActor = GetWorld()->SpawnActor<ACeilingActor>(ACeilingActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	   //     break;
	   // }
	   // default: {
	   //     SelectedActorType = EBuildingCreationType::Wall;
	   //     SelectedActor = GetWorld()->SpawnActor<AWallActor>(AWallActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	   //     break;
	   // }
    //}

    switch (Type) {
	    case EBuildingCreationType::Wall:
            SetSubMode(WallCreationMode);
		    break;
	    case EBuildingCreationType::Floor:
            SetSubMode(FloorCreationMode);
		    break;
	    case EBuildingCreationType::Ceiling:
            SetSubMode(CeilingCreationMode);
		    break;
    }
}

void UBuildingCreationMode::ApplyMaterialWallProceduralMesh(const FMaterialData& MeshData)
{
    UMaterialInterface* BaseMaterial = MeshData.Type;
    if (!BaseMaterial)
    {
        UE_LOG(LogTemp, Error, TEXT("BaseMaterial is nullptr in AArchVizPlayerController"));
    }

    if (auto DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this))
    {
    	CurrentBuildingCreationSubMode->SetMaterial(DynamicMaterial);
    }
}
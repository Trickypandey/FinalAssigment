#include "ArchVizPlayerController.h"
#include "FinalAssigment/WallActor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SlabActor.h"
#include "UiWidget.h"
#include "Kismet/GameplayStatics.h"

AArchVizPlayerController::AArchVizPlayerController()
{
    PrimaryActorTick.bCanEverTick = true;
    bShowMouseCursor = true;
    bIsWallCreationMode = false;
    bIsRoadConstructionMode = false;
    bIsActorSpawning = false;
    bIsAddingDoor = true;
    SelectedActor = nullptr;
    SelectedActorType = EObjectType::Wall;
   
}

void AArchVizPlayerController::SpawnSelectedActor(EObjectType Type)
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    // Destroy previous selected actor if exists
    if (SelectedActor)
    {
        SelectedActor->Destroy();
    }

    switch (Type) {
    case EObjectType::Wall: {
        SelectedActorType = EObjectType::Wall; 
        SelectedActor = GetWorld()->SpawnActor<AWallActor>(AWallActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
        break;
    }
    case EObjectType::Slab: {
        SelectedActorType = EObjectType::Slab; 
        SelectedActor = GetWorld()->SpawnActor<ASlabActor>(ASlabActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
        break;
    }
    default: {
        SelectedActorType = EObjectType::Wall; 
        SelectedActor = GetWorld()->SpawnActor<AWallActor>(AWallActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
        break;
    }
    }
}


void AArchVizPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    SetupEnhancedInputBindings();
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

void AArchVizPlayerController::AddCurrentModeMappingContext() const
{
    auto* SubSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (!SubSystem || (bIsWallCreationMode && bIsRoadConstructionMode))
    {
        return;
    }
    SubSystem->ClearAllMappings();

    UInputMappingContext* ContextToRemove = bIsWallCreationMode ? WallMappingContext : RoadMappingContext;
    UInputMappingContext* ContextToAdd = bIsRoadConstructionMode ? RoadMappingContext : WallMappingContext;

    if (ContextToRemove)
    {
        SubSystem->RemoveMappingContext(ContextToRemove);
    }
    if (ContextToAdd)
    {
        SubSystem->AddMappingContext(ContextToAdd, 0);
    }
}

void AArchVizPlayerController::SetupEnhancedInputBindings()
{
    UEnhancedInputComponent* Eic = Cast<UEnhancedInputComponent>(InputComponent);
    WallMappingContext = NewObject<UInputMappingContext>(this);
    RoadMappingContext = NewObject<UInputMappingContext>(this);

    OnLeftClick = NewObject<UInputAction>(this);
    OnLeftClick->ValueType = EInputActionValueType::Boolean;

    OnRightClick = NewObject<UInputAction>(this);
    OnRightClick->ValueType = EInputActionValueType::Boolean;

    OnRotate = NewObject<UInputAction>(this);
    OnRotate->ValueType = EInputActionValueType::Boolean;

    // Set up input actions within the mapping contexts
    WallMappingContext->MapKey(OnLeftClick, EKeys::LeftMouseButton);
    WallMappingContext->MapKey(OnRightClick, EKeys::RightMouseButton);
    WallMappingContext->MapKey(OnRotate, EKeys::R);

    // Ensure the enhanced input component is valid before binding actions
    check(Eic);
    Eic->BindAction(OnLeftClick, ETriggerEvent::Started, this, &AArchVizPlayerController::LeftClickProcess);
    Eic->BindAction(OnRightClick, ETriggerEvent::Started, this, &AArchVizPlayerController::RightClickProcess);
    Eic->BindAction(OnRotate, ETriggerEvent::Started, this, &AArchVizPlayerController::RotateSelectedActor);

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

    if (!UiWidgetInstance && UiWidgetClass )
    {
        UiWidgetInstance = CreateWidget<UUiWidget>(this, UiWidgetClass);
        if (UiWidgetInstance)
        {
            UiWidgetInstance->AddToViewport();
        }
    }
}

void AArchVizPlayerController::LeftClickProcess()
{
    FHitResult HitResult;
    GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

    // Reset actor spawning state since we are interacting with existing actors
    bIsActorSpawning = false;

    if (auto [WallActor, LocalClickLocation] = IsWallWallActor(HitResult); WallActor)
    {
        // Clicked on a wall actor, update the selected actor
        SelectedActor = WallActor;
        AWallActor* tempwallactor= Cast<AWallActor>(WallActor);
        if (bIsAddingDoor && tempwallactor)
        {
            // If adding a door is requested, update the wall actor
            tempwallactor->SetIsDoorAdded(bIsAddingDoor);
            tempwallactor->SetDoorLocation(LocalClickLocation.X);
            tempwallactor->CreateWallMesh();
        }
    }
    else if (SelectedActor)
    {
        // Clicked on empty space, move the selected actor to the hit location
        //SelectedActor->SetActorLocation(HitResult.Location);
        bIsActorSpawning = true;
    }
}

void AArchVizPlayerController::RightClickProcess()
{
    FHitResult HitResult;
    GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
    bIsActorSpawning = true;
    if (HitResult.bBlockingHit)
    {
        FVector spawnLocation = HitResult.Location;
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        if (SelectedActorType == EObjectType::Wall)
        {
	        
			SelectedActor = GetWorld()->SpawnActor<AWallActor>(AWallActor::StaticClass(), spawnLocation, FRotator::ZeroRotator, SpawnParams);
        }
        if (SelectedActorType == EObjectType::Slab)
        {
			SelectedActor = GetWorld()->SpawnActor<ASlabActor>(ASlabActor::StaticClass(), spawnLocation, FRotator::ZeroRotator, SpawnParams);
	        
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

void AArchVizPlayerController::ModeChangeHandle(const FString& Mode)
{
    if (Mode == "Wall Creation Mode")
    {
        if(WallWidgetClass)
        {
	        WallWidgetInstance=CreateWidget<UWallConstructionWidget>(this, WallWidgetClass);
            WallWidgetInstance->AddToViewport();
            bIsWallCreationMode = true;
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("asdfasdfasfdasdfas"));
            AddCurrentModeMappingContext();
        }
    }
}



TPair<ACubeActor*, FVector> AArchVizPlayerController::IsWallWallActor(const FHitResult& HitResult)
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


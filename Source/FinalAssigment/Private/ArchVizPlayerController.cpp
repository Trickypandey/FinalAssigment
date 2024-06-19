#include "ArchVizPlayerController.h"
#include "FinalAssigment/WallActor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

AArchVizPlayerController::AArchVizPlayerController()
{
    PrimaryActorTick.bCanEverTick = true;
    bShowMouseCursor = true;
    bIsWallCrationMode = true;
    bIsRoadConstructionMode = false;
    SelectedActor = nullptr;
}

void AArchVizPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    SetupEnhancedInputBindings();
}

void AArchVizPlayerController::SnapWall()
{
    if (SelectedActor && bIsWallCrationMode) {
        auto CurrentLocation = SelectedActor->GetActorLocation();

        CurrentLocation.X = FMath::RoundToFloat(CurrentLocation.X / WallSnapValue) * WallSnapValue;

        CurrentLocation.Y = FMath::RoundToFloat(CurrentLocation.Y / WallSnapValue) * WallSnapValue;

        SelectedActor->SetActorLocation(CurrentLocation);
    }
}

void AArchVizPlayerController::AddCurrentModeMappingContext() const
{
    auto* SubSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (!SubSystem || (bIsWallCrationMode && bIsRoadConstructionMode))
    {
        return;
    }
    SubSystem->ClearAllMappings();

    UInputMappingContext* ContextToRemove = bIsWallCrationMode ? WallMappingContext : RoadMappingContext;
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

    AddCurrentModeMappingContext();

}

void AArchVizPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (SelectedActor)
    {
        FHitResult HitResult;
        FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, SelectedActor);

        FVector CursorWorldLocation;
        FVector CursorWorldDirection;
        DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);

        if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams)) {
            FVector NewLocation = HitResult.Location;
            //NewLocation.Z = 0;
            SelectedActor->SetActorRelativeLocation(NewLocation);
            SnapWall();
        }
    }
}

void AArchVizPlayerController::LeftClickProcess()
{
    FHitResult HitResult;
    GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

    if (auto [WallActor, LocalClickLocation] = IsWallWallActor(HitResult); WallActor)
    {
        WallActor->SetDoorLocartion(LocalClickLocation.X);
        WallActor->CreateWallMesh();

        if (SelectedActor && SelectedActor != WallActor)
        {
            SelectedActor->SetActorLocation(OriginalLocation);
        }

        SelectedActor = WallActor;
        OriginalLocation = SelectedActor->GetActorLocation();
    }
    else if (HitResult.GetActor())
    {
        if (SelectedActor)
        {
            SelectedActor->SetActorLocation(OriginalLocation);
        }

        SelectedActor = HitResult.GetActor();
        OriginalLocation = SelectedActor->GetActorLocation();
    }
    else
    {
        if (SelectedActor)
        {
            SelectedActor->SetActorLocation(OriginalLocation);
        }

        SelectedActor = nullptr;
    }
}

void AArchVizPlayerController::RightClickProcess()
{
    FHitResult HitResult;
    GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

    if (HitResult.bBlockingHit)
    {
        FVector spawnLocation = HitResult.Location;
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        auto* SpawnedActor = GetWorld()->SpawnActor<AWallActor>(AWallActor::StaticClass(), spawnLocation, FRotator::ZeroRotator, SpawnParams);
        SelectedActor = SpawnedActor;
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Actor spawned at cursor location"));
    }
}

void AArchVizPlayerController::RotateSelectedActor()
{
    if (SelectedActor)
    {
        SelectedActor->AddActorLocalRotation(FRotator(0.f, 80.f, 0.f));
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Actor rotated by 80 degrees"));
    }
}

TPair<AWallActor*, FVector> AArchVizPlayerController::IsWallWallActor(const FHitResult& HitResult)
{
    if (HitResult.bBlockingHit)
    {
        const FVector ClickLocation = HitResult.Location;

        if (AWallActor* WallActor = Cast<AWallActor>(HitResult.GetActor()))
        {
            FVector LocalClickLocation = WallActor->GetTransform().InverseTransformPosition(ClickLocation);
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("World Clicked Location: X=%f, Y=%f, Z=%f"), ClickLocation.X, ClickLocation.Y, ClickLocation.Z));
            return TPair<AWallActor*, FVector>(WallActor, LocalClickLocation);
        }
    }
    return TPair<AWallActor*, FVector>(nullptr, FVector::ZeroVector);
}

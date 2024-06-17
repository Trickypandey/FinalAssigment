// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizPlayerController.h"

#include "FinalAssigment/WallActor.h"

AArchVizPlayerController::AArchVizPlayerController()
{
    PrimaryActorTick.bCanEverTick = true;
    bShowMouseCursor = true;
	
}

void AArchVizPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	SetupEnhancedInputBindings();
}

void AArchVizPlayerController::SetupEnhancedInputBindings()
{
    UEnhancedInputComponent* Eic = Cast<UEnhancedInputComponent>(InputComponent);
    MappingContext = NewObject<UInputMappingContext>(this);

    OnLeftClick = NewObject<UInputAction>(this);
    OnLeftClick->ValueType = EInputActionValueType::Boolean;


    check(Eic)
        Eic->BindAction(OnLeftClick, ETriggerEvent::Started, this, &AArchVizPlayerController::LeftClickProcess);

    if (MappingContext) {
        MappingContext->MapKey(OnLeftClick, EKeys::LeftMouseButton);

        if (auto* SubSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())

        {
            SubSystem->AddMappingContext(MappingContext, 0);
        }
    }
}

void AArchVizPlayerController::LeftClickProcess()
{
    FHitResult HitResult;
    GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

    if (auto [WallActor , LocalClickLocation] = IsWallWallActor(HitResult); WallActor)
    {
        WallActor->SetDoorLocartion(LocalClickLocation.X);
        WallActor->CreateWallMesh();


        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Local Clicked Location: X=%f, Y=%f, Z=%f"), LocalClickLocation.X, LocalClickLocation.Y, LocalClickLocation.Z));


    
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
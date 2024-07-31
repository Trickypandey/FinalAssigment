#include "WallActor.h"

AWallActor::AWallActor()
{
    PrimaryActorTick.bCanEverTick = true;
    ConstructionType = EBuildingCreationType::Wall;
    WallState = EBuildingSubModeState::Moving;
    Length = 100;
    Width = 20;
    Height = 400;
    
    SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    SceneComponent->RegisterComponentWithWorld(GetWorld());
    RootComponent = SceneComponent;
}

void AWallActor::BeginPlay()
{
    Super::BeginPlay();
  
    /*if (DoorMesh && IsDoorAdded)
    {
        AttachDoor(DoorMesh);
    }*/
}

void AWallActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    CreateWallSegment();
   
    /*if (DoorMesh && IsDoorAdded)
    {
        AttachDoor(DoorMesh);
    }*/
}



void AWallActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    switch (WallState) {
	    case EBuildingSubModeState::Placed:
	        HandlePlacedState();
	        break;
	    case EBuildingSubModeState::Moving:
	        HandleMovingState();
	        break;
		case EBuildingSubModeState::Constructing:
            HandleConstructingState();
    	    break;
    }

   
}



void AWallActor::SetWallEndLocation(FVector X)
{
    for (int32 i = WallSegments.Num() - 1; i >= 0; --i)
    {
        if (WallSegments.IsValidIndex(i))
        {
            auto* WallToRemove = WallSegments[i];
            if (WallToRemove)
            {
                WallToRemove->Destroy();
                WallToRemove = nullptr;
            }
        }
    }
    WallSegments.Empty();
    bHasDoorWall.Empty();

   

    Endlocation = Utility::SnapToGrid(X, FVector(100,100,10));;
    CreateWallSegment();
}

void AWallActor::SetWallStartLocation(FVector X)
{
    Startlocation = Utility::SnapToGrid(X, FVector(100,100,10));
}

FVector AWallActor::GetWallEndLocation()
{
    return Endlocation;
}

FVector AWallActor::GetWallStartLocation()
{
    return Startlocation;
}

void AWallActor::SetIsDoorAdded(bool Flag)
{
    IsDoorAdded = Flag;
   
}

bool AWallActor::GetDoorFlag()
{
    return IsDoorAdded;
}

void AWallActor::SetDimension(int32 _Length, int32 _Width)
{
    this->Length = _Length;
    this->Width = _Width;
    
}



void AWallActor::SetMaterial(UMaterialInterface* NewMaterial)
{
    if (NewMaterial)
    {
        Material = NewMaterial;
        for (auto Wall : WallSegments)
        {
            if (Wall)
            {
                Wall->GetProceduralMeshComponent()->SetMaterial(0, Material);
            }
        }
        for (auto* DoorWall : DoorWalls)
        {
            if (DoorWall)
            {
                DoorWall->GetProceduralMeshComponent()->SetMaterial(0, Material);
            }
        }
    }
}

void AWallActor::HandleMovingState()
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        FHitResult HitResult;
        FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true);

        // Ignore this actor and its segments
        TraceParams.AddIgnoredActor(this);
    /*    for (ACubeActor* WallSegment : WallSegments)
        {
            TraceParams.AddIgnoredActor(WallSegment);
        }*/

        FVector CursorWorldLocation;
        FVector CursorWorldDirection;
        PlayerController->DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);

        FVector Start = CursorWorldLocation;
        FVector End = CursorWorldLocation + CursorWorldDirection * 10000;

        if (GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, FCollisionObjectQueryParams(ECC_WorldStatic), TraceParams))
        {
            FVector NewLocation = Utility::SnapToGrid(HitResult.Location,FVector(20)); ;
            SetActorLocation(NewLocation);


        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Line trace did not hit any actor."));
        }
    }
}



void AWallActor::HandleConstructingState()
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {

        FHitResult HitResult;
        FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, this);

        FVector CursorWorldLocation;
        FVector CursorWorldDirection;
        PlayerController->DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);

        if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams))
        {
           
            SetWallEndLocation(HitResult.Location);
         
            //CreateWallSegment();
           
        }

    }

}

void AWallActor::HandlePlacedState()
{
}

void AWallActor::CreateWallSegment()
{   
    float Distance = 0;
    FVector& StartPoint = Startlocation;
    FVector& EndPoint = Endlocation;
    if (FMath::Abs(StartPoint.X - EndPoint.X) > FMath::Abs(StartPoint.Y - EndPoint.Y))
    {
        Distance = FMath::Abs(StartPoint.X - EndPoint.X);
        EndPoint.Y = StartPoint.Y;
    }
    else
    {
        Distance = FMath::Abs(StartPoint.Y - EndPoint.Y);
        EndPoint.X = StartPoint.X;
    }

    NumberOfSegment = FMath::CeilToInt(Distance / 100.0f);
    //bHasDoorWall.Reserve(NumberOfWalls);
    for (int i = 0; i < NumberOfSegment; i++)
    {
        bHasDoorWall.Add(false);
    }
    FVector Direction = (EndPoint - StartPoint).GetSafeNormal();

    FVector RoundedDirection;
    if (FMath::Abs(Direction.X) >= FMath::Abs(Direction.Y))
    {
        RoundedDirection = FVector(FMath::RoundToInt(Direction.X), 0.0f, 0.0f).GetSafeNormal();
    }
    else
    {
        RoundedDirection = FVector(0.0f, FMath::RoundToInt(Direction.Y), 0.0f).GetSafeNormal();
    }

    Rotation = RoundedDirection.Rotation();

    FVector MidPoint = (StartPoint + EndPoint) / 2.0f;
    //SetActorLocation(MidPoint);

    for (int i = 0; i < NumberOfSegment - 1; ++i)
    {
        FVector SpawnLocation = StartPoint + RoundedDirection * i * 100.0f;
        FVector end = StartPoint + RoundedDirection * (i + 1) * 100.0f;

        FVector Offset = FVector::ZeroVector;

        float Yaw = Rotation.Yaw;

        if (FMath::Abs(Yaw) <= 5 || FMath::Abs(Yaw - 360) <= 5)
        {
            Offset.X = Width / 2.0f;
        }
        else if (FMath::Abs(Yaw - 180) <= 5)
        {
            Offset.X = -Width / 2.0f;
        }
        else if (FMath::Abs(Yaw - 90) <= 5)
        {
            Offset.Y = Width / 2.0f;
        }
        else if (FMath::Abs(Yaw + 90) <= 5)
        {
            Offset.Y = -Width / 2.0f;
        }

        SpawnLocation += Offset;
        end += Offset;
        SpawnMesh((SpawnLocation + end )/2);
        //SpawnMesh(SpawnLocation);
    }

}



void AWallActor::SpawnMesh(FVector SpawnLocation)
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    //ACubeActor* NewWall = GetWorld()->SpawnActor<ACubeActor>(ACubeActor::StaticClass(), SpawnLocation, Rotation, SpawnParams);
    ACubeActor* NewWall = NewObject<ACubeActor>(this);
    if (NewWall)
    {

        NewWall->SetDimension(Length, Width, Height);
        NewWall->AttachToComponent(SceneComponent, FAttachmentTransformRules::KeepRelativeTransform);
        NewWall->SetActorLocation(SpawnLocation);
        NewWall->SetActorRotation(Rotation);
	    NewWall->RegisterAllComponents();
        if(Material)
        {
            NewWall->GetProceduralMeshComponent()->SetMaterial(0, Material);
        }
        WallSegments.Add(NewWall);
    }

}


void AWallActor::ReplaceDoorWithWall()
{
    if (currentDoorActor)
    {
        // Find the index of the current door actor in the DoorWalls array
        int32 DoorIndex = DoorWalls.IndexOfByKey(currentDoorActor);
        if (DoorIndex != INDEX_NONE)
        {
            // Set the corresponding bHasDoorWall entry to false (no door)
            bHasDoorWall.RemoveAt(DoorIndex);
            bHasDoorWall.Add( false);
        }

    	currentDoorActor->ProceduralMesh->SetRenderCustomDepth(false);
        FVector DoorLocation = currentDoorActor->GetActorLocation();
        currentDoorActor->Destroy();
        currentDoorActor = nullptr;

        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        //ACubeActor* NewWall = GetWorld()->SpawnActor<ACubeActor>(ACubeActor::StaticClass(), SpawnLocation, Rotation, SpawnParams);
        ACubeActor* NewWall = NewObject<ACubeActor>(this);
        if (NewWall)
        {

            NewWall->SetDimension(Length, Width, Height);
            NewWall->AttachToComponent(SceneComponent, FAttachmentTransformRules::KeepRelativeTransform);
            NewWall->SetActorLocation(DoorLocation);
            NewWall->SetActorRotation(Rotation);
            NewWall->RegisterAllComponents();
            if (Material)
            {
                NewWall->GetProceduralMeshComponent()->SetMaterial(0, Material);
            }
            WallSegments.Add(NewWall);
        }
    }
}


void AWallActor::AddDoor(ACubeActor*& actor)
{
    
    for (int i = 0; i < WallSegments.Num(); i++)
    {
        if (WallSegments[i] == actor)
        {
             bHasDoorWall[i] = true;
            break;
        }
    }
    FVector location = actor->GetActorLocation();
    actor->Destroy();
    actor = nullptr;
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    // Spawn the door actor
    if (AAWallDoorActor* NewDoor = GetWorld()->SpawnActor<AAWallDoorActor>(location, Rotation, SpawnParams))
    {
        NewDoor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
        NewDoor->SetParentActor(this);
        
        currentDoorActor = NewDoor;

        if (Material)
        {
            NewDoor->SetMaterial(Material);
        }
        DoorWalls.Add(NewDoor);
    }

}

void AWallActor::HighlightSelectedActor()
{
    // Highlight this actor's components
    

    // Loop through each component and set custom depth
    for (auto* ActorComponent : WallSegments)
    {
        if (ActorComponent && ActorComponent->GetProceduralMeshComponent())
        {
            ActorComponent->GetProceduralMeshComponent()->SetRenderCustomDepth(true);
            ActorComponent->GetProceduralMeshComponent()->SetCustomDepthStencilValue(2);
        }
    }

    // Highlight the attached door walls' procedural meshes
	if (currentDoorActor && IsDoorAdded)
    {
        currentDoorActor->ProceduralMesh->SetRenderCustomDepth(true);
        currentDoorActor->ProceduralMesh->SetCustomDepthStencilValue(2);
    }
    
}

void AWallActor::UnhighlightDeselectedActor()
{
    TSet<UActorComponent*> ActorComponents = GetComponents();

    if (currentDoorActor && IsDoorAdded)
    {
        currentDoorActor->ProceduralMesh->SetRenderCustomDepth(false);
        
    }

    for (auto* ActorComponent : WallSegments)
    {
        if (ActorComponent && ActorComponent->GetProceduralMeshComponent())
        {
            ActorComponent->GetProceduralMeshComponent()->SetRenderCustomDepth(false);
        }
    }

}

void AWallActor::RotateWall(float Angle)
{
    SetActorRotation(GetActorRotation() + FRotator(0,Angle,0));
}



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

int AWallActor::GetWallIndexFromLocation(FVector Location) const
{
    FVector WallDirection = (Endlocation - Startlocation).GetSafeNormal();
    float DistanceAlongWall = FVector::DotProduct(Location - Startlocation, WallDirection);

    int32 WallIndex = FMath::FloorToInt(DistanceAlongWall / Length);
    WallIndex = FMath::Clamp(WallIndex, 0, bHasDoorWall.Num() - 1);
    

    return WallIndex;
	
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


void AWallActor::ReplaceDoorWithWall(AAWallDoorActor* DoorWallActor)
{
    if (DoorWallActor)
    {
        /*FVector DoorWallLocation = DoorWallActor->GetActorLocation();
        DoorWallActor->Destroy();

        int32 IndexToRemove = DoorWalls.IndexOfByKey(DoorWallActor);
        if (IndexToRemove != INDEX_NONE)
        {
            DoorWalls.RemoveAt(IndexToRemove);
        }
        int32 doorwallI = GetWallIndexFromLocation(DoorWallLocation);
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Wall Index: %d"), doorwallI));
        bHasDoorWall[doorwallI] = false;
        SpawnMesh(DoorWallLocation);*/
    }
}

void AWallActor::AddDoor(const FVector& Vector)
{
    int ReplaceIdx = GetWallIndexFromLocation(Vector);
    ReplaceWallWithDoor(ReplaceIdx);

}

void AWallActor::ReplaceWallWithDoor(int32 index)
{
    FVector Location = GetLocationFromIndex(index);
    ReplaceWallWithDoor(Location);
    

}

void AWallActor::ReplaceWallWithDoor(FVector HitLocation)
{
    float ClosestDistance = FLT_MAX;
    int ClosestIndex = -1;

    for (int32 i = 0; i < WallSegments.Num(); ++i)
    {
        float Distance = FVector::Dist(WallSegments[i]->GetActorLocation(), HitLocation);
        if (Distance < ClosestDistance)
        {
            ClosestDistance = Distance;
            ClosestIndex = i;
        }
    }

    FVector WallLocation = FVector::ZeroVector;
    if (ClosestIndex != -1)
    {
        //bHasDoorWall.Add(ClosestIndex);

        ACubeActor* WallToRemove = WallSegments[ClosestIndex];
        if (WallToRemove)
        {
            WallLocation = WallToRemove->GetActorLocation();
            WallToRemove->Destroy();
            WallToRemove = nullptr;
            WallSegments.RemoveAt(ClosestIndex);
        }
        int32 wallI = GetWallIndexFromLocation(WallLocation);

        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Wall Index: %d"), wallI));

        bHasDoorWall[wallI] = true;
        SelectedDoorIdx = wallI;

        SpawnDoorActor(WallLocation);
    }
}

void AWallActor::SpawnDoorActor(FVector SpawnLocation)
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    // Spawn the door actor
    if (AAWallDoorActor* NewDoor = GetWorld()->SpawnActor<AAWallDoorActor>(SpawnLocation, Rotation, SpawnParams))
    {
        // Attach the door to this wall actor
        NewDoor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

        // Set the parent actor
        NewDoor->SetParentActor(this);

        // Optionally, add it to a container if needed (e.g., if you need to manage them later)
        if(Material)
        {
            NewDoor->SetMaterial(Material);
        }
        DoorWalls.Add(NewDoor);
    }
}

FVector AWallActor::GetLocationFromIndex(int32 Index) const
{
    // Ensure the index is within bounds
    if (Index < 0 || Index >= WallSegments.Num() -1 )
    {
        return FVector::ZeroVector; // Return zero vector if index is out of bounds
    }

    FVector Direction = (Endlocation - Startlocation).GetSafeNormal();

    FVector RoundedDirection;
    if (FMath::Abs(Direction.X) >= FMath::Abs(Direction.Y))
    {
        RoundedDirection = FVector(FMath::RoundToInt(Direction.X), 0.0f, 0.0f).GetSafeNormal();
    }
    else
    {
        RoundedDirection = FVector(0.0f, FMath::RoundToInt(Direction.Y), 0.0f).GetSafeNormal();
    }

    FVector SpawnLocation = Startlocation + RoundedDirection * Index * 100.f;

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
    return SpawnLocation;
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
    if (IsDoorAdded)
    {
        /*AAWallDoorActor* DoorWall = DoorWalls[SelectedDoorIdx];
        if (DoorWall && DoorWall->ProceduralMesh)
        {
            DoorWall->ProceduralMesh->SetRenderCustomDepth(true);
            DoorWall->ProceduralMesh->SetCustomDepthStencilValue(2);
        }*/
    }
    else
    {
	    for (AAWallDoorActor* DoorWall : DoorWalls)
	    {
	        if (DoorWall && DoorWall->ProceduralMesh)
	        {
	            DoorWall->ProceduralMesh->SetRenderCustomDepth(true);
	            DoorWall->ProceduralMesh->SetCustomDepthStencilValue(2);
	        }
	    }
    }
}


void AWallActor::UnhighlightDeselectedActor()
{
    TSet<UActorComponent*> ActorComponents = GetComponents();

    for (auto* ActorComponent : WallSegments)
    {
        if (ActorComponent && ActorComponent->GetProceduralMeshComponent())
        {
            ActorComponent->GetProceduralMeshComponent()->SetRenderCustomDepth(false);
        }
    }

    for (AAWallDoorActor* DoorWall : DoorWalls)
    {
        if (DoorWall)
        {
            DoorWall->ProceduralMesh->SetRenderCustomDepth(false);
        }
    }
}

void AWallActor::RotateWall(float Angle)
{
    SetActorRotation(GetActorRotation() + FRotator(0,Angle,0));
}



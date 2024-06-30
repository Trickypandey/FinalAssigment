// Fill out your copyright notice in the Description page of Project Settings.


#include "InteriorDesignActor.h"

// Sets default values
AInteriorDesignActor::AInteriorDesignActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InteriorMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Interior Mesh Component"));
	RootComponent = InteriorMeshComponent;

	InteriorState = EBuildingSubModeState::Placed;

}

UStaticMesh* AInteriorDesignActor::GetStaticMesh()
{
	return InteriorMeshComponent->GetStaticMesh();
}

void AInteriorDesignActor::SetStaticMesh(UStaticMesh* StaticMesh)
{
	if (InteriorMeshComponent && StaticMesh)
		InteriorMeshComponent->SetStaticMesh(StaticMesh);
}

// Called when the game starts or when spawned
void AInteriorDesignActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteriorDesignActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	switch (InteriorState) {
		case EBuildingSubModeState::Placed:
			HandlePlacedState();
			break;
		case EBuildingSubModeState::Moving:
			HandleMovingState();
			break;
	}
}

void AInteriorDesignActor::HandlePlacedState()
{


}

void AInteriorDesignActor::HandleMovingState()
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
			FVector NewLocation = HitResult.Location;
			FVector SnappedLocation = Utility::SnapToGrid(NewLocation, FVector(20));
			SetActorLocation(NewLocation);
		}

	}
}

void AInteriorDesignActor::SetActorAttachebalType(EBuildingAttachable Attacheble)
{
	AttachebleTo = Attacheble;
}



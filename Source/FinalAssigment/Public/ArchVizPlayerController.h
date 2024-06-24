#include "CoreMinimal.h"
#include "CubeActor.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "RoadActor.h"
#include "RoadCreationWidget.h"
#include "UiWidget.h"
#include "ArchVizPlayerController.generated.h"

class AWallActor;

UCLASS()
class FINALASSIGMENT_API AArchVizPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AArchVizPlayerController();
    void SetIsAddingDoor(bool);
   
    void ModeChangeHandle(const FString&);
    void DeleteSelectedActor();
    void SpawnSelectedActor(EObjectType  Type);

	ACubeActor* GetSelectedActor();
    UFUNCTION()
    void ApplyMaterialWallProceduralMesh(const FMaterialData& MeshData);

    UFUNCTION()
    void AddMaterialToRoad(const FMaterialData& MeshData);

protected:
    virtual void SetupInputComponent() override;
    void SnapWall();
    virtual void Tick(float DeltaTime) override;
    virtual void BeginPlay() override;

private:
    void RoadLeftClick();
    void RoadRightClick();
    void SetupEnhancedInputBindings();
    void AddCurrentModeMappingContext() const;
    void WallLeftClickProcess();
    void WallRightClickProcess();
    void RotateSelectedActor();

    TPair<ACubeActor*, FVector> IsWallWallActor(const FHitResult& HitResult);

    UPROPERTY()
    UInputMappingContext* WallMappingContext;

    UPROPERTY()
    UInputMappingContext* RoadMappingContext;

    UPROPERTY()
    UUiWidget* UiWidgetInstance;

	UPROPERTY()
    UWallConstructionWidget* WallWidgetInstance;

	UPROPERTY()
    URoadCreationWidget* RoadWidgetInstance;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> UiWidgetClass;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> WallWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> RoadWidgetClass;

    UPROPERTY()
    UInputAction* OnWallLeftClick;

    UPROPERTY()
    UInputAction* OnWallRightClick;

    UPROPERTY()
    UInputAction* OnWallRotate;

	UPROPERTY()
    UInputAction* OnWallDelete;

	UPROPERTY()
    UInputAction* OnRoadAddPoint;

    UPROPERTY()
    UInputAction* OnRoadRightClick;


    bool bIsWallCreationMode;
    bool bIsRoadConstructionMode;
    bool bIsActorSpawning;
    bool bIsAddingDoor;
    bool IsSlapOrFloor = false;
    float WallSnapValue = 20;

    FVector OriginalLocation;
    UMaterialInstanceDynamic* DynamicMaterial;;

    UPROPERTY()
    ACubeActor* SelectedActor;

	UPROPERTY()
    ACubeActor* PreviousSelectedActor;

	UPROPERTY()
    AActor* HoveringActor;

    EObjectType SelectedActorType;

    UPROPERTY()
    TArray<ARoadActor*> RoadArray;

    UPROPERTY()
    ARoadActor* CurrentRoadActor;
};

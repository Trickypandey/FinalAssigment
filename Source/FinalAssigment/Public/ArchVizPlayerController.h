#include "CoreMinimal.h"
#include "CubeActor.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
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

	ACubeActor& GetSelectedActor();

protected:
    virtual void SetupInputComponent() override;
    void SnapWall();
    virtual void Tick(float DeltaTime) override;
    virtual void BeginPlay() override;

private:
    void SetupEnhancedInputBindings();
    void AddCurrentModeMappingContext() const;
    void LeftClickProcess();
    void RightClickProcess();
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

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> UiWidgetClass;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> WallWidgetClass;

    UPROPERTY()
    UInputAction* OnLeftClick;

    UPROPERTY()
    UInputAction* OnRightClick;

    UPROPERTY()
    UInputAction* OnRotate;

	UPROPERTY()
    UInputAction* OnDelete;


    bool bIsWallCreationMode;
    bool bIsRoadConstructionMode;
    bool bIsActorSpawning;
    bool bIsAddingDoor;;
    bool IsSlapOrFloor = false;
    float WallSnapValue = 15;

    UPROPERTY()
    ACubeActor* SelectedActor;
    FVector OriginalLocation;
	UPROPERTY()
    AActor* HoveringActor;

    EObjectType SelectedActorType;
   
};

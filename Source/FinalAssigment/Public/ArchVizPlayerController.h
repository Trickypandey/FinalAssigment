#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "ArchVizPlayerController.generated.h"

class AWallActor;

UCLASS()
class FINALASSIGMENT_API AArchVizPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AArchVizPlayerController();

protected:
    virtual void SetupInputComponent() override;
    void SnapWall();
    virtual void Tick(float DeltaTime) override;

private:
    void SetupEnhancedInputBindings();
    void AddCurrentModeMappingContext() const;
    void LeftClickProcess();
    void RightClickProcess();
    void RotateSelectedActor();

    TPair<AWallActor*, FVector> IsWallWallActor(const FHitResult& HitResult);

    UPROPERTY()
    UInputMappingContext* WallMappingContext;

    UPROPERTY()
    UInputMappingContext* RoadMappingContext;

    UPROPERTY()
    UInputAction* OnLeftClick;

    UPROPERTY()
    UInputAction* OnRightClick;

    UPROPERTY()
    UInputAction* OnRotate;

    bool bIsWallCrationMode;
    bool bIsRoadConstructionMode;
    float WallSnapValue = 20;

    AActor* SelectedActor;
    FVector OriginalLocation;
};

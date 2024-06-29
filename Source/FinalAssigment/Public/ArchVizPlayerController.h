#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UiWidget.h"
#include "FinalAssigment/Public/Modes/BaseMode.h"
#include "FinalAssigment/Public/Modes/BuildingCreationMode.h"
#include "FinalAssigment/Public/Modes/InteriorDesignMode.h"
#include "FinalAssigment/Public/Modes/RoadCreationMode.h"
#include "ArchVizPlayerController.generated.h"



UCLASS()
class FINALASSIGMENT_API AArchVizPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AArchVizPlayerController();
    void SetIsAddingDoor(bool);
   
    void ModeChangeHandle(EModes Mode);

    void SaveGame();
    void LoadGame();
  
protected:
    virtual void SetupInputComponent() override;
    virtual void Tick(float DeltaTime) override;
    virtual void BeginPlay() override;

private:


    void SetArchVizMode(UBaseMode* NewArchVizMode);


    UPROPERTY()
    UUiWidget* UiWidgetInstance;


    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> UiWidgetClass;


    UPROPERTY()
    UBaseMode* CurrentModeClass;

    
    EModes CurrentMode;

    FInputModeGameAndUI InputMode;

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mode", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<URoadCreationMode> RoadConstructionModeRef;

    UPROPERTY(VisibleDefaultsOnly, Category = "Mode")
    URoadCreationMode* RoadConstructionMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mode", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<UBuildingCreationMode> BuildingConstructionModeRef;

    UPROPERTY(VisibleDefaultsOnly, Category = "Mode")
    UBuildingCreationMode* BuildingConstructionMode;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mode", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<UInteriorDesignMode> InteriorDesignModeRef;

    UPROPERTY(VisibleDefaultsOnly, Category = "Mode")
    UInteriorDesignMode* InteriorDesignMode;
};

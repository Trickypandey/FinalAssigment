#pragma once

#include "CoreMinimal.h"
#include "LoadSlotWidget.h"
#include "SaveGameWidget.h"
#include "ToastMessageWidget.h"
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

    bool SaveGame(FString SlotName);
    void LoadGame(FString SlotName);
  
	UPROPERTY()
    USaveGameWidget* SaveWidgetInstance;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> SaveWidgetClass;

    UPROPERTY()
    ULoadSlotWidget* LoadSlotWidgetInstance;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> LoadSlotWidgetClass;
    void BroadcastToast(const FString& Message);
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
	UToastMessageWidget* ToastMessageWidgetInstance;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> ToastMessageWidgetClass;

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

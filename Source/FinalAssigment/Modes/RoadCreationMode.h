// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseMode.h"
#include "InputAction.h"
#include "RoadActor.h"
#include "RoadCreationMode.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class FINALASSIGMENT_API URoadCreationMode : public UBaseMode
{
	GENERATED_BODY()

public:
	URoadCreationMode();
	virtual void Setup() override;
	;
	virtual void SetupInputMapping() override;
	virtual void EnterMode() override;
	virtual void ExitMode() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RoadConstrcution | Actor")
	TSubclassOf<ARoadActor> RoadActorRef;
	
	UPROPERTY()
	UInputAction* OnLeftClickAction;

	UPROPERTY()
	UInputAction* OnRoadRightClick;

private:
	UFUNCTION()
	void HandleLeftClickAction();

	UFUNCTION()
	void HandleRightClickAction();

	UPROPERTY(VisibleDefaultsOnly, Category = "RoadConstrcution | Actor")
	ARoadActor* CurrentRoadActor;

	
};

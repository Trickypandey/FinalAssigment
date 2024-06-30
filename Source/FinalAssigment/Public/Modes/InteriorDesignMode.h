// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssertManager.h"
#include "BaseMode.h"
#include "InputAction.h"
#include "InteriorDesignActor.h"
#include "InteriorDesignMode.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class FINALASSIGMENT_API UInteriorDesignMode : public UBaseMode
{
	GENERATED_BODY()
public:
	UInteriorDesignMode();

	virtual void Setup() override;
	virtual void SetupInputMapping() override;
	virtual void EnterMode() override;
	virtual void ExitMode() override;
	virtual void CleanUp() override;

	void SetMeshData(const FFurnitureData& FurnitureData);

private:
	UFUNCTION()
	void HandleLeftClickAction();

	UFUNCTION()
	void HandleRightClickAction();

	UPROPERTY()
	AInteriorDesignActor* InteriorDesignActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InteriorDesign | Actor", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AInteriorDesignActor> InteriorDesignActorRef;

	UPROPERTY()
	UInputAction* OnLeftClickAction;

	UPROPERTY()
	UInputAction* OnRightClickAction;

	UPROPERTY()
	UInputAction* OnRotateAction;
	FFurnitureData InteriorCurrentData;
	void  RotateSelectedActor();
};

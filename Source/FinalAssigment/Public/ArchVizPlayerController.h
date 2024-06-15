// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "ArchVizPlayerController.generated.h"

class AWallActor;
/**
 * 
 */
UCLASS()
class FINALASSIGMENT_API AArchVizPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AArchVizPlayerController();
	virtual void SetupInputComponent() override;
	void SetupEnhancedInputBindings();
	void LeftClickProcess();

	TPair<AWallActor*, FVector> IsWallWallActor(const FHitResult& HitResult);

	UPROPERTY()
	UInputMappingContext* MappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Controller")
	UInputAction* OnLeftClick;


	
	
};

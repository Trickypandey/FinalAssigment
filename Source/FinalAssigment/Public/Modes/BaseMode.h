// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UObject/NoExportTypes.h"

#include "BaseMode.generated.h"

class UInputMappingContext;
/**
 * 
 */
UCLASS(ABSTRACT)
class FINALASSIGMENT_API UBaseMode : public UObject
{
	GENERATED_BODY()
public:

	virtual void Setup() PURE_VIRTUAL(UBaseMode::Setup(), );
	virtual void SetupInputMapping() PURE_VIRTUAL(UBaseMode::SetupInputMapping(), );
	virtual void EnterMode() PURE_VIRTUAL(UBaseMode::EnterMode(), );
	virtual void ExitMode() PURE_VIRTUAL(UBaseMode::ExitMode(), );
	virtual void CleanUp() PURE_VIRTUAL(UBaseMode::CleanUp(), );

	virtual void InitParam(APlayerController* Controller);
	virtual void ShowWidget();
	virtual void HideWidget();

	FHitResult GetHitResult(const TArray<AActor*>& ActorsToIgnore) const;

protected:

	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY()
	UInputMappingContext* InputMappingContext;

	//Widget
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> WidgetRef;

	UPROPERTY()
	UUserWidget* Widget;
	
};

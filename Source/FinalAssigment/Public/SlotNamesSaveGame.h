// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SlotNamesSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class FINALASSIGMENT_API USlotNamesSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = "SaveGame")
	TArray<FString> SlotNames;

	
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Modes/InteriorDesignMode.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InteriorDesignWidget.h"

UInteriorDesignMode::UInteriorDesignMode()
{
}

void UInteriorDesignMode::Setup()
{
	if (IsValid(InteriorDesignActorRef) && !IsValid(InteriorDesignActor)) {
		InteriorDesignActor = GetWorld()->SpawnActor<AInteriorDesignActor>(InteriorDesignActorRef, FTransform{});
	}
	if (IsValid(WidgetRef) && !IsValid(Widget)) {
		Widget = CreateWidget<UInteriorDesignWidget>(PlayerController, WidgetRef, "Interior Mode Widget");
	}
}

void UInteriorDesignMode::SetupInputMapping()
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent)) {
		InputMappingContext = NewObject<UInputMappingContext>(this);

		OnLeftClickAction = NewObject<UInputAction>(this);
		OnLeftClickAction->ValueType = EInputActionValueType::Boolean;

		

		if (InputMappingContext)
		{
			InputMappingContext = NewObject<UInputMappingContext>(this);
			InputMappingContext->MapKey(OnLeftClickAction, EKeys::LeftMouseButton);

		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("InputMappingContext Not Populated"));
		}

		if (EnhancedInputComponent)
		{
			EnhancedInputComponent->BindAction(OnLeftClickAction, ETriggerEvent::Completed, this, &UInteriorDesignMode::HandleLeftClickAction);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("EnhancedInputComponent Not Populated"));
		}


	}
}

void UInteriorDesignMode::EnterMode()
{

	if (IsValid(PlayerController)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}

		ShowWidget();
	}
}

void UInteriorDesignMode::ExitMode()
{
	if (IsValid(PlayerController)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->RemoveMappingContext(InputMappingContext);
		}

		HideWidget();
	}
}

void UInteriorDesignMode::SetMeshData(const FFurnitureData& FurnitureData)
{
	InteriorDesignActor->SetStaticMesh(FurnitureData.Type);

}

void UInteriorDesignMode::HandleLeftClickAction()
{

}

// Fill out your copyright notice in the Description page of Project Settings.


#include "ToastMessageWidget.h"

#include "Utility.h"

void UToastMessageWidget::NativeConstruct()
{
	Super::NativeConstruct();

    ShowMessage.AddDynamic(this, &UToastMessageWidget::OnShowToastMessage);
}

void UToastMessageWidget::OnShowToastMessage(const FString& Message)
{
    if (ToastText)
    {
        ToastText->SetText(FText::FromString(Message));

        if (PopUpAnimation)
        {
            PlayAnimation(PopUpAnimation);
        }
    }
}

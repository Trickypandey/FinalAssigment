// SAssertWidget.cpp

#include "SAssertWidget.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAssertWidget::Construct(const FArguments& InArgs)
{
    MeshDataAsset = InArgs._InMeshDataAsset;
    ThumbnailSize = 100;

    ScrollBox = SNew(SScrollBox).Visibility(EVisibility::Visible);

    ChildSlot
        [
            ScrollBox.ToSharedRef()
        ];

    RefreshContent();
}

void SAssertWidget::RefreshContent() const
{
    ScrollBox->ClearChildren();
    if (MeshDataAsset.IsValid())
    {
        DisplayMaterials(MeshDataAsset->MaterialDataArray);
    }
}

void SAssertWidget::DisplayMaterials(const TArray<FMaterialData>& DataArray) const
{
    ScrollBox->ClearChildren();
    ScrollBox->SetOrientation(Orient_Vertical);

    for (const FMaterialData& Data : DataArray)
    {

        if (Data.Type && Data.Icon)
        {
        UE_LOG(LogTemp, Warning, TEXT("in mESS: %d  %s"), DataArray.Num(), *Data.Name.ToString());
            if (UTexture2D* ThumbnailTexture = Cast<UTexture2D>(Data.Icon))
            {
                if (ThumbnailTexture)
                {
                    UE_LOG(LogTemp, Warning, TEXT("Valid Texture for Material: %s"), *Data.Name.ToString());

                    FSlateBrush* SlateBrush = new FSlateBrush();
                    SlateBrush->SetResourceObject(Data.Icon);
                    SlateBrush->ImageSize = FVector2D(ThumbnailSize);

                    if (SlateBrush->GetResourceObject() == nullptr)
                    {
                        UE_LOG(LogTemp, Error, TEXT("SlateBrush resource object is null for Material: %s"), *Data.Name.ToString());
                    }
                    else
                    {
                        UE_LOG(LogTemp, Warning, TEXT("SlateBrush successfully created for Material: %s"), *Data.Name.ToString());
                    }

                    TSharedPtr<SImage> ThumbnailImage = SNew(SImage)
                        .Image(SlateBrush)
                        .Cursor(EMouseCursor::Hand)
                        .OnMouseButtonDown_Lambda([this, Data](const FGeometry& Geometry, const FPointerEvent& MouseEvent)
                            {
                                if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
                                {
                                    OnMaterialThumbnailSelected.ExecuteIfBound(Data);
                                    return FReply::Handled();
                                }
                                return FReply::Unhandled();
                            });

                    TSharedPtr<STextBlock> TextBox = SNew(STextBlock)
                        .Text(Data.Name)
                        .ColorAndOpacity(FSlateColor(FLinearColor::White))
                        .Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 16));

                    TSharedPtr<SVerticalBox> VerticalBox = SNew(SVerticalBox)
                        + SVerticalBox::Slot()
                        .AutoHeight()
                        .Padding(5)
                        [
                            ThumbnailImage.ToSharedRef()
                        ]
                        + SVerticalBox::Slot()
                        .AutoHeight()
                        .Padding(5)
                        [
                            TextBox.ToSharedRef()
                        ];

                    ScrollBox->AddSlot()
                        [
                            VerticalBox.ToSharedRef()
                        ];
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Invalid Texture for Material: %s"), *Data.Name.ToString());
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Icon is not a UTexture2D: %s"), *Data.Name.ToString());
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Invalid Type or Icon for Material: %s"), *Data.Name.ToString());
        }
    }
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

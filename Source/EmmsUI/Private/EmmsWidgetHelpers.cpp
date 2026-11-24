#include "EmmsWidgetHelpers.h"
#include "EmmsEventListener.h"
#include "EmmsSlotHelpers.h"

#include "MMListViewEntryWidget.h"
#include "MMComboBoxString.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Spacer.h"
#include "Components/ListView.h"
#include "Components/SpinBox.h"
#include "Components/Slider.h"
#include "Components/SizeBox.h"
#include "Components/Border.h"
#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"
#include "Brushes/SlateRoundedBoxBrush.h"
#include "Brushes/SlateColorBrush.h"
#include "Brushes/SlateImageBrush.h"
#include "Widgets/Text/STextBlock.h"
#include "EmmsDefaultWidgetStyles.h"
#include "Styling/AppStyle.h"
#include "InputCoreTypes.h"
#include "UObject/Package.h"

#include "AngelscriptBinds.h"
#include "AngelscriptDocs.h"
#include "AngelscriptManager.h"
#include "Components/EditableText.h"

FEmmsAttributeSpecification* UEmmsWidgetHelpers::Attr_UWidget_RenderTransform;
FEmmsAttributeSpecification* UEmmsWidgetHelpers::Attr_UWidget_ToolTipText;
FEmmsAttributeSpecification* UEmmsWidgetHelpers::Attr_UTextBlock_Text;
FEmmsAttributeSpecification* UEmmsWidgetHelpers::Attr_UTextBlock_Font;
FEmmsAttributeSpecification* UEmmsWidgetHelpers::Attr_UTextBlock_ColorAndOpacity;
FEmmsAttributeSpecification* UEmmsWidgetHelpers::Attr_UTextBlock_AutoWrapText;
FMulticastDelegateProperty* UEmmsWidgetHelpers::Event_UButton_OnClicked;
FEmmsAttributeSpecification* UEmmsWidgetHelpers::Attr_UButton_WidgetStyle;
FEmmsAttributeSpecification* UEmmsWidgetHelpers::Attr_USpacer_Size;
FEmmsAttributeSpecification* UEmmsWidgetHelpers::Attr_USizeBox_WidthOverride;
FEmmsAttributeSpecification* UEmmsWidgetHelpers::Attr_USizeBox_HeightOverride;
FEmmsAttributeSpecification* UEmmsWidgetHelpers::Attr_USizeBox_MinDesiredWidth;
FEmmsAttributeSpecification* UEmmsWidgetHelpers::Attr_USizeBox_MaxDesiredWidth;
FEmmsAttributeSpecification* UEmmsWidgetHelpers::Attr_USizeBox_MinDesiredHeight;
FEmmsAttributeSpecification* UEmmsWidgetHelpers::Attr_USizeBox_MaxDesiredHeight;
FEmmsAttributeSpecification* UEmmsWidgetHelpers::Attr_USizeBox_MinAspectRatio;
FEmmsAttributeSpecification* UEmmsWidgetHelpers::Attr_USizeBox_MaxAspectRatio;
FEmmsAttributeSpecification* UEmmsWidgetHelpers::Attr_UEditableTextBox_Text;
FEmmsAttributeSpecification* UEmmsWidgetHelpers::Attr_UEditableText_Text;
FEmmsAttributeSpecification* UEmmsWidgetHelpers::Attr_UEditableText_HintText;
FEmmsAttributeSpecification* UEmmsWidgetHelpers::Attr_UEditableText_WidgetStyle;
FEmmsAttributeSpecification* UEmmsWidgetHelpers::Attr_UBorder_Background;
FDelegateProperty* UEmmsWidgetHelpers::Event_UBorder_OnMouseButtonDownEvent;
FDelegateProperty* UEmmsWidgetHelpers::Event_UBorder_OnMouseDoubleClickEvent;
FEmmsAttributeSpecification* UEmmsWidgetHelpers::Attr_UImage_Brush;
FEmmsAttributeSpecification* UEmmsWidgetHelpers::Attr_UImage_ColorAndOpacity;
FEmmsAttributeSpecification* UEmmsWidgetHelpers::Attr_UListView_EntryWidgetClass;
FEmmsAttributeSpecification* UEmmsWidgetHelpers::Attr_USpinBox_Value;
FEmmsAttributeSpecification* UEmmsWidgetHelpers::Attr_USpinBox_MinValue;
FEmmsAttributeSpecification* UEmmsWidgetHelpers::Attr_USpinBox_MaxValue;
FEmmsAttributeSpecification* UEmmsWidgetHelpers::Attr_USpinBox_MinSliderValue;
FEmmsAttributeSpecification* UEmmsWidgetHelpers::Attr_USpinBox_MaxSliderValue;
FEmmsAttributeSpecification* UEmmsWidgetHelpers::Attr_USpinBox_Delta;
FEmmsAttributeSpecification* UEmmsWidgetHelpers::Attr_USlider_Value;
FEmmsAttributeSpecification* UEmmsWidgetHelpers::Attr_USlider_MinValue;
FEmmsAttributeSpecification* UEmmsWidgetHelpers::Attr_USlider_MaxValue;
FEmmsAttributeSpecification* UEmmsWidgetHelpers::Attr_UCheckBox_CheckedState;
FEmmsAttributeSpecification* UEmmsWidgetHelpers::Attr_UScrollBox_Orientation;

FEmmsAttributeSpecification* UEmmsWidgetHelpers::GetWidgetAttrSpec(FName Name, UClass* Class)
{
	auto* Property = Class->FindPropertyByName(Name);
	check(Property != nullptr);
	auto* Spec = FEmmsAttributeSpecification::AttributeSpecsByProperty.FindRef(Property);
	check(Spec != nullptr);
	return Spec;
}

FMulticastDelegateProperty* UEmmsWidgetHelpers::GetWidgetEvent(FName Name, UClass* Class)
{
	auto* Property = CastField<FMulticastDelegateProperty>(Class->FindPropertyByName(Name));
	check(Property != nullptr);
	return Property;
}

FDelegateProperty* UEmmsWidgetHelpers::GetWidgetDelegate(FName Name, UClass* Class)
{
	auto* Property = CastField<FDelegateProperty>(Class->FindPropertyByName(Name));
	check(Property != nullptr);
	return Property;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::Button(const FString& LabelText)
{
	FEmmsWidgetHandle Button = UEmmsStatics::BeginWidget(UButton::StaticClass());
	Text(LabelText);
	UEmmsStatics::EndWidget();
	return Button;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::Button_Colored(const FString& LabelText, const FLinearColor& ButtonColor)
{
	FEmmsWidgetHandle Button = UEmmsWidgetHelpers::Button(LabelText);
	UEmmsWidgetHelpers::SetButtonStyleColor(&Button, ButtonColor);
	return Button;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::Button_IconBrush(const FString& LabelText, const FSlateBrush& IconBrush, const FVector2D& IconSize, const FLinearColor& IconColor)
{
	FEmmsWidgetHandle Button = UEmmsStatics::BeginWidget(UButton::StaticClass());
	UEmmsStatics::BeginWidget(UHorizontalBox::StaticClass());

	UEmmsSlotHelpers::VAlign_Center();
	UEmmsSlotHelpers::Padding_HorizVert(4, 2);
	Image_Brush(IconBrush, IconSize.X, IconSize.Y, IconColor);

	UEmmsSlotHelpers::VAlign_Center();
	UEmmsSlotHelpers::Padding_HorizVert(4, 0);
	Text(LabelText);

	UEmmsStatics::EndWidget();
	UEmmsStatics::EndWidget();
	return Button;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::Button_IconStyleBrush(const FString& LabelText, const FName& IconBrush, const FVector2D& IconSize, const FLinearColor& IconColor)
{
	FEmmsWidgetHandle Button = UEmmsStatics::BeginWidget(UButton::StaticClass());
	UEmmsStatics::BeginWidget(UHorizontalBox::StaticClass());

	UEmmsSlotHelpers::VAlign_Center();
	Image_StyleBrush(IconBrush, IconSize.X, IconSize.Y, IconColor);

	UEmmsSlotHelpers::VAlign_Center();
	UEmmsSlotHelpers::Padding_HorizVert(4, 0);
	Text(LabelText);

	UEmmsStatics::EndWidget();
	UEmmsStatics::EndWidget();
	return Button;
}

static const FName NAME_Typeface_Bold("Bold");
FEmmsWidgetHandle UEmmsWidgetHelpers::Text(const FString& Text, float FontSize, const FLinearColor& Color, bool bWrap, bool bBold)
{
	FEmmsWidgetHandle TextBlock = UEmmsStatics::AddWidget(UTextBlock::StaticClass());

	if (FText* TextValue = GetPartialPendingAttribute<FText>(TextBlock, Attr_UTextBlock_Text))
		*TextValue = FText::FromString(Text);

	if (FontSize != 0 || bBold)
	{
		if (FSlateFontInfo* FontValue = GetPartialPendingAttribute<FSlateFontInfo>(TextBlock, Attr_UTextBlock_Font))
		{
			*FontValue = UEmmsDefaultWidgetStyles::GetDefaultFont();
			if (FontSize != 0)
				FontValue->Size = FontSize;
			if (bBold)
				FontValue->TypefaceFontName = NAME_Typeface_Bold;
		}
	}

	if (Color != FLinearColor::White)
	{
		if (FSlateColor* ColorValue = GetPartialPendingAttribute<FSlateColor>(TextBlock, Attr_UTextBlock_ColorAndOpacity))
			*ColorValue = FSlateColor(Color);
	}

	if (bWrap)
	{
		if (bool* WrapValue = GetPartialPendingAttribute<bool>(TextBlock, Attr_UTextBlock_AutoWrapText))
			*WrapValue = bWrap;
	}

	return TextBlock;
}

void UEmmsWidgetHelpers::SetTextFontSize(FEmmsWidgetHandle* Widget, float FontSize)
{
	if (FSlateFontInfo* FontValue = GetPartialPendingAttribute<FSlateFontInfo>(*Widget, Attr_UTextBlock_Font))
	{
		if (!FontValue->HasValidFont())
			*FontValue = UEmmsDefaultWidgetStyles::GetDefaultFont();
		FontValue->Size = FontSize;
	}
}

void UEmmsWidgetHelpers::SetTextFontFace(FEmmsWidgetHandle* Widget, FName FontFace)
{
	if (FSlateFontInfo* FontValue = GetPartialPendingAttribute<FSlateFontInfo>(*Widget, Attr_UTextBlock_Font))
	{
		if (!FontValue->HasValidFont())
			*FontValue = UEmmsDefaultWidgetStyles::GetDefaultFont();
		FontValue->TypefaceFontName = FontFace;
	}
}

void UEmmsWidgetHelpers::SetTextBold(FEmmsWidgetHandle* Widget, bool bBold)
{
	if (FSlateFontInfo* FontValue = GetPartialPendingAttribute<FSlateFontInfo>(*Widget, Attr_UTextBlock_Font))
	{
		if (!FontValue->HasValidFont())
			*FontValue = UEmmsDefaultWidgetStyles::GetDefaultFont();
		if (bBold)
			FontValue->TypefaceFontName = NAME_Typeface_Bold;
		else
			FontValue->TypefaceFontName = NAME_None;
	}
}

bool UEmmsWidgetHelpers::Button_ImplBoolConv(FEmmsWidgetHandle* Widget)
{
	if (Widget->Element == nullptr)
		return false;

	UEmmsEventListener* EventListener = UEmmsStatics::GetOrCreateEventListener(
		Widget, Event_UButton_OnClicked
	);

	return EventListener->ConsumeTriggered();
}

void UEmmsWidgetHelpers::SetButtonStyleColor(FEmmsWidgetHandle* Widget, const FLinearColor& StyleColor)
{
	if (Widget->Element == nullptr)
		return;

	if (FButtonStyle* Value = GetPartialPendingAttribute<FButtonStyle>(*Widget, Attr_UButton_WidgetStyle))
	{
		*Value = UEmmsDefaultWidgetStyles::GetDefaultButtonStyle();
		Value->Normal.TintColor = StyleColor;
		Value->Hovered.TintColor = StyleColor;
		Value->Pressed.TintColor = StyleColor;
	}
}

void UEmmsWidgetHelpers::SetButtonInnerPadding(FEmmsWidgetHandle* Widget, float Horizontal, float Vertical)
{
	if (Widget->Element == nullptr)
		return;

	if (Widget->Element->PendingChildren.Num() == 0)
	{
		// No child yet, add the padding as implicit pending
		if (FMargin* Value = UEmmsSlotHelpers::GetPartialImplicitPendingSlotAttribute<FMargin>(UEmmsSlotHelpers::Attr_Padding))
			*Value = FMargin(Horizontal, Vertical);
	}
	else
	{
		// Add the padding to the child
		if (FMargin* Value = GetPartialPendingSlotAttribute<FMargin>(FEmmsWidgetHandle{Widget->WidgetTree, Widget->Element->PendingChildren[0]}, UEmmsSlotHelpers::Attr_Padding))
			*Value = FMargin(Horizontal, Vertical);
	}
}

FEmmsWidgetHandle UEmmsWidgetHelpers::Spacer_Uniform(float Size)
{
	FEmmsWidgetHandle Spacer = UEmmsStatics::AddWidget(USpacer::StaticClass());
	if (FVector2D* SizeValue = GetPartialPendingAttribute<FVector2D>(Spacer, Attr_USpacer_Size))
		*SizeValue = FVector2D(Size, Size);
	return Spacer;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::Spacer(float Width, float Height)
{
	FEmmsWidgetHandle Spacer = UEmmsStatics::AddWidget(USpacer::StaticClass());
	if (FVector2D* SizeValue = GetPartialPendingAttribute<FVector2D>(Spacer, Attr_USpacer_Size))
		*SizeValue = FVector2D(Width, Height);
	return Spacer;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::BeginSizeBox(float Width, float Height, float MinWidth, float MaxWidth, float MinHeight, float MaxHeight)
{
	FEmmsWidgetHandle SizeBox = UEmmsStatics::BeginWidget(USizeBox::StaticClass());

	if (Width != 0)
	{
		if (float* Value = GetPartialPendingAttribute<float>(SizeBox, Attr_USizeBox_WidthOverride))
			*Value = Width;
	}

	if (MinWidth != 0)
	{
		if (float* Value = GetPartialPendingAttribute<float>(SizeBox, Attr_USizeBox_MinDesiredWidth))
			*Value = MinWidth;
	}

	if (MaxWidth != 0)
	{
		if (float* Value = GetPartialPendingAttribute<float>(SizeBox, Attr_USizeBox_MaxDesiredWidth))
			*Value = MaxWidth;
	}

	if (Height != 0)
	{
		if (float* Value = GetPartialPendingAttribute<float>(SizeBox, Attr_USizeBox_HeightOverride))
			*Value = Height;
	}

	if (MinHeight != 0)
	{
		if (float* Value = GetPartialPendingAttribute<float>(SizeBox, Attr_USizeBox_MinDesiredHeight))
			*Value = MinHeight;
	}

	if (MaxHeight != 0)
	{
		if (float* Value = GetPartialPendingAttribute<float>(SizeBox, Attr_USizeBox_MaxDesiredHeight))
			*Value = MaxHeight;
	}

	return SizeBox;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::WithinSizeBox(float Width, float Height, float MinWidth, float MaxWidth, float MinHeight, float MaxHeight)
{
	FEmmsWidgetHandle SizeBox = UEmmsStatics::WithinWidget(USizeBox::StaticClass());

	if (Width != 0)
	{
		if (float* Value = GetPartialPendingAttribute<float>(SizeBox, Attr_USizeBox_WidthOverride))
			*Value = Width;
	}

	if (MinWidth != 0)
	{
		if (float* Value = GetPartialPendingAttribute<float>(SizeBox, Attr_USizeBox_MinDesiredWidth))
			*Value = MinWidth;
	}

	if (MaxWidth != 0)
	{
		if (float* Value = GetPartialPendingAttribute<float>(SizeBox, Attr_USizeBox_MaxDesiredWidth))
			*Value = MaxWidth;
	}

	if (Height != 0)
	{
		if (float* Value = GetPartialPendingAttribute<float>(SizeBox, Attr_USizeBox_HeightOverride))
			*Value = Height;
	}

	if (MinHeight != 0)
	{
		if (float* Value = GetPartialPendingAttribute<float>(SizeBox, Attr_USizeBox_MinDesiredHeight))
			*Value = MinHeight;
	}

	if (MaxHeight != 0)
	{
		if (float* Value = GetPartialPendingAttribute<float>(SizeBox, Attr_USizeBox_MaxDesiredHeight))
			*Value = MaxHeight;
	}

	return SizeBox;
}

void UEmmsWidgetHelpers::SetRenderScale_Uniform(FEmmsWidgetHandle* Widget, float UniformScale)
{
	if (FWidgetTransform* Value = GetPartialPendingAttribute<FWidgetTransform>(*Widget, Attr_UWidget_RenderTransform))
		Value->Scale = FVector2D(UniformScale, UniformScale);
}

void UEmmsWidgetHelpers::SetRenderScale_Vector(FEmmsWidgetHandle* Widget, const FVector2D& Scale)
{
	if (FWidgetTransform* Value = GetPartialPendingAttribute<FWidgetTransform>(*Widget, Attr_UWidget_RenderTransform))
		Value->Scale = Scale;
}

void UEmmsWidgetHelpers::SetRenderScale(FEmmsWidgetHandle* Widget, float HorizontalScale, float VerticalScale)
{
	if (FWidgetTransform* Value = GetPartialPendingAttribute<FWidgetTransform>(*Widget, Attr_UWidget_RenderTransform))
		Value->Scale = FVector2D(HorizontalScale, VerticalScale);
}

void UEmmsWidgetHelpers::SetRenderTranslation_Vector(FEmmsWidgetHandle* Widget, const FVector2D& Translation)
{
	if (FWidgetTransform* Value = GetPartialPendingAttribute<FWidgetTransform>(*Widget, Attr_UWidget_RenderTransform))
		Value->Translation = Translation;
}

void UEmmsWidgetHelpers::SetRenderTranslation(FEmmsWidgetHandle* Widget, float XTranslation, float YTranslation)
{
	if (FWidgetTransform* Value = GetPartialPendingAttribute<FWidgetTransform>(*Widget, Attr_UWidget_RenderTransform))
		Value->Translation = FVector2D(XTranslation, YTranslation);
}

void UEmmsWidgetHelpers::SetRenderTransformAngle(FEmmsWidgetHandle* Widget, float Angle)
{
	if (FWidgetTransform* Value = GetPartialPendingAttribute<FWidgetTransform>(*Widget, Attr_UWidget_RenderTransform))
		Value->Angle = Angle;
}

void UEmmsWidgetHelpers::SetToolTipText(FEmmsWidgetHandle* Widget, const FString& Text)
{
	if (FText* Value = GetPartialPendingAttribute<FText>(*Widget, Attr_UWidget_ToolTipText))
		*Value = FText::FromString(Text);
}

FEmmsWidgetHandle UEmmsWidgetHelpers::EditableTextBox(FString& OutTextValue)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::AddWidget(UEditableTextBox::StaticClass());
	if (Widget.Element == nullptr)
		return Widget;

	auto& AttributeState = Widget.Element->Attributes.FindOrAdd(Attr_UEditableTextBox_Text);

	FText CurrentInputValue = CastChecked<UEditableTextBox>(Widget.Element->UMGWidget)->GetText();
	FText PreviousInputValue;
	if (!AttributeState.MirroredValue.IsEmpty())
	{
		PreviousInputValue = *(FText*)AttributeState.MirroredValue.GetDataPtr();
	}
	else
	{
		PreviousInputValue = CurrentInputValue;
		Attr_UEditableTextBox_Text->InitializeValue(AttributeState.MirroredValue);
	}

	FText PreviousAttributeValue;
	if (!AttributeState.CurrentValue.IsEmpty())
		PreviousAttributeValue = *(FText*)AttributeState.CurrentValue.GetDataPtr();
	else
		PreviousAttributeValue = CurrentInputValue;

	FText NewAttributeValue = FText::FromString(OutTextValue);

	*(FText*)AttributeState.MirroredValue.GetDataPtr() = CurrentInputValue;

	if (!CurrentInputValue.EqualTo(PreviousInputValue))
	{
		OutTextValue = CurrentInputValue.ToString();
		AttributeState.SetPendingValue(Attr_UEditableTextBox_Text, &PreviousAttributeValue);
	}
	else
	{
		if (PreviousAttributeValue.EqualTo(NewAttributeValue))
			OutTextValue = CurrentInputValue.ToString();
		else
			*(FText*)AttributeState.MirroredValue.GetDataPtr() = NewAttributeValue;
		AttributeState.SetPendingValue(Attr_UEditableTextBox_Text, &NewAttributeValue);
	}

	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::EditableText(FString& OutTextValue, const FString& HintText, float FontSize, const FLinearColor& Color, bool bBold)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::AddWidget(UEditableText::StaticClass());
	if (Widget.Element == nullptr)
		return Widget;

	auto& AttributeState = Widget.Element->Attributes.FindOrAdd(Attr_UEditableText_Text);

	FText CurrentInputValue = CastChecked<UEditableText>(Widget.Element->UMGWidget)->GetText();
	FText PreviousInputValue;
	if (!AttributeState.MirroredValue.IsEmpty())
	{
		PreviousInputValue = *(FText*)AttributeState.MirroredValue.GetDataPtr();
	}
	else
	{
		PreviousInputValue = CurrentInputValue;
		Attr_UEditableText_Text->InitializeValue(AttributeState.MirroredValue);
	}

	FText PreviousAttributeValue;
	if (!AttributeState.CurrentValue.IsEmpty())
		PreviousAttributeValue = *(FText*)AttributeState.CurrentValue.GetDataPtr();
	else
		PreviousAttributeValue = CurrentInputValue;

	FText NewAttributeValue = FText::FromString(OutTextValue);

	*(FText*)AttributeState.MirroredValue.GetDataPtr() = CurrentInputValue;

	if (!CurrentInputValue.EqualTo(PreviousInputValue))
	{
		OutTextValue = CurrentInputValue.ToString();
		AttributeState.SetPendingValue(Attr_UEditableText_Text, &PreviousAttributeValue);
	}
	else
	{
		if (PreviousAttributeValue.EqualTo(NewAttributeValue))
			OutTextValue = CurrentInputValue.ToString();
		else
			*(FText*)AttributeState.MirroredValue.GetDataPtr() = NewAttributeValue;
		AttributeState.SetPendingValue(Attr_UEditableText_Text, &NewAttributeValue);
	}

	if (FText* Value = GetPartialPendingAttribute<FText>(Widget, Attr_UEditableText_HintText))
	{
		*Value = FText::FromString(HintText);
	}
	
	if (FontSize != 0 || bBold || Color != FLinearColor::White)
	{
		if (FEditableTextStyle* Value = GetPartialPendingAttribute<FEditableTextStyle>(Widget, Attr_UEditableText_WidgetStyle))
		{
			*Value = UEmmsDefaultWidgetStyles::GetDefaultEditableTextStyle();
			FSlateFontInfo FontValue = UEmmsDefaultWidgetStyles::GetDefaultFont();
			if (FontSize != 0)
				FontValue.Size = FontSize;
			if (bBold)
				FontValue.TypefaceFontName = NAME_Typeface_Bold;
			if (Color != FLinearColor::White)
				Value->ColorAndOpacity = Color;
			
			Value->Font = FontValue;
		}
	}

	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::SpinBox(double& OutValue)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::AddWidget(USpinBox::StaticClass());
	if (Widget.Element == nullptr)
		return Widget;

	auto& AttributeState = Widget.Element->Attributes.FindOrAdd(Attr_USpinBox_Value);

	float CurrentInputValue = CastChecked<USpinBox>(Widget.Element->UMGWidget)->GetValue();
	float PreviousInputValue;
	if (!AttributeState.MirroredValue.IsEmpty())
	{
		PreviousInputValue = *(float*)AttributeState.MirroredValue.GetDataPtr();
	}
	else
	{
		PreviousInputValue = CurrentInputValue;
		Attr_USpinBox_Value->InitializeValue(AttributeState.MirroredValue);
	}

	float PreviousAttributeValue;
	if (!AttributeState.CurrentValue.IsEmpty())
		PreviousAttributeValue = *(float*)AttributeState.CurrentValue.GetDataPtr();
	else
		PreviousAttributeValue = CurrentInputValue;

	float NewAttributeValue = (float)OutValue;

	*(float*)AttributeState.MirroredValue.GetDataPtr() = CurrentInputValue;

	if (CurrentInputValue != PreviousInputValue)
	{

		OutValue = (double)CurrentInputValue;
		AttributeState.SetPendingValue(Attr_USpinBox_Value, &PreviousAttributeValue);
	}
	else
	{
		if (PreviousAttributeValue == NewAttributeValue)
			OutValue = (double)CurrentInputValue;
		else
			*(float*)AttributeState.MirroredValue.GetDataPtr() = NewAttributeValue;

		AttributeState.SetPendingValue(Attr_USpinBox_Value, &NewAttributeValue);
	}

	// Sort the value attribute last in the list, so it doesn't get clamped by old Min/Max values
	Widget.Element->Attributes.KeySort([](const FEmmsAttributeSpecification& A, const FEmmsAttributeSpecification& B) -> bool
	{
		if (&A == Attr_USpinBox_Value)
			return false;
		else if (&B == Attr_USpinBox_Value)
			return true;
		else
			return &A < &B;
	});

	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::SpinBox_Constrained(double& OutValue, float MinValue, float MaxValue, float Delta)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::AddWidget(USpinBox::StaticClass());
	if (Widget.Element == nullptr)
		return Widget;

	if (float* Value = GetPartialPendingAttribute<float>(Widget, Attr_USpinBox_MinValue))
		*Value = MinValue;
	if (float* Value = GetPartialPendingAttribute<float>(Widget, Attr_USpinBox_MinSliderValue))
		*Value = MinValue;
	
	if (float* Value = GetPartialPendingAttribute<float>(Widget, Attr_USpinBox_MaxValue))
		*Value = MaxValue;
	if (float* Value = GetPartialPendingAttribute<float>(Widget, Attr_USpinBox_MaxSliderValue))
		*Value = MaxValue;

	if (Delta != 0)
	{
		if (float* Value = GetPartialPendingAttribute<float>(Widget, Attr_USpinBox_Delta))
			*Value = Delta;
	}

	{
		auto& AttributeState = Widget.Element->Attributes.FindOrAdd(Attr_USpinBox_Value);

		float CurrentInputValue = CastChecked<USpinBox>(Widget.Element->UMGWidget)->GetValue();
		float PreviousInputValue;
		if (!AttributeState.MirroredValue.IsEmpty())
		{
			PreviousInputValue = *(float*)AttributeState.MirroredValue.GetDataPtr();
		}
		else
		{
			PreviousInputValue = CurrentInputValue;
			Attr_USpinBox_Value->InitializeValue(AttributeState.MirroredValue);
		}

		float PreviousAttributeValue;
		if (!AttributeState.CurrentValue.IsEmpty())
			PreviousAttributeValue = *(float*)AttributeState.CurrentValue.GetDataPtr();
		else
			PreviousAttributeValue = CurrentInputValue;

		float NewAttributeValue = (float)OutValue;

		*(float*)AttributeState.MirroredValue.GetDataPtr() = CurrentInputValue;

		if (CurrentInputValue != PreviousInputValue)
		{
			OutValue = (double)CurrentInputValue;
			AttributeState.SetPendingValue(Attr_USpinBox_Value, &PreviousAttributeValue);
		}
		else
		{
			if (PreviousAttributeValue == NewAttributeValue)
				OutValue = (double)CurrentInputValue;
			else
				*(float*)AttributeState.MirroredValue.GetDataPtr() = NewAttributeValue;
			AttributeState.SetPendingValue(Attr_USpinBox_Value, &NewAttributeValue);
		}
	}

	// Sort the value attribute last in the list, so it doesn't get clamped by old Min/Max values
	Widget.Element->Attributes.KeySort([](const FEmmsAttributeSpecification& A, const FEmmsAttributeSpecification& B) -> bool
	{
		if (&A == Attr_USpinBox_Value)
			return false;
		else if (&B == Attr_USpinBox_Value)
			return true;
		else
			return &A < &B;
	});

	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::Slider(double& OutValue, float MinValue, float MaxValue)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::AddWidget(USlider::StaticClass());
	if (Widget.Element == nullptr)
		return Widget;

	{
		auto& AttributeState = Widget.Element->Attributes.FindOrAdd(Attr_USlider_Value);

		float CurrentInputValue = CastChecked<USlider>(Widget.Element->UMGWidget)->GetValue();
		float PreviousInputValue;
		if (!AttributeState.MirroredValue.IsEmpty())
		{
			PreviousInputValue = *(float*)AttributeState.MirroredValue.GetDataPtr();
		}
		else
		{
			PreviousInputValue = CurrentInputValue;
			Attr_USlider_Value->InitializeValue(AttributeState.MirroredValue);
		}

		float PreviousAttributeValue;
		if (!AttributeState.CurrentValue.IsEmpty())
			PreviousAttributeValue = *(float*)AttributeState.CurrentValue.GetDataPtr();
		else
			PreviousAttributeValue = CurrentInputValue;

		float NewAttributeValue = (float)OutValue;

		*(float*)AttributeState.MirroredValue.GetDataPtr() = CurrentInputValue;

		if (CurrentInputValue != PreviousInputValue)
		{
			OutValue = (double)CurrentInputValue;
			AttributeState.SetPendingValue(Attr_USlider_Value, &PreviousAttributeValue);
		}
		else
		{
			if (PreviousAttributeValue == NewAttributeValue)
				OutValue = (double)CurrentInputValue;
			else
				*(float*)AttributeState.MirroredValue.GetDataPtr() = NewAttributeValue;
			AttributeState.SetPendingValue(Attr_USlider_Value, &NewAttributeValue);
		}
	}

	if (float* Value = GetPartialPendingAttribute<float>(Widget, Attr_USlider_MinValue))
		*Value = MinValue;

	if (float* Value = GetPartialPendingAttribute<float>(Widget, Attr_USlider_MaxValue))
		*Value = MaxValue;

	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::CheckBox(bool& OutValue)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::AddWidget(UCheckBox::StaticClass());
	if (Widget.Element == nullptr)
		return Widget;

	{
		auto& AttributeState = Widget.Element->Attributes.FindOrAdd(Attr_UCheckBox_CheckedState);

		ECheckBoxState CurrentInputValue = CastChecked<UCheckBox>(Widget.Element->UMGWidget)->GetCheckedState();
		ECheckBoxState PreviousInputValue;
		if (!AttributeState.MirroredValue.IsEmpty())
		{
			PreviousInputValue = *(ECheckBoxState*)AttributeState.MirroredValue.GetDataPtr();
		}
		else
		{
			PreviousInputValue = CurrentInputValue;
			Attr_UCheckBox_CheckedState->InitializeValue(AttributeState.MirroredValue);
		}

		ECheckBoxState PreviousAttributeValue;
		if (!AttributeState.CurrentValue.IsEmpty())
			PreviousAttributeValue = *(ECheckBoxState*)AttributeState.CurrentValue.GetDataPtr();
		else
			PreviousAttributeValue = CurrentInputValue;

		ECheckBoxState NewAttributeValue = OutValue ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;

		*(ECheckBoxState*)AttributeState.MirroredValue.GetDataPtr() = CurrentInputValue;

		if (CurrentInputValue != PreviousInputValue)
		{
			OutValue = (CurrentInputValue == ECheckBoxState::Checked);
			AttributeState.SetPendingValue(Attr_UCheckBox_CheckedState, &PreviousAttributeValue);
		}
		else
		{
			if (PreviousAttributeValue == NewAttributeValue)
				OutValue = (CurrentInputValue == ECheckBoxState::Checked);
			else
				*(ECheckBoxState*)AttributeState.MirroredValue.GetDataPtr() = NewAttributeValue;
			AttributeState.SetPendingValue(Attr_UCheckBox_CheckedState, &NewAttributeValue);
		}
	}

	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::CheckBox_Label(bool& OutValue, const FString& Label)
{
	FEmmsWidgetHandle Widget = CheckBox(OutValue);
	if (Widget.Element == nullptr)
		return Widget;
	UEmmsStatics::BeginExistingWidget(Widget);
	UEmmsStatics::WithinWidget(USizeBox::StaticClass());
	UEmmsSlotHelpers::Padding_HorizVert(4, 0);
	Text(Label);
	UEmmsStatics::EndWidget();
	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::ListView(const TArray<UObject*>& ListItems)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::AddWidget(UListView::StaticClass());
	if (Widget.Element == nullptr)
		return FEmmsWidgetHandle{};
	UListView* ListView = Cast<UListView>(Widget.Element->UMGWidget);
	if (ListView == nullptr)
		return FEmmsWidgetHandle{};

	// Only trigger a refresh if the list has changed
	if (ListItems != ListView->GetListItems())
	{
		ListView->SetListItems(ListItems);
	}

	if (TSubclassOf<UUserWidget>* Value = GetPartialPendingAttribute<TSubclassOf<UUserWidget>>(Widget, Attr_UListView_EntryWidgetClass))
		*Value = UMMListViewEntryWidget::StaticClass();

	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::ListView_Indexed(int ItemCount)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::AddWidget(UListView::StaticClass());
	if (Widget.Element == nullptr)
		return FEmmsWidgetHandle{};
	UListView* ListView = Cast<UListView>(Widget.Element->UMGWidget);
	if (ListView == nullptr)
		return FEmmsWidgetHandle{};

	// Only trigger a refresh if the list has changed
	int PrevItemCount = ListView->GetListItems().Num();
	if (PrevItemCount != ItemCount)
	{
		// Keep old item objects
		TArray<UObject*> ListItems = ListView->GetListItems();
		ListItems.SetNum(ItemCount);

		// Create new objects for new list items
		for (int i = PrevItemCount; i < ItemCount; ++i)
			ListItems[i] = NewObject<UMMListViewEmptyItemObject>(GetTransientPackage(), NAME_None);

		ListView->SetListItems(ListItems);
	}

	if (TSubclassOf<UUserWidget>* Value = GetPartialPendingAttribute<TSubclassOf<UUserWidget>>(Widget, Attr_UListView_EntryWidgetClass))
		*Value = UMMListViewEntryWidget::StaticClass();

	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::ListView_Widgets(const TArray<UObject*>& ListItems, const TSubclassOf<UUserWidget>& ItemWidgetClass)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::AddWidget(UListView::StaticClass());
	if (Widget.Element == nullptr)
		return FEmmsWidgetHandle{};
	UListView* ListView = Cast<UListView>(Widget.Element->UMGWidget);
	if (ListView == nullptr)
		return FEmmsWidgetHandle{};

	// Only trigger a refresh if the list has changed
	if (ListItems != ListView->GetListItems())
	{
		ListView->SetListItems(ListItems);
	}

	if (ItemWidgetClass != nullptr)
	{
		if (TSubclassOf<UUserWidget>* Value = GetPartialPendingAttribute<TSubclassOf<UUserWidget>>(Widget, Attr_UListView_EntryWidgetClass))
			*Value = ItemWidgetClass;
	}

	return Widget;
}

void UEmmsWidgetHelpers::SetListItems(FEmmsWidgetHandle* ListView, const TArray<UObject*>& ListItems)
{
	if (ListView->Element == nullptr)
		return;
	UListView* Widget = Cast<UListView>(ListView->Element->UMGWidget);
	if (Widget == nullptr)
		return;

	// Only trigger a refresh if the list has changed
	if (ListItems != Widget->GetListItems())
	{
		Widget->SetListItems(ListItems);
	}
}

UObject* UEmmsWidgetHelpers::GetSelectedItem(FEmmsWidgetHandle* ListView, const TSubclassOf<UObject>& ItemType)
{
	if (ListView->Element == nullptr)
		return nullptr;
	UListView* Widget = Cast<UListView>(ListView->Element->UMGWidget);
	if (Widget == nullptr)
		return nullptr;

	// Only trigger a refresh if the list has changed
	UObject* Object = Widget->GetSelectedItem();
	if (Object != nullptr && Object->IsA(ItemType))
		return Object;
	else
		return nullptr;
}

int UEmmsWidgetHelpers::GetSelectedIndex(FEmmsWidgetHandle* ListView)
{
	if (ListView->Element == nullptr)
		return -1;
	UListView* Widget = Cast<UListView>(ListView->Element->UMGWidget);
	if (Widget == nullptr)
		return -1;

	// Only trigger a refresh if the list has changed
	UObject* Object = Widget->GetSelectedItem();
	if (Object != nullptr)
		return Widget->GetIndexForItem(Object);
	else
		return -1;
}

void UEmmsWidgetHelpers::SetSelectedIndex(FEmmsWidgetHandle* ListView, int Index)
{
	if (ListView->Element == nullptr)
		return;
	UListView* Widget = Cast<UListView>(ListView->Element->UMGWidget);
	if (Widget == nullptr)
		return;

	if ( Widget->GetListItems().IsValidIndex(Index))
		Widget->SetSelectedItem(Widget->GetListItems()[Index]);
}

bool UEmmsWidgetHelpers::IsItemSelected(FEmmsWidgetHandle* ListView, int Index)
{
	if (ListView->Element == nullptr)
		return false;
	UListView* Widget = Cast<UListView>(ListView->Element->UMGWidget);
	if (Widget == nullptr)
		return false;
	if (Widget->GetSelectionMode() != ESelectionMode::Multi)
		return false;

	if ( Widget->GetListItems().IsValidIndex(Index))
		return Widget->IsItemSelected(Widget->GetListItems()[Index]);
	
	return false;
}

void UEmmsWidgetHelpers::SetItemSelection(FEmmsWidgetHandle* ListView, int Index, bool bSelected)
{
	if (ListView->Element == nullptr)
		return;
	UListView* Widget = Cast<UListView>(ListView->Element->UMGWidget);
	if (Widget == nullptr)
		return;
	if (Widget->GetSelectionMode() != ESelectionMode::Multi)
		return;

	if ( Widget->GetListItems().IsValidIndex(Index))
		Widget->SetItemSelection(Widget->GetListItems()[Index], bSelected);
}

void UEmmsWidgetHelpers::SetListViewDefaultSelectedIndex(FEmmsWidgetHandle* ListView, int Index)
{
	if (ListView->Element == nullptr)
		return;
	UListView* Widget = Cast<UListView>(ListView->Element->UMGWidget);
	if (Widget == nullptr)
		return;

	if (Widget->GetSelectedItem() == nullptr && Widget->GetListItems().IsValidIndex(Index))
		Widget->SetSelectedItem(Widget->GetListItems()[Index]);
}

bool UEmmsWidgetHelpers::IsWidgetValid(FEmmsWidgetHandle* Widget)
{
	return (Widget->Element != nullptr);
}

bool UEmmsWidgetHelpers::IsHovered(FEmmsWidgetHandle* Widget)
{
	if (Widget->Element == nullptr)
		return false;
	return Widget->Element->UMGWidget->IsHovered();
}

bool UEmmsWidgetHelpers::HasAnyUserFocus(FEmmsWidgetHandle* Widget)
{
	if (Widget->Element == nullptr)
		return false;
	return Widget->Element->UMGWidget->HasAnyUserFocus();
}

const FGeometry& UEmmsWidgetHelpers::GetCachedGeometry(FEmmsWidgetHandle* Widget)
{
	if (Widget->Element == nullptr)
	{
		static FGeometry EmptyGeometry;
		return EmptyGeometry;
	}
	return Widget->Element->UMGWidget->GetCachedGeometry();
}

FEmmsWidgetHandle UEmmsWidgetHelpers::BeginBorder()
{
	FEmmsWidgetHandle Widget = UEmmsStatics::BeginWidget(UBorder::StaticClass());
	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::BeginBorder_Color(const FLinearColor& Color)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::BeginWidget(UBorder::StaticClass());
	if (FSlateBrush* Value = GetPartialPendingAttribute<FSlateBrush>(Widget, Attr_UBorder_Background))
		*Value = FSlateColorBrush(Color);
	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::BeginBorder_RoundedColor(const FLinearColor& Color, float RoundedCornerRadius, const FLinearColor& OutlineColor, float OutlineWidth)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::BeginWidget(UBorder::StaticClass());
	if (FSlateBrush* Value = GetPartialPendingAttribute<FSlateBrush>(Widget, Attr_UBorder_Background))
	{
		if (RoundedCornerRadius == 0.f)
		{
			*Value = FSlateColorBrush(Color);
		}
		else
		{
			*Value = FSlateRoundedBoxBrush(Color, RoundedCornerRadius, OutlineColor, OutlineWidth);
		}
	}
	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::BeginBorder_Texture(UTexture2D* Texture)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::BeginWidget(UBorder::StaticClass());
	if (Texture != nullptr)
	{
		if (FSlateBrush* Value = GetPartialPendingAttribute<FSlateBrush>(Widget, Attr_UBorder_Background))
			*Value = FSlateImageBrush(Texture, FVector2f(Texture->GetSizeX(), Texture->GetSizeY()));
	}
	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::BeginBorder_Material(UMaterialInterface* Material)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::BeginWidget(UBorder::StaticClass());
	if (Material != nullptr)
	{
		if (FSlateBrush* Value = GetPartialPendingAttribute<FSlateBrush>(Widget, Attr_UBorder_Background))
			*Value = FSlateImageBrush(Material, FVector2f(0, 0));
	}
	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::BeginBorder_Brush(const FSlateBrush& Brush)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::BeginWidget(UBorder::StaticClass());
	if (FSlateBrush* Value = GetPartialPendingAttribute<FSlateBrush>(Widget, Attr_UBorder_Background))
		*Value = Brush;
	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::BeginBorder_StyleBrush(const FName& StyleName)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::BeginWidget(UBorder::StaticClass());
	if (FSlateBrush* Value = GetPartialPendingAttribute<FSlateBrush>(Widget, Attr_UBorder_Background))
		*Value = *FAppStyle::GetOptionalBrush(StyleName);
	return Widget;
}

UMaterialInstanceDynamic* UEmmsWidgetHelpers::GetBorderDynamicMaterial(FEmmsWidgetHandle* Widget)
{
	if (Widget->Element == nullptr)
		return nullptr;
	UBorder* Border = Cast<UBorder>(Widget->Element->UMGWidget);
	if (Border == nullptr)
		return nullptr;
	return Border->GetDynamicMaterial();
}

static bool WasBorderMouseButtonDown(FEmmsWidgetHandle* Widget, FDelegateProperty* Event, FKey Key)
{
	UEmmsEventListener* EventListener = UEmmsStatics::GetOrCreateEventListener(
		Widget, Event
	);

	bool bTriggered = EventListener->ConsumeTriggered();
	if (!bTriggered)
		return false;
	if (EventListener->TriggeredParameters == nullptr)
		return false;
	if (!EventListener->bTriggeredParametersInitialized)
		return false;

	struct FParams
	{
		FGeometry Geometry;
		FPointerEvent PointerEvent;
		FEventReply ReturnValue;
	};
	FParams* Params = (FParams*)EventListener->TriggeredParameters;
	if (Params->PointerEvent.GetEffectingButton() == Key)
		return true;
	else
		return false;
}

bool UEmmsWidgetHelpers::WasBorderClicked(FEmmsWidgetHandle* Widget)
{
	return WasBorderMouseButtonDown(Widget, Event_UBorder_OnMouseButtonDownEvent, EKeys::LeftMouseButton);
}

bool UEmmsWidgetHelpers::WasBorderRightClicked(FEmmsWidgetHandle* Widget)
{
	return WasBorderMouseButtonDown(Widget, Event_UBorder_OnMouseButtonDownEvent, EKeys::RightMouseButton);
}

bool UEmmsWidgetHelpers::WasBorderMiddleClicked(FEmmsWidgetHandle* Widget)
{
	return WasBorderMouseButtonDown(Widget, Event_UBorder_OnMouseButtonDownEvent, EKeys::MiddleMouseButton);
}

bool UEmmsWidgetHelpers::WasBorderDoubleClicked(FEmmsWidgetHandle* Widget)
{
	return WasBorderMouseButtonDown(Widget, Event_UBorder_OnMouseDoubleClickEvent, EKeys::LeftMouseButton);
}

FEmmsWidgetHandle UEmmsWidgetHelpers::WithinBorder()
{
	FEmmsWidgetHandle Widget = UEmmsStatics::WithinWidget(UBorder::StaticClass());
	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::WithinBorder_Color(const FLinearColor& Color)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::WithinWidget(UBorder::StaticClass());
	if (FSlateBrush* Value = GetPartialPendingAttribute<FSlateBrush>(Widget, Attr_UBorder_Background))
		*Value = FSlateColorBrush(Color);
	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::WithinBorder_RoundedColor(const FLinearColor& Color, float RoundedCornerRadius, const FLinearColor& OutlineColor, float OutlineWidth)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::WithinWidget(UBorder::StaticClass());
	if (FSlateBrush* Value = GetPartialPendingAttribute<FSlateBrush>(Widget, Attr_UBorder_Background))
	{
		if (RoundedCornerRadius == 0.f)
		{
			*Value = FSlateColorBrush(Color);
		}
		else
		{
			*Value = FSlateRoundedBoxBrush(Color, RoundedCornerRadius, OutlineColor, OutlineWidth);
		}
	}
	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::WithinBorder_Texture(UTexture2D* Texture)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::WithinWidget(UBorder::StaticClass());
	if (Texture != nullptr)
	{
		if (FSlateBrush* Value = GetPartialPendingAttribute<FSlateBrush>(Widget, Attr_UBorder_Background))
			*Value = FSlateImageBrush(Texture, FVector2f(Texture->GetSizeX(), Texture->GetSizeY()));
	}
	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::WithinBorder_Material(UMaterialInterface* Material)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::WithinWidget(UBorder::StaticClass());
	if (Material != nullptr)
	{
		if (FSlateBrush* Value = GetPartialPendingAttribute<FSlateBrush>(Widget, Attr_UBorder_Background))
			*Value = FSlateImageBrush(Material, FVector2f(0, 0));
	}
	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::WithinBorder_Brush(const FSlateBrush& Brush)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::WithinWidget(UBorder::StaticClass());
	if (FSlateBrush* Value = GetPartialPendingAttribute<FSlateBrush>(Widget, Attr_UBorder_Background))
		*Value = Brush;
	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::WithinBorder_StyleBrush(const FName& StyleName)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::WithinWidget(UBorder::StaticClass());
	if (FSlateBrush* Value = GetPartialPendingAttribute<FSlateBrush>(Widget, Attr_UBorder_Background))
		*Value = *FAppStyle::GetOptionalBrush(StyleName);
	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::Image_Texture(UTexture2D* Texture, float Width, float Height, const FLinearColor& Color)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::AddWidget(UImage::StaticClass());
	if (Texture != nullptr)
	{
		if (FSlateBrush* Value = GetPartialPendingAttribute<FSlateBrush>(Widget, Attr_UImage_Brush))
		{
			*Value = FSlateImageBrush(Texture, FVector2f(
				Width == 0.f ? Texture->GetSizeX() : Width,
				Height == 0.f ? Texture->GetSizeX() : Height
			));
		}
	}
	if (Color != FLinearColor::White)
	{
		if (FLinearColor* Value = GetPartialPendingAttribute<FLinearColor>(Widget, Attr_UImage_ColorAndOpacity))
			*Value = Color;
	}
	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::Image_Material(UMaterialInterface* Material, float Width, float Height, const FLinearColor& Color)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::AddWidget(UImage::StaticClass());
	if (Material != nullptr)
	{
		if (FSlateBrush* Value = GetPartialPendingAttribute<FSlateBrush>(Widget, Attr_UImage_Brush))
			*Value = FSlateImageBrush(Material, FVector2f(Width, Height));
	}
	if (Color != FLinearColor::White)
	{
		if (FLinearColor* Value = GetPartialPendingAttribute<FLinearColor>(Widget, Attr_UImage_ColorAndOpacity))
			*Value = Color;
	}
	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::Image_Brush(const FSlateBrush& Brush, float Width, float Height, const FLinearColor& Color)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::AddWidget(UImage::StaticClass());
	if (FSlateBrush* Value = GetPartialPendingAttribute<FSlateBrush>(Widget, Attr_UImage_Brush))
	{
		*Value = Brush;
		if (Width != 0.f)
			Value->ImageSize.X = Width;
		if (Height != 0.f)
			Value->ImageSize.Y = Height;
	}
	if (Color != FLinearColor::White)
	{
		if (FLinearColor* Value = GetPartialPendingAttribute<FLinearColor>(Widget, Attr_UImage_ColorAndOpacity))
			*Value = Color;
	}
	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::Image_StyleBrush(const FName& BrushName, float Width, float Height, const FLinearColor& Color)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::AddWidget(UImage::StaticClass());
	if (FSlateBrush* Value = GetPartialPendingAttribute<FSlateBrush>(Widget, Attr_UImage_Brush))
	{
		*Value = *FAppStyle::GetOptionalBrush(BrushName);
		if (Width != 0.f)
			Value->ImageSize.X = Width;
		if (Height != 0.f)
			Value->ImageSize.Y = Height;
	}
	if (Color != FLinearColor::White)
	{
		if (FLinearColor* Value = GetPartialPendingAttribute<FLinearColor>(Widget, Attr_UImage_ColorAndOpacity))
			*Value = Color;
	}
	return Widget;
}

UMaterialInstanceDynamic* UEmmsWidgetHelpers::GetImageDynamicMaterial(FEmmsWidgetHandle* Widget)
{
	if (Widget->Element == nullptr)
		return nullptr;
	UImage* Image = Cast<UImage>(Widget->Element->UMGWidget);
	if (Image == nullptr)
		return nullptr;
	return Image->GetDynamicMaterial();
}

FEmmsWidgetHandle UEmmsWidgetHelpers::ComboBox_Enum(void* DataPtr, int TypeId)
{
	UEnum* Enum = FAngelscriptManager::Get().GetUnrealEnumFromAngelscriptTypeId(TypeId);
	if (Enum == nullptr)
	{
		FAngelscriptManager::Throw("Parameter must be a reference to an unreal UENUM value");
		return FEmmsWidgetHandle{};
	}

	int64 CurrentValue = *(uint8*)DataPtr;
	int CurrentEnumIndex = -1;
	int CurrentItemIndex = -1;

	TArray<FString> Items;
	TArray<int64, TInlineAllocator<32>> Values;
	for (int i = 0, Count = Enum->NumEnums() - 1; i < Count; ++i)
	{
#if WITH_EDITOR
		if (Enum->HasMetaData(TEXT("Hidden"), i))
			continue;
#endif

		int64 Value = Enum->GetValueByIndex(i);
		if (Value == CurrentValue)
		{
			CurrentEnumIndex = i;
			CurrentItemIndex = Items.Num();
		}

		Items.Add(Enum->GetDisplayNameTextByIndex(i).ToString());
		Values.Add(Value);
	}

	FEmmsWidgetHandle Widget = UEmmsWidgetHelpers::ComboBox_Strings_NoOut(Items);
	if (Widget.Element == nullptr)
		return Widget;

	UMMComboBoxString* ComboBox = CastChecked<UMMComboBoxString>(Widget.Element->UMGWidget);
	if (CurrentItemIndex != -1 && ComboBox->DefaultSelectedItem != Enum->GetNameStringByIndex(CurrentEnumIndex))
	{
		ComboBox->DefaultSelectedItem = Enum->GetNameStringByIndex(CurrentEnumIndex);
		ComboBox->SetSelectedIndex(CurrentItemIndex);
	}

	int SelectedIndex = ComboBox->GetSelectedIndex();
	if (Items.IsValidIndex(SelectedIndex))
	{
		*(uint8*)DataPtr = (uint8)Values[SelectedIndex];
	}
	else if (Values.Num() != 0)
	{
		ComboBox->SetSelectedIndex(0);
		*(uint8*)DataPtr = (uint8)Values[0];
	}

	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::ComboBox_Names(const TArray<FName>& Items, FName& OutSelectedItem)
{
	FEmmsWidgetHandle Widget = UEmmsWidgetHelpers::ComboBox_Names_NoOut(Items);
	if (Widget.Element == nullptr)
		return Widget;

	UMMComboBoxString* ComboBox = CastChecked<UMMComboBoxString>(Widget.Element->UMGWidget);
	FString PrevItemStr = OutSelectedItem.ToString();
	if (ComboBox->DefaultSelectedItem != PrevItemStr)
	{
		ComboBox->DefaultSelectedItem = PrevItemStr;
		ComboBox->SetSelectedOption(PrevItemStr);
	}

	int SelectedIndex = ComboBox->GetSelectedIndex();
	if (Items.IsValidIndex(SelectedIndex))
	{
		OutSelectedItem = Items[SelectedIndex];
	}
	else if (Items.Num() != 0)
	{
		ComboBox->SetSelectedIndex(0);
		OutSelectedItem = Items[0];
	}

	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::ComboBox_Names_NoOut(const TArray<FName>& Items)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::AddWidget(UMMComboBoxString::StaticClass());
	if (Widget.Element == nullptr)
		return Widget;

	UMMComboBoxString* ComboBox = CastChecked<UMMComboBoxString>(Widget.Element->UMGWidget);
	TArray<FString>& ActiveOptions = ComboBox->ActiveOptions;

	bool bItemsChanged = false;
	if (ActiveOptions.Num() != Items.Num())
	{
		bItemsChanged = true;
	}
	else
	{
		for (int i = 0, Count = Items.Num(); i < Count; ++i)
		{
			if (Items[i] != ActiveOptions[i])
			{
				bItemsChanged = true;
				break;
			}
		}
	}

	if (bItemsChanged)
	{
		int PrevSelectedIndex = ComboBox->GetSelectedIndex();
		FString PrevSelectedItem = ComboBox->GetOptionAtIndex(PrevSelectedIndex);

		ActiveOptions.SetNum(Items.Num());
		for (int i = 0, Count = Items.Num(); i < Count; ++i)
			ActiveOptions[i] = Items[i].ToString();

		ComboBox->ClearOptions();
		for (int i = 0, Count = ActiveOptions.Num(); i < Count; ++i)
			ComboBox->AddOption(ActiveOptions[i]);

		if (!PrevSelectedItem.IsEmpty())
			ComboBox->SetSelectedOption(PrevSelectedItem);
	}

	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::ComboBox_Strings(const TArray<FString>& Items, FString& OutSelectedItem)
{
	FEmmsWidgetHandle Widget = UEmmsWidgetHelpers::ComboBox_Strings_NoOut(Items);
	if (Widget.Element == nullptr)
		return Widget;

	UMMComboBoxString* ComboBox = CastChecked<UMMComboBoxString>(Widget.Element->UMGWidget);
	if (ComboBox->DefaultSelectedItem != OutSelectedItem)
	{
		ComboBox->SetSelectedOption(OutSelectedItem);
		ComboBox->DefaultSelectedItem = OutSelectedItem;
	}

	int SelectedIndex = ComboBox->GetSelectedIndex();
	if (Items.IsValidIndex(SelectedIndex))
	{
		OutSelectedItem = Items[SelectedIndex];
	}
	else if (Items.Num() != 0)
	{
		ComboBox->SetSelectedIndex(0);
		OutSelectedItem = Items[0];
	}
	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::ComboBox_Strings_NoOut(const TArray<FString>& Items)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::AddWidget(UMMComboBoxString::StaticClass());
	if (Widget.Element == nullptr)
		return Widget;

	UMMComboBoxString* ComboBox = CastChecked<UMMComboBoxString>(Widget.Element->UMGWidget);
	TArray<FString>& ActiveOptions = ComboBox->ActiveOptions;
	if (ActiveOptions != Items)
	{
		ActiveOptions = Items;

		int PrevSelectedIndex = ComboBox->GetSelectedIndex();
		FString PrevSelectedItem = ComboBox->GetOptionAtIndex(PrevSelectedIndex);

		ComboBox->ClearOptions();
		for (int i = 0, Count = Items.Num(); i < Count; ++i)
			ComboBox->AddOption(Items[i]);

		if (!PrevSelectedItem.IsEmpty())
			ComboBox->SetSelectedOption(PrevSelectedItem);
	}

	return Widget;
}

int UEmmsWidgetHelpers::GetComboBoxSelectedIndex(FEmmsWidgetHandle* Widget)
{
	if (Widget->Element == nullptr)
		return -1;

	UComboBoxString* ComboBox = Cast<UComboBoxString>(Widget->Element->UMGWidget);
	if (ComboBox == nullptr)
		return -1;

	return ComboBox->GetSelectedIndex();
}

FEmmsWidgetHandle UEmmsWidgetHelpers::BeginScrollBox(EOrientation Orientation)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::BeginWidget(UScrollBox::StaticClass());
	if (TEnumAsByte<EOrientation>* Value = GetPartialPendingAttribute<TEnumAsByte<EOrientation>>(Widget, Attr_UScrollBox_Orientation))
		*Value = Orientation;
	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::WithinScrollBox(EOrientation Orientation)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::WithinWidget(UScrollBox::StaticClass());
	if (TEnumAsByte<EOrientation>* Value = GetPartialPendingAttribute<TEnumAsByte<EOrientation>>(Widget, Attr_UScrollBox_Orientation))
		*Value = Orientation;
	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::BeginVerticalBox()
{
	FEmmsWidgetHandle Widget = UEmmsStatics::BeginWidget(UVerticalBox::StaticClass());
	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::BeginVerticalBox_Alignment(EHorizontalAlignment DefaultAlignment)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::BeginWidget(UVerticalBox::StaticClass());
	UEmmsStatics::SetDefaultChildSlotAttributeValue(Widget, UEmmsSlotHelpers::Attr_HAlign, &DefaultAlignment);
	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::BeginVerticalBox_Padding(double DefaultPadding, EHorizontalAlignment DefaultAlignment)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::BeginWidget(UVerticalBox::StaticClass());
	FMargin Padding(DefaultPadding);
	UEmmsStatics::SetDefaultChildSlotAttributeValue(Widget, UEmmsSlotHelpers::Attr_Padding, &Padding);
	UEmmsStatics::SetDefaultChildSlotAttributeValue(Widget, UEmmsSlotHelpers::Attr_HAlign, &DefaultAlignment);
	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::BeginVerticalBox_Padding2D(double DefaultHorizontalPadding, double DefaultVerticalPadding, EHorizontalAlignment DefaultAlignment)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::BeginWidget(UVerticalBox::StaticClass());
	FMargin Padding(DefaultHorizontalPadding, DefaultVerticalPadding);
	UEmmsStatics::SetDefaultChildSlotAttributeValue(Widget, UEmmsSlotHelpers::Attr_Padding, &Padding);
	UEmmsStatics::SetDefaultChildSlotAttributeValue(Widget, UEmmsSlotHelpers::Attr_HAlign, &DefaultAlignment);
	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::BeginVerticalBox_Padding4D(double DefaultLeftPadding, double DefaultTopPadding, double DefaultRightPadding, double DefaultBottomPadding, EHorizontalAlignment DefaultAlignment)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::BeginWidget(UVerticalBox::StaticClass());
	FMargin Padding(DefaultLeftPadding, DefaultTopPadding, DefaultRightPadding, DefaultBottomPadding);
	UEmmsStatics::SetDefaultChildSlotAttributeValue(Widget, UEmmsSlotHelpers::Attr_Padding, &Padding);
	UEmmsStatics::SetDefaultChildSlotAttributeValue(Widget, UEmmsSlotHelpers::Attr_HAlign, &DefaultAlignment);
	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::BeginHorizontalBox()
{
	FEmmsWidgetHandle Widget = UEmmsStatics::BeginWidget(UHorizontalBox::StaticClass());
	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::BeginHorizontalBox_Alignment(EVerticalAlignment DefaultAlignment)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::BeginWidget(UHorizontalBox::StaticClass());
	UEmmsStatics::SetDefaultChildSlotAttributeValue(Widget, UEmmsSlotHelpers::Attr_VAlign, &DefaultAlignment);
	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::BeginHorizontalBox_Padding(double DefaultPadding, EVerticalAlignment DefaultAlignment)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::BeginWidget(UHorizontalBox::StaticClass());
	FMargin Padding(DefaultPadding);
	UEmmsStatics::SetDefaultChildSlotAttributeValue(Widget, UEmmsSlotHelpers::Attr_Padding, &Padding);
	UEmmsStatics::SetDefaultChildSlotAttributeValue(Widget, UEmmsSlotHelpers::Attr_VAlign, &DefaultAlignment);
	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::BeginHorizontalBox_Padding2D(double DefaultHorizontalPadding, double DefaultVerticalPadding, EVerticalAlignment DefaultAlignment)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::BeginWidget(UHorizontalBox::StaticClass());
	FMargin Padding(DefaultHorizontalPadding, DefaultVerticalPadding);
	UEmmsStatics::SetDefaultChildSlotAttributeValue(Widget, UEmmsSlotHelpers::Attr_Padding, &Padding);
	UEmmsStatics::SetDefaultChildSlotAttributeValue(Widget, UEmmsSlotHelpers::Attr_VAlign, &DefaultAlignment);
	return Widget;
}

FEmmsWidgetHandle UEmmsWidgetHelpers::BeginHorizontalBox_Padding4D(double DefaultLeftPadding, double DefaultTopPadding, double DefaultRightPadding, double DefaultBottomPadding, EVerticalAlignment DefaultAlignment)
{
	FEmmsWidgetHandle Widget = UEmmsStatics::BeginWidget(UHorizontalBox::StaticClass());
	FMargin Padding(DefaultLeftPadding, DefaultTopPadding, DefaultRightPadding, DefaultBottomPadding);
	UEmmsStatics::SetDefaultChildSlotAttributeValue(Widget, UEmmsSlotHelpers::Attr_Padding, &Padding);
	UEmmsStatics::SetDefaultChildSlotAttributeValue(Widget, UEmmsSlotHelpers::Attr_VAlign, &DefaultAlignment);
	return Widget;
}

AS_FORCE_LINK const FAngelscriptBinds::FBind Bind_EmmsWidgetHelpers((int32)FAngelscriptBinds::EOrder::Late + 250, []
{
	{
		FAngelscriptBinds::FNamespace ns("mm");

		UEmmsWidgetHelpers::Attr_UTextBlock_Text = UEmmsWidgetHelpers::GetWidgetAttrSpec("Text", UTextBlock::StaticClass());
		UEmmsWidgetHelpers::Attr_UTextBlock_Font = UEmmsWidgetHelpers::GetWidgetAttrSpec("Font", UTextBlock::StaticClass());
		UEmmsWidgetHelpers::Attr_UTextBlock_ColorAndOpacity = UEmmsWidgetHelpers::GetWidgetAttrSpec("ColorAndOpacity", UTextBlock::StaticClass());
		UEmmsWidgetHelpers::Attr_UTextBlock_AutoWrapText = UEmmsWidgetHelpers::GetWidgetAttrSpec("AutoWrapText", UTextBlock::StaticClass());
		FAngelscriptBinds::BindGlobalFunction("mm<UTextBlock> Text(const FString& Text, float32 FontSize = 0, const FLinearColor& Color = FLinearColor::White, bool bWrap = false, bool bBold = false)", &UEmmsWidgetHelpers::Text);
		SCRIPT_BIND_DOCUMENTATION("Add a TextBlock widget with the specified text and settings");
		FAngelscriptBinds::BindGlobalFunction("mm<UButton> Button(const FString& Label)", &UEmmsWidgetHelpers::Button);
		SCRIPT_BIND_DOCUMENTATION("Add a button with the specified label");
		FAngelscriptBinds::BindGlobalFunction("mm<UButton> Button(const FString& Label, const FLinearColor& ButtonColor)", &UEmmsWidgetHelpers::Button_Colored);
		SCRIPT_BIND_DOCUMENTATION("Add a colored button with the specified label");
		FAngelscriptBinds::BindGlobalFunction("mm<UButton> Button(const FString& Label, const FSlateBrush& IconBrush, const FVector2D& IconSize = FVector2D(0, 0), const FLinearColor& IconColor = FLinearColor::White)", &UEmmsWidgetHelpers::Button_IconBrush);
		SCRIPT_BIND_DOCUMENTATION("Add a button with the specified label and an icon image");
		FAngelscriptBinds::BindGlobalFunction("mm<UButton> Button(const FString& Label, const FName& IconStyleBrush, const FVector2D& IconSize = FVector2D(0, 0), const FLinearColor& IconColor = FLinearColor::White)", &UEmmsWidgetHelpers::Button_IconStyleBrush);
		SCRIPT_BIND_DOCUMENTATION("Add a button with the specified label and an icon image");

		UEmmsWidgetHelpers::Attr_USpacer_Size = UEmmsWidgetHelpers::GetWidgetAttrSpec("Size", USpacer::StaticClass());
		FAngelscriptBinds::BindGlobalFunction("mm<USpacer> Spacer(float32 Size)", &UEmmsWidgetHelpers::Spacer_Uniform);
		SCRIPT_BIND_DOCUMENTATION("Add an empty spacer widget with a uniform specified size");
		FAngelscriptBinds::BindGlobalFunction("mm<USpacer> Spacer(float32 Width, float32 Height)", &UEmmsWidgetHelpers::Spacer);
		SCRIPT_BIND_DOCUMENTATION("Add an empty spacer widget with the specifiec width and height");

		UEmmsWidgetHelpers::Attr_USizeBox_WidthOverride = UEmmsWidgetHelpers::GetWidgetAttrSpec("WidthOverride", USizeBox::StaticClass());
		UEmmsWidgetHelpers::Attr_USizeBox_WidthOverride->ResetToDefaultFunction = [](FEmmsAttributeSpecification*, void* Container) { ((USizeBox*)Container)->ClearWidthOverride(); };
		UEmmsWidgetHelpers::Attr_USizeBox_MinDesiredWidth = UEmmsWidgetHelpers::GetWidgetAttrSpec("MinDesiredWidth", USizeBox::StaticClass());
		UEmmsWidgetHelpers::Attr_USizeBox_MinDesiredWidth->ResetToDefaultFunction = [](FEmmsAttributeSpecification*, void* Container) { ((USizeBox*)Container)->ClearMinDesiredWidth(); };
		UEmmsWidgetHelpers::Attr_USizeBox_MaxDesiredWidth = UEmmsWidgetHelpers::GetWidgetAttrSpec("MaxDesiredWidth", USizeBox::StaticClass());
		UEmmsWidgetHelpers::Attr_USizeBox_MaxDesiredWidth->ResetToDefaultFunction = [](FEmmsAttributeSpecification*, void* Container) { ((USizeBox*)Container)->ClearMaxDesiredWidth(); };
		UEmmsWidgetHelpers::Attr_USizeBox_HeightOverride = UEmmsWidgetHelpers::GetWidgetAttrSpec("HeightOverride", USizeBox::StaticClass());
		UEmmsWidgetHelpers::Attr_USizeBox_HeightOverride->ResetToDefaultFunction = [](FEmmsAttributeSpecification*, void* Container) { ((USizeBox*)Container)->ClearHeightOverride(); };
		UEmmsWidgetHelpers::Attr_USizeBox_MinDesiredHeight = UEmmsWidgetHelpers::GetWidgetAttrSpec("MinDesiredHeight", USizeBox::StaticClass());
		UEmmsWidgetHelpers::Attr_USizeBox_MinDesiredHeight->ResetToDefaultFunction = [](FEmmsAttributeSpecification*, void* Container) { ((USizeBox*)Container)->ClearMinDesiredHeight(); };
		UEmmsWidgetHelpers::Attr_USizeBox_MaxDesiredHeight = UEmmsWidgetHelpers::GetWidgetAttrSpec("MaxDesiredHeight", USizeBox::StaticClass());
		UEmmsWidgetHelpers::Attr_USizeBox_MaxDesiredHeight->ResetToDefaultFunction = [](FEmmsAttributeSpecification*, void* Container) { ((USizeBox*)Container)->ClearMaxDesiredHeight(); };
		UEmmsWidgetHelpers::Attr_USizeBox_MinAspectRatio = UEmmsWidgetHelpers::GetWidgetAttrSpec("MinAspectRatio", USizeBox::StaticClass());
		UEmmsWidgetHelpers::Attr_USizeBox_MinAspectRatio->ResetToDefaultFunction = [](FEmmsAttributeSpecification*, void* Container) { ((USizeBox*)Container)->ClearMinAspectRatio(); };
		UEmmsWidgetHelpers::Attr_USizeBox_MaxAspectRatio = UEmmsWidgetHelpers::GetWidgetAttrSpec("MaxAspectRatio", USizeBox::StaticClass());
		UEmmsWidgetHelpers::Attr_USizeBox_MaxAspectRatio->ResetToDefaultFunction = [](FEmmsAttributeSpecification*, void* Container) { ((USizeBox*)Container)->ClearMaxAspectRatio(); };

		FAngelscriptBinds::BindGlobalFunction("mm<USizeBox> BeginSizeBox(float32 Width = 0, float32 Height = 0, float32 MinWidth = 0, float32 MaxWidth = 0, float32 MinHeight = 0, float32 MaxHeight = 0)", &UEmmsWidgetHelpers::BeginSizeBox);
		SCRIPT_BIND_DOCUMENTATION("Begin an immediate SizeBox panel with the specified sizes. All subsequent widgets will be contained within this panel until EndSizeBox() is called");
		FAngelscriptBinds::BindGlobalFunction("mm<USizeBox> WithinSizeBox(float32 Width = 0, float32 Height = 0, float32 MinWidth = 0, float32 MaxWidth = 0, float32 MinHeight = 0, float32 MaxHeight = 0)", &UEmmsWidgetHelpers::WithinSizeBox);
		SCRIPT_BIND_DOCUMENTATION("Begin an immediate SizeBox panel with the specified sizes. Only the one single widget that is drawn right after the WithinSizeBox() call will be placed inside the panel");

		UEmmsWidgetHelpers::Attr_UEditableTextBox_Text = UEmmsWidgetHelpers::GetWidgetAttrSpec("Text", UEditableTextBox::StaticClass());
		FAngelscriptBinds::BindGlobalFunction("mm<UEditableTextBox> EditableTextBox(FString&out OutValue)", &UEmmsWidgetHelpers::EditableTextBox);
		SCRIPT_BIND_DOCUMENTATION("Add an editable text box widget. The out string value will be set to whatever the user has entered");

		UEmmsWidgetHelpers::Attr_UEditableText_Text = UEmmsWidgetHelpers::GetWidgetAttrSpec("Text", UEditableText::StaticClass());
		UEmmsWidgetHelpers::Attr_UEditableText_HintText = UEmmsWidgetHelpers::GetWidgetAttrSpec("HintText", UEditableText::StaticClass());
		UEmmsWidgetHelpers::Attr_UEditableText_WidgetStyle = UEmmsWidgetHelpers::GetWidgetAttrSpec("WidgetStyle", UEditableText::StaticClass());
		FAngelscriptBinds::BindGlobalFunction("mm<UEditableText> EditableText(FString&out OutValue, const FString& HintText, float32 FontSize = 0, const FLinearColor& Color = FLinearColor::White, bool bBold = false)", &UEmmsWidgetHelpers::EditableText);
		SCRIPT_BIND_DOCUMENTATION("Add an editable text box widget. The out string value will be set to whatever the user has entered");
		
		UEmmsWidgetHelpers::Attr_USpinBox_Value = UEmmsWidgetHelpers::GetWidgetAttrSpec("Value", USpinBox::StaticClass());
		UEmmsWidgetHelpers::Attr_USpinBox_MinValue = UEmmsWidgetHelpers::GetWidgetAttrSpec("MinValue", USpinBox::StaticClass());
		UEmmsWidgetHelpers::Attr_USpinBox_MinValue->ResetToDefaultFunction = [](FEmmsAttributeSpecification*, void* Container) { ((USpinBox*)Container)->ClearMinValue(); };
		UEmmsWidgetHelpers::Attr_USpinBox_MaxValue = UEmmsWidgetHelpers::GetWidgetAttrSpec("MaxValue", USpinBox::StaticClass());
		UEmmsWidgetHelpers::Attr_USpinBox_MaxValue->ResetToDefaultFunction = [](FEmmsAttributeSpecification*, void* Container) { ((USpinBox*)Container)->ClearMaxValue(); };
		UEmmsWidgetHelpers::Attr_USpinBox_MinSliderValue = UEmmsWidgetHelpers::GetWidgetAttrSpec("MinSliderValue", USpinBox::StaticClass());
		UEmmsWidgetHelpers::Attr_USpinBox_MinSliderValue->ResetToDefaultFunction = [](FEmmsAttributeSpecification*, void* Container) { ((USpinBox*)Container)->ClearMinSliderValue(); };
		UEmmsWidgetHelpers::Attr_USpinBox_MaxSliderValue = UEmmsWidgetHelpers::GetWidgetAttrSpec("MaxSliderValue", USpinBox::StaticClass());
		UEmmsWidgetHelpers::Attr_USpinBox_MaxSliderValue->ResetToDefaultFunction = [](FEmmsAttributeSpecification*, void* Container) { ((USpinBox*)Container)->ClearMaxSliderValue(); };
		UEmmsWidgetHelpers::Attr_USpinBox_Delta = UEmmsWidgetHelpers::GetWidgetAttrSpec("Delta", USpinBox::StaticClass());
		FAngelscriptBinds::BindGlobalFunction("mm<USpinBox> SpinBox(float64&out OutValue)", &UEmmsWidgetHelpers::SpinBox);
		SCRIPT_BIND_DOCUMENTATION("Add an editable spinbox widget. The out float value will be set to whatever the user has entered");
		FAngelscriptBinds::BindGlobalFunction("mm<USpinBox> SpinBox(float64&out OutValue, float32 MinValue, float32 MaxValue, float32 Delta = 0)", &UEmmsWidgetHelpers::SpinBox_Constrained);
		SCRIPT_BIND_DOCUMENTATION("Add an editable spinbox widget. The out float value will be set to whatever the user has entered");

		UEmmsWidgetHelpers::Attr_USlider_Value = UEmmsWidgetHelpers::GetWidgetAttrSpec("Value", USlider::StaticClass());
		UEmmsWidgetHelpers::Attr_USlider_MinValue = UEmmsWidgetHelpers::GetWidgetAttrSpec("MinValue", USlider::StaticClass());
		UEmmsWidgetHelpers::Attr_USlider_MaxValue = UEmmsWidgetHelpers::GetWidgetAttrSpec("MaxValue", USlider::StaticClass());
		FAngelscriptBinds::BindGlobalFunction("mm<USlider> Slider(float64&out OutValue, float32 MinValue = 0, float32 MaxValue = 1)", &UEmmsWidgetHelpers::Slider);
		SCRIPT_BIND_DOCUMENTATION("Add an editable slider widget. The out float value will be set to whatever the user has entered");

		UEmmsWidgetHelpers::Attr_UBorder_Background = UEmmsWidgetHelpers::GetWidgetAttrSpec("Background", UBorder::StaticClass());
		// Optimized way of setting the background brush that does not require the widget to be recreated
		UEmmsWidgetHelpers::Attr_UBorder_Background->bRequiresWidgetRebuild = false;
		UEmmsWidgetHelpers::Attr_UBorder_Background->CompareValueFunction = [](FEmmsAttributeSpecification*, void* Container, void* PrevValue, void* NewValue)
		{
			FSlateBrush* PrevBrush = (FSlateBrush*)PrevValue;
			FSlateBrush* NewBrush = (FSlateBrush*)NewValue;
			return *PrevBrush != *NewBrush;
		};
		UEmmsWidgetHelpers::Attr_UBorder_Background->AssignValueFunction = [](FEmmsAttributeSpecification*, void* Container, void* Value)
		{
			((UBorder*)Container)->SetBrush(*(FSlateBrush*)Value);
		};

		FAngelscriptBinds::BindGlobalFunction("mm<UBorder> BeginBorder()", &UEmmsWidgetHelpers::BeginBorder);
		SCRIPT_BIND_DOCUMENTATION("Begin an immediate Border panel. All subsequent widgets will be contained within this panel until EndBorder() is called");
		FAngelscriptBinds::BindGlobalFunction("mm<UBorder> BeginBorder(const FLinearColor& Color)", &UEmmsWidgetHelpers::BeginBorder_Color);
		SCRIPT_BIND_DOCUMENTATION("Begin an immediate Border with the specified background color. All subsequent widgets will be contained within this panel until EndBorder() is called");
		FAngelscriptBinds::BindGlobalFunction("mm<UBorder> BeginBorder(const FLinearColor& Color, float32 RoundedCornerRadius, const FLinearColor& OutlineColor = FLinearColor::Black, float32 OutlineWidth = 0)", &UEmmsWidgetHelpers::BeginBorder_RoundedColor);
		SCRIPT_BIND_DOCUMENTATION("Begin an immediate Border with the specified background color and rounded corners. All subsequent widgets will be contained within this panel until EndBorder() is called");
		FAngelscriptBinds::BindGlobalFunction("mm<UBorder> BeginBorder(UTexture2D Texture)", &UEmmsWidgetHelpers::BeginBorder_Texture);
		SCRIPT_BIND_DOCUMENTATION("Begin an immediate Border with the specified background texture. All subsequent widgets will be contained within this panel until EndBorder() is called");
		FAngelscriptBinds::BindGlobalFunction("mm<UBorder> BeginBorder(UMaterialInterface Material)", &UEmmsWidgetHelpers::BeginBorder_Material);
		SCRIPT_BIND_DOCUMENTATION("Begin an immediate Border with the specified background material. All subsequent widgets will be contained within this panel until EndBorder() is called");
		FAngelscriptBinds::BindGlobalFunction("mm<UBorder> BeginBorder(const FSlateBrush& Brush)", &UEmmsWidgetHelpers::BeginBorder_Brush);
		SCRIPT_BIND_DOCUMENTATION("Begin an immediate Border with the specified background brush. All subsequent widgets will be contained within this panel until EndBorder() is called");
		FAngelscriptBinds::BindGlobalFunction("mm<UBorder> BeginBorder(const FName& BrushName)", &UEmmsWidgetHelpers::BeginBorder_StyleBrush);
		SCRIPT_BIND_DOCUMENTATION("Begin an immediate Border with the specified background brush. All subsequent widgets will be contained within this panel until EndBorder() is called");

		FAngelscriptBinds::BindGlobalFunction("mm<UBorder> WithinBorder()", &UEmmsWidgetHelpers::WithinBorder);
		SCRIPT_BIND_DOCUMENTATION("Begin an immediate Border panel. Only the one single widget that is drawn right after the WithinBorder() call will be placed inside the panel");
		FAngelscriptBinds::BindGlobalFunction("mm<UBorder> WithinBorder(const FLinearColor& Color)", &UEmmsWidgetHelpers::WithinBorder_Color);
		SCRIPT_BIND_DOCUMENTATION("Begin an immediate Border panel with the specified background color. Only the one single widget that is drawn right after the WithinBorder() call will be placed inside the panel");
		FAngelscriptBinds::BindGlobalFunction("mm<UBorder> WithinBorder(const FLinearColor& Color, float32 RoundedCornerRadius, const FLinearColor& OutlineColor = FLinearColor::Black, float32 OutlineWidth = 0)", &UEmmsWidgetHelpers::WithinBorder_RoundedColor);
		SCRIPT_BIND_DOCUMENTATION("Begin an immediate Border panel with the specified background color and rounded corners. Only the one single widget that is drawn right after the WithinBorder() call will be placed inside the panel");
		FAngelscriptBinds::BindGlobalFunction("mm<UBorder> WithinBorder(UTexture2D Texture)", &UEmmsWidgetHelpers::WithinBorder_Texture);
		SCRIPT_BIND_DOCUMENTATION("Begin an immediate Border panel with the specified background texture. Only the one single widget that is drawn right after the WithinBorder() call will be placed inside the panel");
		FAngelscriptBinds::BindGlobalFunction("mm<UBorder> WithinBorder(UMaterialInterface Material)", &UEmmsWidgetHelpers::WithinBorder_Material);
		SCRIPT_BIND_DOCUMENTATION("Begin an immediate Border panel with the specified background material. Only the one single widget that is drawn right after the WithinBorder() call will be placed inside the panel");
		FAngelscriptBinds::BindGlobalFunction("mm<UBorder> WithinBorder(const FSlateBrush& Brush)", &UEmmsWidgetHelpers::WithinBorder_Brush);
		SCRIPT_BIND_DOCUMENTATION("Begin an immediate Border panel with the specified background brush. Only the one single widget that is drawn right after the WithinBorder() call will be placed inside the panel");
		FAngelscriptBinds::BindGlobalFunction("mm<UBorder> WithinBorder(const FName& BrushName)", &UEmmsWidgetHelpers::WithinBorder_StyleBrush);
		SCRIPT_BIND_DOCUMENTATION("Begin an immediate Border panel with the specified background brush. Only the one single widget that is drawn right after the WithinBorder() call will be placed inside the panel");

		UEmmsWidgetHelpers::Attr_UImage_Brush = UEmmsWidgetHelpers::GetWidgetAttrSpec("Brush", UImage::StaticClass());
		UEmmsWidgetHelpers::Attr_UImage_ColorAndOpacity = UEmmsWidgetHelpers::GetWidgetAttrSpec("ColorAndOpacity", UImage::StaticClass());
		FAngelscriptBinds::BindGlobalFunction("mm<UImage> Image(UTexture2D Texture, float32 Width = 0, float32 Height = 0, const FLinearColor& ColorAndOpacity = FLinearColor::White)", &UEmmsWidgetHelpers::Image_Texture);
		SCRIPT_BIND_DOCUMENTATION("Add an image widget displaying the specified texture");
		FAngelscriptBinds::BindGlobalFunction("mm<UImage> Image(UMaterialInterface Material, float32 Width, float32 Height, const FLinearColor& ColorAndOpacity = FLinearColor::White)", &UEmmsWidgetHelpers::Image_Material);
		SCRIPT_BIND_DOCUMENTATION("Add an image widget displaying the specified material");
		FAngelscriptBinds::BindGlobalFunction("mm<UImage> Image(const FSlateBrush& Brush, float32 Width = 0, float32 Height = 0, const FLinearColor& ColorAndOpacity = FLinearColor::White)", &UEmmsWidgetHelpers::Image_Brush);
		SCRIPT_BIND_DOCUMENTATION("Add an image widget displaying the specified brush");
		FAngelscriptBinds::BindGlobalFunction("mm<UImage> Image(const FName& BrushName, float32 Width = 0, float32 Height = 0, const FLinearColor& ColorAndOpacity = FLinearColor::White)", &UEmmsWidgetHelpers::Image_StyleBrush);
		SCRIPT_BIND_DOCUMENTATION("Add an image widget displaying the specified brush");

		UEmmsWidgetHelpers::Attr_UListView_EntryWidgetClass = UEmmsWidgetHelpers::GetWidgetAttrSpec("EntryWidgetClass", UListView::StaticClass());
		FAngelscriptBinds::BindGlobalFunction("mm<UListView> ListView(int ItemCount)", &UEmmsWidgetHelpers::ListView_Indexed);
		SCRIPT_BIND_DOCUMENTATION("Add a ListView widget with ItemCount items in the list");
		FAngelscriptBinds::BindGlobalFunction("mm<UListView> ListView(const TArray<UObject>& ListItems)", &UEmmsWidgetHelpers::ListView);
		SCRIPT_BIND_DOCUMENTATION("Add a ListView widget with an item for each passed in object");
		FAngelscriptBinds::BindGlobalFunction("mm<UListView> ListView(const TArray<UObject>& ListItems, const TSubclassOf<UUserWidget>& ItemWidgetClass)", &UEmmsWidgetHelpers::ListView_Widgets);
		SCRIPT_BIND_DOCUMENTATION("Add a ListView widget with an item for each passed in object. Item widgets will use the given widget class to display");

		FAngelscriptBinds::BindGlobalFunction("mm<UComboBoxString> ComboBox(?&out EnumValue)", &UEmmsWidgetHelpers::ComboBox_Enum);
		SCRIPT_BIND_DOCUMENTATION("Add an editable combobox displaying the options of an enum. The out value must be an enum and will be modified to what the user has selected");
		FAngelscriptBinds::BindGlobalFunction("mm<UComboBoxString> ComboBox(const TArray<FName>& Items, FName&out OutSelectedItem)", &UEmmsWidgetHelpers::ComboBox_Names);
		SCRIPT_BIND_DOCUMENTATION("Add an editable combobox displaying the specified options. The out value will be modified to the FName selected by the user");
		FAngelscriptBinds::BindGlobalFunction("mm<UComboBoxString> ComboBox(const TArray<FName>& Items)", &UEmmsWidgetHelpers::ComboBox_Names_NoOut);
		SCRIPT_BIND_DOCUMENTATION("Add an editable combobox displaying the specified options");
		FAngelscriptBinds::BindGlobalFunction("mm<UComboBoxString> ComboBox(const TArray<FString>& Items, FString&out OutSelectedItem)", &UEmmsWidgetHelpers::ComboBox_Strings);
		SCRIPT_BIND_DOCUMENTATION("Add an editable combobox displaying the specified options. The out value will be modified to the FString selected by the user");
		FAngelscriptBinds::BindGlobalFunction("mm<UComboBoxString> ComboBox(const TArray<FString>& Items)", &UEmmsWidgetHelpers::ComboBox_Strings_NoOut);
		SCRIPT_BIND_DOCUMENTATION("Add an editable combobox displaying the specified options");

		UEmmsWidgetHelpers::Attr_UCheckBox_CheckedState = UEmmsWidgetHelpers::GetWidgetAttrSpec("CheckedState", UCheckBox::StaticClass());
		FAngelscriptBinds::BindGlobalFunction("mm<UCheckBox> CheckBox(bool&out OutValue)", &UEmmsWidgetHelpers::CheckBox);
		SCRIPT_BIND_DOCUMENTATION("Add an editable checkbox. The out boolean will be modified depending on whether the user checks the box");
		FAngelscriptBinds::BindGlobalFunction("mm<UCheckBox> CheckBox(bool&out OutValue, const FString& Label)", &UEmmsWidgetHelpers::CheckBox_Label);
		SCRIPT_BIND_DOCUMENTATION("Add an editable checkbox with the specified label text. The out boolean will be modified depending on whether the user checks the box");

		UEmmsWidgetHelpers::Attr_UScrollBox_Orientation = UEmmsWidgetHelpers::GetWidgetAttrSpec("Orientation", UScrollBox::StaticClass());
		FAngelscriptBinds::BindGlobalFunction("mm<UScrollBox> BeginScrollBox(EOrientation Orientation = EOrientation::Orient_Vertical)", &UEmmsWidgetHelpers::BeginScrollBox);
		SCRIPT_BIND_DOCUMENTATION("Begin an immediate ScrollBox with the given scroll orientation. All widgets will be placed inside it until mm::EndScrollBox() is called");
		FAngelscriptBinds::BindGlobalFunction("mm<UScrollBox> WithinScrollBox(EOrientation Orientation = EOrientation::Orient_Vertical)", &UEmmsWidgetHelpers::WithinScrollBox);
		SCRIPT_BIND_DOCUMENTATION("Begin an immediate ScrollBox with the given scroll orientation. Only the one single widget that is drawn right after the WithinScrollBox() call will be placed inside the panel");

		FAngelscriptBinds::BindGlobalFunction("mm<UVerticalBox> BeginVerticalBox()", &UEmmsWidgetHelpers::BeginVerticalBox);
		SCRIPT_BIND_DOCUMENTATION("Begin an immediate Vertical Box panel. All subsequent widgets will be contained within this panel until EndVerticalBox() is called");
		FAngelscriptBinds::BindGlobalFunction("mm<UVerticalBox> BeginVerticalBox(EHorizontalAlignment DefaultAlignment)", &UEmmsWidgetHelpers::BeginVerticalBox_Alignment);
		SCRIPT_BIND_DOCUMENTATION("Begin an immediate Vertical Box panel. The panel's slots will default to the specified alignment. All subsequent widgets will be contained within this panel until EndVerticalBox() is called");
		FAngelscriptBinds::BindGlobalFunction("mm<UVerticalBox> BeginVerticalBox(float64 DefaultPadding, EHorizontalAlignment DefaultAlignment = EHorizontalAlignment::HAlign_Fill)", &UEmmsWidgetHelpers::BeginVerticalBox_Padding);
		SCRIPT_BIND_DOCUMENTATION("Begin an immediate Vertical Box panel. The panel's slots will default to the specified padding and alignment. All subsequent widgets will be contained within this panel until EndVerticalBox() is called");
		FAngelscriptBinds::BindGlobalFunction("mm<UVerticalBox> BeginVerticalBox(float64 DefaultHorizontalPadding, float64 DefaultVerticalPadding, EHorizontalAlignment DefaultAlignment = EHorizontalAlignment::HAlign_Fill)", &UEmmsWidgetHelpers::BeginVerticalBox_Padding2D);
		SCRIPT_BIND_DOCUMENTATION("Begin an immediate Vertical Box panel. The panel's slots will default to the specified padding and alignment. All subsequent widgets will be contained within this panel until EndVerticalBox() is called");
		FAngelscriptBinds::BindGlobalFunction("mm<UVerticalBox> BeginVerticalBox(float64 DefaultLeftPadding, float64 DefaultTopPadding, float64 DefaultRightPadding, float64 DefaultBottomPadding, EHorizontalAlignment DefaultAlignment = EHorizontalAlignment::HAlign_Fill)", &UEmmsWidgetHelpers::BeginVerticalBox_Padding4D);
		SCRIPT_BIND_DOCUMENTATION("Begin an immediate Vertical Box panel. The panel's slots will default to the specified padding and alignment. All subsequent widgets will be contained within this panel until EndVerticalBox() is called");

		FAngelscriptBinds::BindGlobalFunction("mm<UHorizontalBox> BeginHorizontalBox()", &UEmmsWidgetHelpers::BeginHorizontalBox);
		SCRIPT_BIND_DOCUMENTATION("Begin an immediate Horizontal Box panel. All subsequent widgets will be contained within this panel until EndHorizontalBox() is called");
		FAngelscriptBinds::BindGlobalFunction("mm<UHorizontalBox> BeginHorizontalBox(EVerticalAlignment DefaultAlignment)", &UEmmsWidgetHelpers::BeginHorizontalBox_Alignment);
		SCRIPT_BIND_DOCUMENTATION("Begin an immediate Horizontal Box panel. The panel's slots will default to the specified alignment. All subsequent widgets will be contained within this panel until EndHorizontalBox() is called");
		FAngelscriptBinds::BindGlobalFunction("mm<UHorizontalBox> BeginHorizontalBox(float64 DefaultPadding, EVerticalAlignment DefaultAlignment = EVerticalAlignment::VAlign_Fill)", &UEmmsWidgetHelpers::BeginHorizontalBox_Padding);
		SCRIPT_BIND_DOCUMENTATION("Begin an immediate Horizontal Box panel. The panel's slots will default to the specified padding and alignment. All subsequent widgets will be contained within this panel until EndHorizontalBox() is called");
		FAngelscriptBinds::BindGlobalFunction("mm<UHorizontalBox> BeginHorizontalBox(float64 DefaultHorizontalPadding, float64 DefaultHorizontalPadding, EVerticalAlignment DefaultAlignment = EVerticalAlignment::VAlign_Fill)", &UEmmsWidgetHelpers::BeginHorizontalBox_Padding2D);
		SCRIPT_BIND_DOCUMENTATION("Begin an immediate Horizontal Box panel. The panel's slots will default to the specified padding and alignment. All subsequent widgets will be contained within this panel until EndHorizontalBox() is called");
		FAngelscriptBinds::BindGlobalFunction("mm<UHorizontalBox> BeginHorizontalBox(float64 DefaultLeftPadding, float64 DefaultTopPadding, float64 DefaultRightPadding, float64 DefaultBottomPadding, EVerticalAlignment DefaultAlignment = EVerticalAlignment::VAlign_Fill)", &UEmmsWidgetHelpers::BeginHorizontalBox_Padding4D);
		SCRIPT_BIND_DOCUMENTATION("Begin an immediate Horizontal Box panel. The panel's slots will default to the specified padding and alignment. All subsequent widgets will be contained within this panel until EndHorizontalBox() is called");

		FAngelscriptBinds::BindGlobalFunction("void Tooltip(const FString& TooltipText)", &UEmmsStatics::SetPendingTooltipText);
		SCRIPT_BIND_DOCUMENTATION("Set a tooltip for the next immediate widget that is added after this");
	}

	{
		auto mmUButton_ = FAngelscriptBinds::ExistingClass("mm<UButton>");
		UEmmsWidgetHelpers::Attr_UButton_WidgetStyle = UEmmsWidgetHelpers::GetWidgetAttrSpec("WidgetStyle", UButton::StaticClass());
		UEmmsWidgetHelpers::Event_UButton_OnClicked = UEmmsWidgetHelpers::GetWidgetEvent("OnClicked", UButton::StaticClass());
		mmUButton_.Method("bool opImplConv() const", &UEmmsWidgetHelpers::Button_ImplBoolConv);
		mmUButton_.Method("void SetButtonStyleColor(const FLinearColor& StyleColor) const", &UEmmsWidgetHelpers::SetButtonStyleColor);
		mmUButton_.Method("void SetInnerPadding(float32 Horizontal, float32 Vertical) const", &UEmmsWidgetHelpers::SetButtonInnerPadding);
		SCRIPT_BIND_DOCUMENTATION("Set the padding of the label widget inside the button");
	}

	{
		auto mmUComboBoxString_ = FAngelscriptBinds::ExistingClass("mm<UComboBoxString>");
		mmUComboBoxString_.Method("int GetSelectedIndex() const", &UEmmsWidgetHelpers::GetComboBoxSelectedIndex);
		SCRIPT_BIND_DOCUMENTATION("Get the index of the item that's currently selected in the combo box");
	}

	{
		auto mmUTextBlock_ = FAngelscriptBinds::ExistingClass("mm<UTextBlock>");
		mmUTextBlock_.Method("void SetFontSize(float32 FontSize) const", &UEmmsWidgetHelpers::SetTextFontSize);
		SCRIPT_BIND_DOCUMENTATION("Change the font size for the displayed text");
		mmUTextBlock_.Method("void SetFontFace(FName FontFace) const", &UEmmsWidgetHelpers::SetTextFontFace);
		SCRIPT_BIND_DOCUMENTATION("Change the font typeface for the displayed text");
		mmUTextBlock_.Method("void SetBold(bool bBold) const", &UEmmsWidgetHelpers::SetTextBold);
		SCRIPT_BIND_DOCUMENTATION("Change the font typeface to bold for the displayed text");
	}

	{
		auto mmUListView_ = FAngelscriptBinds::ExistingClass("mm<UListView>");
		mmUListView_.Method("void SetListItems(const TArray<UObject>& ListItems) const", &UEmmsWidgetHelpers::SetListItems);
		SCRIPT_BIND_DOCUMENTATION("Update the list items displayed by the ListView");
		mmUListView_.Method("int GetSelectedIndex() const", &UEmmsWidgetHelpers::GetSelectedIndex);
		SCRIPT_BIND_DOCUMENTATION("Get the index of the currently selected ListView item");
		mmUListView_.Method("void SetSelectedIndex(int Index) const", &UEmmsWidgetHelpers::SetSelectedIndex);
		SCRIPT_BIND_DOCUMENTATION("Set the index of the item that will be selected");
		mmUListView_.Method("bool IsItemSelected(int Index) const", &UEmmsWidgetHelpers::IsItemSelected);
		SCRIPT_BIND_DOCUMENTATION("Get selection for the item at index, only supported in 'Multi' selection mode");
		mmUListView_.Method("void SetItemSelection(int Index, bool bSelected) const", &UEmmsWidgetHelpers::SetItemSelection);
		SCRIPT_BIND_DOCUMENTATION("Set selection for the item at index, only supported in 'Multi' selection mode");
		mmUListView_.Method("void SetDefaultSelectedIndex(int Index) const", &UEmmsWidgetHelpers::SetListViewDefaultSelectedIndex);
		SCRIPT_BIND_DOCUMENTATION("Set the index of the item that will be selected if no other item is currently selected");
		mmUListView_.Method("UObject GetSelectedItem(const TSubclassOf<UObject>& ItemType) const", &UEmmsWidgetHelpers::GetSelectedItem);
		FAngelscriptBinds::SetPreviousBindArgumentDeterminesOutputType(0);
		SCRIPT_BIND_DOCUMENTATION("Get the item object associated with the currently selected item. The item object will be retrieved if it has the same type that was passed in");

		struct FMMListViewIterator
		{
			UListView* ListView = nullptr;
			UMMListViewEntryWidget* NextWidget = nullptr;
			int VisibleIndex = 0;
		};

		FBindFlags ItFlags;
		auto FMMListViewIterator_ = FAngelscriptBinds::ValueClass<FMMListViewIterator>("FMMListViewIterator", ItFlags);
		FMMListViewIterator_.Constructor("void f()", [](void* Memory)
		{
			new (Memory) FMMListViewIterator;
		});

		FMMListViewIterator_.Constructor("void f(const FMMListViewIterator& Other)", [](void* Memory, const FMMListViewIterator& Other)
		{
			new (Memory) FMMListViewIterator(Other);
		});

		FMMListViewIterator_.Method("bool GetCanProceed() const property", [](FMMListViewIterator& Iterator) -> bool
		{
			return Iterator.NextWidget != nullptr;
		});

		FMMListViewIterator_.Method("UMMListViewEntryWidget Proceed() const", [](FMMListViewIterator& Iterator) -> UMMListViewEntryWidget*
		{
			if (Iterator.ListView == nullptr)
				return nullptr;

			UMMListViewEntryWidget* CurrentWidget = Iterator.NextWidget;
			Iterator.NextWidget = nullptr;

			auto& Entries = Iterator.ListView->GetDisplayedEntryWidgets();
			Iterator.VisibleIndex += 1;
			while (Iterator.VisibleIndex < Entries.Num())
			{
				UMMListViewEntryWidget* EntryWidget = Cast<UMMListViewEntryWidget>(Entries[Iterator.VisibleIndex]);
				if (EntryWidget == nullptr || EntryWidget->GetItemIndex() == -1)
				{
					Iterator.VisibleIndex += 1;
					continue;
				}

				Iterator.NextWidget = EntryWidget;
				break;
			}

			return CurrentWidget;
		});

		FMMListViewIterator_.Method("UMMListViewEntryWidget& if_handle_then_const Iterate() const", [](FMMListViewIterator& Iterator) -> void*
		{
			if (Iterator.ListView == nullptr)
				return nullptr;

			auto& Entries = Iterator.ListView->GetDisplayedEntryWidgets();

			while (true)
			{
				if (Iterator.VisibleIndex >= Entries.Num())
					return nullptr;

				UMMListViewEntryWidget* EntryWidget = Cast<UMMListViewEntryWidget>(Entries[Iterator.VisibleIndex]);
				if (EntryWidget == nullptr || EntryWidget->GetItemIndex() == -1)
				{
					Iterator.VisibleIndex += 1;
					continue;
				}

				Iterator.NextWidget = EntryWidget;
				break;
			}

			Iterator.VisibleIndex += 1;
			return &Iterator.NextWidget;
		});

		mmUListView_.Method("const FMMListViewIterator Iterator() const",
		   [](FEmmsWidgetHandle& Handle) -> FMMListViewIterator
		   {
				FMMListViewIterator Iterator;
				if (Handle.Element != nullptr)
				{
					Iterator.ListView = Cast<UListView>(Handle.Element->UMGWidget);
				}

				auto& Entries = Iterator.ListView->GetDisplayedEntryWidgets();
				Iterator.VisibleIndex = 0;
				while (Iterator.VisibleIndex < Entries.Num())
				{
					UMMListViewEntryWidget* EntryWidget = Cast<UMMListViewEntryWidget>(Entries[Iterator.VisibleIndex]);
					if (EntryWidget == nullptr || EntryWidget->GetItemIndex() == -1)
					{
						Iterator.VisibleIndex += 1;
						continue;
					}

					Iterator.NextWidget = EntryWidget;
					break;
				}

				return Iterator;
		   });
	}

	{
		auto mmUWidget_ = FAngelscriptBinds::ExistingClass("mm<UWidget>");

		UEmmsWidgetHelpers::Attr_UWidget_RenderTransform = UEmmsWidgetHelpers::GetWidgetAttrSpec("RenderTransform", UWidget::StaticClass());
		mmUWidget_.Method("bool IsHovered() const", &UEmmsWidgetHelpers::IsHovered);
		SCRIPT_BIND_DOCUMENTATION("Whether the widget is currently hovered by the user");
		mmUWidget_.Method("bool IsValid() const", &UEmmsWidgetHelpers::IsWidgetValid);
		SCRIPT_BIND_DOCUMENTATION("Whether this widget currently exists");
		mmUWidget_.Method("bool HasAnyUserFocus() const", &UEmmsWidgetHelpers::HasAnyUserFocus);
		SCRIPT_BIND_DOCUMENTATION("Whether the widget is currently focused by the user");
		mmUWidget_.Method("const FGeometry& GetCachedGeometry() const", &UEmmsWidgetHelpers::GetCachedGeometry);
		SCRIPT_BIND_DOCUMENTATION("Get the geometry that the widget was rendered at the previous frame");

		mmUWidget_.Method("void SetRenderScale(float32 UniformScale) const", &UEmmsWidgetHelpers::SetRenderScale_Uniform);
		SCRIPT_BIND_DOCUMENTATION("Set the render scaling of the widget");
		mmUWidget_.Method("void SetRenderScale(const FVector2D& Scale) const", &UEmmsWidgetHelpers::SetRenderScale_Vector);
		SCRIPT_BIND_DOCUMENTATION("Set the render scaling of the widget");
		mmUWidget_.Method("void SetRenderScale(float32 HorizontalScale, float32 VerticalScale) const", &UEmmsWidgetHelpers::SetRenderScale);
		SCRIPT_BIND_DOCUMENTATION("Set the render scaling of the widget");

		mmUWidget_.Method("void SetRenderTranslation(const FVector2D& Translation) const", &UEmmsWidgetHelpers::SetRenderTranslation_Vector);
		SCRIPT_BIND_DOCUMENTATION("Set the render translation of the widget");
		mmUWidget_.Method("void SetRenderTranslation(float32 XTranslation, float32 YTranslation) const", &UEmmsWidgetHelpers::SetRenderTranslation);
		SCRIPT_BIND_DOCUMENTATION("Set the render translation of the widget");

		mmUWidget_.Method("void SetRenderTransformAngle(float32 Angle) const", &UEmmsWidgetHelpers::SetRenderTransformAngle);
		SCRIPT_BIND_DOCUMENTATION("Set the rotation angle the widget is rendered at");

		UEmmsWidgetHelpers::Attr_UWidget_ToolTipText = UEmmsWidgetHelpers::GetWidgetAttrSpec("ToolTipText", UWidget::StaticClass());
		mmUWidget_.Method("void SetToolTipText(const FString& Text) const", &UEmmsWidgetHelpers::SetToolTipText);
		SCRIPT_BIND_DOCUMENTATION("Set the tooltip displayed when hovering over the widget");
	}

	{
		auto mmUBorder_ = FAngelscriptBinds::ExistingClass("mm<UBorder>");
		UEmmsWidgetHelpers::Event_UBorder_OnMouseButtonDownEvent = UEmmsWidgetHelpers::GetWidgetDelegate("OnMouseButtonDownEvent", UBorder::StaticClass());
		UEmmsWidgetHelpers::Event_UBorder_OnMouseDoubleClickEvent = UEmmsWidgetHelpers::GetWidgetDelegate("OnMouseDoubleClickEvent", UBorder::StaticClass());
		mmUBorder_.Method("UMaterialInstanceDynamic GetDynamicMaterial() const", &UEmmsWidgetHelpers::GetBorderDynamicMaterial);
		SCRIPT_BIND_DOCUMENTATION("Get a dynamic material instance for the material currently being displayed by the border");
		mmUBorder_.Method("bool WasClicked() const", &UEmmsWidgetHelpers::WasBorderClicked);
		SCRIPT_BIND_DOCUMENTATION("Whether the user clicked the left mouse button on the border this last frame");
		mmUBorder_.Method("bool WasRightClicked() const", &UEmmsWidgetHelpers::WasBorderRightClicked);
		SCRIPT_BIND_DOCUMENTATION("Whether the user clicked the right mouse button on the border this last frame");
		mmUBorder_.Method("bool WasMiddleClicked() const", &UEmmsWidgetHelpers::WasBorderMiddleClicked);
		SCRIPT_BIND_DOCUMENTATION("Whether the user clicked the middle mouse button on the border this last frame");
		mmUBorder_.Method("bool WasDoubleClicked() const", &UEmmsWidgetHelpers::WasBorderDoubleClicked);
		SCRIPT_BIND_DOCUMENTATION("Whether the user double clicked on the border this last frame");
	}

	{
		auto mmUImage_ = FAngelscriptBinds::ExistingClass("mm<UImage>");
		mmUImage_.Method("UMaterialInstanceDynamic GetDynamicMaterial() const", &UEmmsWidgetHelpers::GetImageDynamicMaterial);
		SCRIPT_BIND_DOCUMENTATION("Get a dynamic material instance for the material currently being displayed by the image");
	}
});

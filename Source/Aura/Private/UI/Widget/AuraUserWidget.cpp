// Copyright C&D Games


#include "UI/Widget/AuraUserWidget.h"

void UAuraUserWidget::SetPositionInViewportAnchored(FVector2D Position, FVector2D Anchor, bool bRemoveDPIScale)
{
	FVector2D PositionChange = - GetWidgetSize() * Anchor;
	SetPositionInViewport(Position + PositionChange, bRemoveDPIScale);
}

void UAuraUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	OnWidgetControllerSet();
}

FVector2D UAuraUserWidget::GetWidgetSize_Implementation()
{
	return GetCachedGeometry().GetLocalSize();
}

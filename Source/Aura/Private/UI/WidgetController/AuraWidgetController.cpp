// Copyright C&D Games


#include "UI/WidgetController/AuraWidgetController.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& ControllerParams)
{
	PlayerController = ControllerParams.PlayerController;
	AbilitySystemComponent = ControllerParams.AbilitySystemComponent;
	PlayerState = ControllerParams.PlayerState;
	AttributeSet = ControllerParams.AttributeSet;
}

void UAuraWidgetController::BindCallbacksToDependencies()
{

}

void UAuraWidgetController::BroadcastInitialValues()
{

}

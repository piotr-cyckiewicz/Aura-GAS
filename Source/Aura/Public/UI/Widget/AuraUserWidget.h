// Copyright C&D Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintCallable)
	void SetPositionInViewportAnchored(FVector2D Position, FVector2D Anchor, bool bRemoveDPIScale = true);

	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector2D GetWidgetSize();

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnWidgetControllerSet();
};
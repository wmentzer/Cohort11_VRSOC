// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/UMG/Public/Slate/WidgetRenderer.h"
#include "Components/StereoLayerComponent.h"
#include "Engine/Classes/Engine/TextureRenderTarget2D.h"
#include "SOCStereoLayerBase.generated.h"

/**
 * 
 */
UCLASS()
class VR_SOC_API USOCStereoLayerBase : public UStereoLayerComponent
{
	GENERATED_BODY()
	

protected:
	USOCStereoLayerBase();

	FWidgetRenderer* WidgetRenderer;

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction);

	UUserWidget* targetWidget;

public:

	UFUNCTION(BlueprintCallable, Category = RenderTexture)
	void WidgetToTexture(class UUserWidget* widget, FVector2D drawSize);

	UPROPERTY(BlueprintReadWrite, Category = RenderTexture)
	class UTextureRenderTarget2D* TextureRenderTarget;

	UFUNCTION(BlueprintCallable, Category = RenderTexture)
	void SetUserWidget(UUserWidget* widget);

	UFUNCTION(BlueprintCallable, Category = RenderTexture)
	void ClearUserWidget();

};

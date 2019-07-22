// Fill out your copyright notice in the Description page of Project Settings.

#include "SOCStereoLayerBase.h"
#include "Engine/Classes/Engine/TexturerenderTarget2D.h"
#include "Runtime/Slate/Public/Framework/Application/SlateApplication.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Slate/WidgetRenderer.h"


USOCStereoLayerBase::USOCStereoLayerBase()
{

	TextureRenderTarget = CreateDefaultSubobject<UTextureRenderTarget2D>(TEXT("TextureTarget"));

	WidgetRenderer = new FWidgetRenderer(true);

	this->bLiveTexture = true;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}



void USOCStereoLayerBase::WidgetToTexture(class UUserWidget* widget, FVector2D drawSize)
{

	if (widget != nullptr)
	{

		TSharedPtr<SWidget> SlateWidget(widget->TakeWidget());
		if (!SlateWidget) return;

		 this->SetTexture(WidgetRenderer->DrawWidget(SlateWidget.ToSharedRef(), drawSize));


	}


}

void USOCStereoLayerBase::TickComponent(float DeltaTime, enum ELevelTick TickType,FActorComponentTickFunction * ThisTickFunction)
{

	if (targetWidget != nullptr)
	{
		
		this->WidgetToTexture(targetWidget, FVector2D(1920.0f, 1080.0f));

	}
	else
	{

		

	}

}

void USOCStereoLayerBase::SetUserWidget(UUserWidget* widget)
{
	UE_LOG(LogTemp, Warning, TEXT("ORACLE OH ORACLE MY SKIN HAS MANY POREACLES"));
	targetWidget = widget;

	return;

}
void USOCStereoLayerBase::ClearUserWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("ORACLE OH ORACLE please stop"));
	targetWidget = nullptr;

	return;

}
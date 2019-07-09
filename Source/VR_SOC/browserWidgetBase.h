// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "browserWidgetBase.generated.h"

/**
 * Essentially an empty class with a boolean in it, designed to be a c++ interactable template class for the browser widget. 
 */
UCLASS()
class VR_SOC_API UbrowserWidgetBase : public UUserWidget
{
	GENERATED_BODY()


protected:

public:

	// property that changes the checkbox in the browser widget, follow up in blueprints.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool problemFound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BPURL)
	FString url;


};

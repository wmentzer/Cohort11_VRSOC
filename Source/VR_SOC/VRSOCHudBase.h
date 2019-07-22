// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "VRSOCHudBase.generated.h"

/**
 * 
 */
UCLASS()
class VR_SOC_API AVRSOCHudBase : public AHUD
{
	GENERATED_BODY()





public:
	AVRSOCHudBase();

protected:
	virtual void BeginPlay();


	
};

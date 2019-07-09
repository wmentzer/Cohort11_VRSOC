// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZoomableActorBase.generated.h"

UCLASS()
class VR_SOC_API AZoomableActorBase : public AActor
{
	GENERATED_BODY()

		FTransform baseTransform;

	
	
public:	
	// Sets default values for this actor's properties
	AZoomableActorBase();

	UFUNCTION(BlueprintCallable)
		void ResetPosition();
	UFUNCTION(BlueprintCallable)
		void DetachNoChanges();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

};

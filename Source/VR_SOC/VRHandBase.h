// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MotionControllerComponent.h"
#include "AnimationBase.h"
#include "Runtime/Engine/Classes/Animation/AnimSequence.h"
#include "VRHandBase.generated.h"

/**
 * 
 */
UCLASS()
class VR_SOC_API UVRHandBase : public UMotionControllerComponent
{
	GENERATED_BODY()

	/* Mesh for the hand*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* handMesh;

	/* Widget interaction component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetInteractionComponent* interactionComponent;

	/* Spline for teleport line, unused*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USplineComponent* teleportSpline;

	/* Animation blueprint for the hand. */
	UAnimationBase* handMeshAnimation;

	/* The name of the static mesh socket on the hand mesh.*/
	FString handSocket;

	/* If the hand is attached to something. */
	bool handAttached;

	int8 delayTimer;
	
	/* Simple toggle variable to allow the pointer key release events to fire. */
	bool pointerKeyPressed;


public:


	UVRHandBase();

protected:
	virtual void BeginPlay();

	

		

public:

	/* Function to grab objects, called by attached pawn. Unimplemented. */
	void Grab();

	/* Function to release objects, called by attached pawn. Unimplemented. */
	void GrabRelease();

	/* Flips the scale for the left hand. */
	void SetLeft();

	/* Selects widgets, called by attached pawn. */
	void SelectWidget();

	/* Grabs a widget or toggles a button, called by attached pawn. */
	void ZoomWidget();

	/* Drops the grabbed widget, called by attached pawn. */
	void DropWidget();

	/* Fires a pointer release event to satisfy the requirements of widgets. */
	void ZoomWidgetRelease();

};

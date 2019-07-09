// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WidgetInteractionComponent.h"
#include "WidgetComponent.h"
#include "VRHandBase.h"
#include "SOCStereoLayerBase.h"
#include "Engine/Classes/Components/StereoLayerComponent.h"
#include "VRCharacterBase.generated.h"

UCLASS()
class VR_SOC_API AVRCharacterBase : public ACharacter
{
	GENERATED_BODY()

	/* First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	/* Motion controller (right hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UVRHandBase* R_MotionController;

	/* Motion controller (left hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UVRHandBase* L_MotionController;

	/* Player Controller */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	APlayerController* myController;
	/* Animation for our body mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAnimInstance* bodyMeshAnimation;

public:
	// Sets default values for this character's properties
	AVRCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay();

public:	

	/* Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/* Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;



protected:

	/** Resets HMD orientation and position in VR. */
	void OnResetVR();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/* Called via input. Calls the Grab function of each hand. */
	void Grab();

	/* Called via input. Calls the GrabRelease function of each hand. */
	void GrabRelease();

	/* Unused function to deal with additional window interaction. */
	void ExpandWindow();

	/* Called via input. Calls the select function of each hand. */
	void SelectWidget();

	void SelectWidgetRelease();
	

	
protected:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	/* Unused function for future screen implementation. */
	void setScreen(int screenNum, FString* url);

};

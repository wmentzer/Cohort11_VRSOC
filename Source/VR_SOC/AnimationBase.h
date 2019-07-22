// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimationBase.generated.h"

// Abstractable animation enumeration. 
UENUM(BlueprintType)
enum class GenericAnimationEnum : uint8 
{
	DEFAULTSTATE,
	ANIMATIONSTATE1,
	ANIMATIONSTATE2,
	ANIMATIONSTATE3,
	ANIMATIONSTATE4,
	ANIMATIONSTATE5


};


UCLASS()
class VR_SOC_API UAnimationBase : public UAnimInstance
{
	GENERATED_BODY()



public:

	//A bunch of passer properties that can be used by C++ classes to pass down commands to an animation instance.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CPPBool;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int64 CPPInt64;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString CPPString;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float CPPFloat;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	GenericAnimationEnum CPPAnimation;


	
};

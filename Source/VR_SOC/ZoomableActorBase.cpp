// Fill out your copyright notice in the Description page of Project Settings.


#include "ZoomableActorBase.h"

// Sets default values
AZoomableActorBase::AZoomableActorBase()
{

}

// Called when the game starts or when spawned
void AZoomableActorBase::BeginPlay()
{
	Super::BeginPlay();
	
	baseTransform = this->GetTransform(); // Gets the widgets transform when the game starts so that it can reset positions. 
}


void AZoomableActorBase::ResetPosition()
{

	if (this->GetAttachParentActor() != nullptr)
	{
		this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);  // FDetachmentTransformRules is an enumerator that specifies to the system how it deals with detachment.
		this->SetActorTransform(baseTransform);                                // Resets to its base transform. 
		
	}

}

void AZoomableActorBase::DetachNoChanges()
{

	if (this->GetAttachParentActor() != nullptr)
	{

		this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);  // FDetachmentTransformRules is an enumerator that specifies to the system how it deals with detachment.

	}

}
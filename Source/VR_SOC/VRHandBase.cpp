// Fill out your copyright notice in the Description page of Project Settings.


#include "VRHandBase.h"
#include "Engine.h"
#include "WidgetInteractionComponent.h"
#include "WidgetComponent.h"
#include "ZoomableActorBase.h"
#include "AnimationBase.h"
#include "Runtime/Engine/Classes/Animation/AnimSequence.h"
#include "browserWidgetBase.h"

UVRHandBase::UVRHandBase()
{

	// Adding subcomponents to components can only be done in c++. There is no component dropdown in the blueprint editor.
	handMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Handmesh")); 
	handMesh->SetupAttachment(this);                                                 // Components do not have a "RootComponent" identifier, but if you attach to "this" is achieves the same thing.

	// See VRCharacterBase for an explanation of ConstructorHelpers.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> handSkeletalMesh(TEXT("/Game/VirtualReality/Mannequin/Character/Mesh/MannequinHand_Right"));
	if (handSkeletalMesh.Succeeded()) 
	{
		handMesh->SetSkeletalMesh(handSkeletalMesh.Object);
	}

	// Set up the widget interaction component.
	interactionComponent = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("InteractionComponent"));
	interactionComponent->SetupAttachment(this);

	// The hand mesh makes no sense, sizewise, for a human hand. At 75% it ends up being mostly reasonable.
	handMesh->SetRelativeScale3D(FVector(0.75f, 0.75f, 0.75f));
	handMesh->SetWorldRotation(FRotator(0.0f, 0.0f, 90.0f));

	// Gets the animation blueprint and sets the animation class of the static mesh. 
	static ConstructorHelpers::FObjectFinder<UClass> handAnimationFinder(TEXT("Class'/Game/VirtualReality/Mannequin/Animations/RightHand_AnimBP.RightHand_AnimBP_C'"));
	handMesh->SetAnimInstanceClass(handAnimationFinder.Object);

	

	handSocket = FString(TEXT("HandGrabSocket"));  // HandGrabSocket is the name of the static mesh socket. 

	delayTimer = 0; // Unused as of yet, for delayed setup. 

}

// Called when the game starts or the object is instantiated. 
void UVRHandBase::BeginPlay()
{

	Super::BeginPlay();

	// Uncomment this for laser pointer lines. 
	interactionComponent->bShowDebug = true; 

	// Adjusts the mesh to a location closer to your wrist.
	handMesh->SetRelativeLocation(FVector(-10.0f, 0.0f, 0.0f));
	

	// Makes the hands invisible in multiplayer.
	handMesh->SetOnlyOwnerSee(true);

	handMeshAnimation = (UAnimationBase*)handMesh->GetAnimInstance();

	

}

// Called from the owning pawn, grabs objects. 
void UVRHandBase::Grab()
{
	

}
// Called from the owning pawn, releases objects.
void UVRHandBase::GrabRelease()
{


}

// Flips the hand model for the left hand. 
void UVRHandBase::SetLeft()
{

	handMesh->SetRelativeScale3D(FVector(0.75f, -0.75f, 0.75f));
	handMesh->SetWorldRotation(FRotator(0.0f, 0.0f, -90.0f));

}

// Selects whatever widget is currently being hovered.
void UVRHandBase::SelectWidget()
{
	// A pointer to the widget component currently being hovered. 
	UWidgetComponent* hoveredComp;

	if (interactionComponent->GetHoveredWidgetComponent() != nullptr)                            // Checks if an object is hovered at all
	{
		hoveredComp = interactionComponent->GetHoveredWidgetComponent();                         
		if (hoveredComp->GetUserWidgetObject()->IsA(UbrowserWidgetBase::StaticClass()))          // Uses the IsA function to check its class against the C++ browser widget base class.
		{
			UbrowserWidgetBase* temp = (UbrowserWidgetBase *)hoveredComp->GetUserWidgetObject(); // Casts the widget to the browser widget base in order to access the c++ variable within.
			temp->problemFound = !temp->problemFound;                                            // Sets problemFound, which changes the checkbox.

		}
		else
		{
			
		} 
	}
	

	
}

// Grabs whatever widget is currently being hovered. 
void UVRHandBase::ZoomWidget()
{
	
	if (interactionComponent->GetHoveredWidgetComponent() != nullptr && handAttached == false)                            // Checks if an object is hovered at all
	{
		if(interactionComponent->GetHoveredWidgetComponent()->GetOwner()->IsA(AZoomableActorBase::StaticClass()))
		{
			interactionComponent->GetHoveredWidgetComponent()->GetAttachmentRootActor()->AttachToComponent(this->handMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(*handSocket)); //Attach it to the mesh socket
			handAttached = true;                                                                                                        // handAttached toggles the behavior of this function. 
			interactionComponent->GetHoveredWidgetComponent()->GetOwner()->SetActorRelativeRotation(FRotator(0.0f, -30.0f, 0.0f));      // The animation rotation causes some rotation, this is a hacky fix to that.
			handMeshAnimation->CPPAnimation = GenericAnimationEnum::ANIMATIONSTATE2;                                                    // Change to the grabbing animation. 
		}
		else
		{
			interactionComponent->PressPointerKey(FKey(TEXT("LeftMouseButton")));         // Click on things if you cant grab them.
			pointerKeyPressed = true;                                                     // This lets you release the click. 
		}

		
		
	}
	else if (handAttached == true)
	{

		TArray<USceneComponent*> tempComp = handMesh->GetAttachChildren();
		for(USceneComponent* comp : tempComp)           // Finds the zoomable widget child and then detaches it. 
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *comp->GetOwner()->GetFName().ToString());
			if (comp->GetOwner()->IsA(AZoomableActorBase::StaticClass()))
			{
				AZoomableActorBase* detach = (AZoomableActorBase *)comp->GetOwner();
				detach->ResetPosition();

				handAttached = false;
				handMeshAnimation->CPPAnimation = GenericAnimationEnum::DEFAULTSTATE;
			}

		}

		

	}


}

// Drops a grabbed widget without resetting it to its original position. 
// This is the same as what happens in zoomwidget but is called on a different button press and calls a different function on the zoomable actor. 
void UVRHandBase::DropWidget()
{

	if (handAttached == true)
	{
		TArray<USceneComponent*> tempComp = handMesh->GetAttachChildren();                   
		for (USceneComponent* comp : tempComp)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *comp->GetOwner()->GetFName().ToString());
			if (comp->GetOwner()->IsA(AZoomableActorBase::StaticClass()))
			{
				AZoomableActorBase* detach = (AZoomableActorBase *)comp->GetOwner();
				detach->DetachNoChanges();

				handAttached = false;
				handMeshAnimation->CPPAnimation = GenericAnimationEnum::DEFAULTSTATE;
			}

		}

	}


}

void UVRHandBase::ZoomWidgetRelease()
{

	if (pointerKeyPressed == true)
	{

		interactionComponent->ReleasePointerKey(FKey(TEXT("LeftMouseButton")));
		pointerKeyPressed = false;

	}

}

// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacterBase.h"
#include "Engine.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/HUD.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "IXRTrackingSystem.h"
#include "VRHandBase.h"
#include "VRSOCHudBase.h"
#include "WidgetComponent.h"
#include "SOCStereoLayerBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/Classes/Components/StereoLayerComponent.h"

// Class constructor. Sets up the components of the player object.
// C++ Constructor is called before the game is ever begun, it is called on C++ class compilation which happens before playing in the editor/during packaging.
AVRCharacterBase::AVRCharacterBase()
{
	// Set size for collision capsule. The ACharacter class comes with a collision capsule inherently.
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input.
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent, then attach it to the capsule component. 
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->bUsePawnControlRotation = false;                        // If this is true then the controllers will be bound to the movement of the HMD, which we don't want.


	// Create VR Controllers
	// CreateDefaultSubobject is only usable in a constructor, if you want to create actors or components later use GWorld->SpawnActor or ConstructObject + RegisterComponent.
	R_MotionController = CreateDefaultSubobject<UVRHandBase>(TEXT("RMotionController")); 
	R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;      // Motion controllers default to the left hand as a motion source, you have to set the right manually.
	L_MotionController = CreateDefaultSubobject<UVRHandBase>(TEXT("LMotionController"));
	L_MotionController->SetLeft();                                                      // Internal function to flip the left hand model.
	R_MotionController->SetupAttachment(RootComponent);                                 // Attach both hands to the RootComponent.
	L_MotionController->SetupAttachment(RootComponent);

	// Set up the multiplayer skeletal mesh 
	// ConstructorHelpers::FObjectFinder is a constructor only way to find resources, such as objects (FObjectFinder) or classes (FClassFinder). 
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> bodyMeshFinder(TEXT("/Game/Mannequin/Character/Mesh/SK_Mannequin")); 
	this->GetMesh()->SetSkeletalMesh(bodyMeshFinder.Object);                           // Characters come with a subcomponent for a skeletal mesh, use it if you can rather than make new subobjects.

	// Set up the animation class for the skeletal mesh
	// This is an FObjectFinder getting a UClass instead of a FClassFinder because you need to instantiate an object of an animation blueprint to use it for your skeletal mesh.
	// Getting a blueprint from an FObjectFinder you need to use the specifier ("Class'/[path]/[blueprintname].[blueprintname]_C'") as you are getting the UClass associated with the blueprint.
	static ConstructorHelpers::FObjectFinder<UClass> bodyAnimationFinder(TEXT("Class'/Game/Mannequin/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP_C'")); 
	this->GetMesh()->SetAnimInstanceClass(bodyAnimationFinder.Object);


	//PrimaryActorTick.bCanEverTick = true;
	//PrimaryActorTick.bStartWithTickEnabled = true;

}

// Called when the game starts or when spawned.
void AVRCharacterBase::BeginPlay()
{
	Super::BeginPlay(); 
	
	// Can't get your individual controller in the constructor because they are set at runtime, but you need to make sure you have one before you do it. 
	//TODO: add an async call to get the controller if this fails.
	if(GetController())
	{
		myController = (APlayerController *)GetController();
	}

	this->GetMesh()->SetOwnerNoSee(true); // Setting your mesh to only be visible in multiplayer is best done at runtime for indeterminate reasons.

	

}


// Called to bind functionality to input.
void AVRCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Check is a standard UE4 assertion that can verify the existence of something with a a Hard Kill assosciated with it (that's not a technical term btw).
	// If the playerinputcomponent is screwed up your runtime is totally boned so the check will just end your program.
	check(PlayerInputComponent); 

	PlayerInputComponent->BindAction("ExpandWindow", IE_Pressed, this, &AVRCharacterBase::ExpandWindow);
	PlayerInputComponent->BindAction("Grab", IE_Pressed, this, &AVRCharacterBase::Grab);
	PlayerInputComponent->BindAction("GrabRelease", IE_Released, this, &AVRCharacterBase::GrabRelease);
	PlayerInputComponent->BindAction("Select", IE_Pressed, this, &AVRCharacterBase::SelectWidget);
	PlayerInputComponent->BindAction("SelectRelease", IE_Released, this, &AVRCharacterBase::SelectWidgetRelease);

	// Binds the camera to the motion of the HMD
	PlayerInputComponent->BindAxis("TurnRate", this, &AVRCharacterBase::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AVRCharacterBase::LookUpAtRate);

	// Uncomment these if you want to use thumbstick locomotion.
	/*PlayerInputComponent->BindAxis("MoveForward", this, &AVRCharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AVRCharacterBase::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);*/


}

// Standard VR functionality, should be included just to help reorient on a calibration.
void AVRCharacterBase::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

// Grab function.
// Checks which grab key was pressed and then registers the event on the hand components.
// Hard coded which controller button is used; could easily be reworked to be able to modify the keybind.
void AVRCharacterBase::Grab() 
{
	if (myController->WasInputKeyJustPressed(FKey(TEXT("MotionController_Left_Grip1"))))
	{
		L_MotionController->Grab();
	}
	if (myController->WasInputKeyJustPressed(FKey(TEXT("MotionController_Right_Grip1"))))
	{

		R_MotionController->Grab();

	}

}

// Grab release function. 
// See Grab for more detail.
void AVRCharacterBase::GrabRelease()
{

	if (myController->WasInputKeyJustReleased(FKey(TEXT("MotionController_Left_Grip1"))))
	{
		L_MotionController->GrabRelease();
	}
	if (myController->WasInputKeyJustReleased(FKey(TEXT("MotionController_Right_Grip1"))))
	{

		R_MotionController->GrabRelease();

	}


}

void AVRCharacterBase::MoveForward(float Val)
{
	
	if (Val != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Val);
	}


}

void AVRCharacterBase::MoveRight(float Val)
{
	if (Val != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Val);
	}

}

void AVRCharacterBase::TurnAtRate(float Rate) 
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());

}

void AVRCharacterBase::LookUpAtRate(float Rate)
{
	
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());

}
// Unused function for future screen implementation.
void AVRCharacterBase::setScreen(int num, FString* url)
{



}
// Unused function for future screen implementation.
void AVRCharacterBase::ExpandWindow() 
{

	if (myController->WasInputKeyJustPressed(FKey(TEXT("MotionController_Left_FaceButton1")))) 
	{

		L_MotionController->ZoomWidget();

	}
	else if (myController->WasInputKeyJustPressed(FKey(TEXT("MotionController_Right_FaceButton1"))))
	{

		R_MotionController->ZoomWidget();

	}
	else if (myController->WasInputKeyJustPressed(FKey(TEXT("MotionController_Left_FaceButton2"))))
	{

		L_MotionController->DropWidget();

	}
	else if (myController->WasInputKeyJustPressed(FKey(TEXT("MotionController_Right_FaceButton2"))))
	{

		R_MotionController->DropWidget();

	}

}

// Select function.
// Called to use the widget interaction components from the hands.
void AVRCharacterBase::SelectWidget()
{

	if (myController->WasInputKeyJustPressed(FKey(TEXT("MotionController_Left_Trigger"))) )
	{


		L_MotionController->SelectWidget();

	}
	if (myController->WasInputKeyJustPressed(FKey(TEXT("MotionController_Right_Trigger"))) || myController->WasInputKeyJustPressed(FKey(TEXT("LeftMouseButton"))))
	{

		R_MotionController->SelectWidget();

	}


}

void AVRCharacterBase::SelectWidgetRelease()
{

	if (myController->WasInputKeyJustReleased(FKey(TEXT("MotionController_Left_FaceButton1"))))
	{
		L_MotionController->ZoomWidgetRelease();
	}
	if (myController->WasInputKeyJustReleased(FKey(TEXT("MotionController_Right_FaceButton1"))))
	{

		R_MotionController->ZoomWidgetRelease();

	}


}
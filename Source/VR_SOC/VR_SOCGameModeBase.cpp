// Fill out your copyright notice in the Description page of Project Settings.


#include "VR_SOCGameModeBase.h"
#include "VRCharacterBase.h"
#include "VRSOCHudBase.h"
#include "UObject/ConstructorHelpers.h"

AVR_SOCGameModeBase::AVR_SOCGameModeBase() : Super()
{
	// Sets the default pawn
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/SOCContent/Blueprints/ClientPlayer"));
	if (PlayerPawnClassFinder.Succeeded()) 
	{
		DefaultPawnClass = PlayerPawnClassFinder.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not find default player."));
	}

	points = 0;



	// Uncomment this if you use the HUD class.
	// HUDClass = AVRSOCHudBase::StaticClass();
}

void AVR_SOCGameModeBase::BeginPlay()
{

	// Randomizes the issue, then casts it to the enumerator. 
	currentIssueNetwork1 = (IssueTypes)FMath::RandRange(0, 21);
	currentIssueNetwork2 = (IssueTypes)FMath::RandRange(0, 21);
	if (currentIssueNetwork1 == IssueTypes::NOISSUE && currentIssueNetwork2 == IssueTypes::NOISSUE) // In the case that they both roll a 0
	{

		int32 whichOne = FMath::RandRange(0, 1); // Have one of them roll a nonzero number. 
		if (whichOne == 0)
		{

			currentIssueNetwork1 = (IssueTypes)FMath::RandRange(1, 21);

		}
		else
		{

			currentIssueNetwork2 = (IssueTypes)FMath::RandRange(1, 21);

		}


	}


}

void AVR_SOCGameModeBase::SubmitAnswer(bool correct, bool whichScreen)
{

	if (correct == true && whichScreen == true)
	{
		
		points++;

		currentIssueNetwork2 = (IssueTypes)FMath::RandRange(0, 21);
		if (currentIssueNetwork1 == IssueTypes::NOISSUE && currentIssueNetwork2 == IssueTypes::NOISSUE)
		{

			int32 whichOne = FMath::RandRange(0, 1);
			if (whichOne == 0)
			{

				currentIssueNetwork1 = (IssueTypes)FMath::RandRange(1, 21);

			}
			else
			{

				currentIssueNetwork2 = (IssueTypes)FMath::RandRange(1, 21);

			}


		}

	}
	else if (correct == true && whichScreen == false)
	{

		points++;

		currentIssueNetwork1 = (IssueTypes)FMath::RandRange(0, 21);
		if (currentIssueNetwork1 == IssueTypes::NOISSUE && currentIssueNetwork2 == IssueTypes::NOISSUE)
		{

			int32 whichOne = FMath::RandRange(0, 1);
			if (whichOne == 0)
			{

				currentIssueNetwork1 = (IssueTypes)FMath::RandRange(1, 21);

			}
			else
			{

				currentIssueNetwork2 = (IssueTypes)FMath::RandRange(1, 21);

			}


		}

	}
	else
	{

		points--;

	}



}
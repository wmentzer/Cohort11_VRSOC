// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VR_SOCGameModeBase.generated.h"


// Enum of all the possible issue scenarios. 
UENUM(BlueprintType)
enum class IssueTypes : uint8
{

	NOISSUE,
	DDOS,
	MALWAREADNS,
	MALWAREDATABASE,
	MALWAREEMAIL,
	MALWAREWORKSTATION,
	MULTIPLEADDNS,
	MULTIPLELINUX,
	MULTIPLEWORKSTATION,
	PHISHINGWORKSTATION,
	RANSOMWAREWORK,
	ROOTACCESSWORK,
	SUSPICIOUSPORTDMZ,
	SUSPICIOUSPORTLAN,
	SUSPICIOUSPORTLAN2,
	UNAUTHDNS,
	UNAUTHEMAIL,
	UNAUTHFILESHARE,
	UNAUTHLINUX,
	UNAUTHTRACKER,
	UNAUTHWORKSTATION,
	WEBSITEDEFACE



};
UCLASS()
class VR_SOC_API AVR_SOCGameModeBase : public AGameModeBase
{
	GENERATED_BODY()


protected:
	virtual void BeginPlay() override;
	
public:
	AVR_SOCGameModeBase();
	
	/* The players score. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float points;
	
	/* The issues for network one and two. Read by the widget blueprints that control the video widget and other logic. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	IssueTypes currentIssueNetwork1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	IssueTypes currentIssueNetwork2;

	/* Called by the submit button on the network threat widget to increment or deincrement the score and change the scenarios. */
	UFUNCTION(BlueprintCallable)
	void SubmitAnswer(bool correct, bool whichScreen);
	
	
};

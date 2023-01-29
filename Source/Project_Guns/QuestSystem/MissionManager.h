// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MissionObjective.h"
#include "MissionManager.generated.h"

/**
 * 
 */

UCLASS(Blueprintable, BlueprintType)
class PROJECT_GUNS_API UMissionManager : public UObject
{
	GENERATED_BODY()

public:
	virtual void PostInitProperties() override;
	UWorld* GetWorld() const;
	
	virtual void BeginPlay();
	void InitiateMissionManager();
	void MissionTerminated(bool bWasSuccessful);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "List of Missions")
		TArray<TSubclassOf<UMissionObjective>> MissionClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "List of Missions")
		TSubclassOf<UMissionObjective> AlternateClass;

	UPROPERTY(BlueprintReadWrite, Category = "List of Missions")
		UMissionManager* BlueprintDefaults;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Manage Mission")
		UMissionObjective* MissionObj;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Manage Mission")
		int32 CurrMission = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Manage Mission")
		int32 CurrTask = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Manage Mission")
		int32 TotalTask = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Manage Mission")
		bool MissionStarted = false;

	// Delegate Bound Function
	void ObjectiveCompleted(FName TaskName, bool bWasSuccessful);

	// Setting Up Mission
	void SetupMission();

	/**
	 * Variables held for each mission.
	 *  Reward for the Variable Values.
	 * How am I going to perceive this at the End.
	 */



};

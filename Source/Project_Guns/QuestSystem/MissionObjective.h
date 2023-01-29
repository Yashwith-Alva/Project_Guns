// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MissionObjective.generated.h"

/**
 *
 *  
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FObjectiveCompleteSignature, FName, TaskName, bool, bWasSuccessful);

UENUM(BlueprintType)
enum class ETaskType : uint8
{
	ETT_GoToLocation	UMETA(DisplayName = "Go To Location"),
	ETT_Survive			UMETA(DisplayName = "Survive"),
	ETT_TimeVsLocation	UMETA(DisplayName = "Time Vs Location"),
	EIT_DeathParade		UMETA(DisplayName = "Death Parade"),
	EIT_KillBoss		UMETA(DisplayName = "KillBoss"),
	EIT_Retrieve		UMETA(DisplayName = "Retrieve"),
	EIT_Fixing			UMETA(DisplayName = "Fixing Objects"),

	ETT_MAX				UMETA(DisplayName = "Default MAX")
};

USTRUCT(BlueprintType)
struct FTaskInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName TaskName = FName(TEXT("Task Name - 01"));

	UPROPERTY( EditAnywhere, BlueprintReadWrite)
		ETaskType TaskType = ETaskType::ETT_GoToLocation;

	UPROPERTY( EditAnywhere, BlueprintReadWrite)
		FVector TriggerSpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> TriggerBox;

	UPROPERTY( EditAnywhere, BlueprintReadWrite)
		AActor* DefendActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SurviveTime;

	UPROPERTY( EditAnywhere, BlueprintReadWrite)
		float TotalTime;

	UPROPERTY(Instanced, EditAnywhere, BlueprintReadWrite)
		AActor* RetrieveActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ACharacter* Boss;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ACharacter* ActorToKill;
};

UCLASS(Blueprintable, BlueprintType)
class PROJECT_GUNS_API UMissionObjective : public UObject
{
	GENERATED_BODY()
	
public:

	//
	// From the POV of Mission
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission")
		int32 CurrentTask = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission")
		bool bCompleted = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission")
		FName MissionName = FName(TEXT("MissionName - 01"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission")
		FName MissionDescription = FName(TEXT("Mission Description"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "List of Task")
		TArray<FTaskInfo> TaskInfo;

	//
	// Custom Delegate to bind callbacks to.
	//
	FObjectiveCompleteSignature ObjectiveCompleteSignature;

	void SetupTask();

	//
	// Not called outside this class
	//
	void OnObjectiveComplete(FName TaskName, bool bWasSuccessful);

private:
	FDelegateHandle ObjectiveCompleteHandle;
	
	//
	// Setting up Environment based on TaskType
	//
	void GoToLocationSetup(FTaskInfo Task);
	void DeathParadeSetup(FTaskInfo Task);
};

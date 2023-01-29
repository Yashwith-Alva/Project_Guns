/*****************************************************************//**
 * \file   MissionManager.cpp
 * \brief  This is the first thing to be run after instantiation of the object.
 *		   Once the Object is instantiated we need the Manager Setup to find the
 *		   correct mission class and instantiate it.
 *         Once Instantiated the mission must be binded to the ObjectCompleteSignature.
 *         Once you are triggered from the Delegate, Check if there is any task left.
 *         If there are then move on to it.
 *         If not terminate the mission by detaching it from the delegate.
 * 
 * 
 * \NOTE  At a time there is only one mission running.
 * 
 * \author Yashwith D Alva
 * \date   January 2023
 *********************************************************************/
#include "MissionManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void UMissionManager::PostInitProperties()
{
	Super::PostInitProperties();

	if (ThisClass::GetWorld())
	{
		BeginPlay();
	}
}

void UMissionManager::BeginPlay()
{
	InitiateMissionManager();
}

UWorld* UMissionManager::GetWorld() const
{
	if (GIsEditor && !GIsPlayInEditorWorld)
	{
		return nullptr;
	}
	else if (GetOuter())
	{
		return GetOuter()->GetWorld();
	}

	return nullptr;
}

void UMissionManager::InitiateMissionManager()
{	
	if (MissionClass.IsValidIndex(CurrTask))
	{
		TSubclassOf<UMissionObjective> CurrentMissionClass = MissionClass[CurrTask];
		check(CurrentMissionClass);

		if (CurrentMissionClass)
		{
			MissionObj = NewObject<UMissionObjective>(CurrentMissionClass);
			check(MissionObj);
			if (MissionObj)
			{
				UE_LOG(LogTemp, Error, TEXT("I have Reached till creating the Mission Obj"));
				MissionObj->ObjectiveCompleteSignature.AddDynamic(this, &UMissionManager::ObjectiveCompleted);
				SetupMission();
			}
		}
	}
	UE_LOG(LogTemp, Error, TEXT("Something went Wrong"));
	return;
}

void UMissionManager::ObjectiveCompleted(FName TaskName, bool bWasSuccessful)
{
	// Terminate the current task and start a new one.
	// New task will be followed by the game mode, and game mode must do the arrangement.

	/*bool bIsValid = MissionObj
		&& MissionObj->TaskInfo.Num() > CurrTask;

 	if (bIsValid)
	{
		CurrTask += 1;
	}
	else
	{
		MissionTerminated(true);
	}*/

	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, FString::Printf(TEXT("Mission Completed!!")));
}

void UMissionManager::MissionTerminated(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		MissionObj->ObjectiveCompleteSignature.Clear();
		MissionObj = nullptr;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange, FString::Printf(TEXT("Mission Coundn't be terminated!!")));
	}
}

void UMissionManager::SetupMission()
{
	if (MissionObj)
	{
		MissionObj->SetupTask();
	}
}

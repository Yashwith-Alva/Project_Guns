/*****************************************************************//**
 * \file   GunsGameMode.cpp
 * \brief  1. Mission Manager is Instantiated.
 *		   2. ManagerObj is updated from gameinstance.
 *		   3. Call setup of manager which creates the first mission.
 *		   4. Get things ready for that mission.
 * 
 * \author Yashwith D Alva
 * \date   January 2023
 *********************************************************************/


#include "GunsGameMode.h"

void AGunsGameMode::BeginPlay()
{
	Super::BeginPlay();

	MissionManagerObj = NewObject<UMissionManager>(this, MissionManagerClass);

	//Collect info from game instance and modify MissionManagerObj and then Setup.

	if (MissionManagerObj)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange, FString::Printf(TEXT("Instantiated Manager Class")));
	}
}

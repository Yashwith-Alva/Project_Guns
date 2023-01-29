/*****************************************************************//**
 * \file   GunsGameMode.h
 * \brief  This is the main Game Mode for GUNS Game, this is going to
 *		   instantiate UMissionManager and holds a object reference of it.
 * 
 * \author Yashwith D Alva
 * \date   January 2023
 *********************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Project_Guns/QuestSystem/MissionManager.h"
#include "GunsGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GUNS_API AGunsGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission Initiate")
		TSubclassOf<UMissionManager> MissionManagerClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mission Initiate")
		UMissionManager* MissionManagerObj;

protected:
	virtual void BeginPlay() override;
};

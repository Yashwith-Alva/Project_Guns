/*****************************************************************//**
 * \file   Objective.h
 * \brief  
 * 
 * \author yashw
 * \date   January 2023
 *********************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Objective.generated.h"

/**
 * 
*/


UCLASS()
class PROJECT_GUNS_API UObjective : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Details")
		int32 TaskId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Details")
		FName TaskName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Details")
		float sim;
};
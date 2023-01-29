/*****************************************************************//**
 * \file   EnemyLogicController.h
 * \brief  Defines Enemy Logic and Movement towards the player.
 * 
 * \author Yashwith D Alva
 * \date   January 2023
 *********************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyLogicController.generated.h"

/**
 * 
 */

class UBehaviorTree;

UCLASS()
class PROJECT_GUNS_API AEnemyLogicController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyLogicController();
	virtual void Tick(float DeltaTime);
	bool IsDead() const;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
		UBehaviorTree* AIBehavior;


};

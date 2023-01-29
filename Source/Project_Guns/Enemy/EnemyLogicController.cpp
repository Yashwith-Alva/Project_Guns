/*****************************************************************//**
 * \file   EnemyLogicController.cpp
 * \brief  This expands on certain logic written in the Base Enemy
 *	       class. Currently not really trying to do something extremly
 *	       outside the box. Simple random AI system with some technique
 *         that I have thought of.
 * 
 * \author Yashwith D Alva
 * \date   January 2023
 *********************************************************************/

#include "EnemyLogicController.h"
#include "Kismet/GameplayStatics.h"
#include "BaseEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyLogicController::AEnemyLogicController()
{
	//
}

void AEnemyLogicController::BeginPlay()
{
	Super::BeginPlay();

	if (AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
	}
}

void AEnemyLogicController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AEnemyLogicController::IsDead() const
{
	ABaseEnemy* EnemyCharacter = Cast<ABaseEnemy>(GetPawn());
	if (EnemyCharacter)
	{
		return EnemyCharacter->bIsCrouched;
	}

	return true;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MissionObjective.h"
#include "Project_Guns/Character/BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Project_Guns/Environment/Trigger.h"

/**
 * Whenever I complete this task, External taask will call this function.
 * This inturn calls broadcast which reaches the manager function bound
 * to this delegate.
 * Manager function hence will update the task to next one and then wait
 * for the delegate to be active.
 * 
 * \param TaskName			-> Name of the Task that got completed.
 * \param bWasSuccessful
 */
void UMissionObjective::OnObjectiveComplete(FName TaskName, bool bWasSuccessful)
{
	ABaseCharacter* Player = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (Player)
	{
		Player->SetCharacterSpeed(10000.f);
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("[Objective]: OnObjectiveComplete")));
	}

	//Storing Delegate in Delegate handle is not implemented.
	ObjectiveCompleteSignature.Broadcast(TaskName, bWasSuccessful);
}

void UMissionObjective::SetupTask()
{
	if (!GEngine)
		return;

	// Create the task related setup in the environment.
	// Loop through the info and place objects in the world!.
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Setting Up Task!!")));

	if (TaskInfo.IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("Task IS empty!!")));
		return;
	}

	FTaskInfo* Task = TaskInfo.GetData();
	for (int i = 0; i < TaskInfo.Num(); i++)
	{
		switch (Task[i].TaskType)
		{
		case ETaskType::ETT_GoToLocation:
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("TLocation Setup!!")));
			GoToLocationSetup(Task[i]);
			break;
		case ETaskType::EIT_DeathParade:
			DeathParadeSetup(Task[i]);
			break;
		default:
			break;
		}
	}
}

void UMissionObjective::GoToLocationSetup(FTaskInfo Task)
{	
	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail;

	auto SpawnedActor = GetWorld()->SpawnActor<AActor>(Task.TriggerBox, Task.TriggerSpawnLocation, FRotator::ZeroRotator ,SpawnParams);
	ATrigger* Trigger = Cast<ATrigger>(SpawnedActor);
	if (Trigger)
	{
		Trigger->bTaskActive = true;
		Trigger->TaskName = Task.TaskName;
		Trigger->TaskPriority = CurrentTask;
	}
}

void UMissionObjective::DeathParadeSetup(FTaskInfo Task)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), Task.ActorToKill->GetClass(), FoundActors);

	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Magenta, FString::Printf(TEXT("Total Actor: %d"), FoundActors.Num()));
}

/*****************************************************************//**
 * \file   Trigger.cpp
 * \brief  Trigger when Player Character Enters.
 * 
 * \author Yashwith D Alva
 * \date   January 2023
 *********************************************************************/

#include "Trigger.h"
#include "Components/BoxComponent.h"
#include "Project_Guns/Character/BaseCharacter.h"
#include "Project_Guns/QuestSystem/MissionObjective.h"
#include "Project_Guns/QuestSystem/MissionManager.h"
#include "Project_Guns/GUNSGameMode/GunsGameMode.h"
#include "Kismet/GameplayStatics.h"

ATrigger::ATrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	SetRootComponent(TriggerBox);
}

void ATrigger::BeginPlay()
{
	Super::BeginPlay();
	GunsMode = Cast<AGunsGameMode>(UGameplayStatics::GetGameMode(this));
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, FString::Printf(TEXT("New Trigger")));
}

void ATrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		PlayerCharacter = Cast<ABaseCharacter>(OtherActor);
		if (PlayerCharacter && GunsMode && bTaskActive)
		{
			GunsMode->MissionManagerObj->MissionObj->OnObjectiveComplete(TaskName, true);
		}
	}
}

void ATrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		PlayerCharacter = Cast<ABaseCharacter>(OtherActor);
		if (PlayerCharacter && GunsMode)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("Exit the trigger Box")));
		}
	}
}



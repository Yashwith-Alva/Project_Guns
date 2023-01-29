/*****************************************************************//**
 * \file   Trigger.h
 * \brief  Trigger based on the character Entering it.
 *		   I might modify it to do something more than just that.
 * 
 * \author Yashwith D Alva
 * \date   January 2023
 *********************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Trigger.generated.h"

class UBoxComponent;

UCLASS()
class PROJECT_GUNS_API ATrigger : public AActor
{
	GENERATED_BODY()

public:
	ATrigger();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BoxComp")
		UBoxComponent* TriggerBox;

	class AGunsGameMode* GunsMode;
	class ABaseCharacter* PlayerCharacter;

public:
	// Overlap Begin and End Function.
	// Go to gamemode acess the manager, then access the mission obj, then fire the event.
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Task Details")
		FName TaskName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Task Details")
		int32 TaskPriority = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Task Details")
		bool bTaskActive = false;
};


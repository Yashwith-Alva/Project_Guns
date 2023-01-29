/*****************************************************************//**
 * \file   Casing.h
 * \brief  Casing fall from the side of gun.
 * 
 * \author Yashwith D Alva
 * \date   January 2023
 *********************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Casing.generated.h"

UCLASS()
class PROJECT_GUNS_API ACasing : public AActor
{
	GENERATED_BODY()
	
public:	
	ACasing();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* CasingMesh;

	UPROPERTY(EditDefaultsOnly)
		float ShellEjectionImpulse;

	UPROPERTY(EditDefaultsOnly)
		class USoundCue* ShellSound;

};

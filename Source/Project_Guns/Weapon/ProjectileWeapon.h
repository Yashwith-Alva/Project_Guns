/*****************************************************************//**
 * \file   ProjectileWeapon.h
 * \brief  Extension of Weapon class, Can fire from a gun both automatic and semi-automatic.
 * 
 * \author Yashwith D Alva
 * \date   January 2023
 *********************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "ProjectileWeapon.generated.h"

/**
 * 
 */
class AProjectileBullet;

UCLASS()
class PROJECT_GUNS_API AProjectileWeapon : public AWeapon
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Fire(const FVector& HitTarget) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Damage")
		TSubclassOf<AProjectileBullet> BulletClass;
};

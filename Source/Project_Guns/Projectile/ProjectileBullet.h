/*****************************************************************//**
 * \file   ProjectileBullet.h
 * \brief  Projectile Bullet is a projectile type for guns.
 * 
 * \author yashw
 * \date   January 2023
 *********************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "ProjectileBullet.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GUNS_API AProjectileBullet : public AProjectile
{
	GENERATED_BODY()

protected:
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

};

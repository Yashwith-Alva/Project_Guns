/*****************************************************************//**
 * \file   ProjectileWeapon.cpp
 * \brief  Extending Weapon Class. This class does Ammo Management,
 *		   Weapon Damage, Reload, Mag capacity and Firing related
 *		   Queries.
 * 
 * \author Yashwith D Alva
 * \date   January 2023
 *********************************************************************/
#include "ProjectileWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Project_Guns/Projectile/ProjectileBullet.h"
#include "Project_Guns/Character/BaseCharacter.h"
#include "DrawDebugHelpers.h"

void AProjectileWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectileWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);

	const USkeletalMeshSocket* ProjectileSocket = GetMesh()->GetSocketByName(FName("MuzzleFlash"));
	if (ProjectileSocket && BulletClass)
	{
		
		FTransform SocketTransform = ProjectileSocket->GetSocketTransform(WeaponMesh);
		FVector ToTarget = HitTarget - SocketTransform.GetLocation();
		FRotator TargetRotation = ToTarget.Rotation();
	
		FActorSpawnParameters SpawnParams;
		if (GetOwner())
		{
			ABaseCharacter* PlayerCharacter = Cast<ABaseCharacter>(GetOwner());
			if (PlayerCharacter)
			{
				SpawnParams.Owner = GetOwner();
				SpawnParams.Instigator = PlayerCharacter->GetInstigator();
			}
		}
		AProjectileBullet* SpawnedBullet = GetWorld()->SpawnActor<AProjectileBullet>(BulletClass, SocketTransform.GetLocation(), TargetRotation, SpawnParams);
	}
}
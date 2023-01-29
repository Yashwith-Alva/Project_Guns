/*****************************************************************//**
 * \file   ProjectileBullet.cpp
 * \brief  Projectile Bullet is used for guns.
 * 
 * \author yashw
 * \date   January 2023
 *********************************************************************/


#include "ProjectileBullet.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Project_Guns/Character/BaseCharacter.h"
#include "Project_Guns/GunsComponent/CombatComponent.h"
#include "Project_Guns/Weapon/Weapon.h"

void AProjectileBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (OtherActor && OwnerCharacter && OwnerCharacter->GetCombat() && OwnerCharacter->GetCombat()->EquippedWeapon)
	{	
		AController* OwnerController = OwnerCharacter->Controller;
		UGameplayStatics::ApplyDamage(OtherActor, OwnerCharacter->GetCombat()->EquippedWeapon->GetWeaponDamage(), OwnerController, this, UDamageType::StaticClass());
	}

	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}

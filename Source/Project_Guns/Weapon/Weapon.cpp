/*****************************************************************//**
 * \file   Weapon.cpp
 * \brief  This class consist of weapon that could be equipped and dropped.
 *		   It cant fire. The fire function is implemented on a inherited class.
 * 
 * \author Yashwith D Alva
 * \date   January 2023
 *********************************************************************/

#include "Weapon.h"
#include "Project_Guns/Character/BaseCharacter.h"
#include "Project_Guns/Projectile/Casing.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	EquipBox = CreateDefaultSubobject<UBoxComponent>(TEXT("EquipBox"));
	EquipBox->SetupAttachment(GetRootComponent());
	EquipBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	EquipBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	EquipBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
	EquipBox->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnBoxEndOverlap);

	CurrMag = MagCapacity;
	CurrAmmo = MaxAmmoCapacity;
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseCharacter* PlayerCharacter = Cast<ABaseCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		PlayerCharacter->SetOverlappingWeapon(this);
	}
}

void AWeapon::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABaseCharacter* PlayerCharacter = Cast<ABaseCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		PlayerCharacter->SetOverlappingWeapon(nullptr);
	}
}

#pragma region Weapon Utility
void AWeapon::DropWeapon()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	WeaponMesh->SetSimulatePhysics(true);
	SetOwner(nullptr);
}

void AWeapon::Fire(const FVector& HitTarget)
{
	if (FireAnimation)
	{
		WeaponMesh->PlayAnimation(FireAnimation, false);
	}

	const USkeletalMeshSocket* EjectSocket = WeaponMesh->GetSocketByName("AmmoEject");
	if (EjectSocket && CasingClass)
	{
		FTransform SocketTransform = EjectSocket->GetSocketTransform(WeaponMesh);
		FActorSpawnParameters SpawnParams;
		GetWorld()->SpawnActor<ACasing>(CasingClass, SocketTransform.GetLocation(), SocketTransform.GetRotation().Rotator(), SpawnParams);
	}
	CurrMag -= 1;
}

bool AWeapon::CanReload()
{
	return ( CurrMag < MagCapacity && CurrAmmo > 0);
}

void AWeapon::Reload()
{
	if (!CanReload())
		return;

	if (CurrAmmo > MagCapacity)
	{
		CurrAmmo -= MagCapacity - CurrMag;
		CurrMag = MagCapacity;
	}
	else if (CurrAmmo <= MagCapacity)
	{
		if (MagCapacity - CurrMag < CurrAmmo)
		{
			CurrAmmo -= MagCapacity - CurrMag;
			CurrMag = MagCapacity;
		}
		else
		{
			CurrMag += CurrAmmo;
			CurrAmmo = 0;
		}
	}
	else if (CurrAmmo < 1)
	{
		return;
	}
}
#pragma endregion

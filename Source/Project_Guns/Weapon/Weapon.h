/*****************************************************************//**
 * \file   Weapon.h
 * \brief  Weapon related functionality for the character.
 * 
 * \author Yashwith D Alva
 * \date   January 2023
 *********************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UAnimationAsset;
class UBoxComponent;
class ACasing;
class UParticleSystem;
class USkeletalMeshComponent;
class USoundBase;
class UTexture2D;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_AssualtRifle	UMETA(DisplayName="Rifle"),
	EWT_SMG				UMETA(DisplayName="SMG"),
	EWT_Shotgun			UMETA(DisplayName="Shotgun"),
	EWT_Sniper			UMETA(DisplayName="Sniper"),
	EWT_Pistol			UMETA(DisplayName="Pistol"),
	
	EWT_None			UMETA(DisplayName="None"),
	EWT_MAX			UMETA(DisplayName="DefaultMAX")
};

UCLASS()
class PROJECT_GUNS_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();
	virtual void Tick(float DeltaTime) override;
	virtual void Fire(const FVector& HitTarget);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void OnBoxOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
		void OnBoxEndOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Ammo", meta = (AllowPrivateAccess = "true"))
		int32 MagCapacity = 30;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Ammo", meta = (AllowPrivateAccess = "true"))
		int32 MaxAmmoCapacity = 90;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Damage", meta = (AllowPrivateAccess = "true"))
		int32 DamageAmount = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Damage", meta = (AllowPrivateAccess = "true"))
		float HeadShotMultiplier = 1.25f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Damage", meta = (AllowPrivateAccess = "true"))
		float BodyShotMultiplier = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Damage", meta = (AllowPrivateAccess = "true"))
		float LegShotMultiplier = 0.75f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Properties")
		UAnimationAsset* FireAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Properties")
		UTexture2D* WeaponIcon;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Properties")
		TSubclassOf<ACasing> CasingClass;

	UPROPERTY(EditAnywhere, Category = "Weapon | Fire")
		float FireDelay = 0.15f;

	int32 CurrMag;
	int32 CurrAmmo;

	bool CanReload();
	
public:
	UPROPERTY(VisibleAnywhere, Category = "WeaponMesh")
		USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "EquipBox")
		UBoxComponent* EquipBox;

	UPROPERTY(EditAnywhere, Category = "Weapon | Weapon Type")
		EWeaponType WeaponType = EWeaponType::EWT_Pistol;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Fire")
		bool  bAutoFire = false;

#pragma region Crosshairs
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Crosshairs")
		UTexture2D* CrosshairsCenter;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Crosshairs")
		UTexture2D* CrosshairsLeft;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Crosshairs")
		UTexture2D* CrosshairsRight;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Crosshairs")
		UTexture2D* CrosshairsTop;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Crosshairs")
		UTexture2D* CrosshairsBottom;
#pragma endregion

#pragma region Utility Functions
	void Reload();
	void DropWeapon();
	bool IsEmpty() { return CurrMag < 1; }
#pragma endregion

#pragma region FORCEINLINE
	FORCEINLINE USkeletalMeshComponent* GetMesh() { return WeaponMesh; }
	FORCEINLINE int32 GetMag() { return CurrMag; }
	FORCEINLINE int32 GetAmmo() { return CurrAmmo; }
	FORCEINLINE void SetAmmo(int32 Amount) { CurrAmmo += Amount; }
	FORCEINLINE float GetFireDelay() { return FireDelay; }
	FORCEINLINE UTexture2D* GetWeaponIcon() { return WeaponIcon; }
	FORCEINLINE float GetWeaponDamage() { return DamageAmount; }
#pragma endregion
};

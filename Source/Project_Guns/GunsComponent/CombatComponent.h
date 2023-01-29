/*****************************************************************//**
 * \file   CombatComponent.h
 * \brief  This component helps in combat related queries.
 * 
 * \author Yashwith D Alva
 * \date   January 2023
 *********************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Project_Guns/HUD/GunsHUD.h"
#include "CombatComponent.generated.h"


#define TRACE_LENGTH 900000.f

class ABaseCharacter;
class AProjectileGrenades;
class AWeapon;
class UAnimMontage;

UENUM(BlueprintType)
enum class EInventoryItem : uint8
{
	EII_PrimaryWeapon		UMETA(DisplayName="Primary Weapon"),
	EII_SecondaryWeapon		UMETA(DisplayName="Secondary Weapon"),
	EII_Knife				UMETA(DisplayName="Knife"),
	EII_PrimaryGrenade		UMETA(DisplayName="Primary Grenade"),
	EII_SecondaryGrenade	UMETA(DisplayName="Secondary Grenade"),

	EII_MAX					UMETA(DisplayName="DefaultMAX")
};

USTRUCT(BlueprintType)
struct FInventory
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bag")
		AWeapon* Primary;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bag")
		AWeapon* Secondary;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bag")
		AProjectileGrenades* PrimaryGrenade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bag")
		AProjectileGrenades* SecondaryGrenade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bag")
		int32 PrimaryGrenadeCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bag")
		int32 SecondaryGrenadeCount = 0;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_GUNS_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	ABaseCharacter* PlayerCharacter;
	AWeapon* EquippedWeapon;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory | PrimaryWeapon")
		TSubclassOf<AWeapon> PrimaryWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory | SecondaryWeapon")
		TSubclassOf<AWeapon> SecondaryWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bag")
		FInventory Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory | InventoryType")
		EInventoryItem CurrInventoryItem = EInventoryItem::EII_PrimaryWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Animation | Fire")
		UAnimMontage* FireAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Animation | Reload")
		UAnimMontage* ReloadAnimation;
	
	void EquipWeapon();
	void DropWeapon();
	void SwitchWeapon();
	void Reload();
	void FireButtonPressed(bool bPressed);

private:
	
	/**
	 * Fire Functionality
	 */
	void FireWeapon();
	void StartFireTimer();
	void FireTimerFinished();
	void TraceUnderCrosshairs(FHitResult &TraceHitResult);

	bool bCanFire = true;
	bool bFireButtonPressed = false;
	FVector HitTarget = FVector::ZeroVector;
	FTimerHandle FireTimer;


	/**
	 * Utility Function
	 */
	void AttachWeaponToBag(AWeapon* Weapon);
	void AttachWeaponToHand(AWeapon* Weapon);
	AWeapon* SpawnWeapon(TSubclassOf<AWeapon> WeaponClass);
	void SetHUD();
	void SetHUDCrosshairs(float DeltaTime);

	/**
	 * HUD and Crosshair
	 */
	FHUDPackage HUDPackage;
	float CrosshairSpreadCoeff;
	float CrosshairSpreadAirCoeff;
	float CrosshairSpreadAimCoeff;
	float CrosshairShootCoeff;
	

	/**
	 * Walk and Run
	 */
	float BaseWalkSpeed = 400.0f;
	float SprintSpeed = 800.0f;

};


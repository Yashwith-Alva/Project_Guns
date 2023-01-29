/*****************************************************************//**
 * \file   GunsController.h
 * \brief  Player Controller used for control over input and the 
 *		   possesed character.
 * 
 * \author Yashwith D Alva
 * \date   January 2023
 *********************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GunsController.generated.h"

/**
 * 
 */

class UTexture2D;

UCLASS()
class PROJECT_GUNS_API AGunsController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void OnPossess(APawn* InPawn) override;
	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDAmmo(int32 Ammo);
	void SetHUDCarriedAmmo(int32 CarriedAmmo);
	void SetHUDPrimaryWeaponTexture(UTexture2D* WeaponTexture);
	void SetHUDSecondaryWeaponTexture(UTexture2D* WeaponTexture);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
		class AGunsHUD* GunsHUD;
};

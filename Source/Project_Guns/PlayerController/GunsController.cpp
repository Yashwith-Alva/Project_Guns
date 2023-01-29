/*****************************************************************//**
 * \file   GunsController.cpp
 * \brief  Player Controller of GUNS, setting up HUD and state of
 *		   the game that can't be handled by the Character.
 * 
 * \author Yashwith D Alva
 * \date   January 2023
 *********************************************************************/

#include "GunsController.h"
#include "Project_Guns/HUD/GunsHUD.h"
#include "Project_Guns/HUD/CharacterOverlay.h"
#include "Project_Guns/Character/BaseCharacter.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Styling/SlateColor.h"

void AGunsController::BeginPlay()
{
	GunsHUD = Cast<AGunsHUD>(GetHUD());
}

void AGunsController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ABaseCharacter* PlayerCharacter = Cast<ABaseCharacter>(InPawn);
	if (PlayerCharacter)
	{
		SetHUDHealth(50.f, 100.f);
	}

}

void AGunsController::SetHUDHealth(float Health, float MaxHealth)
{
	GunsHUD = GunsHUD == nullptr ? Cast<AGunsHUD>(GetHUD()) : GunsHUD;
	bool bHUDValid = GunsHUD &&
		GunsHUD->CharacterOverlay &&
		GunsHUD->CharacterOverlay->HealthBar &&
		GunsHUD->CharacterOverlay->HealthText;

	if (bHUDValid)
	{
		const float HealthPercent = Health / MaxHealth;
		GunsHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);
		FString HealthText = FString::Printf(TEXT("%d / %d"), Health, MaxHealth);
		GunsHUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
	}
}

void AGunsController::SetHUDAmmo(int32 Ammo) 
{
	GunsHUD = GunsHUD == nullptr ? Cast<AGunsHUD>(GetHUD()) : GunsHUD;
	bool bHUDValid = GunsHUD &&
		GunsHUD->CharacterOverlay &&
		GunsHUD->CharacterOverlay->WeaponMagAmount;
	if (bHUDValid)
	{
		FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
		GunsHUD->CharacterOverlay->WeaponMagAmount->SetText(FText::FromString(AmmoText));
	}
}

void AGunsController::SetHUDCarriedAmmo(int32 CarriedAmmo)
{
	GunsHUD = GunsHUD == nullptr ? Cast<AGunsHUD>(GetHUD()) : GunsHUD;
	bool bHUDValid = GunsHUD &&
		GunsHUD->CharacterOverlay &&
		GunsHUD->CharacterOverlay->CarriedAmmoAmount;
	if (bHUDValid)
	{
		FString AmmoText = FString::Printf(TEXT("%d"), CarriedAmmo);
		GunsHUD->CharacterOverlay->CarriedAmmoAmount->SetText(FText::FromString(AmmoText));
	}
}

void AGunsController::SetHUDPrimaryWeaponTexture(UTexture2D* WeaponTexture)
{
	GunsHUD = GunsHUD == nullptr ? Cast<AGunsHUD>(GetHUD()) : GunsHUD;
	bool bHUDValid = GunsHUD &&
		GunsHUD->CharacterOverlay &&
		GunsHUD->CharacterOverlay->PrimaryWeaponTexture;

	if (bHUDValid)
	{
		GunsHUD->CharacterOverlay->PrimaryWeaponTexture->SetBrushFromTexture(WeaponTexture, false);
	}
}

void AGunsController::SetHUDSecondaryWeaponTexture(UTexture2D* WeaponTexture)
{
	GunsHUD = GunsHUD == nullptr ? Cast<AGunsHUD>(GetHUD()) : GunsHUD;
	bool bHUDValid = GunsHUD &&
		GunsHUD->CharacterOverlay &&
		GunsHUD->CharacterOverlay->SecondaryWeaponTexture;

	if (bHUDValid)
	{
		GunsHUD->CharacterOverlay->SecondaryWeaponTexture->SetBrushFromTexture(WeaponTexture, false);
	}
}

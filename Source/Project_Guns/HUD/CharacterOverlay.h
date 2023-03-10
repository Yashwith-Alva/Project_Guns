// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterOverlay.generated.h"

/**
 * 
 */

class UProgressBar;
class UTextBlock;
class UImage;

UCLASS()
class PROJECT_GUNS_API UCharacterOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
		UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* HealthText;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* WeaponMagAmount;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* CarriedAmmoAmount;

	UPROPERTY(meta = (BindWidget))
		UImage* PrimaryWeaponTexture;

	UPROPERTY(meta = (BindWidget))
		UImage* SecondaryWeaponTexture;
};

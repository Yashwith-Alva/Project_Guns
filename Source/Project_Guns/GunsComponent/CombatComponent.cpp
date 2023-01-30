/*****************************************************************//**
 * \file   CombatComponent.cpp
 * \brief  This Combat Component is an abstract to the Character Class.
 *		   Adds functionality such as Equipping, Firing Weapon, Droping..etc
 * 
 * \author Yashwith D Alva
 * \date   January 2023
 *********************************************************************/

#include "CombatComponent.h"
#include "Project_Guns/Weapon/Weapon.h"
#include "Project_Guns/Weapon/ProjectileWeapon.h"
#include "Project_Guns/Character/BaseCharacter.h"
#include "Project_Guns/PlayerController/GunsController.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (PlayerCharacter && PrimaryWeapon)
	{
		AWeapon* SpawnedWeapon = SpawnWeapon(PrimaryWeapon);
		if (SpawnedWeapon)
		{
			AttachWeaponToHand(SpawnedWeapon);
			Inventory.Primary = SpawnedWeapon;
			SpawnedWeapon->SetOwner(PlayerCharacter);
			CurrInventoryItem = EInventoryItem::EII_PrimaryWeapon;
			EquippedWeapon = SpawnedWeapon;
		}
	}

	if (SecondaryWeapon)
	{
		AWeapon* SpawnedWeapon = SpawnWeapon(SecondaryWeapon);
		if (SpawnedWeapon)
		{
			AttachWeaponToBag(SpawnedWeapon);
			Inventory.Secondary = SpawnedWeapon;
			SpawnedWeapon->SetOwner(PlayerCharacter);
		}
	}

	SetHUD();
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PlayerCharacter)
	{
		FHitResult HitResult;
		TraceUnderCrosshairs(HitResult);

		if (HitResult.bBlockingHit)
		{
			HitTarget = HitResult.ImpactPoint;
			//DrawDebugLine(GetWorld(), HitResult.TraceStart, HitTarget, FColor::Magenta, false, 6.f);
		}
		else
		{
			HitTarget = HitResult.TraceEnd;
			//DrawDebugLine(GetWorld(), HitResult.TraceStart, HitTarget, FColor::Yellow, false, 6.f);
		}
		

		SetHUDCrosshairs(DeltaTime);
	}
}

void UCombatComponent::SetHUDCrosshairs(float DeltaTime)
{
	if (!PlayerCharacter || !PlayerCharacter->Controller)
		return;

	AGunsController* PlayerController = Cast<AGunsController>(PlayerCharacter->Controller);
	if (PlayerController)
	{
		AGunsHUD* HUD = Cast<AGunsHUD>(PlayerController->GetHUD());
		if (HUD)
		{
			if (EquippedWeapon)
			{
				HUDPackage.CrosshairsCenter = EquippedWeapon->CrosshairsCenter;
				HUDPackage.CrosshairsLeft = EquippedWeapon->CrosshairsLeft;
				HUDPackage.CrosshairsRight = EquippedWeapon->CrosshairsRight;
				HUDPackage.CrosshairsTop = EquippedWeapon->CrosshairsTop;
				HUDPackage.CrosshairsBottom = EquippedWeapon->CrosshairsBottom;
			}
			else
			{
				HUDPackage.CrosshairsCenter = nullptr;
				HUDPackage.CrosshairsLeft = nullptr;
				HUDPackage.CrosshairsRight = nullptr;
				HUDPackage.CrosshairsTop = nullptr;
				HUDPackage.CrosshairsBottom = nullptr;
			}

			// Calculate Crosshair Speed
			FVector2D WalkSpeedRange(0.f, BaseWalkSpeed);
			FVector2D VelocityMultiplierRange(0.f, 1.f);
			FVector Velocity = PlayerCharacter->GetVelocity();
			Velocity.Z = 0;
			CrosshairSpreadCoeff = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, Velocity.Size());

			if (PlayerCharacter->GetCharacterMovement()->IsFalling())
			{
				CrosshairSpreadAirCoeff = FMath::FInterpTo(CrosshairSpreadAirCoeff, 2.25, DeltaTime, 2.25);
			}
			else
			{
				CrosshairSpreadAirCoeff = FMath::FInterpTo(CrosshairSpreadAirCoeff, 0.f, DeltaTime, 30.0f);	
			}

			if (PlayerCharacter->GetIsAiming())
			{
				CrosshairSpreadAimCoeff = FMath::FInterpTo(CrosshairSpreadAimCoeff, 0.58, DeltaTime, 30.f);
			}
			else
			{
				CrosshairSpreadAimCoeff = FMath::FInterpTo(CrosshairSpreadAimCoeff, 0, DeltaTime, 40.f);
			}

			CrosshairShootCoeff = FMath::FInterpTo(CrosshairShootCoeff, 0.f, DeltaTime, 40.f);
			HUDPackage.CrosshairSpread = 0.5f + CrosshairSpreadCoeff + CrosshairSpreadAirCoeff - CrosshairSpreadAimCoeff + CrosshairShootCoeff;
			HUDPackage.CrosshairColor = FLinearColor::White;
			HUD->SetHUDPackage(HUDPackage);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, FString::Printf(TEXT("NO Player CONTROLLER!!")));
	}
}

void UCombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
	FVector2D CrosshairsLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairsWorldPosition;
	FVector CrosshairsWorldDirection;

	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairsLocation,
		CrosshairsWorldPosition,
		CrosshairsWorldDirection
		);

	if (bScreenToWorld)
	{
		FVector Start = CrosshairsWorldPosition;
		if (PlayerCharacter)
		{
			float DistanceToCharacter = (PlayerCharacter->GetActorLocation() - Start).Size();
			Start += CrosshairsWorldDirection;
		}

		FVector End = Start + CrosshairsWorldDirection * TRACE_LENGTH;
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(GetOwner());

		GetWorld()->LineTraceSingleByChannel(
			TraceHitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility,
			CollisionQueryParams);

		if (TraceHitResult.GetActor())
		{
			HUDPackage.CrosshairColor = FLinearColor::Red;
		}
		else
		{
			HUDPackage.CrosshairColor = FLinearColor::White;
		}
	}
}

#pragma region Utility Functions
void UCombatComponent::AttachWeaponToHand(AWeapon* Weapon)
{
	const USkeletalMeshSocket* HandSocket = PlayerCharacter->GetMesh()->GetSocketByName("WeaponRightHand");
	if (HandSocket)
	{
		HandSocket->AttachActor(Weapon, PlayerCharacter->GetMesh());
	}
}

void UCombatComponent::AttachWeaponToBag(AWeapon* Weapon)
{
	const USkeletalMeshSocket* BagSocket = PlayerCharacter->GetMesh()->GetSocketByName("WeaponBagSocket");
	if (BagSocket)
	{
		BagSocket->AttachActor(Weapon, PlayerCharacter->GetMesh());
	}
}

void UCombatComponent::SetHUD()
{
	AGunsController* PlayerController = Cast<AGunsController>(PlayerCharacter->GetController());
	if (PlayerController && EquippedWeapon)
	{
		PlayerController->SetHUDAmmo(EquippedWeapon->GetMag());
		PlayerController->SetHUDCarriedAmmo(EquippedWeapon->GetAmmo());
		if (Inventory.Secondary && Inventory.Primary)
		{
			if (CurrInventoryItem == EInventoryItem::EII_PrimaryWeapon)
			{
				PlayerController->SetHUDPrimaryWeaponTexture(Inventory.Secondary->GetWeaponIcon());
				PlayerController->SetHUDSecondaryWeaponTexture(Inventory.Primary->GetWeaponIcon());
			}
			else
			{
				PlayerController->SetHUDPrimaryWeaponTexture(Inventory.Primary->GetWeaponIcon());
				PlayerController->SetHUDSecondaryWeaponTexture(Inventory.Secondary->GetWeaponIcon());
			}
		}
		else if (!Inventory.Secondary && Inventory.Primary)
		{
			PlayerController->SetHUDSecondaryWeaponTexture(Inventory.Primary->GetWeaponIcon());
		}
	}
}

AWeapon* UCombatComponent::SpawnWeapon(TSubclassOf<AWeapon> WeaponClass)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = PlayerCharacter->GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride;
	AWeapon* SpawnWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, FTransform::Identity, SpawnParams);

	return SpawnWeapon;
}

#pragma endregion

#pragma region User Functions
void UCombatComponent::EquipWeapon()
{
	if (PlayerCharacter && PlayerCharacter->GetOverlappingWeapon())
	{
		if (!SecondaryWeapon)
		{
			Inventory.Secondary = PlayerCharacter->GetOverlappingWeapon();
			AttachWeaponToBag(Inventory.Secondary);
		}
		else
		{
			switch (CurrInventoryItem)
			{
			case EInventoryItem::EII_PrimaryWeapon:
				Inventory.Primary->DropWeapon();
				Inventory.Primary = PlayerCharacter->GetOverlappingWeapon();
				AttachWeaponToHand(Inventory.Primary);
				Inventory.Primary->SetOwner(PlayerCharacter);
				EquippedWeapon = Inventory.Primary;
				break;
			case EInventoryItem::EII_SecondaryWeapon:
				Inventory.Secondary->DropWeapon();
				Inventory.Secondary = PlayerCharacter->GetOverlappingWeapon();
				AttachWeaponToHand(Inventory.Secondary);
				Inventory.Secondary->SetOwner(PlayerCharacter);
				EquippedWeapon = Inventory.Secondary;
				break;
			case EInventoryItem::EII_Knife:
				Inventory.Primary->DropWeapon();
				Inventory.Primary = PlayerCharacter->GetOverlappingWeapon();
				AttachWeaponToHand(Inventory.Primary);
				Inventory.Primary->SetOwner(PlayerCharacter);
				break;
			}
		}
		SetHUD();
	}
}

void UCombatComponent::DropWeapon()
{
	if (CurrInventoryItem == EInventoryItem::EII_PrimaryWeapon)
	{
		Inventory.Primary->DropWeapon();
	}
	else if(CurrInventoryItem == EInventoryItem::EII_SecondaryWeapon)
	{
		Inventory.Secondary->DropWeapon();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 10.f, FColor::Red, FString::Printf(TEXT("Switch to gun to change")));
	}

	SetHUD();
}

void UCombatComponent::SwitchWeapon()
{
	if (EquippedWeapon && Inventory.Secondary)
	{
		if (CurrInventoryItem == EInventoryItem::EII_PrimaryWeapon)
		{
			AttachWeaponToBag(EquippedWeapon);
			EquippedWeapon = Inventory.Secondary;
			AttachWeaponToHand(EquippedWeapon);
			CurrInventoryItem = EInventoryItem::EII_SecondaryWeapon;
		}
		else if (CurrInventoryItem == EInventoryItem::EII_SecondaryWeapon)
		{
			AttachWeaponToBag(EquippedWeapon);
			EquippedWeapon = Inventory.Primary;
			AttachWeaponToHand(EquippedWeapon);
			CurrInventoryItem = EInventoryItem::EII_PrimaryWeapon;
		}
		else
		{
			EquippedWeapon = Inventory.Primary;
			AttachWeaponToHand(EquippedWeapon);
			CurrInventoryItem = EInventoryItem::EII_PrimaryWeapon;
		}
	}
	SetHUD();
}

void UCombatComponent::FireButtonPressed(bool bPressed)
{
	if (EquippedWeapon)
	{
		bFireButtonPressed = bPressed;
		if (bFireButtonPressed)
		{
			FireWeapon();
		}
	}
}

void UCombatComponent::Reload()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Reload();
	}

	SetHUD();
}

#pragma endregion

#pragma region Fire Functionality

void UCombatComponent::FireWeapon()
{
	if (EquippedWeapon && bCanFire && !EquippedWeapon->IsEmpty())
	{
		bCanFire = false;
		EquippedWeapon->Fire(HitTarget);
		StartFireTimer();
		SetHUD();
	}
}

void UCombatComponent::StartFireTimer()
{
	if (!EquippedWeapon && !PlayerCharacter)
		return;

	PlayerCharacter->GetWorldTimerManager().SetTimer(
		FireTimer,
		this,
		&UCombatComponent::FireTimerFinished,
		EquippedWeapon->GetFireDelay()
	);
}

void UCombatComponent::FireTimerFinished()
{
	if (!EquippedWeapon) return;

	bCanFire = true;
	if (bFireButtonPressed && EquippedWeapon->bAutoFire)
	{
		FireWeapon();
	}
}

#pragma endregion

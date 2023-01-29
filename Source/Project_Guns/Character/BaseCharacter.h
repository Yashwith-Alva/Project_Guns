/*****************************************************************//**
 * \file   BaseCharacter.h
 * \brief  It is the parent character class with all the required
 *		   Functionality for Health and Combat.
 * 
 * \author Yashwith D Alva
 * \date   January 2023
 *********************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Project_Guns/GunsTypes/MovementTypes.h"
#include "Project_Guns/GunsTypes/TurningInPlace.h"
#include "Project_Guns/Interface/InteractWithCrosshairInterface.h"
#include "BaseCharacter.generated.h"

UCLASS()
class PROJECT_GUNS_API ABaseCharacter : public ACharacter, public IInteractWithCrosshairInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;
	virtual void Jump() override;

#pragma region User Inputs
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void Sprint();
	void Use();
	void Switch();
	void Inventory();
	void Reload();
	void CrouchPressed();
	void LMBPressed();
	void LMBReleased();
	void RMBPressed();
	void RMBReleased();
#pragma endregion

#pragma region Aim Function
	float CalculateSpeed();
	void TurnInPlace(float DeltaTime);
	void AimOffset(float DeltaTime);
	void CalculateAO_Pitch();
#pragma endregion

private:
	UPROPERTY(VisibleAnywhere, Category = "CameraBoom")
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
		class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
		class UCombatComponent* Combat;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
		float SprintSpeed = 800.f;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
		float WalkSpeed = 600.f;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
		float AimSpeed = 450.f;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
		float CrouchSpeed = 300.f;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
		EMovementType MovementType = EMovementType::EMT_Stand;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		ETurningInPlace TurningInPlace = ETurningInPlace::ETIP_NotTurning;

	class AWeapon* OverlappingWeapon = nullptr;
	bool bIsSprinting = false;
	bool bUsePressed = false;
	bool bInventoryOpen = false;
	bool bLMBPressed = false;
	bool bRMBPressed = false;
	bool bRotateRootBone;
	bool bIsAiming = false;

	//..Aim offset
	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator InitAimRotation;

public:
	FORCEINLINE float GetCharacterSpeed();
	FORCEINLINE void SetCharacterSpeed(float Value);

	FORCEINLINE ETurningInPlace GetTurningInPlace() { return TurningInPlace; }
	FORCEINLINE void SetTurningInPlace(ETurningInPlace newVal) { TurningInPlace = newVal; }
	
	FORCEINLINE bool GetIsAiming() { return bIsAiming; }
	FORCEINLINE void SetIsAiming(bool Aiming) { bIsAiming = Aiming; }
	
	FORCEINLINE AWeapon* GetOverlappingWeapon() { return OverlappingWeapon; }
	FORCEINLINE void SetOverlappingWeapon(AWeapon* Weapon) { OverlappingWeapon = Weapon; }
	
	FORCEINLINE float GetAO_Yaw() { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() { return AO_Pitch; }

	FORCEINLINE UCombatComponent* GetCombat() { return Combat; }
};

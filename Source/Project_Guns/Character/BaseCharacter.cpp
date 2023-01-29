/*****************************************************************//**
 * \file   BaseCharacter.cpp
 * \brief  This is the character class, this is the main character with
 *		   functionality such as Combat, Health and Movement..etc
 * 
 * \author Yashwith D Alva
 * \date   January 2023
 *********************************************************************/

#include "BaseCharacter.h"
#include "Project_Guns/Project_Guns.h"
#include "Project_Guns/GunsComponent/CombatComponent.h"
#include "Project_Guns/Weapon/Weapon.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat"));

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetCharacterMovement()->RotationRate = FRotator(0.f, 0.f, 850.f);
	
	TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
}

void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	if (Combat)
	{
		Combat->PlayerCharacter = this;
	}
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	AimOffset(DeltaTime);
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (PlayerInputComponent)
	{
		PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
		PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);
		PlayerInputComponent->BindAxis("Turn", this, &ABaseCharacter::Turn);
		PlayerInputComponent->BindAxis("LookUp", this, &ABaseCharacter::LookUp);

		PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABaseCharacter::Jump);
		PlayerInputComponent->BindAction("Use", IE_Pressed, this, &ABaseCharacter::Use);
		PlayerInputComponent->BindAction("Switch", IE_Pressed, this, &ABaseCharacter::Switch);
		PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ABaseCharacter::CrouchPressed);
		PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ABaseCharacter::Sprint);

		PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &ABaseCharacter::LMBPressed);
		PlayerInputComponent->BindAction("LMB", IE_Released, this, &ABaseCharacter::LMBReleased);
		PlayerInputComponent->BindAction("RMB", IE_Pressed, this, &ABaseCharacter::RMBPressed);
		PlayerInputComponent->BindAction("RMB", IE_Released, this, &ABaseCharacter::RMBReleased);
		PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ABaseCharacter::Reload);
		PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &ABaseCharacter::Inventory);
		
	}
}

#pragma region Hold to Activate
void ABaseCharacter::MoveForward(float Value)
{
	if (Value != 0.f && Controller)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, Value);
	}
}

void ABaseCharacter::MoveRight(float Value)
{
	if (Value != 0.f && Controller)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		AddMovementInput(Direction, Value);
	}
}

void ABaseCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}
																	   
void ABaseCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ABaseCharacter::Jump()
{
	if (bIsCrouched)
	{
		ACharacter::UnCrouch();
	}
	else
	{
		ACharacter::Jump();
	}
}

void ABaseCharacter::Use()
{
	if (Combat && OverlappingWeapon)
	{
		Combat->EquipWeapon();
	}
}

void ABaseCharacter::Switch()
{
	if (Combat)
	{
		Combat->SwitchWeapon();
	}
}

void ABaseCharacter::Reload()
{
	if (Combat)
	{
		Combat->Reload();
	}
}

#pragma endregion

#pragma region Hold and Release Inputs
void ABaseCharacter::LMBPressed()
{
	if (Combat && Combat->EquippedWeapon)
	{
		Combat->FireButtonPressed(true);
	}
}

void ABaseCharacter::LMBReleased()
{
	if (Combat && Combat->EquippedWeapon)
	{
		Combat->FireButtonPressed(false);
	}
}

void ABaseCharacter::RMBPressed()
{
	//
}

void ABaseCharacter::RMBReleased()
{
	//
}

#pragma endregion

#pragma region Toggle Keys

void ABaseCharacter::CrouchPressed()
{
	if (!bIsCrouched)
	{
		ACharacter::Crouch();
		SetCharacterSpeed(CrouchSpeed);
	}
	else
	{
		ACharacter::UnCrouch();
		SetCharacterSpeed(WalkSpeed);
	}
}

void ABaseCharacter::Sprint()
{
	if (bIsCrouched)
		return;

	if(!bIsSprinting)
	{
		bIsSprinting = true;
		SetCharacterSpeed(SprintSpeed);
	}
	else
	{
		bIsSprinting = false;
		SetCharacterSpeed(WalkSpeed);
	}
}

void ABaseCharacter::Inventory()
{
	//
}

#pragma endregion

#pragma region Aim Offset and Animation

float ABaseCharacter::CalculateSpeed()
{
	FVector Velocity = GetVelocity();
	Velocity.Z = 0;
	return Velocity.Size();
}

void ABaseCharacter::AimOffset(float DeltaTime)
{
	float Speed = CalculateSpeed();
	bool bIsInAir = GetCharacterMovement()->IsFalling();

	if (Speed == 0.f && !bIsInAir)
	{
		bRotateRootBone = true;
		FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, InitAimRotation);
		AO_Yaw = DeltaRotation.Yaw;
		
		if (TurningInPlace == ETurningInPlace::ETIP_NotTurning)
		{
			InterpAO_Yaw = AO_Yaw;
		}
		bUseControllerRotationYaw = true;
		TurnInPlace(DeltaTime);
	}
	
	if (Speed > 0.f || bIsAiming)
	{
		bRotateRootBone = false;
		InitAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		AO_Yaw = 0.f;
		bUseControllerRotationYaw = true;
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	}
	CalculateAO_Pitch();
}

void ABaseCharacter::CalculateAO_Pitch()
{
	AO_Pitch = GetBaseAimRotation().Pitch;
	if (AO_Pitch > 90)
	{
		FVector2D InRange(270.f, 360.f);
		FVector2D OutRange(-90.f, 0.f);
		AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);
	}
}

void ABaseCharacter::TurnInPlace(float DeltaTime)
{
	if (AO_Yaw > 90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Right;
	}
	else if (AO_Yaw < -90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Left;
	}

	if (TurningInPlace != ETurningInPlace::ETIP_NotTurning)
	{
		InterpAO_Yaw = FMath::FInterpTo(InterpAO_Yaw, 0.f, DeltaTime, 4.f);
		AO_Yaw = InterpAO_Yaw;
		if (FMath::Abs(AO_Yaw) < 15)
		{
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
			InitAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		}
	}
}
#pragma endregion

#pragma region FORCEINLINES

float ABaseCharacter::GetCharacterSpeed()
{
	return GetCharacterMovement()->MaxWalkSpeed;	
}

void ABaseCharacter::SetCharacterSpeed(float Value)
{
	GetCharacterMovement()->MaxWalkSpeed = Value;
}

#pragma endregion


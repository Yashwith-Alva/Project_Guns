#pragma once

UENUM(BlueprintType)
enum class EMovementType : uint8
{
	EMT_Stand		UMETA(DisplayName = "Stand"),
	EMT_Crouch		UMETA(DisplayName = "Crouch"),
	EMT_CrouchWalk	UMETA(DisplayName = "Crouch-Walk"),
	EMT_Walk		UMETA(DisplayName = "Walk"),
	EMT_Sprint		UMETA(DisplayName = "Sprint"),
	EMT_MAX			UMETA(DisplayName = "DefaultMAX")
};

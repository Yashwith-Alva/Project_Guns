/*****************************************************************//**
 * \file   BaseEnemy.h
 * \brief  Base Enemy Class
 * 
 * \author Yashwith D Alva
 * \date   January 2023
 *********************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

class UAnimMontage;
class UBoxComponent;
class USkeletalMeshComponent;
class USoundBase;

UCLASS()
class PROJECT_GUNS_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage (float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;
	virtual void Death();

	UPROPERTY(VisibleAnywhere, Category = "Combat Box")
		UBoxComponent* CombatBox;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
		float MaxHealth = 0.f;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Combat | Animation")
		UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat | Animation")
		UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat | Sound")
		USoundBase* ZombieSound;

	UPROPERTY(EditDefaultsOnly, Category = "Combat | Sound")
		USoundBase* AttackSound;

	UPROPERTY(EditDefaultsOnly, Category = "Combat | Sound")
		USoundBase* DeathSound;

	UPROPERTY(EditDefaultsOnly, Category = "Combat | FX")
		UParticleSystem* HitFX;

	UPROPERTY(EditDefaultsOnly, Category = "Combat | FX")
		UParticleSystem* DeathFX;

	UPROPERTY(EditDefaultsOnly, Category = "Combat | Movement")
		bool bOneLegged = false;

	UPROPERTY(EditDefaultsOnly, Category = "Combat | Movement")
		bool bIsCrawling = false;

	float Health;
	bool bInCombat = false;

	void HealthDecrement(float DamageAmount);

public:	
	FORCEINLINE float GetHealth() { return Health; }
	FORCEINLINE void SetHealth(float Amount) { Health = Amount; }
};


/*****************************************************************//**
 * \file   BaseEnemy.cpp
 * \brief  Base Enemy Class
 * 
 * \author Yashwith D Alva
 * \date   January 2023
 *********************************************************************/

#include "BaseEnemy.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "AIController.h"

ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	CombatBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Combat Box"));
	CombatBox->SetupAttachment(GetRootComponent());
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	bInCombat = false;
}

void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ABaseEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	HealthDecrement(DamageAmount);
	return Damage;
}

void ABaseEnemy::HealthDecrement(float DamageAmount)
{
	if (Health - DamageAmount <= 0.f)
	{
		Health = 0.f;
		Death();
		FVector Location = GetActorLocation();
		Location.Z += 100.f;
		if (DeathFX) 
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathFX, Location);
	}
	else
	{
		Health -= DamageAmount;
		if(HitFX) 
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitFX, GetActorLocation());
	}
}

void ABaseEnemy::Death()
{
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
	Destroy();
}

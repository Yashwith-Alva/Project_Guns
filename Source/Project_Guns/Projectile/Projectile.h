/*****************************************************************//**
 * \file   Projectile.h
 * \brief  Projectile Motion for Bullets, Grenades..etc
 * 
 * \author Yashwith D Alva
 * \date   January 2023
 *********************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"


class UBoxComponent;
class UParticleSystem;
class UParticleSystemComponent;
class UProjectileMovementComponent;
class USoundCue;

UCLASS()
class PROJECT_GUNS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
		virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UPROPERTY(EditAnywhere)
		float Damage = 5.f;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Box Component")
		UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile Component")
		UProjectileMovementComponent* ProjectileComponent;

private:
	UPROPERTY(EditDefaultsOnly)
		UParticleSystem* Tracer;

	UParticleSystemComponent* TracerComponent;

	UPROPERTY(EditAnywhere)
		UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere)
		USoundCue* ImpactSound;

	void DestroyBullet();

	FTimerHandle DestroyTimer;
};

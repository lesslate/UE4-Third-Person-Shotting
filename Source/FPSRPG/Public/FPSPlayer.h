// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSPlayer.generated.h"



UCLASS()
class FPSRPG_API AFPSPlayer : public ACharacter
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	AFPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 스프링암 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* TPSpringArm;

	// 카메라 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* FollowCamera;

	// 무기 메시 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	class UStaticMeshComponent* WeaponMesh;

	// 오디오 컴포넌트
	UPROPERTY(BlueprintReadOnly, Category = "Audio")
	class UAudioComponent* AudioComponent;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool GetWeaponState();
	bool GetAimingState();

	void ReloadEnd();
	void Aggro();

	

protected:
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void Sprint();
	void StopSprinting();

	void Fire();
	void StopFire();
	void StartFire();
	
	void Reload();
	void Death();


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CheckWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool Aiming;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsSprint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsReloading;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stat)
	int32 Ammo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stat)
	int32 RemainAmmo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stat)
	int32 Magazine;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stat)
	float PlayerHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stat)
	float PlayerMAXHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stat)
	float Damage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stat)
	float ActualDamage;

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
	class USoundCue* ShotCue;

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
	class USoundCue* MetalCue;

	// 달리기 속도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Movement: Walking")
	float SprintSpeedMultiplier;
	
	UPROPERTY()
	class UGameplayStatics* GameStatic;

	UPROPERTY()
	class UParticleSystem * FireParticle;

	UPROPERTY()
	class UParticleSystem * BloodParticle;

	UPROPERTY()
	class UParticleSystem * SmokeParticle;


private:
	UPROPERTY()
	class UFPSAnimInstance* FPSAnim;

	UPROPERTY()
	class AZombie * Zombie;

	UPROPERTY()
	FTimerHandle timer;

	bool isFiring;
	
};

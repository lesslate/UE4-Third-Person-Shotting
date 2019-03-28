// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSPlayer.generated.h"



UCLASS()
class FPSRPG_API AFPSPlayer : public ACharacter
{
	GENERATED_BODY()

	// 스프링암
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* TPSpringArm;

	// 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	// Sets default values for this character's properties
	AFPSPlayer();

	// 달리기 속도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character Movement: Walking")
	float SprintSpeedMultiplier;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool GetWeaponState();
	bool GetAimingState();

protected:
	void MoveForward(float Value);
	void MoveRight(float Value);

	void Sprint();
	void StopSprinting();

	void Fire();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CheckWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int Ammo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool Aiming;

	
};

// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "FPSPlayer.h"
#include "ConstructorHelpers.h"

UFPSAnimInstance::UFPSAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsFire = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ShotMontage(TEXT("AnimMontage'/Game/Character/Swat/SwatAnim/Firing_Rifle_Montage.Firing_Rifle_Montage'"));
	if (ShotMontage.Succeeded())
	{
		FireMontage = ShotMontage.Object;
	}
}

void UFPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
	}

	auto Character = Cast<AFPSPlayer>(Pawn);
	if (Character)
	{
		IsInAir = Character->GetMovementComponent()->IsFalling();
		CurrentWalkSpeed = Character->GetMovementComponent()->GetMaxSpeed();
		WeaponState = Character->GetWeaponState();
		Aiming = Character->GetAimingState();
	}


}

void UFPSAnimInstance::PlayFire()
{
	Montage_Play(FireMontage, 1.0f);
	IsFire = true;
	UE_LOG(LogTemp, Log, TEXT("Fire"));
}

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

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Reload(TEXT("AnimMontage'/Game/Character/Swat/SwatAnim/Reloading_Montage.Reloading_Montage'"));
	if (Reload.Succeeded())
	{
		ReloadMontage = Reload.Object;
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
	//Montage_Play(FireMontage, 1.0f);
	IsFire = true;
	UE_LOG(LogTemp, Log, TEXT("Fire"));
}

void UFPSAnimInstance::PlayReload()
{
	Montage_Play(ReloadMontage, 1.0f);
}

void UFPSAnimInstance::AnimNotify_ReloadEnd()
{
	FPSPlayer = Cast<AFPSPlayer>(TryGetPawnOwner());
	UE_LOG(LogTemp, Log, TEXT("Reload"));
	FPSPlayer->ReloadEnd();
	
}

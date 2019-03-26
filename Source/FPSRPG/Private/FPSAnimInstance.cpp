// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

UFPSAnimInstance::UFPSAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
}

void UFPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
	}

	auto Character = Cast<ACharacter>(Pawn);
	if (Character)
	{
		IsInAir = Character->GetMovementComponent()->IsFalling();
		CurrentWalkSpeed = Character->GetMovementComponent()->GetMaxSpeed();
	}
}

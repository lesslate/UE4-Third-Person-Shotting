// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ZombieAnimInstance2.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
/**
 * 
 */
UCLASS()
class FPSRPG_API UZombieAnimInstance2 : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UZombieAnimInstance2();
	FOnAttackHitCheckDelegate OnAttackHitCheck2;
	void PlayAttackMontage();
	void PlayDeathMontage();

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* DeathMontage;

	UFUNCTION()
	void AnimNotify_AttackHitCheck();
	
	
};

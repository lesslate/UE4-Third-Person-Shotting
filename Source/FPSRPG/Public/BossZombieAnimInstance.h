// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BossZombieAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);

/**
 * 
 */
UCLASS()
class FPSRPG_API UBossZombieAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UBossZombieAnimInstance();
	FOnAttackHitCheckDelegate OnAttackHitCheck;
	FOnAttackHitCheckDelegate OnAttackHitCheck2;
	FOnAttackHitCheckDelegate OnAttackHitCheck3;
	void PlayAttackMontage();
	void PlayAttackMontage2();
	void PlayAttackMontage3();
	void PlayDeathMontage();

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage2;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage3;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* DeathMontage;

	UFUNCTION()
	void AnimNotify_AttackHitCheck();
	
	UFUNCTION()
	void AnimNotify_AttackHitCheck2();

	UFUNCTION()
	void AnimNotify_AttackHitCheck3();
	
};

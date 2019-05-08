// Fill out your copyright notice in the Description page of Project Settings.

#include "BossZombieAnimInstance.h"
#include "ConstructorHelpers.h"


UBossZombieAnimInstance::UBossZombieAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("AnimMontage'/Game/Character/BossZombie/Animation/mutant_punch_Montage.mutant_punch_Montage'"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DEATH_MONTAGE(TEXT("AnimMontage'/Game/Character/BossZombie/Animation/mutant_dying_Montage.mutant_dying_Montage'"));
	if (DEATH_MONTAGE.Succeeded())
	{
		DeathMontage = DEATH_MONTAGE.Object;
	}
}

void UBossZombieAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.0f);
}

void UBossZombieAnimInstance::PlayDeathMontage()
{
	Montage_Play(DeathMontage, 1.0f);
}

void UBossZombieAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}






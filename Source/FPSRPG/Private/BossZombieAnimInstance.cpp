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

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE2(TEXT("AnimMontage'/Game/Character/BossZombie/Animation/mutant_swiping_Montage.mutant_swiping_Montage'"));
	if (ATTACK_MONTAGE2.Succeeded())
	{
		AttackMontage2 = ATTACK_MONTAGE2.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE3(TEXT("AnimMontage'/Game/Character/BossZombie/Animation/mutant_roaring_Montage.mutant_roaring_Montage'"));
	if (ATTACK_MONTAGE3.Succeeded())
	{
		AttackMontage3 = ATTACK_MONTAGE3.Object;
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

void UBossZombieAnimInstance::PlayAttackMontage2()
{
	Montage_Play(AttackMontage2, 1.0f);
}

void UBossZombieAnimInstance::PlayAttackMontage3()
{
	Montage_Play(AttackMontage3, 1.0f);
}

void UBossZombieAnimInstance::PlayDeathMontage()
{
	Montage_Play(DeathMontage, 1.0f);
}

void UBossZombieAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void UBossZombieAnimInstance::AnimNotify_AttackHitCheck2()
{
	OnAttackHitCheck2.Broadcast();
}

void UBossZombieAnimInstance::AnimNotify_AttackHitCheck3()
{
	OnAttackHitCheck3.Broadcast();
}




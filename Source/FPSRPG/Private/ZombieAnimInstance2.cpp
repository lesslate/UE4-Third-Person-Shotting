// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieAnimInstance2.h"
#include "ConstructorHelpers.h"


UZombieAnimInstance2::UZombieAnimInstance2()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("AnimMontage'/Game/Character/Zombie2/Zombie_Attack_Montage.Zombie_Attack_Montage'"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DEATH_MONTAGE(TEXT("AnimMontage'/Game/Character/Zombie2/Death_From_The_Back_Montage.Death_From_The_Back_Montage'"));
	if (DEATH_MONTAGE.Succeeded())
	{
		DeathMontage = DEATH_MONTAGE.Object;
	}
}

void UZombieAnimInstance2::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.0f);
}

void UZombieAnimInstance2::PlayDeathMontage()
{
	Montage_Play(DeathMontage, 1.0f);
}

void UZombieAnimInstance2::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck2.Broadcast();
}







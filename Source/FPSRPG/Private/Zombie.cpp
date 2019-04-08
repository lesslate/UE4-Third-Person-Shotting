// Fill out your copyright notice in the Description page of Project Settings.

#include "Zombie.h"
#include "Sound/SoundCue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AudioComponent.h"
#include "ConstructorHelpers.h"
#include "ZombieAnimInstance.h"
#include "ZombieAIController.h"
#include "DrawDebugHelpers.h"

// Sets default values
AZombie::AZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AZombieAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 150.0f;

	// 스켈레탈 메시 설정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ZBMESH(TEXT("SkeletalMesh'/Game/zombie/jill.jill'"));
	if (ZBMESH.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(ZBMESH.Object);
	}
	// 애니메이션 블루프린트 속성지정
	static ConstructorHelpers::FClassFinder<UAnimInstance> ZombieAnimBP(TEXT("AnimBlueprint'/Game/Character/AnimBP_zombie.AnimBP_zombie_C'")); // _C를 붙여 클래스정보를 가져옴
	if (ZombieAnimBP.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ZombieAnimBP.Class);
	}

	IsAttacking = false;
}

// Called when the game starts or when spawned
void AZombie::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AZombie::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ZombieAnim = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr != ZombieAnim)
	{
		ZombieAnim->OnMontageEnded.AddDynamic(this, &AZombie::OnAttackMontageEnded);
	}

	ZombieAnim->OnAttackHitCheck.AddUObject(this, &AZombie::AttackCheck);
}


// Called to bind functionality to input
void AZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AZombie::AttackCheck()
{
	UE_LOG(LogTemp, Log, TEXT("zombie Check!"));
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 50.0f,
		FQuat::Identity,
		ECollisionChannel::ECC_EngineTraceChannel5,
		FCollisionShape::MakeSphere(50.0f),
		Params);


	FVector TraceVec = GetActorForwardVector() * 200;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = 50 * 0.5f + 50;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.0f;

#if ENABLE_DRAW_DEBUG
	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		50,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);

#endif
	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			
			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(20, DamageEvent, GetController(), this);
			
				
		}
	}

}

void AZombie::OnAttackMontageEnded(UAnimMontage * Montage, bool bInterrupted)
{
	if (IsAttacking)
	{
		IsAttacking = false;

		OnAttackEnd.Broadcast();
	}
}

void AZombie::Attack()
{
	if (!IsAttacking)
	{
		UE_LOG(LogTemp, Log, TEXT("zombie attack!"));
		ZombieAnim->PlayAttackMontage();
		IsAttacking = true;
	}
}

void AZombie::Run()
{
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	IsRun = true;
}

void AZombie::Walk()
{
	GetCharacterMovement()->MaxWalkSpeed = 150.0f;
	IsRun = false;
}


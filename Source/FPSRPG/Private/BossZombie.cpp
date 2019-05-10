// Fill out your copyright notice in the Description page of Project Settings.

#include "BossZombie.h"
#include "Sound/SoundCue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AudioComponent.h"
#include "ConstructorHelpers.h"
#include "BossZombieAnimInstance.h"
#include "BossZombieAIController.h"
#include "DrawDebugHelpers.h"
#include "FPSPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "FPSGameMode.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"


// Sets default values
ABossZombie::ABossZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HP = 3000000.0f;

	AIControllerClass = ABossZombieAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 150.0f;

	// 스켈레탈 메시 설정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ZBMESH(TEXT("SkeletalMesh'/Game/Character/BossZombie/parasite_l_starkie.parasite_l_starkie'"));
	if (ZBMESH.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(ZBMESH.Object);
	}

	// 애니메이션 블루프린트 속성지정
	static ConstructorHelpers::FClassFinder<UAnimInstance> ZombieAnimBP(TEXT("AnimBlueprint'/Game/Character/AnimBP_BossZombie.AnimBP_BossZombie_C'")); // _C를 붙여 클래스정보를 가져옴
	if (ZombieAnimBP.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ZombieAnimBP.Class);
	}

	// 사운드 큐 저장
	static ConstructorHelpers::FObjectFinder<USoundCue>ZOMBIE_SOUND(TEXT("SoundCue'/Game/Sound/ZombieBreatheCue.ZombieBreatheCue'"));
	if (ZOMBIE_SOUND.Succeeded())
	{
		ZombieSound = ZOMBIE_SOUND.Object;
	}
	// 오디오 컴포넌트 초기화
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ZombieAudio"));
	AudioComponent->bAutoActivate = false;
	AudioComponent->SetupAttachment(GetMesh());

	IsAttacking = false;
	IsDeath = false;
	IsHead = false;
}

// Called when the game starts or when spawned
void ABossZombie::BeginPlay()
{
	Super::BeginPlay();
	AudioComponent->SetSound(ZombieSound);
	AudioComponent->Play();
}

// Called every frame
void ABossZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABossZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABossZombie::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ZombieAnim = Cast<UBossZombieAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr != ZombieAnim)
	{
		ZombieAnim->OnMontageEnded.AddDynamic(this, &ABossZombie::OnAttackMontageEnded);
	}
	ZombieAI = Cast<ABossZombieAIController>(GetController());


	//공격 판정 체크
	ZombieAnim->OnAttackHitCheck.AddUObject(this, &ABossZombie::AttackCheck);
}

void ABossZombie::AttackCheck()
{
	//TArray<FHitResult> HitResult;
	FHitResult HitResult;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 50.0f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel5,
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
		//UE_LOG(LogTemp, Log, TEXT("Zombie Hit: %s"), *HitResult.GetActor()->GetName());
		if (HitResult.GetActor()->ActorHasTag("Player"))
		{
			FDamageEvent DamageEvent;
			// 데미지전달
			HitResult.Actor->TakeDamage(33, DamageEvent, GetController(), this);

		}
	}

}

void ABossZombie::OnAttackMontageEnded(UAnimMontage * Montage, bool bInterrupted)
{
	if (IsAttacking)
	{
		IsAttacking = false;
		OnAttackEnd.Broadcast();
		//UE_LOG(LogTemp, Log, TEXT("Montage End!"));
	}
}

void ABossZombie::Attack()
{
	if (!IsAttacking)
	{
		//UE_LOG(LogTemp, Log, TEXT("zombie attack!"));
		ZombieAnim->PlayAttackMontage();
		IsAttacking = true;
	}
}

void ABossZombie::Run()
{
	GetCharacterMovement()->MaxWalkSpeed = 700.0f;
	IsRun = true;
}

void ABossZombie::Walk()
{
	GetCharacterMovement()->MaxWalkSpeed = 250.0f;
	IsRun = false;
}

void ABossZombie::ReceivePointDamage(float Damage, const UDamageType * DamageType, FVector HitLocation, FVector HitNormal, UPrimitiveComponent * HitComponent, FName BoneName, FVector ShotFromDirection, AController * InstigatedBy, AActor * DamageCauser, const FHitResult & HitInfo)
{
	if (BoneName == TEXT("Head"))
	{
		IsHead = true;
	}
	HP -= Damage;
	if (HP <= 0)
	{
		IsDeath = true;
		Death();
	}
}

void ABossZombie::ReceiveRadialDamage(float DamageReceived, const UDamageType * DamageType, FVector Origin, const FHitResult & HitInfo, AController * InstigatedBy, AActor * DamageCauser)
{
	HP -= DamageReceived;
	if (HP <= 0)
	{
		IsDeath = true;
		Death();
	}
}

void ABossZombie::Death()
{
	ZombieAI->StopAI();
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ZombieAnim->PlayDeathMontage();
	AudioComponent->Stop();

	AFPSGameMode* gameMode = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	gameMode->Victory.Broadcast();

}


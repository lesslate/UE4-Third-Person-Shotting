// Fill out your copyright notice in the Description page of Project Settings.

#include "Zombie.h"
#include "Sound/SoundCue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AudioComponent.h"
#include "ConstructorHelpers.h"
#include "ZombieAnimInstance.h"
#include "ZombieAIController.h"
#include "DrawDebugHelpers.h"
#include "FPSPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "FPSGameMode.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AZombie::AZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HP = 50000.0f;

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

	// 사운드 큐 저장
	static ConstructorHelpers::FObjectFinder<USoundCue>ZOMBIE_SOUND(TEXT("SoundCue'/Game/zombie/189281__huminaatio__zombie-breathing_Cue.189281__huminaatio__zombie-breathing_Cue'"));
	if (ZOMBIE_SOUND.Succeeded())
	{
		ZombieSound = ZOMBIE_SOUND.Object;
	}
	// 오디오 컴포넌트 초기화
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ZombieAudio"));
	AudioComponent->bAutoActivate = false;
	AudioComponent->SetupAttachment(GetMesh());
	
	static ConstructorHelpers::FObjectFinder<UBlueprint> AmmoItem(TEXT("Blueprint'/Game/Weapon/Ammo.Ammo'"));
	if (AmmoItem.Object)
	{
		AmmoBlueprint = (UClass*)AmmoItem.Object->GeneratedClass;
	}
		
	IsAttacking = false;
	IsDeath = false;
	IsHead = false;
}

// Called when the game starts or when spawned
void AZombie::BeginPlay()
{
	Super::BeginPlay();
	AudioComponent->SetSound(ZombieSound);
	AudioComponent->Play();

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
	ZombieAI = Cast<AZombieAIController>(GetController());


	//공격 판정 체크
	ZombieAnim->OnAttackHitCheck.AddUObject(this, &AZombie::AttackCheck);
}

// Called to bind functionality to input
void AZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AZombie::AttackCheck()
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
				HitResult.Actor->TakeDamage(10, DamageEvent, GetController(), this);

			}
	}
	

}

void AZombie::OnAttackMontageEnded(UAnimMontage * Montage, bool bInterrupted)
{
	if (IsAttacking)
	{
		IsAttacking = false;
		OnAttackEnd.Broadcast();
		//UE_LOG(LogTemp, Log, TEXT("Montage End!"));
	}
}

void AZombie::Attack()
{
	if (!IsAttacking)
	{
		//UE_LOG(LogTemp, Log, TEXT("zombie attack!"));
		ZombieAnim->PlayAttackMontage();
		IsAttacking = true;
	}
}

void AZombie::Run()
{
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	IsRun = true;
}

void AZombie::Walk()
{
	GetCharacterMovement()->MaxWalkSpeed = 150.0f;
	IsRun = false;
}

void AZombie::ReceivePointDamage(float Damage, const UDamageType * DamageType, FVector HitLocation, FVector HitNormal, UPrimitiveComponent * HitComponent, FName BoneName, FVector ShotFromDirection, AController * InstigatedBy, AActor * DamageCauser, const FHitResult & HitInfo)
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

void AZombie::ReceiveRadialDamage(float DamageReceived, const UDamageType * DamageType, FVector Origin, const FHitResult & HitInfo, AController * InstigatedBy, AActor * DamageCauser)
{
	HP -= DamageReceived;
	if (HP <= 0)
	{
		IsDeath = true;
		Death();
	}
}

void AZombie::Death()
{
	ZombieAI->StopAI();
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ZombieAnim->PlayDeathMontage();
	AudioComponent->Stop();

	int32 Random=FMath::RandRange(1, 10);
	
	if (Random == 1&&AmmoBlueprint)
	{
		UWorld* world = GetWorld();
		if (world)
		{
			UE_LOG(LogTemp, Log, TEXT("SpawnAmmo"));
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			FRotator rotator;
			FVector  SpawnLocation = GetActorLocation();
			SpawnLocation.Z -= 90.0f;
			
			world->SpawnActor<AActor>(AmmoBlueprint, SpawnLocation, rotator, SpawnParams);
		}
	}
	AFPSGameMode* gameMode = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	gameMode->DecreaseMonster();
	
}


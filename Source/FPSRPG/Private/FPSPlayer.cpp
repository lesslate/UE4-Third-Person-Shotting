// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "ConstructorHelpers.h"
#include "Runtime/Core/Public/Math/Vector.h"
#include "Sound/SoundCue.h"
#include "FPSAnimInstance.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "Math/UnrealMathUtility.h"
#include "BTService_Detect.h"
#include "Zombie.h"
#include "Zombie2.h"
#include "BTService_Detect.h"
#include "ZombieAIController.h"
#include "ZombieAIController2.h"
#include "FPSGameMode.h"

// Sets default values
AFPSPlayer::AFPSPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->JumpZVelocity = 350.0f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	bUseControllerRotationYaw = false;


	// 스프링암 생성
	TPSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("TPSpringArm"));
	TPSpringArm->SetupAttachment(RootComponent);
	TPSpringArm->TargetArmLength = 300.0f;
	TPSpringArm->bUsePawnControlRotation = true;
	TPSpringArm->bInheritPitch = true;
	TPSpringArm->bInheritRoll = true;
	TPSpringArm->bInheritYaw = true;
	TPSpringArm->bDoCollisionTest = true;


	// 카메라 생성
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(TPSpringArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	//무기 소켓 생성
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponHand"));
	WeaponMesh->SetupAttachment(GetMesh(), TEXT("WeaponHand"));



	// 애니메이션 블루프린트 속성지정
	static ConstructorHelpers::FClassFinder<UAnimInstance> FPSPlayerAnim(TEXT("AnimBlueprint'/Game/Character/FPSPlayerAnimBP.FPSPlayerAnimBP_C'")); // _C를 붙여 클래스정보를 가져옴
	if (FPSPlayerAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(FPSPlayerAnim.Class);
	}

	// 달리기 속도 배수
	SprintSpeedMultiplier = 2.0f;

	CheckWeapon = false;
	Aiming = false;
	IsSprint = false;
	Ammo = 0;
	Magazine = 30;
	RemainAmmo = 0;
	PlayerHP = 100.0f;
	PlayerMAXHP = 100.0f;
	Damage = 5000.0f;
	

	// 사운드 큐 저장
	static ConstructorHelpers::FObjectFinder<USoundCue>RifleShot(TEXT("SoundCue'/Game/WeaponEffects/RifleShot_Cue.RifleShot_Cue'"));
	if (RifleShot.Succeeded())
	{
		ShotCue = RifleShot.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue>MetalClick(TEXT("SoundCue'/Game/WeaponEffects/Metal-click_Cue.Metal-click_Cue'"));
	if (MetalClick.Succeeded())
	{
		MetalCue = MetalClick.Object;
	}

	// 오디오 컴포넌트 초기화
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("PlayerAudio"));
	AudioComponent->bAutoActivate = false;
	AudioComponent->SetupAttachment(RootComponent);

	// 파티클 초기화
	static ConstructorHelpers::FObjectFinder<UParticleSystem> Fire(TEXT("ParticleSystem'/Game/WeaponEffects/AssaultRifle_MF.AssaultRifle_MF'"));
	if (Fire.Succeeded())
	{
		FireParticle = Fire.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> Blood(TEXT("ParticleSystem'/Game/WeaponEffects/P_body_bullet_impact.P_body_bullet_impact'"));
	if (Blood.Succeeded())
	{
		BloodParticle = Blood.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> Smoke(TEXT("ParticleSystem'/Game/WeaponEffects/P_AssaultRifle_IH.P_AssaultRifle_IH'"));
	if (Smoke.Succeeded())
	{
		SmokeParticle = Smoke.Object;
	}
}

// Called when the game starts or when spawned
void AFPSPlayer::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AFPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	FPSAnim = Cast<UFPSAnimInstance>(GetMesh()->GetAnimInstance());
}

// Called to bind functionality to input
void AFPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSPlayer::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFPSPlayer::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFPSPlayer::StopSprinting);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSPlayer::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AFPSPlayer::StopFire);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AFPSPlayer::Reload);
}

float AFPSPlayer::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (ActualDamage > 0.f)
	{
		PlayerHP -= ActualDamage;
		if (PlayerHP <= 0.f)
		{
			Death();
		}
	}
	return ActualDamage;
}

void AFPSPlayer::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AFPSPlayer::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AFPSPlayer::Sprint()
{
	if (!Aiming)
	{
		GetCharacterMovement()->MaxWalkSpeed *= SprintSpeedMultiplier;
		IsSprint = true;
	}
}

void AFPSPlayer::StopSprinting()
{
		if (!Aiming)
		{
			GetCharacterMovement()->MaxWalkSpeed = 300.0f;
			IsSprint = false;
		}
}

void AFPSPlayer::Fire()
{
	// 총알이 없을때
	if (Aiming == true && Ammo == 0)
	{
		AudioComponent->SetSound(MetalCue);
		AudioComponent->Play();
	}

	// 총알이 있을때
	if (Aiming == true && Ammo != 0 && isFiring == true)
	{
		Ammo--;
		GameStatic->SpawnEmitterAttached(FireParticle, WeaponMesh, FName("Muzzle")); // 총구화염
		Aggro();
		FHitResult OutHit;
		FVector Start = FollowCamera->GetComponentLocation();
		FVector ForwardVector = FollowCamera->GetForwardVector();
		FVector End = (Start + (ForwardVector*10000.f));
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, true);

		bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);

		if (isHit)
		{
			if (OutHit.GetActor())
			{
				DrawDebugSolidBox(GetWorld(), OutHit.ImpactPoint, FVector(10.f), FColor::Blue, true);
				UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *OutHit.GetActor()->GetName());
				UE_LOG(LogTemp, Log, TEXT("Hit Bone : %s"), *OutHit.BoneName.ToString());
				AActor* HitActor = OutHit.GetActor();
				

				if (OutHit.GetActor()->ActorHasTag("Monster")) 
				{
					if (OutHit.BoneName == TEXT("Head"))
					{
						float HeadDamage = Damage * 2;
						ActualDamage = FMath::RandRange((HeadDamage - (HeadDamage / 100) * 20), HeadDamage + (HeadDamage / 100) * 20);
						GameStatic->SpawnEmitterAtLocation(GetWorld(), BloodParticle, OutHit.ImpactPoint); // 몬스터면 피 파티클 스폰
						GameStatic->ApplyPointDamage(HitActor, ActualDamage, HitActor->GetActorLocation(), OutHit, nullptr, this, nullptr); // 포인트 데미지
					}
					else
					{
						ActualDamage = FMath::RandRange((Damage - (Damage / 100) * 20), Damage + (Damage / 100) * 20);
						GameStatic->SpawnEmitterAtLocation(GetWorld(), BloodParticle, OutHit.ImpactPoint); // 몬스터면 피 파티클 스폰
						GameStatic->ApplyPointDamage(HitActor, ActualDamage, HitActor->GetActorLocation(), OutHit, nullptr, this, nullptr); // 포인트 데미지
					}
				}
				else
				{
					GameStatic->SpawnEmitterAtLocation(GetWorld(), SmokeParticle, (OutHit.ImpactPoint) + (OutHit.ImpactNormal * 20)); // 몬스터가 아니면 연기 스폰
				}
			}

		}


		AudioComponent->SetSound(ShotCue);
		AudioComponent->Play();
		FPSAnim->PlayFire();
		GetWorld()->GetTimerManager().SetTimer(timer, this, &AFPSPlayer::Fire, .1f, false);
		UE_LOG(LogTemp, Log, TEXT("Ammo : %d"), Ammo);

	}
}

void AFPSPlayer::StopFire()
{
	isFiring = false;
}

void AFPSPlayer::StartFire()
{
	isFiring = true;
	Fire();
}

void AFPSPlayer::Reload()
{
	if (Aiming == false && CheckWeapon == true && IsReloading == false && Ammo != Magazine && RemainAmmo != 0)
	{
		/*UE_LOG(LogTemp, Log, TEXT("Reload"));*/
		FPSAnim->PlayReload();
		IsReloading = true;
	}
}

void AFPSPlayer::Death()
{
	FPSAnim->IsDeath = true;
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AFPSPlayer::ReloadEnd()
{
	UE_LOG(LogTemp, Log, TEXT("ReloadComplete"));
	if ((Magazine - Ammo) > RemainAmmo)
	{
		Ammo = (RemainAmmo + Ammo);
		RemainAmmo = 0;
	}
	else
	{
		RemainAmmo = RemainAmmo - (Magazine - Ammo);
		Ammo = (Magazine - Ammo) + Ammo;
	}
	//UE_LOG(LogTemp, Log, TEXT("Current Ammo %d / %d"), Ammo, RemainAmmo);
	IsReloading = false;
}

void AFPSPlayer::Aggro()
{
	float Radius = 3500.0f;
	
	
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam;
	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_Visibility,
		FCollisionShape::MakeSphere(Radius),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto OverlapResult : OverlapResults)
		{
			if (OverlapResult.GetActor()->ActorHasTag("Monster"))
			{
				if (OverlapResult.GetActor()->ActorHasTag("Zombie1"))
				{
					auto Zombie1 = Cast<AZombie>(OverlapResult.GetActor());
					if (!IsValid(Zombie1))
					{
						UE_LOG(LogTemp, Log, TEXT("Zombie1 not found"));
						return;
					}
					auto ZombieAI = Cast<AZombieAIController>(Zombie1->GetController());
					if (!IsValid(ZombieAI))
					{
						return;
					}
					if (nullptr != ZombieAI)
					{
						ZombieAI->Radius = 3500.0f;
					}
				}
				if (OverlapResult.GetActor()->ActorHasTag("Zombie2"))
				{
					auto Zombie2 = Cast<AZombie2>(OverlapResult.GetActor());
					if (!IsValid(Zombie2))
					{
						UE_LOG(LogTemp, Log, TEXT("Zombie2 not found"));
						return;
					}
					auto ZombieAI2 = Cast<AZombieAIController2>(Zombie2->GetController());
					if (!IsValid(ZombieAI2))
					{
						return;
					}
					if (nullptr != ZombieAI2)
					{
						ZombieAI2->Radius = 4500.0f;
					}
				}
			
				//AFPSGameMode* gameMode = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
				//if (gameMode != nullptr)
				//{
				//	gameMode->OnAggro.Broadcast();
				//}
			}
		
		}
	}
		DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 16, FColor::Red, false, 0.2f);
}

bool AFPSPlayer::GetWeaponState()
{
	return CheckWeapon;
}

bool AFPSPlayer::GetAimingState()
{
	return Aiming;
}

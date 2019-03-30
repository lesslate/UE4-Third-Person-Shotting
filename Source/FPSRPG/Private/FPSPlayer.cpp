// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "ConstructorHelpers.h"
#include "Runtime/Core/Public/Math/Vector.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "FPSAnimInstance.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystem.h"

// Sets default values
AFPSPlayer::AFPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->JumpZVelocity = 450.f;
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
	Ammo = 0;
	Aiming = false;

	//사운드 큐
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

	// 오디오 컴포넌트 추가
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("PlayerAudio"));
	AudioComponent->bAutoActivate = false;
	AudioComponent->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> Fire(TEXT("ParticleSystem'/Game/WeaponEffects/AssaultRifle_MF.AssaultRifle_MF'"));
	if (Fire.Succeeded())
	{
		FireParticle = Fire.Object;
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
	GetCharacterMovement()->MaxWalkSpeed *= SprintSpeedMultiplier;
}

void AFPSPlayer::StopSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed /= SprintSpeedMultiplier;
}

void AFPSPlayer::Fire()
{
	// 총알이 없을때
	if (Aiming == true)
	{
		if (Ammo == 0)
		{
			AudioComponent->SetSound(MetalCue);
			AudioComponent->Play();
		}
	}

	// 총알이 있을때
	if (Aiming == true)
	{
		if (Ammo != 0)
		{
			if (isFiring)
			{
				Ammo--;
				Particle->SpawnEmitterAttached(FireParticle, WeaponMesh, FName("Muzzle"));
				FHitResult OutHit;
				FVector Start = FollowCamera->GetComponentLocation();
				FVector ForwardVector = FollowCamera->GetForwardVector();
				FVector End = (Start + (ForwardVector*10000.f));
				FCollisionQueryParams CollisionParams;

				DrawDebugLine(GetWorld(), Start, End, FColor::Green, true);

				bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);

				if (isHit)
				{
					if (OutHit.bBlockingHit)
					{
						UE_LOG(LogTemp, Log, TEXT("Ammo : %s"), *OutHit.GetActor()->GetName());
					}
				}


				AudioComponent->SetSound(ShotCue);
				AudioComponent->Play();
				FPSAnim->PlayFire();
				GetWorld()->GetTimerManager().SetTimer(timer, this, &AFPSPlayer::Fire, .1f, false);
				UE_LOG(LogTemp, Log, TEXT("Ammo : %d"), Ammo);
			}
		}
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

bool AFPSPlayer::GetWeaponState()
{
	return CheckWeapon;
}

bool AFPSPlayer::GetAimingState()
{
	return Aiming;
}

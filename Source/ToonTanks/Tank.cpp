// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

ATank::ATank()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::ShootProjectile);
}

void ATank::Move(const float Value)
{
	const float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	
	FVector MovementValue(0.f);
	MovementValue.X = Value * Speed * DeltaTime;
	AddActorLocalOffset(MovementValue, true);
}

void ATank::Turn(const float Value)
{
	const float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	
	FRotator RotationValue = FRotator::ZeroRotator;
	RotationValue.Yaw = Value * TurnRate * DeltaTime;
	AddActorLocalRotation(RotationValue, true);
}

void ATank::BeginPlay()
{
	Super::BeginPlay();

	TankPlayerController = Cast<APlayerController>(GetController());
	
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TankPlayerController)
	{
		FHitResult HitResult;
		TankPlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);

		RotateTurret(HitResult.ImpactPoint);
		
		DrawDebugSphere(
		GetWorld(),
		HitResult.ImpactPoint,
		25.f,
		8,
		FColor::Red,
		false,
		-1.f);
		
	}
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	bAlive=false;
}

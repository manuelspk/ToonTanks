// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"

void ATower::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (InFireRange())
	{
		RotateTurret(Tank->GetActorLocation());
	}
}

void ATower::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy();
}

void ATower::BeginPlay()
{
	Super::BeginPlay();

	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

void ATower::CheckFireCondition()
{
	if (Tank == nullptr)
		return;
	
	if (InFireRange() && Tank->bAlive)
	{
		ShootProjectile();
	}
}

bool ATower::InFireRange()
{
	if(Tank)
	{
		const float DistanceFromPlayer = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
		if (DistanceFromPlayer < AttackRange)
		{
			return true;
		}
	}
	return false;
}

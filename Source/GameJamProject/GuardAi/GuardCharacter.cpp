// Fill out your copyright notice in the Description page of Project Settings.

#include "GuardCharacter.h"

#include "Components/CapsuleComponent.h"

AGuardCharacter::AGuardCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	m_SpreadFearCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	m_SpreadFearCapsule->InitCapsuleSize(42.0f, 96.0f);
	m_SpreadFearCapsule->SetCollisionProfileName(TEXT("Pawn"));
	m_SpreadFearCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_SpreadFearCapsule->SetCollisionObjectType(ECC_WorldDynamic);
	m_SpreadFearCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	m_SpreadFearCapsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	m_SpreadFearCapsule->SetupAttachment(GetMesh());
	m_SpreadFearCapsule->OnComponentBeginOverlap.AddDynamic(this, &AGuardCharacter::OnCapsuleOverlap);
}

void AGuardCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AGuardCharacter::OnCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && bCanSpreadFear)
	{
		UE_LOG(LogTemp, Warning, TEXT("Guard capsule overlapped with: %s"), *OtherActor->GetName());

		if (APawn* OverlappingPawn = Cast<APawn>(OtherActor))
		{
			if (AGuardCharacter* NPC = Cast<AGuardCharacter>(OverlappingPawn))

				if(!NPC->GetSmoothOperator() || NPC->m_FearLevel >= (m_MaxFearLevel / 2))
				{
					NPC->AddFearLevel(1 + m_HeardAmount);
				}
				else
				{
					this->AddFearLevel(-1 - m_HeardAmount);
				}
			++m_HeardAmount;
		}

		bCanSpreadFear = false;
		GetWorld()->GetTimerManager().SetTimer(FearCooldownTimerHandle, this, &AGuardCharacter::ResetFearCooldown, 3.0f, false);
	}
}

void AGuardCharacter::ResetFearCooldown()
{
	bCanSpreadFear = true;
}

void AGuardCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGuardCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UBehaviorTree* AGuardCharacter::GetBehaviorTree() const
{
	return m_pBehaviorTree;
}

APathPoints* AGuardCharacter::GetPatrolPath() const
{
	return m_pPatrolPath;
}

UAnimMontage* AGuardCharacter::GetAnimMontage() const
{
	return m_pAnimMontage;
}

int32 AGuardCharacter::GetFearLevel() const
{
	return m_FearLevel;
}

void AGuardCharacter::AddFearLevel(int amountToAdd)
{
	m_FearLevel += amountToAdd;
}

bool AGuardCharacter::IsScared() const
{
	return m_FearLevel >= m_MaxFearLevel;
}

bool AGuardCharacter::GetSmoothOperator()
{
	return m_SmoothOperator;
}

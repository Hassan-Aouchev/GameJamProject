// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactables/Interactable.h"
#include "UObject/ObjectPtr.h"
#include "Throwable.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class USceneComponent;
UCLASS()
class GAMEJAMPROJECT_API AThrowable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThrowable();

	UPROPERTY(BlueprintReadOnly, Category = "Scene")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(EditDefaultsOnly, Category = "Rendering")
	TObjectPtr<UStaticMeshComponent> Mesh;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnThrowableHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Sound", meta = (AllowPrivateAccess = "true"))
	float m_MinImpactForceThreshold{20000.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Sound", meta = (AllowPrivateAccess = "true"))
	float m_NoiseCooldown {0.5f};
	
	float m_LastNoiseTime {};
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};

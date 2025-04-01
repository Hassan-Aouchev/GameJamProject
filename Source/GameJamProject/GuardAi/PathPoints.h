// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathPoints.generated.h"

UCLASS()
class GAMEJAMPROJECT_API APathPoints : public AActor
{
	GENERATED_BODY()
	
public:	
	APathPoints();

	FVector GetPathPoint(const int index) const;
	int Num() const;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI", meta = (MakeEditWidget = "true"), meta = (AllowPrivateAccess = "true"))
	TArray<FVector> m_PathPoints;
};

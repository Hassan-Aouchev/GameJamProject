// Fill out your copyright notice in the Description page of Project Settings.

#include "PathPoints.h"

APathPoints::APathPoints()
{
	PrimaryActorTick.bCanEverTick = false;
}

FVector APathPoints::GetPathPoint(const int index) const
{
	return m_PathPoints[index];
}

int APathPoints::Num() const
{
	return m_PathPoints.Num();
}

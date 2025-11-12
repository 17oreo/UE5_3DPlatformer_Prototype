// Fill out your copyright notice in the Description page of Project Settings.


#include "MovePlatform.h"

// Sets default values for this component's properties
UMovePlatform::UMovePlatform()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMovePlatform::BeginPlay()
{
	Super::BeginPlay();

	StartRelativeLocation = GetRelativeLocation();

	MoveOffsetNorm = MoveOffset;
	MoveOffsetNorm.Normalize();
	MaxDistance = MoveOffset.Length();
	
}


// Called every frame
void UMovePlatform::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CurDistance += MoveDirection * Speed * DeltaTime;
	if (CurDistance >= MaxDistance || CurDistance <= 0.0f)
	{
		MoveDirection *= -1;
	}

	//Compute and set current location
	SetRelativeLocation(StartRelativeLocation + MoveOffsetNorm * CurDistance);

}


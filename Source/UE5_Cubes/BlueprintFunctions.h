// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlueprintFunctions.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, DefaultToInstanced)
class UE5_CUBES_API UBlueprintFunctions :public UObject
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintPure, category = "BlueprintFunctions")
	FVector2D SquareToCircle(float XValue, float YValue);
	UFUNCTION(BlueprintPure, category = "BlueprintFunctions")
	FVector2D RoundCoords(float radius, float angle, float x, float y);

};

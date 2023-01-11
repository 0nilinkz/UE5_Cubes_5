// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class UE5_CUBES_API CubeFont
{

public:
	CubeFont();
	~CubeFont();

	//Properties
	int ExpectedCubeSize = 0;
	FString Data;
	FString FileName;
};

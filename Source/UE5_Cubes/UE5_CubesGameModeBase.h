// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "MyCubeActor.h"
#include "InputController.h"
#include "MyCameraController.h"
#include "VisualEffect.h"
#include "Font_VisualEffect.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <UE5_Cubes/CubeFont.h>
#include "GameFramework/Actor.h"
#include "GameFramework/GameModeBase.h"
#include "UE5_CubesGameModeBase.generated.h"


/**
 * 
 */

UCLASS()
class UE5_CUBES_API AUE5_CubesGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	AUE5_CubesGameModeBase();

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	UPROPERTY()
		AMyCameraController* CameraThing;
	UPROPERTY()
		TArray<AMyCubeActor*> Cubes;
	UPROPERTY()
		FTimerHandle MemberTimerHandle;
	UPROPERTY()
		float ZoomRadius = 3000.0f;
	UPROPERTY()
		AMyCameraTransition* newT;
	UPROPERTY()
		int CubeQuantity;
	UPROPERTY()
		UInputComponent* InputThing;
	UPROPERTY()
		UFont_VisualEffect* testObject;
	TArray<CubeFont> FontAStore; 

	//Functions
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VisualEffectVariables")
		AMyCubeActor* CubeParent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VisualEffectVariables")
		TArray<UVisualEffect*> EffectsContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VisualEffectVariables")
		TArray<UFont_VisualEffect*> FontEffectsContainer;

private:
	FString DecodeOneStep(char* filename);
	
	TArray<char> GetVectorFromPng(const TArray<unsigned char>& image, unsigned w, unsigned h);

	UFUNCTION()
		void SpawnCubeCubes();
	void RemoveCubes();
	UFUNCTION()
		void SpinCubes();
	//UFUNCTION is only required for blueprints?
	float GetRandomFloat();
	float GetRandomColorFloat();
	float GetRandomFloat(float Upper, float Lower);
	UFUNCTION()
		float GetRandomFloatLight();
	UFUNCTION()
		void DecayEffect(int lightTimer);
	UFUNCTION()
		void DecayEffectReverse(int TimerA);
	void DecayEffectReverse2(int TimerA);
	void DecayEffect2(int TimerA);
	UFUNCTION()
		FVector GetRandomVector();
	UFUNCTION()
		FVector GetZeroVector();
	UFUNCTION()
		void AttachChildCubes();
	UFUNCTION()
		void RubixShiftRotation(float timer);
	UFUNCTION()
		void LightRubix(int lightTimer);
	UFUNCTION()
		void SolidRandomToBlackFade(int lightTimer);
	void ClearTimers();
	UFUNCTION()
		void RubixRandomToBlackFade(int lightTimer);
	UFUNCTION()
		void SwapCamera();
	UFUNCTION()
		void JaneEffect(int DarkTimer, int LightTimer);
	UFUNCTION()
		void LoadInFontA();
	UFUNCTION()
		void ApplyFontA(FVector TextColor, FVector BorderColor, FVector CoreColor, FString Letter);
	UFUNCTION()
		void ApplyFontBlackoutRandom(FString Letter);
	UFUNCTION(BlueprintCallable, category = "PlayVisualEffect")
		void PlayEffects();
	TArray<FString> GetAllFilesInDirectory(const FString directory, const bool fullPath, const FString onlyFilesStartingWith, const FString onlyFilesWithExtension);
	UFUNCTION()
		FString MirrorFString(FString mirror);
	UFUNCTION()
		FString ShiftRightStep(FString mirror);
	UFUNCTION()
		FString RotateCubeStringRight(FString mirror);
	void AttachEffects();

public:
	UFUNCTION()
		void DarkRubix(int lightTimer, int DarkTimer);
	void RandomColor();
	void ResetCube();
	UFUNCTION()
		void W_Event();
	UFUNCTION()
		void A_Event();
	UFUNCTION()
		void S_Event();
	UFUNCTION()
		void D_Event();

	UFUNCTION()
		void Action1_Event();
	UFUNCTION()
		void Action2_Event();
	UFUNCTION()
		void Action3_Event();
	UFUNCTION()
		void Action4_Event();

	UFUNCTION()
		void LeftShoulder_Event();
	UFUNCTION()
		void RightShoulder_Event();
	UFUNCTION()
		void RightTrigger_Event(float AxisValue);
	UFUNCTION()
		void LeftTrigger_Event(float AxisValue);
	UFUNCTION()
		void RightThumb_Event();
	UFUNCTION()
		void LeftThumb_Event();
	UFUNCTION()
		void LeftAxisX(float AxisValue);
	UFUNCTION()
		void LeftAxisY(float AxisValue);
	UFUNCTION()
		void RightAxisX(float AxisValue);
	UFUNCTION()
		void RightAxisY(float AxisValue);
};

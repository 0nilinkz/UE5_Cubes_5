// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "MyCubeActor.h"
#include "CoreMinimal.h"
#include "CubeFont.h"
#include "MyCubeActor.h"
#include "MyCameraController.h"
#include "MyCameraTransition.h"
#include <UE5_Cubes/CubeFont.h>
#include "UObject/Object.h"
#include "GameFramework/Actor.h"
#include "Engine/Texture2D.h"
#include "UObject/NoExportTypes.h"
#include "VisualEffectListCharacter.h"
#include "Components/Image.h"
#include "VisualEffect.generated.h"


UCLASS(Blueprintable, DefaultToInstanced)
class UE5_CUBES_API UVisualEffect :public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
		void SetTimerDelegate();

	//carefull changing this one, may cause a crash
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VisualEffectVariables")
		int CubeQuantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VisualEffectVariables")
		AMyCubeActor* CubeParent;

	//Time
	UPROPERTY()
		float StartTime = 0;

	UPROPERTY()
		float EffectTimeDuration = 0;

	FTimerDelegate TimerDelegate;

	FTimerHandle* TimerHandle;

	//60000 ms per min
	const float ms = 60000;
	//60,000 (ms) ÷ BPM = duration of a quarter note/beat

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VisualEffectVariables")
		int BPM = 128;

	UFUNCTION()
		float GetTime32th();
	UFUNCTION()
		float GetTime16th();
	UFUNCTION()
		float GetTime8th();
	UFUNCTION()
		float GetTime4th();
	UFUNCTION()
		float GetTimeBeat();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VisualEffectVariables")
		FText EffectName;
}; 

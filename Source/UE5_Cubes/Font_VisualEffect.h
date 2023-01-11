// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VisualEffect.h"
#include "Font_VisualEffect.generated.h"

/**
 * Created to setup effects from UI
 */

UCLASS(Blueprintable, DefaultToInstanced)
class UE5_CUBES_API UFont_VisualEffect : public UVisualEffect
{
	GENERATED_BODY()


public:

	UFont_VisualEffect();

	UFont_VisualEffect(int cubeQuantity);

	TArray<CubeFont> FontAStore;

	UFUNCTION(BlueprintCallable, category="VisualEffectFunctions")
		UFont_VisualEffect* GetDuplicate(UFont_VisualEffect* dupe);

	UFUNCTION()
		void CreateDrawTimers(FString letter);

	UFUNCTION()
		void DrawFontEffect(FString letter);

	UFUNCTION()
		void LoadInFontA();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VisualEffectVariables")
		UImage * FontImage = NewObject<UImage>();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VisualEffectVariables")
		UImage * CoreImage = NewObject<UImage>();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VisualEffectVariables")
		UImage * BackroundImage = NewObject<UImage>();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VisualEffectVariables")
		FText FontText;

	UFUNCTION()
		void SetupTimer();

	UFUNCTION()
		void SetupTimerB(FString inputCharacter);

	//Font Specific functions
	FString DecodeOneStep(char* filename);
	TArray<FString> GetAllFilesInDirectory(const FString directory, const bool fullPath, const FString onlyFilesStartingWith, const FString onlyFilesWithExtension);
	TArray<char> GetVectorFromPng(const TArray<unsigned char>& image, unsigned w, unsigned h);
	FString MirrorFString(FString mirror);
	FString ShiftRightStep(FString mirror);
	FString RotateCubeStringRight(FString mirror);
	float GetRandomFloat();
	float GetRandomFloat(float Upper, float Lower);
};

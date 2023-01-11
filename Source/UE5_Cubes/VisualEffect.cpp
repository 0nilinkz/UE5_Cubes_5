// Fill out your copyright notice in the Description page of Project Settings.
#include "VisualEffect.h"
#include "CoreMinimal.h"
#include "excpt.h"
#include "GameFramework/Actor.h"


void UVisualEffect::SetTimerDelegate()
{
	TimerDelegate.BindUFunction(this, FName("ApplyFontBlackoutRandom"), FString("I"));
}

float UVisualEffect::GetTime8th()
{
	return ms / BPM * 8;
}

float UVisualEffect::GetTime32th()
{
	return ms / BPM * 32;
}

float UVisualEffect::GetTime16th()
{
	return ms / BPM * 16;
}

float UVisualEffect::GetTime4th()
{
	return ms / BPM * 4;
}

float UVisualEffect::GetTimeBeat()
{
	return ms/BPM;
}

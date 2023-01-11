// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyCubeActor.generated.h"

UCLASS()
class UE5_CUBES_API AMyCubeActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMyCubeActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	//Mesh Setup
	UPROPERTY()
		UMaterialInstance* MaterialAsset;

	UPROPERTY()
		UMaterialInterface* MaterialInterface;

	UPROPERTY()
		UStaticMesh* CubeMesh;

	UPROPERTY()
		UStaticMeshComponent* Mesh;

	//Rotation Variables
	UPROPERTY(EditAnywhere, Category = Movement)
		float PitchValue;

	UPROPERTY(EditAnywhere, Category = Movement)
		float YawValue;

	UPROPERTY(EditAnywhere, Category = Movement)
		float RollValue;

	UPROPERTY()
		FTimerHandle MemberTimerHandle;
	UPROPERTY()
		FRotator NewRotation;
	UPROPERTY()
		FQuat QuatRotation;
	UPROPERTY()
		FVector initialLocation;

	//Functions for setting values
	UFUNCTION()
		void SetEmission(int EmmisionStrength);

	UFUNCTION()
		void SetRandomColor();

	//0.744 is the max for each value?
	UFUNCTION()
		void SetColor(float R, float G, float B);
	void SetColor2(float R, float G, float B);
	UFUNCTION()
		void TransformPosition(FVector transform);
	UFUNCTION()
		FVector GetLocation();
	
	UFUNCTION()
		void SetRoughness(float Roughness);
	UFUNCTION()
		void SetMetalic(float Metalic);
	UFUNCTION()
		void SetSpecular(float Specular);
	UFUNCTION()
		void RunVisualScript();
	UFUNCTION()
		void RunVisualScript2();
	UFUNCTION()
		void ApplyRed();

private:
	UFUNCTION()
		float GetRandomFloat();
	UFUNCTION()
		void RandomColorChangeEvent();
	
};
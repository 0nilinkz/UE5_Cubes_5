// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCubeActor.h"

// Sets default values
AMyCubeActor::AMyCubeActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(RootComponent); 
	//MaterialAsset = (UMaterialInstance*)StaticLoadObject(UMaterialInstance::StaticClass(), NULL, TEXT("CubeMaterial'/Game/CubeContent/Materials/CubeMaterial_Inst.CubeMaterial_Inst'"));
	MaterialAsset = (UMaterialInstance*)StaticLoadObject(UMaterialInstance::StaticClass(), NULL, TEXT("CubeMaterial'/Game/CubeContent/Materials/CubeMaterial_Inst.CubeMaterial_Inst'"));
	CubeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TestObject"));

	// Load the Cube mesh
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetStaticMesh(CubeMesh);
	Mesh->SetMaterial(0, MaterialAsset);

	//Dynamic Material Creation
	MaterialInterface = Mesh->GetMaterial(0);

	//Setup Rotation
	PitchValue = 0.f;
	YawValue = 0.f;
	RollValue = 0.f;
}

// Called when the game starts or when spawned
void AMyCubeActor::BeginPlay()
{
	Super::BeginPlay();

	//RunVisualScript();

	//RunVisualScript2();
}

void AMyCubeActor::RunVisualScript2() {

	FTimerDelegate TimerDel;

	float _beat = 0.323f;

	int loopCounter = 100;

	for (int i = 0; i <= loopCounter; i++) {
		FTimerHandle* FTA1 = new FTimerHandle;
		FTimerHandle* FTA2 = new FTimerHandle;
		FTimerHandle* FTA3 = new FTimerHandle;
		FTimerHandle* FTA4 = new FTimerHandle;
		FTimerHandle* FTA5 = new FTimerHandle;
		FTimerHandle* FTA6 = new FTimerHandle;

		TimerDel.BindUFunction(this, FName("SetColor"), 0.05f, 0.05f, i * 0.01f);
		//Calling MyUsefulFunction after 5 seconds without looping
		GetWorldTimerManager().SetTimer(*FTA1, TimerDel, i * 1.323f, false);
		GetWorldTimerManager().SetTimer(*FTA2, TimerDel, i * 2.626f, false);
		GetWorldTimerManager().SetTimer(*FTA3, TimerDel, i * 3.323f, false);
		GetWorldTimerManager().SetTimer(*FTA4, TimerDel, i * 4.323f, false);
		GetWorldTimerManager().SetTimer(*FTA5, TimerDel, i * 5.626f, false);
		GetWorldTimerManager().SetTimer(*FTA6, TimerDel, i * 6.626f, false);
	}

	//for (int i = loopCounter; i >= 0; i--) {
	//	FTimerHandle* FTA1 = new FTimerHandle;
	//	FTimerHandle* FTA2 = new FTimerHandle;
	//	FTimerHandle* FTA3 = new FTimerHandle;
	//	FTimerHandle* FTA4 = new FTimerHandle;
	//	FTimerHandle* FTA5 = new FTimerHandle;

	//	TimerDel.BindUFunction(this, FName("SetColor"), 0.1f, i * 0.01f, i* 0.01f);
	//	//Calling MyUsefulFunction after 5 seconds without looping
	//	GetWorldTimerManager().SetTimer(*FTA1, TimerDel, _beat * i * 1, false);
	//	GetWorldTimerManager().SetTimer(*FTA2, TimerDel, _beat * i * 2, false);
	//	GetWorldTimerManager().SetTimer(*FTA3, TimerDel, _beat * i * 3, false);
	//	GetWorldTimerManager().SetTimer(*FTA4, TimerDel, _beat * i * 4, false);
	//	GetWorldTimerManager().SetTimer(*FTA5, TimerDel, _beat * i * 5, false);
	//}
}

void AMyCubeActor::RunVisualScript()
{

	const int cubeLoopCount = 500;
	float _16th = 0.313f * 16;
	float _8th = 0.313f * 8;
	float _4th = 0.313f * 4;

	//BPM centric 128bpm
	const float _nth = 0.313f;

	TArray<FTimerHandle> timers;
	FTimerDelegate TimerDel;


	for (int i = 0; i <= cubeLoopCount; i++) {
		FTimerHandle* FTA1 = new FTimerHandle;
		TimerDel.BindUFunction(this, FName("SetColor"), i * 0.1f, 1.07f, 2.04f);
		//Calling MyUsefulFunction after 5 seconds without looping
		GetWorldTimerManager().SetTimer(*FTA1, TimerDel, _4th, false);
		//timers.Add(*FTA1);
	}
}

void AMyCubeActor::ApplyRed() {
	SetColor(10.0f, 0.0f, 0.0f);
}


void AMyCubeActor::SetRandomColor()
{
	//Pastel colors
	//Apply a Dynamic material instance to the mesh and color it.
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(MaterialInterface, this);
	Mesh->SetMaterial(0, DynamicMaterial);
	DynamicMaterial->SetScalarParameterValue(TEXT("EmissiveStrength"), 0.01f);
	DynamicMaterial->SetVectorParameterValue(TEXT("Color"), FLinearColor(GetRandomFloat(), GetRandomFloat(), GetRandomFloat(), 1.0f));
}

void AMyCubeActor::SetColor(float R, float G, float B)
{
	//Apply a Dynamic material instance to the mesh and color it.
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(MaterialInterface, this);
	Mesh->SetMaterial(0, DynamicMaterial);
	DynamicMaterial->SetVectorParameterValue(TEXT("Color"), FLinearColor(R, G, B, 0.5f));
}

void AMyCubeActor::SetColor2(float R, float G, float B)
{
	//Apply a Dynamic material instance to the mesh and color it.
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(MaterialInterface, this);
	Mesh->SetMaterial(0, DynamicMaterial);
	DynamicMaterial->SetVectorParameterValue(TEXT("Color"), FLinearColor(R, G, B, 1.0f));
}


void AMyCubeActor::TransformPosition(FVector transform)
{
	this->SetActorLocation(transform);
	//this->SetActorRelativeTransform(FTransform(transform), false, 0, ETeleportType::TeleportPhysics);
}

FVector AMyCubeActor::GetLocation()
{
	UE_LOG(LogTemp, Warning, TEXT("Get Location X: %s"), *FString::SanitizeFloat(this->GetActorLocation().X));
	UE_LOG(LogTemp, Warning, TEXT("Get Location Y: %s"), *FString::SanitizeFloat(this->GetActorLocation().Y));
	UE_LOG(LogTemp, Warning, TEXT("Get Location Z: %s"), *FString::SanitizeFloat(this->GetActorLocation().Z));

	return this->GetActorLocation();
}
	

void AMyCubeActor::SetRoughness(float Roughness)
{
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(MaterialInterface, this);
	Mesh->SetMaterial(0, DynamicMaterial);
	DynamicMaterial->SetScalarParameterValue(TEXT("Roughness"), Roughness);
}

void AMyCubeActor::SetMetalic(float Metalic)
{
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(MaterialInterface, this);
	Mesh->SetMaterial(0, DynamicMaterial);
	DynamicMaterial->SetScalarParameterValue(TEXT("Metalic"), Metalic);
}

void AMyCubeActor::SetSpecular(float Specular)
{
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(MaterialInterface, this);
	Mesh->SetMaterial(0, DynamicMaterial);
	DynamicMaterial->SetScalarParameterValue(TEXT("Metalic"), Specular);
}

float AMyCubeActor::GetRandomFloat()
{
	const float X = 4.0f;
	return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / X));
}

void AMyCubeActor::SetEmission(int emissionStrength) {
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(MaterialInterface, this);
	Mesh->SetMaterial(0, DynamicMaterial);
	DynamicMaterial->SetScalarParameterValue(TEXT("EmissiveStrength"), emissionStrength);
}

void AMyCubeActor::RandomColorChangeEvent()
{

	SetRandomColor();
}

// Called every frame
void AMyCubeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Rotation

	NewRotation = FRotator(PitchValue, YawValue, RollValue);
	QuatRotation = FQuat(NewRotation);
	AddActorLocalRotation(QuatRotation, true, 0, ETeleportType::TeleportPhysics);

	//Transform

	//FTransform AppleTransformA = FTransform(FVector(0.5f,0.5f,0.5f), FVector(0.5f, 0.5f, 0.5f), FVector(0.5f, 0.5f, 0.5f), FVector(-0.5f, -0.5f, -0.5f));
	//AddActorLocalTransform(AppleTransformA, true, NULL, ETeleportType::TeleportPhysics);
}
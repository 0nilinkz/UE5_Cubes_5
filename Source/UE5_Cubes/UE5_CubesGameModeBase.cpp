// Copyright Martin Gleeson
// Last Modified 3/09/22


#include "UE5_CubesGameModeBase.h"

#include "Runtime/Engine/Public/EngineGlobals.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "UObject/Object.h"
#include "MyCameraTransition.h"
#include "MyCubeActor.h"
#include "InputController.h"
#include "VisualEffect.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraActor.h"
#include "MyCameraController.h"

//New FX
#include <Runtime/Engine/Classes/GameFramework/InputSettings.h>
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <UE5_Cubes/lodepng.h>
#include <Runtime/Core/Public/Misc/LocalTimestampDirectoryVisitor.h>
#include <UE5_Cubes/CubeFontEffect.h>
#include <UE5_Cubes/Font_VisualEffect.h>

AUE5_CubesGameModeBase::AUE5_CubesGameModeBase()
{
	//Enabling tick function
	PrimaryActorTick.bStartWithTickEnabled = true;

	//User input class
	DefaultPawnClass = AInputController::StaticClass();

	CameraThing = CreateDefaultSubobject<AMyCameraController>(TEXT("AMyCameraObject"));

	//Loading in the png font images
	LoadInFontA();
	
	//VisualEffect* VFX1 = nullptr;
	////NewFXClasses
	//Init object
	testObject = NewObject<UFont_VisualEffect>();
	testObject->LoadInFontA();
};


void AUE5_CubesGameModeBase::LoadInFontA()
{
	FString ContentDir = FPaths::ProjectContentDir();

	FString directoryToSearch = ContentDir + TEXT("CubeContent/Images/CubeFontA/");
	FString filesStartingWith = TEXT("");
	FString fileExtensions = TEXT("png");

	TArray<FString> filesInDirectory = GetAllFilesInDirectory(directoryToSearch, true, filesStartingWith, fileExtensions);

	UE_LOG(LogTemp, Warning, TEXT("Output: number to process %s"), *FString::FromInt(filesInDirectory.Num()));

	if (FontAStore.Num() == 0) {
		for (int i = 0; i < filesInDirectory.Num(); i++) {
			char* result = TCHAR_TO_ANSI(*filesInDirectory[i]);
			CubeFont x;
			x.FileName = FPaths::GetCleanFilename(result);
			x.Data = DecodeOneStep(result);
			x.ExpectedCubeSize = 7;
			FontAStore.Add(x);
		}
	}
}

void AUE5_CubesGameModeBase::AttachEffects()
{
	//Spawn in the Effects and attach them to the world

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = this;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//Make Array for effects
	EffectsContainer.Add(GetWorld()->SpawnActor<UVisualEffect>(UVisualEffect::StaticClass(), SpawnInfo));

}

TArray<FString> AUE5_CubesGameModeBase::GetAllFilesInDirectory(const FString directory, const bool fullPath, const FString onlyFilesStartingWith, const FString onlyFilesWithExtension)
{
	// Get all files in directory
	TArray<FString> directoriesToSkip;
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	FLocalTimestampDirectoryVisitor Visitor(PlatformFile, directoriesToSkip, directoriesToSkip, false);
	PlatformFile.IterateDirectory(*directory, Visitor);
	TArray<FString> files;

	for (TMap<FString, FDateTime>::TIterator TimestampIt(Visitor.FileTimes); TimestampIt; ++TimestampIt)
	{
		const FString filePath = TimestampIt.Key();
		const FString fileName = FPaths::GetCleanFilename(filePath);
		bool shouldAddFile = true;

		// Check if filename starts with required characters
		if (!onlyFilesStartingWith.IsEmpty())
		{
			const FString left = fileName.Left(onlyFilesStartingWith.Len());

			if (!(fileName.Left(onlyFilesStartingWith.Len()).Equals(onlyFilesStartingWith)))
				shouldAddFile = false;
		}

		// Check if file extension is required characters
		if (!onlyFilesWithExtension.IsEmpty())
		{
			if (!(FPaths::GetExtension(fileName, false).Equals(onlyFilesWithExtension, ESearchCase::IgnoreCase)))
				shouldAddFile = false;
		}

		// Add full path to results
		if (shouldAddFile)
		{
			files.Add(fullPath ? filePath : fileName);
		}
	}
	return files;
}

FString AUE5_CubesGameModeBase::MirrorFString(FString mirror) {
	//Create a mirrored effect for the cube.
	FString newArrangement = "";
	int Sroot = sqrt(mirror.Len());

	for (int r = 0; r < Sroot; r++) {
		FString temp = mirror.Mid(r * Sroot, Sroot).Reverse();
		for (int z = 0; z < temp.Len(); z++) {
			newArrangement += temp[z];
		}
	}
	return newArrangement;
}

FString AUE5_CubesGameModeBase::ShiftRightStep(FString mirror) {
	int Sroot = sqrt(mirror.Len());
	FString Start = mirror.Mid(0, Sroot);
	FString End = mirror.Mid(Sroot, mirror.Len());
	return End + Start;
}

FString AUE5_CubesGameModeBase::RotateCubeStringRight(FString mirror) {
	const int Sroot = sqrt(mirror.Len());
	const int x = mirror.Len();
	TArray<FString> container;
	FString NewString = "";
	for (int i = 0; i < Sroot; i++) {
		for (int f = 0; f < mirror.Len(); f ++) {
			if (f % Sroot == i) {
				NewString += mirror[f];
			}
		}
	}
	return NewString;
}

void AUE5_CubesGameModeBase::ApplyFontA(FVector TextColor, FVector BorderColor, FVector CoreColor, FString Letters) {
	//Effect Discription: Take a single character and display it on cube
	//Get cubes
	UE_LOG(LogTemp, Warning, TEXT("Output: Play Font!"));
	TArray<AActor*> SuperCubeContainer;
	CubeParent->GetAttachedActors(SuperCubeContainer);

	UE_LOG(LogTemp, Warning, TEXT("Output: Letter Provided! %s"), *Letters);

	FString DisplayString = Letters.ToUpper();

	FString Left, Right;

	////Find data string
	FString displayData = "";
	if (DisplayString.Len() > 0) {
		for (int c = 0; c < FontAStore.Num(); c++) {
			if (FontAStore[c].FileName.Contains(".PNG")) {
				//Removing the file extension
				FString temp = FontAStore[c].FileName.LeftChop(4);
				if (DisplayString == temp) {
					displayData = FontAStore[c].Data;
					break;
				}
			}
		}
	}

	////Data Check
	if (displayData == "") {
		return;
	}

	//just add the cubes?
	TArray<AMyCubeActor*> rightData = {};
	TArray<AMyCubeActor*> leftData = {};
	TArray<AMyCubeActor*> topData = {};
	TArray<AMyCubeActor*> bottomData = {};
	TArray<AMyCubeActor*> frontData = {};
	TArray<AMyCubeActor*> backData = {};
	TArray<AMyCubeActor*> centerData = {};

	INT16 botCounter = 0;
	INT16 topCounter = 0;

	for (int z = 0; z < SuperCubeContainer.Num(); z++) {
		botCounter++;
		topCounter++;
		bool used = false;
		if ((z+1) % CubeQuantity == 0) {
			rightData.Add(((AMyCubeActor*)SuperCubeContainer[z]));
			used = true;
		}
		if (z % CubeQuantity == 0) {
			leftData.Add(((AMyCubeActor*)SuperCubeContainer[z]));
			used = true;
		}
		if (z < CubeQuantity * CubeQuantity) {
			frontData.Add(((AMyCubeActor*)SuperCubeContainer[z]));
			used = true;
		}
		if (z >= CubeQuantity * CubeQuantity * CubeQuantity - CubeQuantity * CubeQuantity) {
			backData.Add(((AMyCubeActor*)SuperCubeContainer[z]));
			used = true;
		}
		if (botCounter > CubeQuantity * CubeQuantity - CubeQuantity) {
			bottomData.Add(((AMyCubeActor*)SuperCubeContainer[z]));
			used = true;
		}
		if (botCounter == CubeQuantity * CubeQuantity) {
			botCounter = 0;
		}
		if (topCounter < CubeQuantity + 1) {
			topData.Add(((AMyCubeActor*)SuperCubeContainer[z]));
			used = true;
		}
		if (topCounter == CubeQuantity * CubeQuantity) {
			topCounter = 0;
		}
		if (!used) {
			centerData.Add(((AMyCubeActor*)SuperCubeContainer[z]));
		}
	}

	////Shift display data, Cube orientation is not that same as whats being displayed
	displayData = RotateCubeStringRight(displayData);

	TArray<TArray<AMyCubeActor*>> containerData = { rightData, leftData, topData, bottomData, frontData, backData };
	//Faces
	for (int i = 0; i < containerData.Num(); i++) {
		for (int x = 0; x < containerData[i].Num(); x++) {
			if (containerData[i].Num() == displayData.Len()) {
				AMyCubeActor* cube = containerData[i][x];
				FString displayApplied = displayData;
				
				if (i == 2 || i == 5 ) {
					//Flip to correct orientation
					displayApplied = RotateCubeStringRight(displayApplied);
					displayApplied = MirrorFString(displayApplied);
					displayApplied = RotateCubeStringRight(displayApplied);
				}
				if (displayApplied[x] == 'C') {
					//Text
					cube->SetColor(TextColor.X, TextColor.Y, TextColor.Z);
				}
				else if (displayApplied[x] == 'i') {
					//Background
					cube->SetColor(BorderColor.X, BorderColor.Y, BorderColor.X);
				}
			}
		}
	}

	////Handle Core
	for (int i = 0; i < centerData.Num(); i++) {
		AMyCubeActor* cube = centerData[i];
		//Core
		cube->SetColor(CoreColor.X, CoreColor.Y, CoreColor.Z);
	}
}

void AUE5_CubesGameModeBase::ApplyFontBlackoutRandom(FString Letter) {
	//Effect Discription: Take a single character and display it on cube
	//Get cubes
	TArray<AActor*> SuperCubeContainer;
	CubeParent->GetAttachedActors(SuperCubeContainer);

	UE_LOG(LogTemp, Warning, TEXT("Output: Letter Provided! %s"), *Letter);

	FString DisplayString = Letter.ToUpper();

	FString Left, Right;

	//Find data string
	FString displayData = "";
	if (DisplayString.Len() > 0) {
		for (int c = 0; c < FontAStore.Num(); c++) {
			if (FontAStore[c].FileName.Contains(".PNG")) {
				//Removing the file extension
				FString temp = FontAStore[c].FileName.LeftChop(4);
				if (DisplayString == temp) {
					displayData = FontAStore[c].Data;
					break;
				}
			}
		}
	}

	//Data Check
	if (displayData == "") {
		return;
	}

	//just add the cubes?
	TArray<AMyCubeActor*> rightData = {};
	TArray<AMyCubeActor*> leftData = {};
	TArray<AMyCubeActor*> topData = {};
	TArray<AMyCubeActor*> bottomData = {};
	TArray<AMyCubeActor*> frontData = {};
	TArray<AMyCubeActor*> backData = {};
	TArray<AMyCubeActor*> centerData = {};

	INT16 botCounter = 0;
	INT16 topCounter = 0;

	for (int z = 0; z < SuperCubeContainer.Num(); z++) {
		botCounter++;
		topCounter++;
		bool used = false;
		if ((z + 1) % CubeQuantity == 0) {
			rightData.Add(((AMyCubeActor*)SuperCubeContainer[z]));
			used = true;
		}
		if (z % CubeQuantity == 0) {
			leftData.Add(((AMyCubeActor*)SuperCubeContainer[z]));
			used = true;
		}
		if (z < CubeQuantity * CubeQuantity) {
			frontData.Add(((AMyCubeActor*)SuperCubeContainer[z]));
			used = true;
		}
		if (z >= CubeQuantity * CubeQuantity * CubeQuantity - CubeQuantity * CubeQuantity) {
			backData.Add(((AMyCubeActor*)SuperCubeContainer[z]));
			used = true;
		}
		if (botCounter > CubeQuantity * CubeQuantity - CubeQuantity) {
			bottomData.Add(((AMyCubeActor*)SuperCubeContainer[z]));
			used = true;
		}
		if (botCounter == CubeQuantity * CubeQuantity) {
			botCounter = 0;
		}
		if (topCounter < CubeQuantity + 1) {
			topData.Add(((AMyCubeActor*)SuperCubeContainer[z]));
			used = true;
		}
		if (topCounter == CubeQuantity * CubeQuantity) {
			topCounter = 0;
		}
		if (!used) {
			centerData.Add(((AMyCubeActor*)SuperCubeContainer[z]));
		}
	}

	//Shift display data, Cube orientation is not that same as whats being displayed
	displayData = RotateCubeStringRight(displayData);

	TArray<TArray<AMyCubeActor*>> containerData = { rightData, leftData, topData, bottomData, frontData, backData };

	//RGB values
	float R = GetRandomFloat(0.0f, 1.0f);
	float G = GetRandomFloat(0.0f, 1.0f);
	float B = GetRandomFloat(0.0f, 1.0f);

	while(true) {
		if ((!((R > 0.8) && (G > 0.8) && (B > 0.8f))) || (R+G+B < 0.8)) { //Here
			break;
		}
		else {
			R = GetRandomFloat(0.0f, 1.0f);
			G = GetRandomFloat(0.0f, 1.0f);
			B = GetRandomFloat(0.0f, 1.0f);
		}
	}

	float black = 0.0f;

	UE_LOG(LogTemp, Warning, TEXT("Output: R value: %s"), *FString::SanitizeFloat(R));
	UE_LOG(LogTemp, Warning, TEXT("Output: G value: %s"), *FString::SanitizeFloat(G));
	UE_LOG(LogTemp, Warning, TEXT("Output: B value: %s"), *FString::SanitizeFloat(B));


	//Faces
	for (int i = 0; i < containerData.Num(); i++) {
		for (int x = 0; x < containerData[i].Num(); x++) {
			if (containerData[i].Num() == displayData.Len()) {
				AMyCubeActor* cube = containerData[i][x];
				FString displayApplied = displayData;

				if (i == 2 || i == 5) {
					//Flip to correct orientation
					displayApplied = RotateCubeStringRight(displayApplied);
					displayApplied = MirrorFString(displayApplied);
					displayApplied = RotateCubeStringRight(displayApplied);
				}
				if (displayApplied[x] == 'C') {
					//Text
					cube->SetColor(R, G, B);
				}
				else if (displayApplied[x] == 'i') {
					//Background
					cube->SetColor(black, black, black);
				}
			}
		}
	}

	//Handle Core
	for (int i = 0; i < centerData.Num(); i++) {
		AMyCubeActor* cube = centerData[i];
		//Core
		cube->SetColor(black, black, black);
	}
}

void AUE5_CubesGameModeBase::PlayEffects()
{
	FString StringExample12 = TEXT("Play Effects");
	UE_LOG(LogTemp, Warning, TEXT("Output: %s"), *StringExample12);

	//use x.DrawFontEffect
	float timeCounter = 0;
	int counter = -1;

	//Iterate visual effects
	for (int i = 0; i < FontEffectsContainer.Num(); i++) {
		
		FString StringExample14 = TEXT("iterations");
		UE_LOG(LogTemp, Warning, TEXT("Output: %s"), *StringExample14);
		counter += 1;
		UFont_VisualEffect* x = FontEffectsContainer[i];

		//Each Effect should have a FTimerDelegate, 

		for (int j = 0; j < x->FontText.ToString().Len(); j++) {
			timeCounter++;
			FTimerHandle* FTA1 = new FTimerHandle;
			x->TimerDelegate =  *new FTimerDelegate;
			x->SetupTimerB(FString(x->FontText.ToString().Mid(j, 1)));
			UE_LOG(LogTemp, Warning, TEXT("Output: Passed Charcter = %s"), *FString(x->FontText.ToString().Mid(j, 1)));
			UE_LOG(LogTemp, Warning, TEXT("Output: Passed Charcter = %s"), *FString::SanitizeFloat(timeCounter));
			
			//4*0 = 0
			GetWorldTimerManager().SetTimer(*FTA1, x->TimerDelegate, (timeCounter), false);
		}
	}
}

void AUE5_CubesGameModeBase::SwapCamera() {

	FString StringExample12 = TEXT("Swap camera Called!");
	UE_LOG(LogTemp, Warning, TEXT("Output: %s"), *StringExample12);

	float PosX = 0.0f;
	float PosY = 0.0f;
	float PosZ = 0.0f;

	FRotator rotate = FRotator(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;

	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	CameraThing = GetWorld()->SpawnActor<AMyCameraController>(AMyCameraController::StaticClass(), FVector(PosX, PosY, PosZ), rotate, SpawnInfo);

	//Spawn cameras here
	const FVector aVec(0.0f, 0.0f, 0.0f);
	const FRotator aRot(0.0f, 0.0f, 0.0f);

	newT = GetWorld()->SpawnActor<AMyCameraTransition>(AMyCameraTransition::StaticClass(), aVec, aRot, SpawnInfo);
	newT->CircleRadius = this->ZoomRadius; //Zoom

	//Add to cam array
	CameraThing->AddNewCamera(newT);
}

void AUE5_CubesGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

//This runs the game
void AUE5_CubesGameModeBase::BeginPlay()
{

	FString StringExample2 = TEXT("Play started!");
	UE_LOG(LogTemp, Warning, TEXT("Output: %s"), *StringExample2);

	//This number is cubed
	CubeQuantity = 7; //7 required for font

	//Camera Rotation
	SwapCamera();

	//Spawning
	SpawnCubeCubes();
	//Attaching to parent?
	AttachChildCubes();

	//Running some effects on the cube
	//AttachEffects();


	//SpinCubes();

	//Enable camera swap methods/Start first camera
	
	//This is the default camera rotational method.
	

	//PlayEffects();


	//Timer ticks for like very second
	//FTimerDelegate TimerDel1;
	//FTimerDelegate TimerDel2;
	//FTimerDelegate TimerDel3;
	//FTimerDelegate TimerDel4;
	//FTimerDelegate TimerDel5;
	//FTimerDelegate TimerDel6;
	//FTimerDelegate TimerDel7;
	//FTimerDelegate TimerDel8;
	//FTimerDelegate TimerDel9;
	//FTimerDelegate TimerDel10;
	//FTimerDelegate TimerDel11;
	//FTimerHandle* FTA1 = new FTimerHandle;
	//FTimerHandle* FTA2 = new FTimerHandle;
	//FTimerHandle* FTA3 = new FTimerHandle;
	//FTimerHandle* FTA4 = new FTimerHandle;
	//FTimerHandle* FTA5 = new FTimerHandle;
	//FTimerHandle* FTA6 = new FTimerHandle;
	//FTimerHandle* FTA7 = new FTimerHandle;
	//FTimerHandle* FTA8 = new FTimerHandle;
	//FTimerHandle* FTA9 = new FTimerHandle;
	//FTimerHandle* FTA10 = new FTimerHandle;
	//FTimerHandle* FTA11 = new FTimerHandle;



	//TimerDel10.BindUFunction(this, FName("JaneEffect"), 1, 1);
	//TimerDel2.BindUFunction(this, FName("DarkRubix"), 1, 2);
	//TimerDel3.BindUFunction(this, FName("DarkRubix"), 2, 4);
	//TimerDel4.BindUFunction(this, FName("DarkRubix"), 1, 1);
	//TimerDel5.BindUFunction(this, FName("LightRubix"), 1);
	//TimerDel6.BindUFunction(this, FName("RubixRandomToBlackFade"), 1);
	//TimerDel9.BindUFunction(this, FName("RubixRandomToBlackFade"), 4);

	//GetWorldTimerManager().SetTimer(*FTA1, TimerDel1, 1.0f, false, 3.0f * 1);
	//GetWorldTimerManager().SetTimer(*FTA2, TimerDel2, 1.0f, false, 3.0f * 2);
	//GetWorldTimerManager().SetTimer(*FTA3, TimerDel3, 1.0f, false, 3.0f * 3);
	//GetWorldTimerManager().SetTimer(*FTA4, TimerDel4, 1.0f, false, 3.0f * 4);
	//GetWorldTimerManager().SetTimer(*FTA5, TimerDel5, 1.0f, false, 3.0f * 5);
	//GetWorldTimerManager().SetTimer(*FTA6, TimerDel6, 1.0f, false, 3.0f * 6);

	//GetWorldTimerManager().SetTimer(*FTA2, TimerDel10, 10.0f, false, 12.0f);
	//GetWorldTimerManager().SetTimer(*FTA3, TimerDel3, 10.0f, false, 22.0f);
	//GetWorldTimerManager().SetTimer(*FTA4, TimerDel4, 10.0f, false, 32.0f);
	//GetWorldTimerManager().SetTimer(*FTA5, TimerDel5, 10.0f, false, 52.0f);
	//GetWorldTimerManager().SetTimer(*FTA6, TimerDel6, 10.0f, false, 62.0f);
	//GetWorldTimerManager().SetTimer(*FTA7, TimerDel7, 10.0f, false, 72.0f);
	//GetWorldTimerManager().SetTimer(*FTA8, TimerDel8, 2.0f, false, 2.0f);
	//GetWorldTimerManager().SetTimer(*FTA9, TimerDel9, 2.0f, false, 2.0f);
	//GetWorldTimerManager().SetTimer(*FTA10, TimerDel10, 1.0f, false, 6.0f);
	//GetWorldTimerManager().SetTimer(*FTA11, TimerDel1, 2.0f, false, 72.0f);

}

void AUE5_CubesGameModeBase::RubixShiftRotation(float Timer)
{
	FTimerDelegate TimerDel;
	FTimerDelegate TimerDel2;
	//TArray<AMyCubeActor*> SuperCubeContainer;
	const float timing = 0.313f * Timer;
	const float ReturnOffset = timing * CubeQuantity;

	UE_LOG(LogTemp, Warning, TEXT("Output: %s"), TEXT("Shift Rotation 1"));
	int counter = 0;
	
	for (AActor* cu : Cubes)
	{
		float R = GetRandomFloat(2000.0f, -2000.0f);
		float G = GetRandomFloat(2000.0f, -2000.0f);
		float B = GetRandomFloat(2000.0f, -2000.0f);

		counter++;

		FTimerHandle* FTA1 = new FTimerHandle;

		//Random color
		TimerDel.BindUFunction(cu, FName("TransformPosition"), FVector(R, G, B));
		GetWorldTimerManager().SetTimer(*FTA1, TimerDel, counter * timing, false);
	}

	counter = 0;
	for (AMyCubeActor* cu : Cubes)
	{
		counter++;
		FTimerHandle* FTA4 = new FTimerHandle;
		TimerDel2.BindUFunction(cu, FName("TransformPosition"), cu->initialLocation);
		GetWorldTimerManager().SetTimer(*FTA4, TimerDel2, (Cubes.Num() * timing) + timing * counter, false);
	}
}

void AUE5_CubesGameModeBase::JaneEffect(int DarkTimer, int LightTimer) {

	//Shift randomly on two axis world translation?

	FTimerDelegate TimerDel;
	FTimerDelegate TimerDel2;
	TArray<AActor *> SuperCubeContainer;
	const float timing = 0.05f;
	const float timing2 = 0.05f;
	const float ReturnOffset = timing * CubeQuantity;
	CubeParent->GetAttachedActors(SuperCubeContainer);
	
	UE_LOG(LogTemp, Warning, TEXT("Output: %s"), TEXT("Shift Rotation 1"));
	
	int counter = 0;

	for (AActor* cu: SuperCubeContainer)
	{
		counter++;
		//UE_LOG(LogTemp, Warning, TEXT("Output counter: %s"), *FString::FromInt(counter));

		float R = GetRandomFloat();
		float G = GetRandomFloat();
		float B = GetRandomFloat();
		
		FTimerHandle* FTA1 = new FTimerHandle;
		FTimerHandle* FTA2 = new FTimerHandle;

		//Random color
		TimerDel.BindUFunction(cu, FName("SetColor"),R, G, B);
		TimerDel2.BindUFunction(cu, FName("SetEmission"), 1);
		GetWorldTimerManager().SetTimer(*FTA1, TimerDel, counter * timing * DarkTimer, false);
		GetWorldTimerManager().SetTimer(*FTA2, TimerDel2, counter * timing2 * LightTimer + CubeQuantity, false);
	}
}

void AUE5_CubesGameModeBase::DarkRubix(int TimerA, int TimerB) {

	//Timer A and Timer B are best as values such as 2, 4, 6, 8, 16, 32, 64


	FTimerDelegate TimerDel;
	TArray<TArray<AMyCubeActor*>> SuperCubeContainer;
	const int FadeIterations = 20;
	const float timing = 0.323f;

	for (int i = 0; i < CubeQuantity * CubeQuantity; i++) {
		TArray<AMyCubeActor*> Cubes1;
		SuperCubeContainer.Add(Cubes1);
	}

	//Block %
	const int block = CubeQuantity * CubeQuantity;
	//Create Arrays of Cubes?
	for (int a = 0; a < Cubes.Num(); a++)
	{
		for (int b = 0; b <= block; b++) {
			if (a % block == b) {
				SuperCubeContainer[b].Add(Cubes[a]);
			}
		}
	}

	for (int x = 0; x < SuperCubeContainer.Num(); x++) {
		float R = GetRandomFloat();
		float G = GetRandomFloat();
		float B = GetRandomFloat();
		int rnd = rand() % 8;
		//Sic fade
		for (int z = 0; z < SuperCubeContainer[x].Num(); z++)
		{
			AMyCubeActor* cube = SuperCubeContainer[x][z];
			for (int i = 0; i <= FadeIterations; i++) {
				FTimerHandle* FTA1 = new FTimerHandle;
				switch (rnd) {
				case(0):
					TimerDel.BindUFunction(cube, FName("SetColor"), i * R + R * z, G, B);
					break;
				case(1):
					TimerDel.BindUFunction(cube, FName("SetColor"), R, i * G + G * z, B);
					break;
				case(2):
					TimerDel.BindUFunction(cube, FName("SetColor"), R, G, i * B + B * z);
					break;
				case(3):
					TimerDel.BindUFunction(cube, FName("SetColor"), i * R + R * z, i * G + G * z, B);
					break;
				case(4):
					TimerDel.BindUFunction(cube, FName("SetColor"), R, i * G + G * z, i * B + B * z);
					break;
				case(5):
					TimerDel.BindUFunction(cube, FName("SetColor"), i * R + R * z, G, i * B + B * z);
					break;
				case(6):
					TimerDel.BindUFunction(cube, FName("SetColor"), i * R + R * z, i * G + G * z, i * B + B * z);
					break;
				case(7):
					TimerDel.BindUFunction(cube, FName("SetColor"), R, G, B);
					break;
				}
				//Adjust R G or B?
				GetWorldTimerManager().SetTimer(*FTA1, TimerDel, i * timing * TimerA, false);
			}
			for (int i = FadeIterations; i >= 0; i--) {
				FTimerHandle* FTA1 = new FTimerHandle;
				switch (rnd) {
				case(0):
					TimerDel.BindUFunction(cube, FName("SetColor"), i * R - R * z, G, B);
					break;
				case(1):
					TimerDel.BindUFunction(cube, FName("SetColor"), R, i * G - G * z, B);
					break;
				case(2):
					TimerDel.BindUFunction(cube, FName("SetColor"), R, G, i * B - B * z);
					break;
				case(3):
					TimerDel.BindUFunction(cube, FName("SetColor"), i * R - R * z, i * G - G * z, B);
					break;
				case(4):
					TimerDel.BindUFunction(cube, FName("SetColor"), R, i * G - G * z, i * B - B * z);
					break;
				case(5):
					TimerDel.BindUFunction(cube, FName("SetColor"), i * R - R * z, G, i * B - B * z);
					break;
				case(6):
					TimerDel.BindUFunction(cube, FName("SetColor"), i * R - R * z, i * G - G * z, i * B - B * z);
					break;
				case(7):
					TimerDel.BindUFunction(cube, FName("SetColor"), R, G, B);
					break;
				}
				//Adjust R G or B?
				GetWorldTimerManager().SetTimer(*FTA1, TimerDel, i * TimerB, false);
			}
		}
	}
}

void AUE5_CubesGameModeBase::RandomColor() {
	//Random color for cubes as a collective
	UE_LOG(LogTemp, Warning, TEXT("Random Color"));
	TArray<TArray<AMyCubeActor*>> SuperCubeContainer;

	for (int i = 0; i < CubeQuantity * CubeQuantity; i++) {
		TArray<AMyCubeActor*> Cubes1;
		SuperCubeContainer.Add(Cubes1);
	}

	//Block %
	const int block = CubeQuantity * CubeQuantity;
	//Create Arrays of Cubes?
	for (int a = 0; a < Cubes.Num(); a++)
	{
		for (int b = 0; b <= block; b++) {
			if (a % block == b) {
				SuperCubeContainer[b].Add(Cubes[a]);
			}
		}
	}
	float R = GetRandomColorFloat();
	float G = GetRandomColorFloat();
	float B = GetRandomColorFloat();
	for (int x = 0; x < SuperCubeContainer.Num(); x++) {
		
		//Sic fade
		for (int z = 0; z < SuperCubeContainer[x].Num(); z++)
		{
			AMyCubeActor* cube = SuperCubeContainer[x][z];
			cube->SetColor(R,G,B);
		}
	}
}

void AUE5_CubesGameModeBase::ResetCube() {
	UE_LOG(LogTemp, Warning, TEXT("ResetCube"));

	//Camera
	newT->ResetCamera();

	//Reset Cubes
	RemoveCubes();
	SpawnCubeCubes();
	AttachChildCubes();

	
}

void AUE5_CubesGameModeBase::LightRubix(int TimerA) {

	//Timer A and Timer B are best as values such as 2, 4, 6, 8, 16, 32, 64
	FTimerDelegate TimerDel;
	TArray<TArray<AMyCubeActor*>> SuperCubeContainer;
	const int FadeIterations = 20;
	const float timing = 0.323f;

	for (int i = 0; i < CubeQuantity * CubeQuantity; i++) {
		TArray<AMyCubeActor*> Cubes1;
		SuperCubeContainer.Add(Cubes1);
	}

	//Block %
	const int block = CubeQuantity * CubeQuantity;
	//Create Arrays of Cubes?
	for (int a = 0; a < Cubes.Num(); a++)
	{
		for (int b = 0; b <= block; b++) {
			if (a % block == b) {
				SuperCubeContainer[b].Add(Cubes[a]);
			}
		}
	}

	for (int x = 0; x < SuperCubeContainer.Num(); x++) {
		float R = GetRandomFloat();
		float G = GetRandomFloat();
		float B = GetRandomFloat();
		int rnd = rand() % 8;
		//Sic fade
		for (int z = 0; z < SuperCubeContainer[x].Num(); z++)
		{
			AMyCubeActor* cube = SuperCubeContainer[x][z];
			for (int i = 0; i <= FadeIterations; i++) {
				FTimerHandle* FTA1 = new FTimerHandle;

				//Random Descision (R, G or B modified)
				switch (rnd) {
				case(0):
					TimerDel.BindUFunction(cube, FName("SetColor"), i * R + R * z, G, B);
					break;
				case(1):
					TimerDel.BindUFunction(cube, FName("SetColor"), R, i * G + G * z, B);
					break;
				case(2):
					TimerDel.BindUFunction(cube, FName("SetColor"), R, G, i * B + B * z);
					break;
				case(3):
					TimerDel.BindUFunction(cube, FName("SetColor"), i * R + R * z, i * G + G * z, B);
					break;
				case(4):
					TimerDel.BindUFunction(cube, FName("SetColor"), R, i * G + G * z, i * B + B * z);
					break;
				case(5):
					TimerDel.BindUFunction(cube, FName("SetColor"), i * R + R * z, G, i * B + B * z);
					break;
				case(6):
					TimerDel.BindUFunction(cube, FName("SetColor"), i * R + R * z, i * G + G * z, i * B + B * z);
					break;
				case(7):
					TimerDel.BindUFunction(cube, FName("SetColor"), R, G, B);
					break;
				}
				//Adjust R G or B?
				GetWorldTimerManager().SetTimer(*FTA1, TimerDel, i * timing * TimerA, false);
			}
		}
	}
}

void AUE5_CubesGameModeBase::SolidRandomToBlackFade(int TimerA) {

	FTimerDelegate TimerDel;
	TArray<TArray<AMyCubeActor*>> SuperCubeContainer;
	const int FadeIterations = 40;
	const float timing = 0.323f;
	const float min = 0.1f;

	for (int i = 0; i < CubeQuantity * CubeQuantity; i++) {
		TArray<AMyCubeActor*> Cubes1;
		SuperCubeContainer.Add(Cubes1);
	}

	//Block %
	const int block = CubeQuantity * CubeQuantity;
	//Create Arrays of Cubes?
	for (int a = 0; a < Cubes.Num(); a++)
	{
		for (int b = 0; b <= block; b++) {
			if (a % block == b) {
				SuperCubeContainer[b].Add(Cubes[a]);
			}
		}
	}

	float R = GetRandomFloatLight();
	float G = GetRandomFloatLight();
	float B = GetRandomFloatLight();

	for (int x = 0; x < SuperCubeContainer.Num(); x++) {

		int rnd = rand() % 8;
	//	//Sic fade
		for (int z = 0; z < SuperCubeContainer[x].Num(); z++)
		{
			AMyCubeActor* cube = SuperCubeContainer[x][z];
			for (int i = 0; i <= FadeIterations; i++) {
				FTimerHandle* FTA1 = new FTimerHandle;
				//TimerDel.BindUFunction(cube, FName("SetColor"),R, G, B);
				TimerDel.BindUFunction(cube, FName("SetColor"), R - (i * min), G - (i * min), B - (i * min));

				//Adjust R G or B?
				GetWorldTimerManager().SetTimer(*FTA1, TimerDel, i * timing * TimerA, false);
				
			}
		}
	}
}

void AUE5_CubesGameModeBase::ClearTimers() {
	
	GetWorldTimerManager().ListTimers();
}

void AUE5_CubesGameModeBase::RubixRandomToBlackFade(int TimerA) {

	//Timer A and Timer B are best as values such as 2, 4, 6, 8, 16, 32, 64
	FTimerDelegate TimerDel;
	TArray<TArray<AMyCubeActor*>> SuperCubeContainer;
	const int FadeIterations = 40;
	const float timing = 0.323f;
	const float min = 0.1f;

	for (int i = 0; i < CubeQuantity * CubeQuantity; i++) {
		TArray<AMyCubeActor*> Cubes1;
		SuperCubeContainer.Add(Cubes1);
	}

	//Block %
	const int block = CubeQuantity * CubeQuantity;
	//Create Arrays of Cubes?
	for (int a = 0; a < Cubes.Num(); a++)
	{
		for (int b = 0; b <= block; b++) {
			if (a % block == b) {
				SuperCubeContainer[b].Add(Cubes[a]);
			}
		}
	}

	for (int x = 0; x < SuperCubeContainer.Num(); x++) {
		float R = GetRandomFloatLight();
		float G = GetRandomFloatLight();
		float B = GetRandomFloatLight();
		int rnd = rand() % 8;
		//Sic fade
		for (int z = 0; z < SuperCubeContainer[x].Num(); z++)
		{
			AMyCubeActor* cube = SuperCubeContainer[x][z];
			for (int i = 0; i <= FadeIterations; i++) {
				FTimerHandle* FTA1 = new FTimerHandle;
				//TimerDel.BindUFunction(cube, FName("SetColor"),R, G, B);
				TimerDel.BindUFunction(cube, FName("SetColor"), R - (i * min), G - (i * min), B - (i * min));

				//Adjust R G or B?
				GetWorldTimerManager().SetTimer(*FTA1, TimerDel, i * timing * TimerA, false);
			}
		}
	}
}

void AUE5_CubesGameModeBase::DecayEffect(int TimerA) {

	//Timer A and Timer B are best as values such as 2, 4, 6, 8, 16, 32, 64
	FTimerDelegate TimerDel;
	TArray<TArray<AMyCubeActor*>> SuperCubeContainer;
	const int FadeIterations = 4;
	const float timing = 0.323f;

	for (int i = 0; i < CubeQuantity * CubeQuantity; i++) {
		TArray<AMyCubeActor*> Cubes1;
		SuperCubeContainer.Add(Cubes1);
	}

	//Block %
	const int block = CubeQuantity * CubeQuantity;
	//Create Arrays of Cubes?
	for (int a = 0; a < Cubes.Num(); a++)
	{
		for (int b = 0; b <= block; b++) {
			if (a % block == b) {
				SuperCubeContainer[b].Add(Cubes[a]);
			}
		}
	}

	for (int x = 0; x < SuperCubeContainer.Num(); x++) {
		float R = GetRandomFloatLight();
		float G = GetRandomFloatLight();
		float B = GetRandomFloatLight();
		//Sic fade
		for (int z = 0; z < SuperCubeContainer[x].Num(); z++)
		{
			AMyCubeActor* cube = SuperCubeContainer[x][z];
			for (int i = FadeIterations; i >= 0; i--) {
				FTimerHandle* FTA1 = new FTimerHandle;

				TimerDel.BindUFunction(cube, FName("SetColor"), R - i, G - i, B - i);
				GetWorldTimerManager().SetTimer(*FTA1, TimerDel, i * timing * TimerA, false);
			}
		}
	}
}

void AUE5_CubesGameModeBase::DecayEffectReverse(int TimerA) {

	//Timer A and Timer B are best as values such as 2, 4, 6, 8, 16, 32, 64
	FTimerDelegate TimerDel;
	TArray<TArray<AMyCubeActor*>> SuperCubeContainer;
	const int FadeIterations = 4;
	const float timing = 0.323f;

	for (int i = 0; i < CubeQuantity * CubeQuantity; i++) {
		TArray<AMyCubeActor*> Cubes1;
		SuperCubeContainer.Add(Cubes1);
	}

	//Block %
	const int block = CubeQuantity * CubeQuantity;
	//Create Arrays of Cubes?
	for (int a = 0; a < Cubes.Num(); a++)
	{
		for (int b = 0; b <= block; b++) {
			if (a % block == b) {
				SuperCubeContainer[b].Add(Cubes[a]);
			}
		}
	}

	for (int x = 0; x < SuperCubeContainer.Num(); x++) {
		float R = GetRandomFloatLight();
		float G = GetRandomFloatLight();
		float B = GetRandomFloatLight();
		//Sic fade
		for (int z = 0; z < SuperCubeContainer[x].Num(); z++)
		{
			AMyCubeActor* cube = SuperCubeContainer[x][z];
			for (int i = FadeIterations; i >= 0; i--) {
				FTimerHandle* FTA1 = new FTimerHandle;

				TimerDel.BindUFunction(cube, FName("SetColor"), R + i, G + i, B + i);
				GetWorldTimerManager().SetTimer(*FTA1, TimerDel, i * timing * TimerA, false);
			}
		}
	}
}

void AUE5_CubesGameModeBase::DecayEffectReverse2(int TimerA) {

	//Timer A and Timer B are best as values such as 2, 4, 6, 8, 16, 32, 64
	FTimerDelegate TimerDel;
	const int FadeIterations = 4;
	const float timing = 0.323f;
	for (AMyCubeActor* cube : Cubes) {
		float R = GetRandomFloatLight();
		float G = GetRandomFloatLight();
		float B = GetRandomFloatLight();
		//Sic fade
		for (int i = FadeIterations; i >= 0; i--) {
			FTimerHandle* FTA1 = new FTimerHandle;

			TimerDel.BindUFunction(cube, FName("SetColor"), R + i, G + i, B + i);
			GetWorldTimerManager().SetTimer(*FTA1, TimerDel, i * timing * TimerA, false);
		}
	}
}

void AUE5_CubesGameModeBase::DecayEffect2(int TimerA) {

	//Timer A and Timer B are best as values such as 2, 4, 6, 8, 16, 32, 64
	FTimerDelegate TimerDel;
	const int FadeIterations = 4;
	const float timing = 0.323f;
	for (AMyCubeActor* cube : Cubes) {
		float R = GetRandomFloatLight();
		float G = GetRandomFloatLight();
		float B = GetRandomFloatLight();
		//Sic fade
		for (int i = FadeIterations; i >= 0; i--) {
			FTimerHandle* FTA1 = new FTimerHandle;

			TimerDel.BindUFunction(cube, FName("SetColor"), R - i, G - i, B - i);
			GetWorldTimerManager().SetTimer(*FTA1, TimerDel, i * timing * TimerA, false);
		}
	}
}

FString AUE5_CubesGameModeBase::DecodeOneStep(char* filename)
{
	TArray<unsigned char> image; //the raw pixels
	unsigned width, height;

	//decode
	unsigned error = lodepng::decode(image, width, height, filename);

	//if there's an error, display it
	if (error) {
		UE_LOG(LogTemp, Warning, TEXT("Decoder Error: %s"), lodepng_error_text(error));
	}

	TArray<char> example = GetVectorFromPng(image, width, height);
	
	std::string output = &example[0];
	std::string built = "";
	//This works just output to the cube
	for (int i = 0; i < 49; i++)
	{
		//C content
		if (output[i] == 'C') {
			built += "C";
		}
		//i Background
		else if (output[i] == 'i') {
			built += "i";
		}
	}
	return FString(built.c_str());
}

TArray<char> AUE5_CubesGameModeBase::GetVectorFromPng(const TArray<unsigned char>& image, unsigned w, unsigned h)
{
	const unsigned SquareWidth = 7;
	const unsigned loopUpper = SquareWidth + 1;

	TArray<char> OutputCharArray;

	if (w > 0 && h > 0) {
		unsigned w2 = loopUpper;
		unsigned h2 = loopUpper;

		for (unsigned y = 1; y < loopUpper; y++) {
			for (unsigned x = 1; x < loopUpper; x++) {
				unsigned x2 = x * w / w2;
				unsigned y2 = y * h / h2;
				int r = image[y2 * w * 4 + x2 * 4 + 0];
				int g = image[y2 * w * 4 + x2 * 4 + 1];
				int b = image[y2 * w * 4 + x2 * 4 + 2];
				int a = image[y2 * w * 4 + x2 * 4 + 3];
				int lightness = ((r + g + b) / 3) * a / 255;
				int min = (r < g&& r < b) ? r : (g < b ? g : b);
				int max = (r > g && r > b) ? r : (g > b ? g : b);
				int saturation = max - min;
				int letter = 'i'; //i for grey, or r,y,g,c,b,m for colors
				if (saturation > 32) {
					int j = lightness >= (min + max) / 2;
					if (j) letter = (min == r ? 'c' : (min == g ? 'm' : 'y'));
					else letter = (max == r ? 'r' : (max == g ? 'g' : 'b'));
				}
				int symbol = ' ';
				if (lightness > 224) symbol = '@';
				else if (lightness > 128) symbol = letter - 32;
				else if (lightness > 32) symbol = letter;
				else if (lightness > 16) symbol = '.';
				OutputCharArray.Add((TCHAR)symbol);
			}
		}
	}
	return OutputCharArray;
}

void AUE5_CubesGameModeBase::SpawnCubeCubes() {
	float BaseCubeX = 0.0f;
	float BaseCubeY = 0.0f;
	float BaseCubeZ = 0.0f;
	//150f
	//Cube Seperation
	const float floatIncrement = 150.0f; 
	FRotator rotate = FRotator(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = this;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (int x = 0; x < CubeQuantity; x++) {
		for (int y = 0; y < CubeQuantity; y++) {
			for (int z = 0; z < CubeQuantity; z++) {
				AMyCubeActor* newCube = GetWorld()->SpawnActor<AMyCubeActor>(AMyCubeActor::StaticClass(), FVector(BaseCubeX, BaseCubeY, BaseCubeZ), rotate, SpawnInfo);
				newCube->initialLocation = newCube->GetLocation();
				Cubes.Add(newCube);
				BaseCubeZ += floatIncrement;
			}
			BaseCubeZ = 0.0f;
			BaseCubeY += floatIncrement;
		}
		BaseCubeY = 0.0f;
		BaseCubeX += floatIncrement;
	}
}

void AUE5_CubesGameModeBase::RemoveCubes() {

	for (AMyCubeActor* cube : Cubes)
	{
		cube->Destroy();
	}
	Cubes.Empty();
	CubeParent->Destroy();
}

void AUE5_CubesGameModeBase::SpinCubes()
{
	FString StringExample2 = TEXT("Spin Cubes!");
	UE_LOG(LogTemp, Warning, TEXT("Output: %s"), *StringExample2);

	//Move all cubes by 90 degrees

	for (AMyCubeActor* cube : Cubes)
	{
		cube->RollValue = GetRandomFloat();
		cube->PitchValue = GetRandomFloat();
		cube->YawValue = GetRandomFloat();
	}
	//Spinning all cubes individuallly....

}

float AUE5_CubesGameModeBase::GetRandomFloat()
{
	const float X = 0.008f;
	return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / X));
}

float AUE5_CubesGameModeBase::GetRandomColorFloat()
{
	const float X = 4.0f;
	return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / X));
}

float AUE5_CubesGameModeBase::GetRandomFloat(float Upper, float Lower)
{
	return Lower + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (Upper - Lower)));
}

float AUE5_CubesGameModeBase::GetRandomFloatLight()
{
	const float Upper = 4.0f;
	const float Lower = 1.0f;
	return Lower + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (Upper - Lower)));
}

FVector AUE5_CubesGameModeBase::GetRandomVector() {
	return FVector(GetRandomFloat(), GetRandomFloat(), GetRandomFloat());
}

FVector AUE5_CubesGameModeBase::GetZeroVector() {
	return FVector(0.0f, 0.0f, 0.0f);
}

void AUE5_CubesGameModeBase::AttachChildCubes()
{
	const float floatIncrement = 150.0f;
	float BaseCubeX = CubeQuantity / 2 * floatIncrement;
	float BaseCubeY = CubeQuantity / 2 * floatIncrement;
	float BaseCubeZ = CubeQuantity / 2 * floatIncrement;
	FRotator rotate = FRotator(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = this;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	CubeParent = GetWorld()->SpawnActor<AMyCubeActor>(AMyCubeActor::StaticClass(), FVector(BaseCubeX, BaseCubeY, BaseCubeZ), rotate, SpawnInfo);

	FVector origin;
	FVector boxExtent;
	CubeParent->GetActorBounds(false, origin, boxExtent);
	UE_LOG(LogTemp, Warning, TEXT("Basic cube dimentions: %s"), *FString::SanitizeFloat(boxExtent.X));

	for (AMyCubeActor* cube : Cubes)
	{
		cube->AttachToActor(CubeParent, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
	}

	//Overall Cube rotation
	/*CubeParent->YawValue = 0.3f;
	CubeParent->RollValue = 0.1f;
	CubeParent->PitchValue = 0.2f;*/

	//Hide parent cube
	CubeParent->SetActorHiddenInGame(true);
}


//This is an example of running effect with a button press.
void AUE5_CubesGameModeBase::W_Event()
{
	UE_LOG(LogTemp, Warning, TEXT("W_Event"));
	ApplyFontBlackoutRandom("upArrow");
}

void AUE5_CubesGameModeBase::A_Event()
{
	UE_LOG(LogTemp, Warning, TEXT("A_Event"));
	ApplyFontBlackoutRandom("leftArrow");
}

void AUE5_CubesGameModeBase::S_Event()
{
	//ApplyFontA(FVector(168, 50, 50),FVector(58, 50, 168),FVector(58, 50, 168), "A");
	UE_LOG(LogTemp, Warning, TEXT("S_Event"));
	ApplyFontBlackoutRandom("HEART");
}

void AUE5_CubesGameModeBase::D_Event()
{
	UE_LOG(LogTemp, Warning, TEXT("D_Event"));
	ApplyFontBlackoutRandom("rightArrow");
}

void AUE5_CubesGameModeBase::Action1_Event() {
	UE_LOG(LogTemp, Warning, TEXT("Action1_Event"));
	//JaneEffect(1, 4);
	ApplyFontA(FVector(111, 0, 0), FVector(1, 1, 1), FVector(1, 1, 1), "C");
}

void AUE5_CubesGameModeBase::Action2_Event() {
	UE_LOG(LogTemp, Warning, TEXT("Action2_Event"));
	SolidRandomToBlackFade(2);
}

void AUE5_CubesGameModeBase::Action3_Event() {
	UE_LOG(LogTemp, Warning, TEXT("Action3_Event"));
	RubixShiftRotation(0.1);
}

void AUE5_CubesGameModeBase::Action4_Event() {
	UE_LOG(LogTemp, Warning, TEXT("Action4_Event"));
	//Random color
	RandomColor(); // Make more colorfull
}

void AUE5_CubesGameModeBase::LeftShoulder_Event()
{
	UE_LOG(LogTemp, Warning, TEXT("LeftShoulder_Event"));
	DecayEffect(1);
	DecayEffectReverse2(4);
}

void AUE5_CubesGameModeBase::RightShoulder_Event()
{
	UE_LOG(LogTemp, Warning, TEXT("RightShoulder_Event"));
	//Reset cube
	ResetCube();
	
}

void AUE5_CubesGameModeBase::RightTrigger_Event(float AxisValue)
{
	UE_LOG(LogTemp, Warning, TEXT("RightTrigger_Event 1"));
	newT->ZoomIn(AxisValue);
}

void AUE5_CubesGameModeBase::LeftTrigger_Event(float AxisValue)
{
	UE_LOG(LogTemp, Warning, TEXT("LeftTrigger_Event"));
	newT->ZoomOut(AxisValue);
}

void AUE5_CubesGameModeBase::RightThumb_Event()
{
	UE_LOG(LogTemp, Warning, TEXT("RightThumb_Event"));
	LightRubix(4);
}

void AUE5_CubesGameModeBase::LeftThumb_Event()
{
	UE_LOG(LogTemp, Warning, TEXT("LeftThumb_Event"));
	DecayEffect2(2);
	DecayEffectReverse2(8);
}

void AUE5_CubesGameModeBase::LeftAxisX(float AxisValue)
{
	UE_LOG(LogTemp, Warning, TEXT("LeftAxisX"));
	this->ZoomRadius += AxisValue * 0.03f;
	//CameraThing->PauseCamera();
	newT->UpdateRadius(ZoomRadius);

}

void AUE5_CubesGameModeBase::LeftAxisY(float AxisValue)
{
	//CameraThing->PauseCamera();
	newT->UpdateRoll(AxisValue * 0.03f);
	UE_LOG(LogTemp, Warning, TEXT("LeftAxisY ?"));
}

void AUE5_CubesGameModeBase::RightAxisX(float AxisValue)
{
	UE_LOG(LogTemp, Warning, TEXT("RightAxisX"));
	newT->UpdatePitch(AxisValue * 0.03f);
}

void AUE5_CubesGameModeBase::RightAxisY(float AxisValue)
{
	/*FTimerDelegate TimerDel;
	FTimerHandle* FTA1 = new FTimerHandle;
	TimerDel.BindUFunction(this, FName("ApplyFontA"), FVector(168, 50, 50), FVector(58, 50, 168), FVector(58, 50, 168), "CUBES");
	GetWorldTimerManager().SetTimer(*FTA1, TimerDel, 1.0f, false, 3.0f * 1);*/
	
	UE_LOG(LogTemp, Warning, TEXT("RightAxisY"));
}

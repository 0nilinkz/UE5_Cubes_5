// Fill out your copyright notice in the Description page of Project Settings.

#include "Font_VisualEffect.h"
#include "VisualEffect.h"
#include <Runtime/Core/Public/Misc/LocalTimestampDirectoryVisitor.h>
#include <UE5_Cubes/lodepng.h>
#include "Kismet/GameplayStatics.h"
#include <math.h> 
#include <UE5_Cubes/UE5_CubesGameModeBase.h>


UFont_VisualEffect::UFont_VisualEffect(){}

UFont_VisualEffect::UFont_VisualEffect(int cubeQuantity)
{
	CubeQuantity = cubeQuantity;
}

void UFont_VisualEffect::SetupTimer()
{
	UE_LOG(LogTemp, Warning, TEXT("Output: color and opacity provided %s"), *FString("Setting up timers"));
	UE_LOG(LogTemp, Warning, TEXT("Output: color and opacity provided %s"), *FontText.ToString());
	
	TimerDelegate.BindUFunction(this, FName("DrawFontEffect"), FontText.ToString());
}

void UFont_VisualEffect::SetupTimerB(FString inputCharacter)
{
	TimerDelegate.BindUFunction(this, FName("DrawFontEffect"), inputCharacter);
}

UFont_VisualEffect* UFont_VisualEffect::GetDuplicate(UFont_VisualEffect* dupe)
{

	UE_LOG(LogTemp, Warning, TEXT("Output: color and opacity provided %s"), *FString("Creating reference"));
	
	UFont_VisualEffect* ReturnedCopy;

	ReturnedCopy = CreateDefaultSubobject<UFont_VisualEffect>(TEXT("VisualEffect"));

	//Log and see if it crashes here.l

	UE_LOG(LogTemp, Warning, TEXT("Output: color and opacity provided %s"), *FString(dupe->BackroundImage->ColorAndOpacity.ToString()));
	
	ReturnedCopy->BackroundImage->ColorAndOpacity = dupe->BackroundImage->ColorAndOpacity;
	ReturnedCopy->CoreImage->ColorAndOpacity = dupe->BackroundImage->ColorAndOpacity;
	ReturnedCopy->FontImage->ColorAndOpacity = dupe->FontImage->ColorAndOpacity;
	ReturnedCopy->CubeParent = dupe->CubeParent;
	ReturnedCopy->BPM = dupe->BPM;
	ReturnedCopy->EffectName = dupe->EffectName;
	ReturnedCopy->EffectTimeDuration = dupe->EffectTimeDuration;
	ReturnedCopy->StartTime = dupe->StartTime;
	ReturnedCopy->FontText = dupe->FontText;
	ReturnedCopy->FontAStore = dupe->FontAStore;

	//Timers will need adjusting
	ReturnedCopy->TimerDelegate = dupe->TimerDelegate;
	ReturnedCopy->TimerHandle = dupe->TimerHandle;

	//Create a function that process the timers for each effect
	return ReturnedCopy;
}

void UFont_VisualEffect::CreateDrawTimers(FString letter)
{


}

void UFont_VisualEffect::DrawFontEffect(FString Letter) {
	UE_LOG(LogTemp, Warning, TEXT("Output: Draw Font Effect char recived: %s"), *Letter);

	TArray<AActor*> SuperCubeContainer;
	CubeParent->GetAttachedActors(SuperCubeContainer);
	
	FString DisplayString = Letter.ToUpper();

	if (FontAStore.Num() == 0) {
		UE_LOG(LogTemp, Warning, TEXT("Output: Loading Font A Data %s"));
		LoadInFontA();
	}

	if (FontAStore.Num() == 0) {
		UE_LOG(LogTemp, Warning, TEXT("Output: Unable to load fonts"));
		return;
	}
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

	if (FontAStore.Num() == 0) {
		UE_LOG(LogTemp, Warning, TEXT("Output: Font Data not loaded %s"), *Letter);
		return;
	}
	
	//Data Check
	if (displayData == "") {
		UE_LOG(LogTemp, Warning, TEXT("Output: No valid FString provided. %s"), *Letter);
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

	CubeQuantity = cbrt(SuperCubeContainer.Num());

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

	//RGB values - random colors
	//float R = GetRandomFloat(0.0f, 1.0f);
	//float G = GetRandomFloat(0.0f, 1.0f);
	//float B = GetRandomFloat(0.0f, 1.0f);

	//while (true) {
	//	if ((!((R > 0.8) && (G > 0.8) && (B > 0.8f))) || (R + G + B < 0.8)) { //Here
	//		break;
	//	}
	//	else {
	//		R = GetRandomFloat(0.0f, 1.0f);
	//		G = GetRandomFloat(0.0f, 1.0f);
	//		B = GetRandomFloat(0.0f, 1.0f);
	//	}
	//}

	//float black = 0.0f;

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
					cube->SetColor(FontImage->ColorAndOpacity.R, FontImage->ColorAndOpacity.G, FontImage->ColorAndOpacity.B);
				}
				else if (displayApplied[x] == 'i') {
					//Background
					cube->SetColor(BackroundImage->ColorAndOpacity.R, BackroundImage->ColorAndOpacity.G, BackroundImage->ColorAndOpacity.B);
				}
			}
		}
	}

	//Handle cube core
	for (int i = 0; i < centerData.Num(); i++) {
		AMyCubeActor* cube = centerData[i];
		//Core - apply core color
		cube->SetColor(CoreImage->ColorAndOpacity.R, CoreImage->ColorAndOpacity.G, CoreImage->ColorAndOpacity.B);
	}

	UE_LOG(LogTemp, Warning, TEXT("Output: DrawFontEffectComplete %s"), *Letter);
}

void UFont_VisualEffect::LoadInFontA()
{
	UE_LOG(LogTemp, Warning, TEXT("Output: number to process %s"), *FString("Loading in font A"));

	FString ContentDir = FPaths::ProjectContentDir();

	FString directoryToSearch = ContentDir + TEXT("CubeContent/Images/CubeFontA/");
	FString filesStartingWith = TEXT("");
	FString fileExtensions = TEXT("png");

	TArray<FString> filesInDirectory = GetAllFilesInDirectory(directoryToSearch, true, filesStartingWith, fileExtensions);

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

FString UFont_VisualEffect::DecodeOneStep(char* filename)
{
	TArray<unsigned char> image; //the raw pixels
	unsigned width, height;

	//decode
	unsigned error = lodepng::decode(image, width, height, filename);

	TArray<char> example = GetVectorFromPng(image, width, height);

	std::string output = &example[0];
	std::string built = "";

	//change loop size, can i call length here? 
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

TArray<FString> UFont_VisualEffect::GetAllFilesInDirectory(const FString directory, const bool fullPath, const FString onlyFilesStartingWith, const FString onlyFilesWithExtension)
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

TArray<char> UFont_VisualEffect::GetVectorFromPng(const TArray<unsigned char>& image, unsigned w, unsigned h)
{
	const unsigned CubeWidth = 7;
	const unsigned loopUpper = CubeWidth + 1;

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

FString UFont_VisualEffect::MirrorFString(FString mirror) {
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

FString UFont_VisualEffect::ShiftRightStep(FString mirror) {
	int Sroot = sqrt(mirror.Len());
	FString Start = mirror.Mid(0, Sroot);
	FString End = mirror.Mid(Sroot, mirror.Len());
	return End + Start;
}

FString UFont_VisualEffect::RotateCubeStringRight(FString mirror) {
	const int Sroot = sqrt(mirror.Len());
	const int x = mirror.Len();
	TArray<FString> container;
	FString NewString = "";
	for (int i = 0; i < Sroot; i++) {
		for (int f = 0; f < mirror.Len(); f++) {
			if (f % Sroot == i) {
				NewString += mirror[f];
			}
		}
	}
	return NewString;
}

float UFont_VisualEffect::GetRandomFloat()
{
	const float X = 0.008f;
	return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / X));
}

float UFont_VisualEffect::GetRandomFloat(float Upper, float Lower)
{
	return Lower + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (Upper - Lower)));
}
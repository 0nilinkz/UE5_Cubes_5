// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintFunctions.h"
#include "Kismet/KismetMathLibrary.h"
#include <cmath>

FVector2D UBlueprintFunctions::SquareToCircle(float x, float y)
{
    
    FVector2D input;
    input.X = x;
    input.Y = y;
    input.SizeSquared();
    if (input.SizeSquared() == 0)
    {
        FVector2D zero;
        zero.X = 0;
        zero.Y = 0;
        return zero; // Avoid division by 0
    }
    FVector2D output;
    float multiplier = 1.0f / FMath::Max(input.X, input.Y);
    output = input * multiplier; // Extend the line to the edge of the square
    float divisor = sqrtf((output.X * output.Y) + (output.Y * output.Y));
    output = input / divisor; // Contract the original point away from the circle's edge
    return output;
}

FVector2D UBlueprintFunctions::RoundCoords(float radius, float angle, float x, float y) {
    
    FVector2D output;
    const int pinOffset = 6;
    //Rounding for the color picker's pin
    output.Y = radius * FMath::Cos(FMath::DegreesToRadians(angle-180));
    output.X = radius * FMath::Sin(FMath::DegreesToRadians(angle));

    // Add or remove the offset?
    output.X += radius- pinOffset;
    output.Y += radius- pinOffset;
    
    //Distance comparison and selecting the X Y pair
    //float d1 = sqrt(xa - xb)^2 + (ya - yb)^2
    float distance = sqrtf(FMath::Square(x - pinOffset) + FMath::Square(y - pinOffset));
    float distance2 = sqrtf(FMath::Square(output.X - pinOffset) + FMath::Square(output.Y - pinOffset));

    /*distance = sqrt(pow(x - pinOffset, 2) + pow(y - pinOffset, 2));
    distance2 = sqrt(pow(output.X - pinOffset, 2) + pow(output.Y - pinOffset, 2));*/

    float diff = distance2 - distance;
    UE_LOG(LogTemp, Warning, TEXT("Angle: %s"), *FString::SanitizeFloat(angle));
    UE_LOG(LogTemp, Warning, TEXT("Angle Mod: %s"), *FString::SanitizeFloat(angle-180));
    UE_LOG(LogTemp, Warning, TEXT("Distance: 1 %s"), *FString::SanitizeFloat(distance));
    UE_LOG(LogTemp, Warning, TEXT("Distance: 2 %s"), *FString::SanitizeFloat(distance2));
    UE_LOG(LogTemp, Warning, TEXT("Difference: 3 %s"), *FString::SanitizeFloat(diff));
    UE_LOG(LogTemp, Warning, TEXT("X Provided: %s"), *FString::SanitizeFloat(x));
    UE_LOG(LogTemp, Warning, TEXT("Y Provided: %s"), *FString::SanitizeFloat(y));
    //Reverse for this quadrant (#2)
    
    if (angle >= 270 - pinOffset && diff >= 0) {
        UE_LOG(LogTemp, Warning, TEXT("Play B"));

        if (diff < 0 && angle > 250) {
            output.X = x + pinOffset;
            output.Y = y + pinOffset;
            return output;
        }
        return output; //reverse here
    }
    if (diff >= 0) {
        UE_LOG(LogTemp, Warning, TEXT("Play C"));
        output.X = x + pinOffset;
        output.Y = y + pinOffset;
        return output;
    }
    if (angle >= 270 - pinOffset) {
        UE_LOG(LogTemp, Warning, TEXT("Play A"));
        output.X = x;
        output.Y = y;
        return output; //reverse here
    }
    else {
        if (diff <= 0 && angle < 20) { //Error?
            UE_LOG(LogTemp, Warning, TEXT("Play D1"));
            output.X = x + pinOffset;
            output.Y = y + pinOffset;
            return output;
        }
        if (diff < 0 && angle > 250) { // kinda works but SUPER WONK
            UE_LOG(LogTemp, Warning, TEXT("Play D2"));
            output.X = x + pinOffset;
            output.Y = y + pinOffset;
            return output;
        }
        UE_LOG(LogTemp, Warning, TEXT("Play D"));
        return output;
    }
}


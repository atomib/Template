// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSTemplate.h"
#include "BuyAreaVolume.h"

ABuyAreaVolume::ABuyAreaVolume()
{
	GetBrushComponent()->SetCollisionProfileName(FName("Trigger"));
	GetBrushComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetBrushComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}
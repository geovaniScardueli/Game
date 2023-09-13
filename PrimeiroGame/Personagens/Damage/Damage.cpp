// Fill out your copyright notice in the Description page of Project Settings.


#include "Damage.h"


int32 UDamage::CalcaleteHealth(const int32 Power, const int32 Defense, const int32 AddPower)
{
	return Power + AddPower - Defense;
}

int32 UDamage::CalcaletePosture(const int32 Power, const int32 Defense, const int32 AddDefense)
{
	return Power - Defense + AddDefense;
}
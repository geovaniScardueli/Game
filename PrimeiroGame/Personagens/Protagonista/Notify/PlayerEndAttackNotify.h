// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "PlayerEndAttackNotify.generated.h"

/**
 * 
 */
UCLASS()
class PRIMEIROGAME_API UPlayerEndAttackNotify : public UAnimNotify
{
	GENERATED_BODY()
protected:
	virtual bool ShouldFireInEditor() override { return false; }

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};

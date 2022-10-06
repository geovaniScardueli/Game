// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DefaulEnemyHead.generated.h"

UCLASS()
class PRIMEIROGAME_API ADefaulEnemyHead : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADefaulEnemyHead();

	UFUNCTION()
	void FireHead();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Config Head")
	int XVelocity = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Config Head")
	int YVelocity = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Config Head")
	int ZVelocity = 0;

};

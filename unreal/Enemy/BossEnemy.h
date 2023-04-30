// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicEnemy.h"
#include "BossEnemy.generated.h"

/**
 * 
 */
UCLASS()
class NOTEBOOK_API ABossEnemy : public ABasicEnemy
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	void RunCoolTime(float DeltaTime);
	void CalculateDistFromPlayer();
	void CheckSuddenAttack();
	void CheckBackAttack();
	void Turn();
	bool CheckNearGround();
	void BossDie();

public:
	ABossEnemy();
	virtual void Attack() override;
	virtual void MyTakeDamage(AActor* attacker, float damage, EnemyHitedState hit, float hitedTime_ = 0.f, FVector launchVec = FVector(0.f, 0.f, 0.f), FName note = TEXT("")) override;

	UFUNCTION(BlueprintCallable)
		void Boss_AttackCheck();

	UFUNCTION(NetMulticast, Reliable)
		 void NormalAttackCombo_Multicast();
	UFUNCTION(NetMulticast, Reliable)
		 void RangeAttack1_Multicast();
	UFUNCTION(NetMulticast, Reliable)
		 void TurnAttack_Multicast();
	UFUNCTION(NetMulticast, Reliable)
		 void RangeAttack2_Multicast();
	UFUNCTION(NetMulticast, Reliable)
		 void BackAttack_Multicast();

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
		UAnimMontage* normalAttackAnim;
	UPROPERTY(EditDefaultsOnly, Category = Pawn)
		UAnimMontage* rangeAttackAnim1;
	UPROPERTY(EditDefaultsOnly, Category = Pawn)
		UAnimMontage* turnAttackAnim;
	UPROPERTY(EditDefaultsOnly, Category = Pawn)
		UAnimMontage* rangeAttackAnim2;
	UPROPERTY(EditDefaultsOnly, Category = Pawn)
		UAnimMontage* backAttackAnim;
	UPROPERTY(EditDefaultsOnly, Category = Pawn)
		UAnimMontage* LDodgeAnim;
	UPROPERTY(EditDefaultsOnly, Category = Pawn)
		UAnimMontage* RDodgeAnim;
	UPROPERTY(EditDefaultsOnly, Category = Pawn)
		UAnimMontage* DieAnim;


	UPROPERTY(EditDefaultsOnly, Category = Pawn)
		UAnimMontage* LTurnAnim;
	UPROPERTY(EditDefaultsOnly, Category = Pawn)
		UAnimMontage* RTurnAnim;
	
	UFUNCTION(BlueprintCallable)
		void RotationArroundToPlayer();
	UFUNCTION(BlueprintCallable)
		void LaunchToPlayer();
	UFUNCTION()
		void ProjectToTarget();
	UFUNCTION()
		void TickParabola(float delta);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isRotationArroundToPlayer = false;

	bool firstAttack = true;

	const float closeAttackRange = 900.f;
	const float originalAttackCoolTime = 2.f;
	float turnCoolTime;
	float backAttackCoolTime;
	float suddenAttackCoolTime;

	bool isParabola=false;
	bool isCheckNearGround=false;
	
	const float gravitational_acceleration=8.f;
	const float vertical_force=140.f;
	float accumulate_down_vertical_force;

	FVector startLocation;
	FVector targetLocation;
	float distance;

};

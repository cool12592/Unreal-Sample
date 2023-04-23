// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "PlayerCharacter.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "MyPlayerController.h"
//#include "Net/UnrealNetwork.h"
#include "Engine.h"
// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	
	isShowInventory = false;
	Inven_Items_MAP.Add(3, 1);
	Inven_Items_MAP.Add(1, 3);
	QuickSlotItemArray.Init(0, 2);

}

void UInventoryComponent::MyInventoryCopyFunc(const UInventoryComponent* other , class AMyPlayerController* controller)
{
	Inven_Items_MAP.Empty();
	if (!other) return;
	Inven_Items_MAP = other->Inven_Items_MAP;
	NowWeaponWearing_number = other->NowWeaponWearing_number;
	QuickSlotItemArray = other->QuickSlotItemArray;
	myOwner = controller;
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryComponent::ShowInventory()
{
	if (isShowInventory == false)
	{
		if (!uiInventoryWidget)
		{
			FName path = TEXT("/Game/My__/UI/Inven/Inventory.Inventory_C");
			UClass* GeneratedBP = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *path.ToString()));
			uiInventoryWidget = CreateWidget<UUserWidget>(GetWorld(), GeneratedBP);
		}
		
		if(uiInventoryWidget && uiInventoryWidget->IsInViewport()==false)
			uiInventoryWidget->AddToViewport();
		
		if (uiInventoryWidget)
			uiInventoryWidget->SetVisibility(ESlateVisibility::Visible);

		if (myOwner)
		myOwner->bShowMouseCursor = true;

		isShowInventory = true;
	}


}

void UInventoryComponent::CloseInventory()
{
	if (isShowShop) return;

	if (uiInventoryWidget)
		uiInventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	
	if (myOwner)
	myOwner->bShowMouseCursor = false;
	isShowInventory = false;
}

void UInventoryComponent::UseItem(int ItemID)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("heal heal")); // 화면출력

	if (!myOwner )
	{
		GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Red, TEXT("aa")); // 화면출력

		return;
	}
	auto* player = Cast<APlayerCharacter>(myOwner->GetCharacter());
	if (!player)
	{
		GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Red, TEXT("bb")); // 화면출력

		return;
	}
	bool me = false;
	if (myOwner == UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		me = true;
	}

	if (me && !Inven_Items_MAP.Find(ItemID))
	{
		GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Red, TEXT("cc")); // 화면출력

		return;
	}
	
	if (me && player->enable_attack == false)
	{
		GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Red, TEXT("dd")); // 화면출력

		return;
	}
	switch (ItemID)
	{
	case 1:
		if(me)
			Inven_Items_MAP[ItemID]--;
		player->HP_HEAL(20.F);

		break;
	case 2:
		if(me)
			Inven_Items_MAP[ItemID]--;
		player->STAMINA_HEAL(20.F);

		break;
	case 3:
	{	
		NowWeaponWearing_number = 3;
		FOutputDeviceNull pAR;
		player->CallFunctionByNameWithArguments(TEXT("TempWeaponChange1"), pAR, nullptr, true);
		if (me && uiInventoryWidget)
		{
			FOutputDeviceNull pAR2;
			uiInventoryWidget->CallFunctionByNameWithArguments(TEXT("SetWeaponUI_POS"), pAR2, nullptr, true);
		}
		break;
	}
	case 4:
	{
		NowWeaponWearing_number = 4;
		FOutputDeviceNull pAR;
		player->CallFunctionByNameWithArguments(TEXT("TempWeaponChange2"), pAR, nullptr, true);
		if (me && uiInventoryWidget)
		{
			FOutputDeviceNull pAR2;
			uiInventoryWidget->CallFunctionByNameWithArguments(TEXT("SetWeaponUI_POS"), pAR2, nullptr, true);
		}
		break;
	}
	default:
		break;
	}
	if (me && Inven_Items_MAP[ItemID] <= 0)
		Inven_Items_MAP.Remove(ItemID);
	if (me&& uiInventoryWidget)
	{
		FOutputDeviceNull pAR;
		uiInventoryWidget->CallFunctionByNameWithArguments(TEXT("UpdateMyInventory"), pAR, nullptr, true);
	}
}


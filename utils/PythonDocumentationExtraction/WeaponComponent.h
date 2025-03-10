#pragma once

#include "CoreMinimal.h"
#include "WeaponAssetDefault.h"
#include "WeaponRuntimeGraph.h"
#include "Miscellaneous/WeaponAttributesModificator.h"
#include "Miscellaneous/DynamicPointer.h"
#include "WeaponComponent.generated.h"

class UStaticMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTriggered);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCooledDown);

/**
 * Component that handles the weapon logic.
 */
UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class WASRUNTIME_API UWeaponComponent : public UPrimitiveComponent
{
	GENERATED_BODY()
	
#pragma region Properties
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWeaponAttributesModificator ManualModificator;

	//Used to cache components with the specific socket
	mutable TMap<FName, TSoftObjectPtr<USceneComponent>> CachedSockets;
	mutable TMap<FName, TSoftObjectPtr<USceneComponent>> CachedTags;

	UPROPERTY(BlueprintAssignable, Category = Trigger)
	FTriggered OnTriggered;

	UPROPERTY(BlueprintAssignable, Category = Trigger)
	FTriggered OnTriggerFailed;
	
	UPROPERTY(BlueprintAssignable, Category = Trigger)
	FCooledDown OnCooledDown;

	UPROPERTY(BlueprintAssignable, Category = Trigger)
	FTriggered OnBeginReload;
	
	UPROPERTY(BlueprintAssignable, Category = Trigger)
	FTriggered OnReload;

	UPROPERTY(BlueprintAssignable, Category = Trigger)
	FTriggered OnFinishReload;
	
	UPROPERTY(BlueprintAssignable, Category = Trigger)
	FTriggered OnReloadFailed;

	UPROPERTY(BlueprintAssignable, Category = Trigger)
	FTriggered OnJamm;

	TDynamicPtr AmmoSource;

	TDynamicPtr AmmoInMagazine;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient)
	class UWeaponAssetDefault* WeaponAsset;
	
	//FAst access to source weapon asset. EDITOR ONLY
	UPROPERTY(EditAnywhere)
	TObjectPtr<const class UWeaponAssetDefault> WeaponAssetSource;
	
	FTimerHandle TriggerCooldown;
	
	FTimerHandle TriggerTimer;
	
	int32 BurstIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 bUseInstancedAsset:1 = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bJammed:1;
	
	uint8 bIsTriggering:1;
	
	uint8 isGenerating:1;

	uint8 bCancelReload:1;

	uint8 bReloading:1;

#pragma endregion
	
#pragma region Methods
public:

#pragma region InitializationAndDestruction

	/*
		ParamsAndReturn some generic description
		@param Param1 some generic description for the first parameter
		@param Param2 some generic description for the second parameter
		@param Return1 some generic description for the first return value
		@param Return2 some generic description for the second return value
		@return some generic description for the return value
	*/
	UFUNCTION(BlueprintCallable, Category = Caca, meta=(DisplayName="ParamsAndReturn"))
	int32 ParamsAndReturn(int32 Param1, int32 Param2, int32& Return1, int32& Return2);

	/*
	cacacaca
	*/
	void ResolveAssetInstantiation();

	//cacacacaca
	UFUNCTION(BlueprintCallable)
	void Init();

	virtual void OnRegister() override;

	virtual void UninitializeComponent() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
#pragma endregion
	
#pragma region Trigger

	UFUNCTION(BlueprintCallable, Category = Trigger)
	virtual void Trigger(bool isFirst = true);

	UFUNCTION(BlueprintCallable, Category = Trigger, CallInEditor)
	void EnterTrigger();

	UFUNCTION(BlueprintCallable, Category = Trigger, CallInEditor)
	void ExitTrigger();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Trigger)
	bool IsTriggering() const { return bIsTriggering; }

	UFUNCTION(BlueprintCallable, Category = Trigger)
	virtual bool CanTrigger() const;

	virtual bool TriggerModeAllowNextTrigger() const;

	UFUNCTION(BlueprintCallable, Category = Trigger)
	bool HasEnoughAmmoToTrigger() const;
	
	void SetCooldown();

	UFUNCTION(BlueprintCallable, Category = Trigger)
	void SetCustomCooldown(float Cooldown);

	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE bool IsCoolingDown() const { return GetWorld()->GetTimerManager().IsTimerActive(TriggerCooldown); }

	UFUNCTION(BlueprintCallable, Category = Trigger)
	void TryJammWeapon();
	
#pragma endregion

#pragma region Build

	UFUNCTION(BlueprintCallable)
	void UpdateMesh();
	
	/**
	 * Builds the weapon asset.
	 * @param Asset The weapon asset to build.
	 */
	void BuildAsset(class UWeaponAssetDefault* Asset);
	
	UFUNCTION(BlueprintCallable)
	void Clean();

protected:
	void AttachCreatedComponents(const TArray<UPrimitiveComponent*>& PrimitiveComponents);
	
#pragma endregion

#pragma region Ammo
public:
	
	UFUNCTION(BlueprintCallable, CustomThunk, meta=(CustomStructureParam="Source"))
	// ReSharper disable once CppFunctionIsNotImplemented
	void SetAmmoSource(UPARAM(Ref) int32& Source, bool ReturnRemainingAmmo = true);

	DECLARE_FUNCTION(execSetAmmoSource);
	
	/**
	 * Sets the value of a map as the source of the ammo. This node exists since there's no find by ref in Map containers unlike in arrays that offer get by ref.
	 * IMPORTANT. Even tho the Key of the map can be of any type, the value must be a value reinterpretable as int64 (eg, int32, enum, uint8, int16, etc) with any other type expect undefined behaviour and crashes
	 * @param Map The map from where the ammo will be collected
	 * @param InKey Key value that must be found in the map in order to use its value as source ammo
	 */
	UFUNCTION(BlueprintCallable, CustomThunk, meta=(MapParam="Map", MapKeyParam="InKey"))
	// ReSharper disable once CppFunctionIsNotImplemented
	void SetAmmoSourceFromMap(const TMap<int32, int32>& Map, const int32& InKey, bool ReturnRemainingAmmo = true);

	DECLARE_FUNCTION(execSetAmmoSourceFromMap);

#pragma endregion

	UFUNCTION()
	void SolveMagazineResize(int32 From, int32 To);

	UFUNCTION()
	void OnUseAmmoUpdated(bool previous, bool current);

	UFUNCTION()
	void OnUseMagazineUpdated(bool previous, bool current);
	
	FORCEINLINE void ClearCache() const {CachedSockets.Empty(); CachedTags.Empty();}
    
	/*
		Finds the used socket
		@param Socket The socket to find
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure)
	USceneComponent* FindSocket(const FName& Socket) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	USceneComponent* GetTaggedComponent(const FName& Tag) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasTaggedComponent(const FName& Tag) const {return !!GetTaggedComponent(Tag);}

	USceneComponent* FindCachedSocket(const FName& Socket) const;
	USceneComponent* GetCachedTaggedComponent(const FName& Tag) const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(AdvancedDisplay="MoveValues"))
	void InitializeWeapon(UPARAM(ref) struct FWeaponInitializationData& Data, bool bRecallDelegates = false, uint8 MoveValues = 1);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(AdvancedDisplay="MoveValues"))
	FWeaponInitializationData GenerateInitializationData();
	
#pragma endregion

#pragma region Debug
#if WITH_EDITORONLY_DATA
public:
	
	UPROPERTY(EditAnywhere)
	TMap<uint8, uint8> Modifications;

	bool HasAnyRecursion() const;
	bool HasAnyRecursion(TArray<const UWeaponAsset*>& PreviousRecursions) const;

	void BindOnModified();
	
#endif
#pragma endregion

#pragma region Accessors
public:

	UFUNCTION(BlueprintCallable, Category = Trigger)
	bool UsesInstancedAsset() const {return bUseInstancedAsset;}
	void SetUseInstancedAsset(bool NewUseInstanced) {bUseInstancedAsset = NewUseInstanced;}

	/**
	 * Updates a modificator in a specific slot. Must be overrided in order to work add features.
	 * @param 	Slot			The slot where the modification is applied.
	 * @param 	Modification	The modification to apply.
	 * @param	bForceUpdate	Forces to update all generation even if no changes were made
	*/
	UFUNCTION(BlueprintCallable)
	void ChangeModificationAtSlot(uint8 Slot, uint8 Modification, bool bForceUpdate = true);
	
	/**
	* Updates all modificators to match the input.
	* @param 	newModifications	Map containing the new mods and slots.
	* @param	bForceUpdate		Forces to update all generation even if no changes were made
	*/
	UFUNCTION(BlueprintCallable)
	void SetWeaponModifications(const TMap<uint8, uint8>& newModifications, bool bForceUpdate = true);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool ExistSlot(int32 slot) const {return GetWeaponModificationsConst().Contains(slot);}
	
	/**
	* Gets a enum of modifiers in the selected slot.
	* @param 	Slot	Index of the slot.
	* @returns	Null if out of bounds, else the modifier in slot.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE uint8 GetModificationAtSlot(uint8 Slot) const {return GetWeaponModificationsConst().Contains(Slot) ? WeaponAsset->ModificationApplied[Slot] : -1;};
	
	/** Gets the modificators */
	UFUNCTION(BlueprintCallable, Category = Trigger)
	TMap<uint8, uint8>& GetWeaponModificationsConst() const {return WeaponAsset->ModificationApplied;}

	/** Gets the modificators */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	const TMap<uint8, uint8>& GetWeaponModifications() const {return WeaponAsset->ModificationApplied;}

	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE void SetWeaponAsset(class UWeaponAsset* Asset);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsGenerating() const {return isGenerating;}

	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE class UWeaponAssetDefault* GetWeaponAsset() const {return WeaponAsset;}

	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE float GetTriggerRate() const {return ManualModificator.TriggerRate.Operator.CalculateOtherAsBase(WeaponAsset->GetTriggerRate()); }
	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE float GetTriggerRateRaw() const {return WeaponAsset->GetTriggerRate(); }
	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE float GetTriggerRateSec() const {return 1.0 / ManualModificator.TriggerRate.Operator.CalculateOtherAsBase(WeaponAsset->GetTriggerRate()); }
	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE float GetTriggerRateSecRaw() const {return 1.0 / WeaponAsset->GetTriggerRateMs(); }

	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE uint8 GetBurstTriggers() const {return ManualModificator.BurstTriggers.Operator.CalculateOtherAsBase(WeaponAsset->GetBurstTriggers());}
	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE uint8 GetBurstTriggersRaw() const {return WeaponAsset->GetBurstTriggers();}

	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE int32 GetMagazineCapability() const {return ManualModificator.MagazineCapability.Operator.CalculateOtherAsBase(WeaponAsset->GetMagazineCapability());}
	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE int32 GetMagazineCapabilityRaw() const {return WeaponAsset->GetMagazineCapability();}

	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE float GetJammProbability() const {return ManualModificator.JammProbability.Operator.CalculateOtherAsBase(WeaponAsset->GetJamProbability());}
	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE float GetJammProbabilityRaw() const {return WeaponAsset->GetJamProbability();}

	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE bool GetUseAmmo() const {return WeaponAsset->GetUsesAmmo();}
	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE bool GetUseMagazine() const {return WeaponAsset->GetUsesMagazine();}
	
	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE int32 GetAmmoInMagazine(int32 Fallback=0) const {return GetUseMagazine() ? AmmoInMagazine : Fallback;}
	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE int32 GetAmmoInMagazineOrSource() const {return GetAmmoInMagazine();}

	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE int64 GetSourceAmmo64() const {return AmmoSource;}
	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE int32 GetSourceAmmo32() const {return AmmoSource;}
	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE uint8 GetSourceAmmo8() const {return AmmoSource;}

	
	
	template<typename T>
	FORCEINLINE T GetSourceAmmo() const {return AmmoSource;}

	template<typename T>
	FORCEINLINE T GetMagazineAmmo() const {return AmmoInMagazine;}

	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE uint8 GetIterationReloadStepAmmount() const {return ManualModificator.IterationReloadStepAmmount.Operator.CalculateOtherAsBase(GetIterationReloadStepAmmountRaw());}
	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE uint8 GetIterationReloadStepAmmountRaw() const {return WeaponAsset->GetIterationReloadStepAmount();}

	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE int32 GetAmmoLoosePerTrigger() const {return ManualModificator.AmmoLoosePerTrigger.Operator.CalculateOtherAsBase(GetAmmoLoosePerTriggerRaw());}
	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE int32 GetAmmoLoosePerTriggerRaw() const {return WeaponAsset->GetAmmoLoosePerTrigger();}

	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE bool GetTriggerIfAmmoLessThanRequired() const {return WeaponAsset->GetTriggerIfAmmoLessThanRequired();}

	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE bool GetCustomTriggerExecutionTiming() const {return WeaponAsset->GetCustomTriggerExecutionTiming();}

	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE bool GetCustomReloadExecutionTiming() const {return WeaponAsset->GetCustomReloadExecutionTiming() || !GetReloadByMagazine();}
	
	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE bool GetLooseExtraAmmoOnReload() const {return WeaponAsset->GetLooseExtraAmmoOnReload();}

	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE bool GetCustomUnjammExecutionTiming() const {return WeaponAsset->GetCustomUnjamExecutionTiming();}

	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE bool GetReloadByMagazine() const {return WeaponAsset->GetReloadByMagazine();}

	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE int32 GetAmmoInUse() const {return GetUseAmmo() ? GetAmmoInMagazineOrSource() : 0; }

	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE bool HasAmmo() const {return !GetUseAmmo() || GetAmmoInUse() > 0; }

	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE TEnumAsByte<EAmmoType> GetAmmoType() const {return WeaponAsset->GetAmmoType(); }

	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE bool IsJammed() const {return !!bJammed;}

	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE bool IsReloading() const {return bReloading;}
	
	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE uint8 GetCurrentBurstIndex() const {return BurstIndex;}

#pragma endregion

	UFUNCTION(BlueprintCallable, CallInEditor, Category = Trigger)
	void Reload();
	
	UFUNCTION(BlueprintCallable, Category = Trigger)
	FORCEINLINE void CancelReload() {bCancelReload = !GetReloadByMagazine();}

	UFUNCTION(BlueprintCallable, Category = Trigger)
	void ReloadLogic();
	
	UFUNCTION(BlueprintCallable, Category = Trigger)
	void ReloadLogicManual(int32 AddAmmo);

	virtual void BeginPlay() override;

	UFUNCTION()
	FORCEINLINE void EndReload() {bReloading = bCancelReload = false;}

};

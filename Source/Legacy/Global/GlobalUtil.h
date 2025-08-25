#pragma once

#include "CoreMinimal.h"

#define GET_MEMBER_NAME(Var) GET_MEMBER_NAME_CHECKED(std::remove_pointer_t<decltype(this)>, Var)

/**
 * Global Utility
 * - 공통적으로 사용 가능한 매크로, 템플릿, 헬퍼 함수
 */

namespace ConstructorUtil
{
    template<typename T>
    static FORCEINLINE T* FindAsset(const TCHAR* Path)
    {
        ConstructorHelpers::FObjectFinder<T> Finder(Path);
        return Finder.Object;
    }
}

/**
 * Enum Utility
 * - 공통적으로 사용 가능한 매크로, 템플릿, 헬퍼 함수
 */
namespace EnumUtil
{
    template <typename TEnum>
    FString GetEnumNameAsString(TEnum EnumValue)
    {
        static_assert(TIsEnum<TEnum>::Value, "Only works with enum types");

        const UEnum* EnumPtr = StaticEnum<TEnum>();
        if (!EnumPtr)
        {
            return FString("InvalidEnum");
        }

        // EEquipWeapon::Rifle → Rifle
        const FString FullName  = EnumPtr->GetNameByValue(static_cast<int64>(EnumValue)).ToString();
        const FString Prefix    = EnumPtr->GetName(); // e.g., "EEquipWeapon"
        const FString SearchKey = Prefix + TEXT("::");

        if (FullName.StartsWith(SearchKey))
        {
            return FullName.RightChop(SearchKey.Len());
        }

        return FullName;
    }
    
    template <typename TEnum>
    FORCEINLINE FName GetEnumDisplayNameAsFName(TEnum InValue)
    {
        static_assert(TIsEnum<TEnum>::Value, "TEnum must be an enum type");
        if (UEnum* Enum = StaticEnum<TEnum>())
        {
            const int32 Index = Enum->GetIndexByValue(static_cast<int64>(InValue));
            if (Index != INDEX_NONE)
            {
                FText DisplayText = Enum->GetDisplayNameTextByIndex(Index);
                return DisplayText.IsEmpty() ? FName(*Enum->GetNameStringByIndex(Index)) : FName(*DisplayText.ToString());
            }
        }
        return NAME_None;
    }

    template <typename TEnum>
    void ForEachEnum(TFunctionRef<void(TEnum)> Func)
    {
        static_assert(TIsEnum<TEnum>::Value, "TEnum must be an enum type");

        const UEnum* EnumPtr = StaticEnum<TEnum>();
        if (!EnumPtr) return;

        const int32 EnumCount = EnumPtr->NumEnums();

        // 마지막 항목(Max)은 Hidden 처리된 항목일 가능성이 높으므로 제외
        for (int32 i = 0; i < EnumCount - 1; ++i)
        {
            int64 Value = EnumPtr->GetValueByIndex(i);
            Func(static_cast<TEnum>(Value));
        }
    }

    template<typename TEnum>
    constexpr TEnum ToggleEnum(TEnum Current)
    {
        static_assert(std::is_enum_v<TEnum>, "ToggleEnum requires an enum type.");

        using Underlying = std::underlying_type_t<TEnum>;

        Underlying NextIndex = static_cast<Underlying>(Current) + 1;

        if (NextIndex >= static_cast<Underlying>(TEnum::Max))
        {
            NextIndex = 0;
        }

        return static_cast<TEnum>(NextIndex);
    }
}

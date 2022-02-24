#pragma once

#include <Windows.h>

using namespace System;

enum class Rarity : UInt32 {
	Rarity1 = 0x10100000,
	Rarity2 = 0x10100001,
	Rarity3 = 0x10100002,
	Rarity4 = 0x10100003,
	Rarity5 = 0x10100004,
	Rarity6 = 0x10100005,
	Rarity7 = 0x10100006,
	Rarity4_Novice = 0x10100007,
	Rarity3_Kinship = 0x10100008,
	Rarity12 = 0x10100009,
	Rarity2_Legacy = 0x1010000A
};

enum Offsets : uintptr_t {
	// Items
	ItemBox = 0x78,

	InventoryList = 0x18,
	InventorySize = 0x18,

	Items = 0x10,
	ItemInfo = 0x20,
	ItemCount = 0x14,
	ItemID = 0x10,

	FINDME = 0x08,

	// Equipment
	EquipmentBox = 0x80,

	EquipmentList = 0x28,
	EquipmentItems = 0x10,
	EquipmentSize = 0x18,

	EquipmentListStart = 0x20,

	// Currency
	HandMoney = 0x58,
	Zenny = 0x18,

	VillagePoints = 0x60,
	Points = 0x10
};

enum class EquipmentType : UInt32 {
	Empty,
	Weapon,
	Armor,
	Talisman,
	LvBuffCage // ???
};

public ref struct CharmData {
	UInt32 Index;
	Rarity Rarity;

	UInt32 Skill1;
	UInt32 Skill2;
	UInt32 Skill1Level;
	UInt32 Skill2Level;

	UInt32 Level1Slots;
	UInt32 Level2Slots;
	UInt32 Level3Slots;
};

ref struct ItemData {
	ItemData(UInt32 id, UInt32 amount)
		: ID(id), Amount(amount)
	{
	}

	UInt32 ID;
	UInt32 Amount;
};

#define OFFSET_EQ_TYPE 0x2C
#define OFFSET_RARITY 0x30
#define OFFSET_IDENTIFIER 0x1BC

#define OFFSET_SLOTS 0x70
#define OFFSET_LEVEL1_SLOTS 0x24
#define OFFSET_LEVEL2_SLOTS 0x28
#define OFFSET_LEVEL3_SLOTS 0x2C

#define OFFSET_SKILLS 0x78
#define OFFSET_SKILL1_ID 0x20
#define OFFSET_SKILL2_ID 0x21

#define OFFSET_LEVELS 0x80
#define OFFSET_SKILL1_LVL 0x20
#define OFFSET_SKILL2_LVL 0x24

#include <vcclr.h>

ref class IniFile
{
	const String^ Path;

public:
	IniFile(const wchar_t* inipath)
	{
		IO::FileInfo^ fi = gcnew IO::FileInfo(gcnew String(inipath));
		Path = fi->FullName;
	}

	inline String^ Read(const wchar_t* Key)
	{
		pin_ptr<const wchar_t> path = PtrToStringChars(Path);
		wchar_t ret[255] = { 0 };
		if (!GetPrivateProfileStringW(L"lang", Key, L"", ret, 255, path))
			Windows::Forms::MessageBox::Show("GetPrivateProfileString: " + GetLastError().ToString());

		return gcnew String(ret);
	}

	inline void Write(const wchar_t* Key, const wchar_t* Value)
	{
		pin_ptr<const wchar_t> path = PtrToStringChars(Path);
		WritePrivateProfileString(L"lang", Key, Value, path);
	}

	inline void DeleteKey(const wchar_t* Key)
	{
		Write(Key, L"");
	}

	inline bool KeyExists(const wchar_t* Key)
	{
		return Read(Key) != "";
	}
};

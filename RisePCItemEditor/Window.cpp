
#include "Window.h"
#include "Data.h"
#include "RawCharmEditor.h"

#include <stdio.h>
#include <TlHelp32.h>
#include <msclr/marshal.h>

struct Params {
	HANDLE hProcess;
	UINT32 uAmount;
	uintptr_t uDataManager;
};

System::Void RisePCItemEditor::Window::AddItemsFunc(uintptr_t dataManager, UINT32 amount, bool clear)
{
	uintptr_t itemBox = NULL;
	ReadProcessMemory(ProcessHandle, LPVOID(dataManager + Offsets::ItemBox), &itemBox, sizeof(uintptr_t), NULL);

	uintptr_t inventoryList = NULL;
	ReadProcessMemory(ProcessHandle, LPVOID(itemBox + Offsets::InventoryList), &inventoryList, sizeof(uintptr_t), NULL);

	uintptr_t items = NULL;
	ReadProcessMemory(ProcessHandle, LPVOID(inventoryList + Offsets::Items), &items, sizeof(uintptr_t), NULL);
	items += Offsets::ItemInfo;

	UINT32 size = 0;
	ReadProcessMemory(ProcessHandle, LPVOID(inventoryList + Offsets::InventorySize), &size, sizeof(UINT32), NULL);

	for (UINT32 i = 0; i < size; ++i)
	{
		if (!clear && !IncludeIllegalItems->Checked)
		{
			if ((i > 494 && i < 518) ||
				(i > 539 && i < 543) ||
				(i > 728 && i < 772) ||
				(i > 789 && i < 796) ||
				(i > 905 && i < 984) ||
				(i > 994 && i < 1004) ||
				(i > 1005 && i < 1014) ||
				(i > 1037 && i < 1041) ||
				(i > 1056 && i < 1062) ||
				(i > 1066 && i < 1075) ||
				(i > 1086)) continue;

			if (ExcludeList->Contains(i)) continue;
		}

		uintptr_t item = NULL;
		ReadProcessMemory(ProcessHandle, LPVOID(items), &item, sizeof(uintptr_t), NULL);

		uintptr_t itemData = NULL;
		ReadProcessMemory(ProcessHandle, LPVOID(item + Offsets::ItemInfo), &itemData, sizeof(uintptr_t), NULL);

		if (itemData)
		{
			if (clear)
			{
				UINT32 null = 0;
				UINT32 empty_id = 0x4000000;

				WriteProcessMemory(ProcessHandle, LPVOID(itemData + Offsets::ItemID), &empty_id, sizeof(null), NULL); // ID
				WriteProcessMemory(ProcessHandle, LPVOID(itemData + Offsets::ItemCount), &null, sizeof(null), NULL); // Amount
			}
			else
			{
				UINT32 count = 0;
				ReadProcessMemory(ProcessHandle, LPVOID(itemData + Offsets::ItemCount), &count, sizeof(UINT32), NULL);

				if (AddAllItems->Checked)
				{
					UINT32 id = i + 0x4100000; // ids are offset by that number for some reason
					WriteProcessMemory(ProcessHandle, LPVOID(itemData + Offsets::ItemID), &id, sizeof(id), NULL);
					WriteProcessMemory(ProcessHandle, LPVOID(itemData + Offsets::ItemCount), &amount, sizeof(amount), NULL);
				}
				else
				{
					if (count > 0)
						WriteProcessMemory(ProcessHandle, LPVOID(itemData + Offsets::ItemCount), &amount, sizeof(amount), NULL);
				}
			}
		}

		items += 0x08;
	}
}

System::Void RisePCItemEditor::Window::OpenGameProcess()
{
	if (ProcessHandle == INVALID_HANDLE_VALUE)
	{
		HWND hwnd = FindWindowA(NULL, "MonsterHunterRise");
		if (!hwnd)
		{
			throw gcnew System::Exception("Could not find MonsterHunterRise window. Please open the game.");
		}

		DWORD pid;
		GetWindowThreadProcessId(hwnd, &pid);

		ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		if (ProcessHandle == INVALID_HANDLE_VALUE)
		{
			throw gcnew System::Exception("Could not open MonsterHunterRise.exe");
		}
	}
}

__declspec(noreturn)
System::Void ThrowWindowsError(String^ msg)
{
	throw gcnew Exception(msg + ": " + GetLastError().ToString());
}

System::String^ FormatAddressAndHandle(LPVOID addr, LPVOID handle)
{
	return "(" + UIntPtr(addr).ToString() + ", " + UIntPtr(handle).ToString() + ")";
}

System::Void RisePCItemEditor::Window::OpenGameProcessByExeName(const wchar_t* name)
{
	if (!ProcessHandle)
	{
		PROCESSENTRY32 entry;
		ZeroMemory(&entry, sizeof(PROCESSENTRY32));
		entry.dwSize = sizeof(PROCESSENTRY32);

		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		if (Process32First(snapshot, &entry))
		{
			while (Process32Next(snapshot, &entry))
			{
				if (wcscmp(name, entry.szExeFile) == 0)
				{
					ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, false, entry.th32ProcessID);
					if (!ProcessHandle || ProcessHandle == INVALID_HANDLE_VALUE)
						ThrowWindowsError(FormatAddressAndHandle(DataManagerAddress, 0) + " Could not open MonsterHunterRise.exe");

					return;
				}
			}
		}

		ThrowWindowsError(FormatAddressAndHandle(DataManagerAddress, 0) + " Could not open MonsterHunterRise.exe");
	}
}

System::UIntPtr RisePCItemEditor::Window::GetDataManager()
{
	OpenGameProcessByExeName(L"MonsterHunterRise.exe");

	uintptr_t dataManager = NULL;
	DWORD old;

	if (!VirtualProtectEx(ProcessHandle, DataManagerAddress, sizeof(uintptr_t), PAGE_EXECUTE_READWRITE, &old))
		ThrowWindowsError(FormatAddressAndHandle(DataManagerAddress, ProcessHandle) + " VirtualProtectEx (1) failed with");

	if (!ReadProcessMemory(ProcessHandle, DataManagerAddress, &dataManager, sizeof(uintptr_t), NULL))
		ThrowWindowsError(FormatAddressAndHandle(DataManagerAddress, ProcessHandle) + " ReadProcessMemory failed with");

	if (!VirtualProtectEx(ProcessHandle, DataManagerAddress, sizeof(uintptr_t), old, &old))
		ThrowWindowsError(FormatAddressAndHandle(DataManagerAddress, ProcessHandle) + " VirtualProtectEx (2) failed with");

	return UIntPtr(dataManager);
}

#define RPM(addr, buffer, size) ReadProcessMemory(ProcessHandle, LPVOID(addr), buffer, size, NULL)
#define WPM(addr, buffer, size) WriteProcessMemory(ProcessHandle, LPVOID(addr), buffer, size, NULL)

System::String^ RisePCItemEditor::Window::FormatCharmName(CharmData^ charm)
{
	String^ format = "{0} | {1} | {2}-{3}-{4}";

	UInt32 slots[3] = { 0 };

	for (UInt32 i = 0; i < charm->Level3Slots; i++)
	{
		for (UInt32& slot : slots)
			if (!slot) { slot = 3; break; }
	}

	for (UInt32 i = 0; i < charm->Level2Slots; i++)
	{
		for (UInt32& slot : slots)
			if (!slot) { slot = 2; break; }
	}

	for (UInt32 i = 0; i < charm->Level1Slots; i++)
	{
		for (UInt32& slot : slots)
			if (!slot) { slot = 1; break; }
	}

	return String::Format(format, SkillNames[charm->Skill1], SkillNames[charm->Skill2], slots[0], slots[1], slots[2]);
}

System::UInt32 RisePCItemEditor::Window::GetFirstSlot(CharmData^ charm)
{
	UInt32 slots[3] = { 0 };

	for (UInt32 i = 0; i < charm->Level3Slots; i++)
	{
		for (UInt32& slot : slots)
			if (!slot) { slot = 3; break; }
	}

	for (UInt32 i = 0; i < charm->Level2Slots; i++)
	{
		for (UInt32& slot : slots)
			if (!slot) { slot = 2; break; }
	}

	for (UInt32 i = 0; i < charm->Level1Slots; i++)
	{
		for (UInt32& slot : slots)
			if (!slot) { slot = 1; break; }
	}

	return slots[0];
}

System::UInt32 RisePCItemEditor::Window::GetSecondSlot(CharmData^ charm)
{
	UInt32 slots[3] = { 0 };

	for (UInt32 i = 0; i < charm->Level3Slots; i++)
	{
		for (UInt32& slot : slots)
			if (!slot) { slot = 3; break; }
	}

	for (UInt32 i = 0; i < charm->Level2Slots; i++)
	{
		for (UInt32& slot : slots)
			if (!slot) { slot = 2; break; }
	}

	for (UInt32 i = 0; i < charm->Level1Slots; i++)
	{
		for (UInt32& slot : slots)
			if (!slot) { slot = 1; break; }
	}

	return slots[1];
}

System::UInt32 RisePCItemEditor::Window::GetThirdSlot(CharmData^ charm)
{
	UInt32 slots[3] = { 0 };

	for (UInt32 i = 0; i < charm->Level3Slots; i++)
	{
		for (UInt32& slot : slots)
			if (!slot) { slot = 3; break; }
	}

	for (UInt32 i = 0; i < charm->Level2Slots; i++)
	{
		for (UInt32& slot : slots)
			if (!slot) { slot = 2; break; }
	}

	for (UInt32 i = 0; i < charm->Level1Slots; i++)
	{
		for (UInt32& slot : slots)
			if (!slot) { slot = 1; break; }
	}

	return slots[2];
}

System::Void RisePCItemEditor::Window::WriteControlsToCharm(CharmData^ charm)
{
	charm->Rarity = Rarity(RarityList->SelectedIndex + 0x10100000);
	charm->Skill1 = Skill1->SelectedIndex;
	charm->Skill2 = Skill2->SelectedIndex;

	charm->Skill1Level = UInt32(Skill1Level->Value);
	charm->Skill2Level = UInt32(Skill2Level->Value);

	charm->Level1Slots = charm->Level2Slots = charm->Level3Slots = 0;

	switch (Slot1->SelectedIndex)
	{
	case 1: charm->Level1Slots++; break;
	case 2: charm->Level2Slots++; break;
	case 3: charm->Level3Slots++; break;
	default: break;
	}

	switch (Slot2->SelectedIndex)
	{
	case 1: charm->Level1Slots++; break;
	case 2: charm->Level2Slots++; break;
	case 3: charm->Level3Slots++; break;
	default: break;
	}

	switch (Slot3->SelectedIndex)
	{
	case 1: charm->Level1Slots++; break;
	case 2: charm->Level2Slots++; break;
	case 3: charm->Level3Slots++; break;
	default: break;
	}
}

System::Void RisePCItemEditor::Window::WriteCharmToGame(CharmData^ charm)
{
	uintptr_t dataManager = GetDataManager().ToUInt64();

	uintptr_t eqBox = NULL;
	ReadProcessMemory(ProcessHandle, LPVOID(dataManager + Offsets::EquipmentBox), &eqBox, sizeof(eqBox), NULL);

	uintptr_t eqList = NULL;
	UInt32 size = 0;
	ReadProcessMemory(ProcessHandle, LPVOID(eqBox + Offsets::EquipmentList), &eqList, sizeof(eqList), NULL);
	ReadProcessMemory(ProcessHandle, LPVOID(eqList + Offsets::EquipmentSize), &size, sizeof(size), NULL); // mSize
	ReadProcessMemory(ProcessHandle, LPVOID(eqList + Offsets::EquipmentItems), &eqList, sizeof(eqList), NULL); // mItems

	eqList += Offsets::EquipmentList + charm->Index * 0x8;
	{
		uintptr_t item = NULL;
		ReadProcessMemory(ProcessHandle, LPVOID(eqList), &item, sizeof(item), NULL);

		if (item)
		{
			EquipmentType eqType = EquipmentType::Empty;
			ReadProcessMemory(ProcessHandle, LPVOID(item + OFFSET_EQ_TYPE), &eqType, sizeof(eqType), NULL);

			UInt32 value = UInt32(charm->Rarity);
			WPM(item + OFFSET_RARITY, &value, sizeof(UInt32));

			value = UInt32(EquipmentType::Talisman);
			WPM(item + OFFSET_EQ_TYPE, &value, sizeof(UInt32));

			uintptr_t subptr = 0;
			RPM(item + OFFSET_SLOTS, &subptr, sizeof(uintptr_t));

			value = charm->Level1Slots;
			WPM(subptr + OFFSET_LEVEL1_SLOTS, &value, sizeof(UInt32));

			value = charm->Level2Slots;
			WPM(subptr + OFFSET_LEVEL2_SLOTS, &value, sizeof(UInt32));

			value = charm->Level3Slots;
			WPM(subptr + OFFSET_LEVEL3_SLOTS, &value, sizeof(UInt32));

			//value = 0x4;
			//WPM(item + OFFSET_IDENTIFIER, &value, sizeof(UInt32));

			RPM(item + OFFSET_SKILLS, &subptr, sizeof(uintptr_t));

			value = charm->Skill1;
			WPM(subptr + OFFSET_SKILL1_ID, &value, sizeof(UINT8));
				
			value = charm->Skill2;
			WPM(subptr + OFFSET_SKILL2_ID, &value, sizeof(UINT8));

			RPM(item + OFFSET_LEVELS, &subptr, sizeof(uintptr_t));

			value = charm->Skill1Level;
			WPM(subptr + OFFSET_SKILL1_LVL, &value, sizeof(UInt32));
				
			value = charm->Skill2Level;
			WPM(subptr + OFFSET_SKILL2_LVL, &value, sizeof(UInt32));
		}
	}
}

System::Void RisePCItemEditor::Window::CollectItems()
{
	if (!Itembox) Itembox = gcnew Generic::List<ItemData^>(0);

	Itembox->Clear();
	ItemboxCtrl->Rows->Clear();

	auto dataMgr = GetDataManager().ToUInt64();

	uintptr_t itemBox = NULL;
	ReadProcessMemory(ProcessHandle, LPVOID(dataMgr + Offsets::ItemBox), &itemBox, sizeof(uintptr_t), NULL);

	uintptr_t inventoryList = NULL;
	ReadProcessMemory(ProcessHandle, LPVOID(itemBox + Offsets::InventoryList), &inventoryList, sizeof(uintptr_t), NULL);

	uintptr_t items = NULL;
	ReadProcessMemory(ProcessHandle, LPVOID(inventoryList + Offsets::Items), &items, sizeof(uintptr_t), NULL);
	items += Offsets::ItemInfo;

	UINT32 size = 0;
	ReadProcessMemory(ProcessHandle, LPVOID(inventoryList + InventorySize), &size, sizeof(UINT32), NULL);

	for (UInt32 i = 0; i < size; i++, items += 0x8)
	{
		uintptr_t item = NULL;
		ReadProcessMemory(ProcessHandle, LPVOID(items), &item, sizeof(uintptr_t), NULL);

		uintptr_t itemData = NULL;
		ReadProcessMemory(ProcessHandle, LPVOID(item + Offsets::ItemInfo), &itemData, sizeof(uintptr_t), NULL);

		if (itemData)
		{
			UInt32 id, count;
			
			RPM(itemData + Offsets::ItemInfo, &id, sizeof(id));
			RPM(itemData + Offsets::ItemCount, &count, sizeof(count));

			Itembox->Add(gcnew ItemData(id, count));

			UInt32 adjusted = (id < 0x4100000 ? 0x4000000 : id - 0x4100000);
			ItemboxCtrl->Rows->Add(adjusted.ToString(), ItemNames[id], count.ToString());
		}
	}
}

System::Void RisePCItemEditor::Window::WriteItems()
{
	auto dataMgr = GetDataManager().ToUInt64();

	uintptr_t itemBox = NULL;
	ReadProcessMemory(ProcessHandle, LPVOID(dataMgr + Offsets::ItemBox), &itemBox, sizeof(uintptr_t), NULL);

	uintptr_t inventoryList = NULL;
	ReadProcessMemory(ProcessHandle, LPVOID(itemBox + Offsets::InventoryList), &inventoryList, sizeof(uintptr_t), NULL);

	uintptr_t items = NULL;
	ReadProcessMemory(ProcessHandle, LPVOID(inventoryList + Offsets::Items), &items, sizeof(uintptr_t), NULL);
	items += Offsets::ItemInfo;

	UINT32 size = 0;
	ReadProcessMemory(ProcessHandle, LPVOID(inventoryList + Offsets::InventorySize), &size, sizeof(UINT32), NULL);

	for (UInt32 i = 0; i < size; i++, items += 0x8)
	{
		uintptr_t item = NULL;
		ReadProcessMemory(ProcessHandle, LPVOID(items), &item, sizeof(uintptr_t), NULL);

		uintptr_t itemData = NULL;
		ReadProcessMemory(ProcessHandle, LPVOID(item + Offsets::ItemInfo), &itemData, sizeof(uintptr_t), NULL);

		if (itemData)
		{
			UInt32 id = Itembox[i]->ID, count = Itembox[i]->Amount;

			WPM(itemData + Offsets::ItemID, &id, sizeof(id));
			WPM(itemData + Offsets::ItemCount, &count, sizeof(count));
		}
	}
}

System::Void RisePCItemEditor::Window::PopulateItembox()
{
	ItemboxCtrl->Rows->Clear();

	for (int i = 0; i < Itembox->Count; i++)
	{
		ItemData^ item = Itembox[i];
		ItemboxCtrl->Rows->Add((item->ID - 0x4100000).ToString(), ItemNames[item->ID], item->Amount.ToString());
	}
}

System::Void RisePCItemEditor::Window::ApplyItembox()
{
	Itembox->Clear();

	for (int i = 0; i < ItemboxCtrl->Rows->Count; i++)
	{
		DataGridViewRow^ row = ItemboxCtrl->Rows[i];
		
		UInt32 id = UInt32::Parse((String^)row->Cells[0]->Value);
		if (id < 0x4000000) id += 0x4100000;

		UInt32 amount = UInt32::Parse((String^)row->Cells[2]->Value);
		Itembox->Add(gcnew ItemData(id, amount));
	}
}

System::Void RisePCItemEditor::Window::InitItemList(String^ langname)
{
	if (!ItemNames)
		ItemNames = gcnew Generic::Dictionary<UInt32, String^>(0);

	String^ path = "./lang/items_" + langname + ".csv";
	if (!IO::File::Exists(path))
	{
		MessageBox::Show("File: '" + path + "' could not be found.", "Error");
		return;
	}

	ItemNames->Clear();

	Generic::List<String^>^ source = gcnew Generic::List<String^>(0);
	IO::FileStream^ stream = IO::File::OpenRead(path);
	IO::StreamReader^ reader = gcnew IO::StreamReader(stream, System::Text::Encoding::UTF8);

	String^ line = "";
	while (line = reader->ReadLine())
	{
		array<String^>^ items = line->Split(gcnew array<wchar_t>(1) { ',' });

		int id = int::Parse(items[0]);
		String^ name = (items[2] == "N/A" ? items[1] : items[2]);
		ItemNames->Add(id, name);
		source->Add(name);
	}

	reader->Close();
	Column_Item->DataSource = source;
}

System::String^ RisePCItemEditor::Window::FormatCharmForFile(CharmData^ charm)
{
	return String::Format("{0},{1},{2},{3},{4},{5},{6},{7}\n",
		SkillNames[charm->Skill1], charm->Skill1Level,
		SkillNames[charm->Skill2], charm->Skill2Level,
		GetFirstSlot(charm), GetSecondSlot(charm), GetThirdSlot(charm),
		int(charm->Rarity)
	);
}

using namespace System::Collections;
Generic::List<CharmData^>^ RisePCItemEditor::Window::GetCharmsFromEquipmentBox()
{
	auto list = gcnew Generic::List<CharmData^>(0);
	uintptr_t dataManager = GetDataManager().ToUInt64();

	uintptr_t eqBox = NULL;
	ReadProcessMemory(ProcessHandle, LPVOID(dataManager + Offsets::EquipmentBox), &eqBox, sizeof(eqBox), NULL);

	uintptr_t eqList = NULL;
	UInt32 size = 0;
	ReadProcessMemory(ProcessHandle, LPVOID(eqBox + Offsets::EquipmentList), &eqList, sizeof(eqList), NULL);
	ReadProcessMemory(ProcessHandle, LPVOID(eqList + Offsets::EquipmentSize), &size, sizeof(size), NULL); // mSize
	ReadProcessMemory(ProcessHandle, LPVOID(eqList + Offsets::EquipmentItems), &eqList, sizeof(eqList), NULL); // mItems

	eqList += Offsets::EquipmentList;
	for (UInt32 i = 0; i < size; ++i, eqList += 0x8)
	{
		uintptr_t item = NULL;
		ReadProcessMemory(ProcessHandle, LPVOID(eqList), &item, sizeof(item), NULL);

		if (item)
		{
			EquipmentType eqType = EquipmentType::Empty;
			ReadProcessMemory(ProcessHandle, LPVOID(item + OFFSET_EQ_TYPE), &eqType, sizeof(eqType), NULL);

			if (eqType == EquipmentType::Talisman)
			{
				CharmData^ charm = gcnew CharmData();

				UInt32 value = 0;
				RPM(item + OFFSET_RARITY, &value, sizeof(UInt32));

				charm->Index = i;
				charm->Rarity = Rarity(value);

				uintptr_t subptr = 0;
				RPM(item + OFFSET_SLOTS, &subptr, sizeof(uintptr_t));

				RPM(subptr + OFFSET_LEVEL1_SLOTS, &value, sizeof(UInt32));
				charm->Level1Slots = value;
				RPM(subptr + OFFSET_LEVEL2_SLOTS, &value, sizeof(UInt32));
				charm->Level2Slots = value;
				RPM(subptr + OFFSET_LEVEL3_SLOTS, &value, sizeof(UInt32));
				charm->Level3Slots = value;

				RPM(item + OFFSET_SKILLS, &subptr, sizeof(uintptr_t));

				RPM(subptr + OFFSET_SKILL1_ID, &value, sizeof(UINT8));
				charm->Skill1 = value & 0xFF; // 1 byte value
				RPM(subptr + OFFSET_SKILL2_ID, &value, sizeof(UINT8));
				charm->Skill2 = value & 0xFF;

				RPM(item + OFFSET_LEVELS, &subptr, sizeof(uintptr_t));

				RPM(subptr + OFFSET_SKILL1_LVL, &value, sizeof(UInt32));
				charm->Skill1Level = value;
				RPM(subptr + OFFSET_SKILL2_LVL, &value, sizeof(UInt32));
				charm->Skill2Level = value;

				list->Add(charm);
			}
		}
	}

	return list;
}

System::Void RisePCItemEditor::Window::ExportCharms_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (Charms->Count == 0) return;

	SaveFileDialog^ dlg = gcnew SaveFileDialog();
	dlg->AddExtension = true;
	dlg->Title = "Save Charms";
	dlg->CheckFileExists = false;
	dlg->Filter = "Text Files | *.txt";
	dlg->DefaultExt = "txt";

	if (dlg->ShowDialog(this) == Windows::Forms::DialogResult::OK)
	{
		IO::FileStream^ stream = IO::File::OpenWrite(dlg->FileName);
		IO::StreamWriter^ writer = gcnew IO::StreamWriter(stream, System::Text::Encoding::UTF8);

		for (int i = 0; i < Charms->Count; i++)
		{
			writer->Write(FormatCharmForFile(Charms[i]));
		}

		writer->Flush();
		stream->Close();
	}
}

System::Void RisePCItemEditor::Window::ExportItembox_Click(System::Object^ sender, System::EventArgs^ e)
{

}

System::Void RisePCItemEditor::Window::ImportCharms_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (!CharmList->Enabled)
		MessageBox::Show("Make sure to click on 'Read Equipment Box' first. Otherwise your imported charms will be overwritten");

	OpenFileDialog^ dlg = gcnew OpenFileDialog();
	dlg->AddExtension = true;
	dlg->CheckFileExists = true;
	dlg->CheckPathExists = true;
	dlg->DefaultExt = "txt";
	dlg->Filter = "Text Files | *.txt";
	dlg->Title = "Import Charms";

	if (dlg->ShowDialog(this) == Windows::Forms::DialogResult::OK)
	{
		IO::FileStream^ stream = IO::File::OpenRead(dlg->FileName);
		IO::StreamReader^ reader = gcnew IO::StreamReader(stream, System::Text::Encoding::UTF8);

		String^ line = "";
		while (line = reader->ReadLine())
		{
			array<String^>^ items = line->Split(gcnew array<wchar_t>(1) { ',' });
			
			CharmData^ charm = gcnew CharmData();
			int n = 0;
			if (int::TryParse(items[0], n)) // Skills by IDs
			{
				charm->Skill1 = UInt32::Parse(items[0]);
				charm->Skill2 = UInt32::Parse(items[2]);
			}
			else // Skills by Names
			{
				for (int i = 0; i < SkillNames->Count; i++)
				{
					if (items[0] == SkillNames[i])
						charm->Skill1 = i;
					if (items[2] == SkillNames[i])
						charm->Skill2 = i;
				}
			}

			charm->Skill1Level = UInt32::Parse(items[1]);
			charm->Skill2Level = UInt32::Parse(items[3]);
			charm->Index = FreeEqBoxIndices[0];
			FreeEqBoxIndices->RemoveAt(0);

			int slot1 = int::Parse(items[4]);
			int slot2 = int::Parse(items[5]);
			int slot3 = int::Parse(items[6]);

			switch (slot1)
			{
			case 1: charm->Level1Slots++; break;
			case 2: charm->Level2Slots++; break;
			case 3: charm->Level3Slots++; break;
			default: break;
			}

			switch (slot2)
			{
			case 1: charm->Level1Slots++; break;
			case 2: charm->Level2Slots++; break;
			case 3: charm->Level3Slots++; break;
			default: break;
			}

			switch (slot3)
			{
			case 1: charm->Level1Slots++; break;
			case 2: charm->Level2Slots++; break;
			case 3: charm->Level3Slots++; break;
			default: break;
			}

			if (items->Length > 7)
				charm->Rarity = Rarity(int::Parse(items[7]));
			else
				charm->Rarity = Rarity::Rarity7;

			Charms->Add(charm);
			CharmList->Items->Add(FormatCharmName(charm));
			WriteCharmToGame(charm);
		}

		stream->Close();
	}
}

System::Void RisePCItemEditor::Window::ImportItembox_Click(System::Object^ sender, System::EventArgs^ e)
{

}

System::Void RisePCItemEditor::Window::RawCharms_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (MessageBox::Show("Only use this if you know what you are doing and you feel it's necessary.\nDo you want to continue?",
		"Raw Charm Editor",
		MessageBoxButtons::YesNo) != Windows::Forms::DialogResult::Yes)
		return;

	RawCharmEditor^ editor = gcnew RawCharmEditor(GetCharmsFromEquipmentBox());
	editor->ShowDialog(this);

	auto charms = editor->GetReturn();
	Charms->Clear();

	for (int i = 0; i < charms->Count; i++)
	{
		WriteCharmToGame(charms[i]);
	}
}

#include <vcclr.h>

System::Void RisePCItemEditor::Window::ChangeMenuLanguage(String^ langname)
{
	String^ path = "./lang/" + langname + ".ini";
	pin_ptr<const wchar_t> str = PtrToStringChars(path);
	IniFile^ ini = gcnew IniFile(str);

	Label_ItemModifer->Text = ini->Read(L"Label_ItemModifier");
	Label_Amount->Text = ini->Read(L"Label_Amount");
	Label_CharmModifier->Text = ini->Read(L"Label_CharmModifier");
	Label_Charms->Text = ini->Read(L"Label_Charms");
	Label_Rarity->Text = ini->Read(L"Label_Rarity");
	Label_Skill1->Text = ini->Read(L"Label_Skill1");
	Label_Skill2->Text = ini->Read(L"Label_Skill2");
	Label_LevelSkill1->Text = ini->Read(L"Label_Level");
	Label_LevelSkill2->Text = ini->Read(L"Label_Level");
	Label_Slot1->Text = ini->Read(L"Label_Slot1");
	Label_Slot2->Text = ini->Read(L"Label_Slot2");
	Label_Slot3->Text = ini->Read(L"Label_Slot3");
	Label_Other->Text = ini->Read(L"Label_Other");
	Label_Zenny->Text = ini->Read(L"Label_Zenny");
	Label_Points->Text = ini->Read(L"Label_Points");
	Tab_ItemboxEditor->Text = ini->Read(L"Label_ItemboxEditor");

	FileMenu->Text = ini->Read(L"Tab_File");
	LanguageMenu->Text = ini->Read(L"Tab_Language");
	ExportCharms->Text = ini->Read(L"TabItem_ExportCharms");
	ExportItembox->Text = ini->Read(L"TabItem_ExportItembox");
	ImportCharms->Text = ini->Read(L"TabItem_ImportCharms");
	ImportItembox->Text = ini->Read(L"TabItem_ImportItembox");

	RunScript->Text = ini->Read(L"Button_RunScript");
	ClearAllItems->Text = ini->Read(L"Button_ClearAll");
	ReadEquipmentBox->Text = ini->Read(L"Button_ReadEquipBox");
	AddNewCharm->Text = ini->Read(L"Button_AddNew");
	SaveButton->Text = ini->Read(L"Button_Save");
	SetZenny->Text = ini->Read(L"Button_SetZenny");
	SetPoints->Text = ini->Read(L"Button_SetPoints");
	ReadItemBox->Text = ini->Read(L"Button_ReadItembox");
	SaveItemBox->Text = ini->Read(L"Button_SaveItembox");

	AddAllItems->Text = ini->Read(L"Checkbox_AddAllItems");
	IncludeIllegalItems->Text = ini->Read(L"Checkbox_IllegalItems");

	RarityList->Items->Clear();
	RarityList->Items->Add(ini->Read(L"Rarity1"));
	RarityList->Items->Add(ini->Read(L"Rarity2"));
	RarityList->Items->Add(ini->Read(L"Rarity3"));
	RarityList->Items->Add(ini->Read(L"Rarity4"));
	RarityList->Items->Add(ini->Read(L"Rarity5"));
	RarityList->Items->Add(ini->Read(L"Rarity6"));
	RarityList->Items->Add(ini->Read(L"Rarity7"));
	RarityList->Items->Add(ini->Read(L"Rarity4Novice")); // DO NOT CHANGE ORDER
	RarityList->Items->Add(ini->Read(L"Rarity3Kinship"));
	RarityList->Items->Add(ini->Read(L"Rarity12"));
	RarityList->Items->Add(ini->Read(L"Rarity2Legacy"));

	Slot1->Items->Clear();
	Slot2->Items->Clear();
	Slot3->Items->Clear();

	String^ none = ini->Read(L"SlotLevel_None"),
		^level1 = ini->Read(L"SlotLevel1"),
		^level2 = ini->Read(L"SlotLevel2"),
		^level3 = ini->Read(L"SlotLevel3");

	Slot1->Items->Add(none);
	Slot2->Items->Add(none);
	Slot3->Items->Add(none);
	Slot1->Items->Add(level1);
	Slot2->Items->Add(level1);
	Slot3->Items->Add(level1);
	Slot1->Items->Add(level2);
	Slot2->Items->Add(level2);
	Slot3->Items->Add(level2);
	Slot1->Items->Add(level3);
	Slot2->Items->Add(level3);
	Slot3->Items->Add(level3);

	Column_ID->HeaderText = ini->Read(L"Tab_ID");
	Column_Item->HeaderText = ini->Read(L"Tab_Item");
	Column_Amount->HeaderText = ini->Read(L"Tab_Amount");

	InitItemList(langname);
	InitSkillNames(langname);
}

System::Void RisePCItemEditor::Window::InitSkillNames(String^ langname)
{
	String^ path = "./lang/skills_" + langname + ".csv";
	if (!IO::File::Exists(path))
	{
		MessageBox::Show("File: '" + path + "' could not be found.", "Error");
		return;
	}

	SkillNames->Clear();
	Skill1->Items->Clear();
	Skill2->Items->Clear();

	IO::FileStream^ stream = IO::File::OpenRead(path);
	IO::StreamReader^ reader = gcnew IO::StreamReader(stream, System::Text::Encoding::UTF8);

	String^ line = "";
	while (line = reader->ReadLine())
	{
		array<String^>^ items = line->Split(gcnew array<wchar_t>(1) { ',' });
		SkillNames->Add(items[1]);
		Skill1->Items->Add(items[1]);
		Skill2->Items->Add(items[1]);
	}

	reader->Close();
}

System::Void RisePCItemEditor::Window::ItemboxCtrl_EditingControlShowing(System::Object^ sender, DataGridViewEditingControlShowingEventArgs^ e)
{
	e->Control->KeyPress -= gcnew KeyPressEventHandler(this, &Window::NumericTextBox_KeyPress);

	if (ItemboxCtrl->CurrentCell->ColumnIndex == 0 || ItemboxCtrl->CurrentCell->ColumnIndex == 2)
	{
		TextBox^ tb = (TextBox^)e->Control;
		if (tb) tb->KeyPress += gcnew KeyPressEventHandler(this, &Window::NumericTextBox_KeyPress);
	}

	try {
		if (ItemboxCtrl->CurrentCell->ColumnIndex == 0)
		{
			TextBox^ tb = (TextBox^)e->Control;
			if (tb) tb->TextChanged += gcnew EventHandler(this, &Window::NumericTextBox_ValueChanged);
		}

		if (ItemboxCtrl->CurrentCell->ColumnIndex == 1)
		{
			ComboBox^ cb = (ComboBox^)e->Control;
			if (cb) cb->SelectedIndexChanged += gcnew EventHandler(this, &Window::Dropdown_SelectedIndexChanged);
		}
	}
	catch (Exception^ e) {
		// Ignore lol
	}
}

System::Void RisePCItemEditor::Window::ReadItemBox_Click(System::Object^ sender, System::EventArgs^ e)
{
	CollectItems();
	// PopulateItembox(); // Is automatically done in CollectItems()
}

System::Void RisePCItemEditor::Window::SaveItemBox_Click(System::Object^ sender, System::EventArgs^ e)
{
	ApplyItembox();
	WriteItems();
}

System::Void RisePCItemEditor::Window::SetZenny_Click(System::Object^ sender, System::EventArgs^ e)
{
	uintptr_t dataManager = GetDataManager().ToUInt64();

	uintptr_t handMoney = NULL;
	RPM(dataManager + Offsets::HandMoney, &handMoney, sizeof(handMoney));

	UInt32 value = Decimal::ToUInt32(ZennyAmount->Value);
	WPM(handMoney + Offsets::Zenny, &value, sizeof(value));
}

System::Void RisePCItemEditor::Window::SetPoints_Click(System::Object^ sender, System::EventArgs^ e)
{
	uintptr_t dataManager = GetDataManager().ToUInt64();

	uintptr_t villagePoints = NULL;
	RPM(dataManager + Offsets::VillagePoints, &villagePoints, sizeof(villagePoints));

	UInt32 value = Decimal::ToUInt32(PointsAmount->Value);
	WPM(villagePoints + Offsets::Points, &value, sizeof(value));
}

System::Void RisePCItemEditor::Window::InitLanguages()
{
	LanguageMenu->MenuItems->Clear();

	array<String^>^ files = IO::Directory::GetFiles("./lang", "*.ini", IO::SearchOption::TopDirectoryOnly);
	for (int i = 0; i < files->Length; i++)
	{
		String^ name = IO::Path::GetFileNameWithoutExtension(files[i]);
		LanguageMenu->MenuItems->Add(gcnew MenuItem(name, gcnew System::EventHandler(this, &Window::Language_Click)));
	}
}

System::Void RisePCItemEditor::Window::Language_Click(Object^ sender, EventArgs^ e)
{
	MenuItem^ item = (MenuItem^)sender;
	ChangeMenuLanguage(item->Text);
}

System::Void RisePCItemEditor::Window::NumericTextBox_KeyPress(System::Object^ sender, KeyPressEventArgs^ e)
{
	if (!Char::IsControl(e->KeyChar) && !Char::IsDigit(e->KeyChar))
	{
		e->Handled = true;
	}
}

System::Void RisePCItemEditor::Window::NumericTextBox_ValueChanged(System::Object^ sender, System::EventArgs^ e)
{
	TextBox^ tb = (TextBox^)sender;
	UInt32 id = UInt32::Parse(tb->Text) + 0x4100000;
	
	DataGridViewCell^ cell = ItemboxCtrl->CurrentCell;
	ItemboxCtrl->Rows[cell->RowIndex]->Cells[1]->Value = ItemNames[id];
}

System::Void RisePCItemEditor::Window::Dropdown_SelectedIndexChanged(System::Object^ sender, EventArgs^ e)
{
	ComboBox^ cb = (ComboBox^)sender;
	Int32 id = cb->SelectedIndex;

	if (id == 0) id = 0x4000000;
	else if (id == 1) id = 0x4000001;
	else id -= 2;

	ItemboxCtrl->Rows[ItemboxCtrl->CurrentCell->RowIndex]->Cells[0]->Value = id.ToString();
}

System::Void RisePCItemEditor::Window::RunScript_Click(System::Object^ sender, System::EventArgs^ e)
{
	UINT32 amount = UINT32(ItemCount->Value);
	uintptr_t dataManager = GetDataManager().ToUInt64();
	AddItemsFunc(dataManager, amount, false);
}

System::Void RisePCItemEditor::Window::ClearAllItems_Click(System::Object^ sender, System::EventArgs^ e)
{
	uintptr_t dataManager = GetDataManager().ToUInt64();
	AddItemsFunc(dataManager, 0, true);
}


System::Void RisePCItemEditor::Window::ReadEquipmentBox_Click(System::Object^ sender, System::EventArgs^ e)
{
	Charms->Clear();
	CharmList->Items->Clear();
	FreeEqBoxIndices->Clear();

	uintptr_t dataManager = GetDataManager().ToUInt64();

	uintptr_t eqBox = NULL;
	ReadProcessMemory(ProcessHandle, LPVOID(dataManager + Offsets::EquipmentBox), &eqBox, sizeof(eqBox), NULL);

	uintptr_t eqList = NULL;
	UInt32 size = 0;
	ReadProcessMemory(ProcessHandle, LPVOID(eqBox + Offsets::EquipmentList), &eqList, sizeof(eqList), NULL);
	ReadProcessMemory(ProcessHandle, LPVOID(eqList + Offsets::EquipmentSize), &size, sizeof(size), NULL); // mSize
	ReadProcessMemory(ProcessHandle, LPVOID(eqList + Offsets::EquipmentItems), &eqList, sizeof(eqList), NULL); // mItems

	eqList += Offsets::EquipmentList;
	for (UInt32 i = 0; i < size; ++i, eqList += 0x8)
	{
		uintptr_t item = NULL;
		ReadProcessMemory(ProcessHandle, LPVOID(eqList), &item, sizeof(item), NULL);

		if (item)
		{
			EquipmentType eqType = EquipmentType::Empty;
			ReadProcessMemory(ProcessHandle, LPVOID(item + OFFSET_EQ_TYPE), &eqType, sizeof(eqType), NULL);

			if (eqType == EquipmentType::Talisman)
			{
				CharmData^ charm = gcnew CharmData();

				UInt32 value = 0;
				RPM(item + OFFSET_RARITY, &value, sizeof(UInt32));

				charm->Index = i;
				charm->Rarity = Rarity(value);

				uintptr_t subptr = 0;
				RPM(item + OFFSET_SLOTS, &subptr, sizeof(uintptr_t));

				RPM(subptr + OFFSET_LEVEL1_SLOTS, &value, sizeof(UInt32));
				charm->Level1Slots = value;
				RPM(subptr + OFFSET_LEVEL2_SLOTS, &value, sizeof(UInt32));
				charm->Level2Slots = value;
				RPM(subptr + OFFSET_LEVEL3_SLOTS, &value, sizeof(UInt32));
				charm->Level3Slots = value;

				RPM(item + OFFSET_SKILLS, &subptr, sizeof(uintptr_t));

				RPM(subptr + OFFSET_SKILL1_ID, &value, sizeof(UINT8));
				charm->Skill1 = value & 0xFF; // 1 byte value
				RPM(subptr + OFFSET_SKILL2_ID, &value, sizeof(UINT8));
				charm->Skill2 = value & 0xFF;

				RPM(item + OFFSET_LEVELS, &subptr, sizeof(uintptr_t));

				RPM(subptr + OFFSET_SKILL1_LVL, &value, sizeof(UInt32));
				charm->Skill1Level = value;
				RPM(subptr + OFFSET_SKILL2_LVL, &value, sizeof(UInt32));
				charm->Skill2Level = value;

				Charms->Add(charm);
				CharmList->Items->Add(FormatCharmName(charm));
			}
			else if (eqType == EquipmentType::Empty)
			{
				FreeEqBoxIndices->Add(i);
			}
		}
	}

	Skill1->Enabled = true;
	Skill2->Enabled = true;
	Skill1Level->Enabled = true;
	Skill2Level->Enabled = true;
	Slot1->Enabled = true;
	Slot2->Enabled = true;
	Slot3->Enabled = true;
	SaveButton->Enabled = true;
	AddNewCharm->Enabled = true;
	RarityList->Enabled = true;
	CharmList->Enabled = true;

	Label_CharmModifier->Enabled = true;
	Label_Charms->Enabled = true;
	Label_Rarity->Enabled = true;
	Label_Skill1->Enabled = true;
	Label_Skill2->Enabled = true;
	Label_LevelSkill1->Enabled = true;
	Label_LevelSkill2->Enabled = true;
	Label_Slot1->Enabled = true;
	Label_Slot2->Enabled = true;
	Label_Slot3->Enabled = true;
}

System::Void RisePCItemEditor::Window::SaveButton_Click(System::Object^ sender, System::EventArgs^ e)
{
	UInt32 index = CharmList->SelectedIndex;
	CharmData^ charm = Charms[index];

	WriteControlsToCharm(charm);
	WriteCharmToGame(charm);
}

System::Void RisePCItemEditor::Window::CharmList_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e)
{
	CharmData^ charm = Charms[CharmList->SelectedIndex];

	Skill1->SelectedIndex = charm->Skill1;
	Skill2->SelectedIndex = charm->Skill2;

	Skill1Level->Value = charm->Skill1Level;
	Skill2Level->Value = charm->Skill2Level;

	Slot1->SelectedIndex = GetFirstSlot(charm);
	Slot2->SelectedIndex = GetSecondSlot(charm);
	Slot3->SelectedIndex = GetThirdSlot(charm);

	if (charm->Rarity == Rarity(0)) charm->Rarity = Rarity::Rarity7;
	RarityList->SelectedIndex = int(charm->Rarity) - 0x10100000;

	this->Invalidate();
}

System::Void RisePCItemEditor::Window::AddNewCharm_Click(System::Object^ sender, System::EventArgs^ e)
{
	CharmData^ charm = gcnew CharmData();
	
	charm->Index = FreeEqBoxIndices[0];
	FreeEqBoxIndices->RemoveAt(0);

	charm->Skill1 = 0;
	charm->Skill2 = 0;

	charm->Skill1Level = 0;
	charm->Skill2Level = 0;

	charm->Level1Slots = 0;
	charm->Level2Slots = 0;
	charm->Level3Slots = 0;

	charm->Rarity = Rarity::Rarity1;

	WriteCharmToGame(charm);

	Charms->Add(charm);
	CharmList->Items->Add(FormatCharmName(charm));

}

#pragma once

#include <Windows.h>

#include "Data.h"

namespace RisePCItemEditor {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Window
	/// </summary>
	public ref class Window : public System::Windows::Forms::Form
	{
	public:
		Window()
		{
			InitializeComponent();
			
			ProcessHandle = nullptr;
			ExcludeList = gcnew Generic::List<UINT32>(0);
			Charms = gcnew Generic::List<CharmData^>(0);
			SkillNames = gcnew Generic::List<String^>(112);
			FreeEqBoxIndices = gcnew Generic::List<UInt32>(0);

			ExcludeList->Add(0);
			ExcludeList->Add(10);
			ExcludeList->Add(18);
			ExcludeList->Add(135);
			ExcludeList->Add(136);
			ExcludeList->Add(164);
			ExcludeList->Add(530);
			ExcludeList->Add(1044);
			ExcludeList->Add(1057);
			ExcludeList->Add(1058);
			ExcludeList->Add(1059);
			ExcludeList->Add(1060);
			ExcludeList->Add(1061);
			ExcludeList->Add(1063);
			ExcludeList->Add(1064);
			ExcludeList->Add(646);
			ExcludeList->Add(647);
			ExcludeList->Add(660);

			InitItemList("English");
			InitSkillNames("English");
			InitLanguages();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Window()
		{
			if (components)
			{
				delete components;
			}
		}
private: System::Windows::Forms::Label^ Label_ItemModifer;
protected:
	protected:

	private: System::Windows::Forms::CheckBox^ AddAllItems;
	private: System::Windows::Forms::NumericUpDown^ ItemCount;
private: System::Windows::Forms::Label^ Label_Amount;


	private: System::Windows::Forms::Button^ RunScript;
private: System::Windows::Forms::Label^ Label_CharmModifier;



	private: System::Windows::Forms::CheckBox^ IncludeIllegalItems;
	private: System::Windows::Forms::Button^ ClearAllItems;
	private: System::Windows::Forms::ComboBox^ CharmList;
private: System::Windows::Forms::Label^ Label_Charms;

	private: System::Windows::Forms::ComboBox^ RarityList;
private: System::Windows::Forms::Label^ Label_Rarity;

	private: System::Windows::Forms::ComboBox^ Skill1;
	private: System::Windows::Forms::ComboBox^ Skill2;
private: System::Windows::Forms::Label^ Label_Skill1;
private: System::Windows::Forms::Label^ Label_Skill2;


	private: System::Windows::Forms::NumericUpDown^ Skill1Level;
	private: System::Windows::Forms::NumericUpDown^ Skill2Level;
private: System::Windows::Forms::Label^ Label_LevelSkill1;
private: System::Windows::Forms::Label^ Label_LevelSkill2;




	private: System::Windows::Forms::Button^ AddNewCharm;
	private: System::Windows::Forms::ComboBox^ Slot1;
	private: System::Windows::Forms::ComboBox^ Slot2;
	private: System::Windows::Forms::ComboBox^ Slot3;
private: System::Windows::Forms::Label^ Label_Slot1;
private: System::Windows::Forms::Label^ Label_Slot2;
private: System::Windows::Forms::Label^ Label_Slot3;




	private: System::Windows::Forms::ToolTip^ Tooltip;
	private: System::Windows::Forms::Button^ SaveButton;
	private: System::Windows::Forms::Button^ ReadEquipmentBox;
private: System::Windows::Forms::Label^ Label_Other;

private: System::Windows::Forms::NumericUpDown^ ZennyAmount;
private: System::Windows::Forms::Label^ Label_Zenny;


private: System::Windows::Forms::NumericUpDown^ PointsAmount;
private: System::Windows::Forms::Label^ Label_Points;


private: System::Windows::Forms::Button^ SetZenny;
private: System::Windows::Forms::Button^ SetPoints;
private: System::Windows::Forms::Label^ label16;
private: System::Windows::Forms::TabControl^ tabControl1;
private: System::Windows::Forms::TabPage^ Tab_ItemboxEditor;
private: System::Windows::Forms::DataGridView^ ItemboxCtrl;



private: System::Windows::Forms::Button^ SaveItemBox;

private: System::Windows::Forms::Button^ ReadItemBox;
private: System::Windows::Forms::DataGridViewTextBoxColumn^ Column_ID;
private: System::Windows::Forms::DataGridViewComboBoxColumn^ Column_Item;
private: System::Windows::Forms::DataGridViewTextBoxColumn^ Column_Amount;
private: System::Windows::Forms::MainMenu^ mainMenu1;
private: System::Windows::Forms::MenuItem^ FileMenu;
private: System::Windows::Forms::MenuItem^ ExportCharms;
private: System::Windows::Forms::MenuItem^ ExportItembox;
private: System::Windows::Forms::MenuItem^ ImportCharms;
private: System::Windows::Forms::MenuItem^ ImportItembox;
private: System::Windows::Forms::MenuItem^ LanguageMenu;
private: System::Windows::Forms::Button^ RawCharms;






















	private: System::ComponentModel::IContainer^ components;





	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(Window::typeid));
			this->Label_ItemModifer = (gcnew System::Windows::Forms::Label());
			this->AddAllItems = (gcnew System::Windows::Forms::CheckBox());
			this->ItemCount = (gcnew System::Windows::Forms::NumericUpDown());
			this->Label_Amount = (gcnew System::Windows::Forms::Label());
			this->RunScript = (gcnew System::Windows::Forms::Button());
			this->Label_CharmModifier = (gcnew System::Windows::Forms::Label());
			this->IncludeIllegalItems = (gcnew System::Windows::Forms::CheckBox());
			this->ClearAllItems = (gcnew System::Windows::Forms::Button());
			this->CharmList = (gcnew System::Windows::Forms::ComboBox());
			this->Label_Charms = (gcnew System::Windows::Forms::Label());
			this->RarityList = (gcnew System::Windows::Forms::ComboBox());
			this->Label_Rarity = (gcnew System::Windows::Forms::Label());
			this->Skill1 = (gcnew System::Windows::Forms::ComboBox());
			this->Skill2 = (gcnew System::Windows::Forms::ComboBox());
			this->Label_Skill1 = (gcnew System::Windows::Forms::Label());
			this->Label_Skill2 = (gcnew System::Windows::Forms::Label());
			this->Skill1Level = (gcnew System::Windows::Forms::NumericUpDown());
			this->Skill2Level = (gcnew System::Windows::Forms::NumericUpDown());
			this->Label_LevelSkill1 = (gcnew System::Windows::Forms::Label());
			this->Label_LevelSkill2 = (gcnew System::Windows::Forms::Label());
			this->AddNewCharm = (gcnew System::Windows::Forms::Button());
			this->Slot1 = (gcnew System::Windows::Forms::ComboBox());
			this->Slot2 = (gcnew System::Windows::Forms::ComboBox());
			this->Slot3 = (gcnew System::Windows::Forms::ComboBox());
			this->Label_Slot1 = (gcnew System::Windows::Forms::Label());
			this->Label_Slot2 = (gcnew System::Windows::Forms::Label());
			this->Label_Slot3 = (gcnew System::Windows::Forms::Label());
			this->SaveButton = (gcnew System::Windows::Forms::Button());
			this->Tooltip = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->ReadEquipmentBox = (gcnew System::Windows::Forms::Button());
			this->Label_Other = (gcnew System::Windows::Forms::Label());
			this->ZennyAmount = (gcnew System::Windows::Forms::NumericUpDown());
			this->Label_Zenny = (gcnew System::Windows::Forms::Label());
			this->PointsAmount = (gcnew System::Windows::Forms::NumericUpDown());
			this->Label_Points = (gcnew System::Windows::Forms::Label());
			this->SetZenny = (gcnew System::Windows::Forms::Button());
			this->SetPoints = (gcnew System::Windows::Forms::Button());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->Tab_ItemboxEditor = (gcnew System::Windows::Forms::TabPage());
			this->SaveItemBox = (gcnew System::Windows::Forms::Button());
			this->ReadItemBox = (gcnew System::Windows::Forms::Button());
			this->ItemboxCtrl = (gcnew System::Windows::Forms::DataGridView());
			this->Column_ID = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_Item = (gcnew System::Windows::Forms::DataGridViewComboBoxColumn());
			this->Column_Amount = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->mainMenu1 = (gcnew System::Windows::Forms::MainMenu(this->components));
			this->FileMenu = (gcnew System::Windows::Forms::MenuItem());
			this->ExportCharms = (gcnew System::Windows::Forms::MenuItem());
			this->ExportItembox = (gcnew System::Windows::Forms::MenuItem());
			this->ImportCharms = (gcnew System::Windows::Forms::MenuItem());
			this->ImportItembox = (gcnew System::Windows::Forms::MenuItem());
			this->LanguageMenu = (gcnew System::Windows::Forms::MenuItem());
			this->RawCharms = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ItemCount))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Skill1Level))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Skill2Level))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ZennyAmount))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->PointsAmount))->BeginInit();
			this->tabControl1->SuspendLayout();
			this->Tab_ItemboxEditor->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ItemboxCtrl))->BeginInit();
			this->SuspendLayout();
			// 
			// Label_ItemModifer
			// 
			this->Label_ItemModifer->AutoSize = true;
			this->Label_ItemModifer->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold));
			this->Label_ItemModifer->Location = System::Drawing::Point(13, 13);
			this->Label_ItemModifer->Name = L"Label_ItemModifer";
			this->Label_ItemModifer->Size = System::Drawing::Size(80, 13);
			this->Label_ItemModifer->TabIndex = 0;
			this->Label_ItemModifer->Text = L"Item Modifier";
			// 
			// AddAllItems
			// 
			this->AddAllItems->AutoSize = true;
			this->AddAllItems->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->AddAllItems->Location = System::Drawing::Point(12, 39);
			this->AddAllItems->Name = L"AddAllItems";
			this->AddAllItems->Size = System::Drawing::Size(87, 17);
			this->AddAllItems->TabIndex = 1;
			this->AddAllItems->Text = L"Add All Items";
			this->AddAllItems->UseVisualStyleBackColor = true;
			// 
			// ItemCount
			// 
			this->ItemCount->Location = System::Drawing::Point(12, 63);
			this->ItemCount->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 9999, 0, 0, 0 });
			this->ItemCount->Name = L"ItemCount";
			this->ItemCount->Size = System::Drawing::Size(87, 20);
			this->ItemCount->TabIndex = 2;
			// 
			// Label_Amount
			// 
			this->Label_Amount->AutoSize = true;
			this->Label_Amount->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Label_Amount->Location = System::Drawing::Point(105, 65);
			this->Label_Amount->Name = L"Label_Amount";
			this->Label_Amount->Size = System::Drawing::Size(43, 13);
			this->Label_Amount->TabIndex = 3;
			this->Label_Amount->Text = L"Amount";
			// 
			// RunScript
			// 
			this->RunScript->Location = System::Drawing::Point(12, 89);
			this->RunScript->Name = L"RunScript";
			this->RunScript->Size = System::Drawing::Size(75, 23);
			this->RunScript->TabIndex = 4;
			this->RunScript->Text = L"Run Script";
			this->RunScript->UseVisualStyleBackColor = true;
			this->RunScript->Click += gcnew System::EventHandler(this, &Window::RunScript_Click);
			// 
			// Label_CharmModifier
			// 
			this->Label_CharmModifier->AutoSize = true;
			this->Label_CharmModifier->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold));
			this->Label_CharmModifier->Location = System::Drawing::Point(13, 148);
			this->Label_CharmModifier->Name = L"Label_CharmModifier";
			this->Label_CharmModifier->Size = System::Drawing::Size(91, 13);
			this->Label_CharmModifier->TabIndex = 5;
			this->Label_CharmModifier->Text = L"Charm Modifier";
			// 
			// IncludeIllegalItems
			// 
			this->IncludeIllegalItems->AutoSize = true;
			this->IncludeIllegalItems->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->IncludeIllegalItems->Location = System::Drawing::Point(108, 39);
			this->IncludeIllegalItems->Name = L"IncludeIllegalItems";
			this->IncludeIllegalItems->Size = System::Drawing::Size(81, 17);
			this->IncludeIllegalItems->TabIndex = 7;
			this->IncludeIllegalItems->Text = L"Illegal Items";
			this->IncludeIllegalItems->UseVisualStyleBackColor = true;
			// 
			// ClearAllItems
			// 
			this->ClearAllItems->Location = System::Drawing::Point(12, 118);
			this->ClearAllItems->Name = L"ClearAllItems";
			this->ClearAllItems->Size = System::Drawing::Size(75, 23);
			this->ClearAllItems->TabIndex = 8;
			this->ClearAllItems->Text = L"Clear All";
			this->ClearAllItems->UseVisualStyleBackColor = true;
			this->ClearAllItems->Click += gcnew System::EventHandler(this, &Window::ClearAllItems_Click);
			// 
			// CharmList
			// 
			this->CharmList->Enabled = false;
			this->CharmList->FormattingEnabled = true;
			this->CharmList->Location = System::Drawing::Point(13, 200);
			this->CharmList->Name = L"CharmList";
			this->CharmList->Size = System::Drawing::Size(226, 21);
			this->CharmList->TabIndex = 9;
			this->CharmList->SelectedIndexChanged += gcnew System::EventHandler(this, &Window::CharmList_SelectedIndexChanged);
			// 
			// Label_Charms
			// 
			this->Label_Charms->AutoSize = true;
			this->Label_Charms->Enabled = false;
			this->Label_Charms->Location = System::Drawing::Point(245, 203);
			this->Label_Charms->Name = L"Label_Charms";
			this->Label_Charms->Size = System::Drawing::Size(42, 13);
			this->Label_Charms->TabIndex = 10;
			this->Label_Charms->Text = L"Charms";
			// 
			// RarityList
			// 
			this->RarityList->DisplayMember = L"Rarity 1";
			this->RarityList->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->RarityList->Enabled = false;
			this->RarityList->FormattingEnabled = true;
			this->RarityList->Items->AddRange(gcnew cli::array< System::Object^  >(11) {
				L"Rarity 1", L"Rarity 2", L"Rarity 3", L"Rarity 4",
					L"Rarity 5", L"Rarity 6", L"Rarity 7", L"Rarity 4 (Novice)", L"Rarity 3 (Kinship)", L"Rarity 12", L"Rarity 2 (Legacy)"
			});
			this->RarityList->Location = System::Drawing::Point(12, 241);
			this->RarityList->Name = L"RarityList";
			this->RarityList->Size = System::Drawing::Size(121, 21);
			this->RarityList->TabIndex = 11;
			// 
			// Label_Rarity
			// 
			this->Label_Rarity->AutoSize = true;
			this->Label_Rarity->Enabled = false;
			this->Label_Rarity->Location = System::Drawing::Point(139, 244);
			this->Label_Rarity->Name = L"Label_Rarity";
			this->Label_Rarity->Size = System::Drawing::Size(34, 13);
			this->Label_Rarity->TabIndex = 12;
			this->Label_Rarity->Text = L"Rarity";
			// 
			// Skill1
			// 
			this->Skill1->Enabled = false;
			this->Skill1->FormattingEnabled = true;
			this->Skill1->Items->AddRange(gcnew cli::array< System::Object^  >(112) {
				L"None", L"Attack Boost", L"Agitator", L"Peak Performance",
					L"Resentment", L"Resuscitate", L"Critical Eye", L"Critical Boost", L"Weakness Exploit", L"Latent Power", L"Maximum Might", L"Critical Element",
					L"Null", L"Fire Attack", L"Water Attack", L"Ice Attack", L"Thunder Attack", L"Dragon Attack", L"Poison Attack", L"Paralysis Attack",
					L"Sleep Attack", L"Blast Attack", L"Handicraft", L"Razor Sharp", L"Spare Shot", L"Protective Polish", L"Mind\'s Eye", L"Aim Booster",
					L"Bludgeoner", L"Bow Charge Plus", L"Focus", L"Power Prolonger", L"Marathon Runner", L"Constitution", L"Stamina Surge", L"Guard",
					L"Guard Up", L"Offensive Guard", L"Critical Draw", L"Punishing Draw", L"Quick Sheath", L"Slugger", L"Stamina Thief", L"Affinity Sliding",
					L"Horn Maestro", L"Artilerry", L"Load Shells", L"Special Ammo Boost", L"Normal/Rapid Up", L"Pierce Up", L"Spread Up", L"Ammo Up",
					L"Reload Speed", L"Recoil Down", L"Steadiness", L"Rapid Fire Up", L"Defense Boost", L"Divine Blessing", L"Recovery Up", L"Recovery Speed",
					L"Speed Eating", L"Earplugs", L"Windproof", L"Tremor Resitance", L"Bubbly Dance", L"Evade Window", L"Evade Extender", L"Fire Resistance",
					L"Water Resistance", L"Ice Resistance", L"Thunder Resistance", L"Dragon Resistance", L"Blight Resistance", L"Poison Resistance",
					L"Paralysis Resistance", L"Sleep Resistance", L"Stun Resistance", L"Muck Resistance", L"Blast Resistance", L"Botanist", L"Geologist",
					L"Partbreaker", L"Capture Master", L"Null", L"Good Luck", L"Speed Sharpening", L"Bombardier", L"Mushroomancer", L"Item Prolonger",
					L"Wide-Range", L"Free Meal", L"Heroics", L"Fortify", L"Flinch Free", L"Jump Master", L"Carving Pro", L"Hunger Resistance", L"Leap of Faith",
					L"Diversion", L"Master Mounter", L"Null", L"Null", L"Null", L"Null", L"Wirebug Whisperer", L"Wall Runner", L"Counterstrike",
					L"Rapid Morph", L"Hellfire Cloak", L"Wind Alignment", L"Thunder Alignment", L"Null"
			});
			this->Skill1->Location = System::Drawing::Point(13, 268);
			this->Skill1->Name = L"Skill1";
			this->Skill1->Size = System::Drawing::Size(121, 21);
			this->Skill1->TabIndex = 13;
			// 
			// Skill2
			// 
			this->Skill2->Enabled = false;
			this->Skill2->FormattingEnabled = true;
			this->Skill2->Items->AddRange(gcnew cli::array< System::Object^  >(112) {
				L"None", L"Attack Boost", L"Agitator", L"Peak Performance",
					L"Resentment", L"Resuscitate", L"Critical Eye", L"Critical Boost", L"Weakness Exploit", L"Latent Power", L"Maximum Might", L"Critical Element",
					L"Null", L"Fire Attack", L"Water Attack", L"Ice Attack", L"Thunder Attack", L"Dragon Attack", L"Poison Attack", L"Paralysis Attack",
					L"Sleep Attack", L"Blast Attack", L"Handicraft", L"Razor Sharp", L"Spare Shot", L"Protective Polish", L"Mind\'s Eye", L"Aim Booster",
					L"Bludgeoner", L"Bow Charge Plus", L"Focus", L"Power Prolonger", L"Marathon Runner", L"Constitution", L"Stamina Surge", L"Guard",
					L"Guard Up", L"Offensive Guard", L"Critical Draw", L"Punishing Draw", L"Quick Sheath", L"Slugger", L"Stamina Thief", L"Affinity Sliding",
					L"Horn Maestro", L"Artilerry", L"Load Shells", L"Special Ammo Boost", L"Normal/Rapid Up", L"Pierce Up", L"Spread Up", L"Ammo Up",
					L"Reload Speed", L"Recoil Down", L"Steadiness", L"Rapid Fire Up", L"Defense Boost", L"Divine Blessing", L"Recovery Up", L"Recovery Speed",
					L"Speed Eating", L"Earplugs", L"Windproof", L"Tremor Resitance", L"Bubbly Dance", L"Evade Window", L"Evade Extender", L"Fire Resistance",
					L"Water Resistance", L"Ice Resistance", L"Thunder Resistance", L"Dragon Resistance", L"Blight Resistance", L"Poison Resistance",
					L"Paralysis Resistance", L"Sleep Resistance", L"Stun Resistance", L"Muck Resistance", L"Blast Resistance", L"Botanist", L"Geologist",
					L"Partbreaker", L"Capture Master", L"Null", L"Good Luck", L"Speed Sharpening", L"Bombardier", L"Mushroomancer", L"Item Prolonger",
					L"Wide-Range", L"Free Meal", L"Heroics", L"Fortify", L"Flinch Free", L"Jump Master", L"Carving Pro", L"Hunger Resistance", L"Leap of Faith",
					L"Diversion", L"Master Mounter", L"Null", L"Null", L"Null", L"Null", L"Wirebug Whisperer", L"Wall Runner", L"Counterstrike",
					L"Rapid Morph", L"Hellfire Cloak", L"Wind Alignment", L"Thunder Alignment", L"Null"
			});
			this->Skill2->Location = System::Drawing::Point(12, 295);
			this->Skill2->Name = L"Skill2";
			this->Skill2->Size = System::Drawing::Size(121, 21);
			this->Skill2->TabIndex = 14;
			// 
			// Label_Skill1
			// 
			this->Label_Skill1->AutoSize = true;
			this->Label_Skill1->Enabled = false;
			this->Label_Skill1->Location = System::Drawing::Point(138, 271);
			this->Label_Skill1->Name = L"Label_Skill1";
			this->Label_Skill1->Size = System::Drawing::Size(35, 13);
			this->Label_Skill1->TabIndex = 15;
			this->Label_Skill1->Text = L"Skill 1";
			// 
			// Label_Skill2
			// 
			this->Label_Skill2->AutoSize = true;
			this->Label_Skill2->Enabled = false;
			this->Label_Skill2->Location = System::Drawing::Point(139, 298);
			this->Label_Skill2->Name = L"Label_Skill2";
			this->Label_Skill2->Size = System::Drawing::Size(35, 13);
			this->Label_Skill2->TabIndex = 16;
			this->Label_Skill2->Text = L"Skill 2";
			// 
			// Skill1Level
			// 
			this->Skill1Level->Enabled = false;
			this->Skill1Level->Location = System::Drawing::Point(188, 269);
			this->Skill1Level->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 7, 0, 0, 0 });
			this->Skill1Level->Name = L"Skill1Level";
			this->Skill1Level->Size = System::Drawing::Size(69, 20);
			this->Skill1Level->TabIndex = 17;
			// 
			// Skill2Level
			// 
			this->Skill2Level->Enabled = false;
			this->Skill2Level->Location = System::Drawing::Point(188, 296);
			this->Skill2Level->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 7, 0, 0, 0 });
			this->Skill2Level->Name = L"Skill2Level";
			this->Skill2Level->Size = System::Drawing::Size(69, 20);
			this->Skill2Level->TabIndex = 18;
			// 
			// Label_LevelSkill1
			// 
			this->Label_LevelSkill1->AutoSize = true;
			this->Label_LevelSkill1->Enabled = false;
			this->Label_LevelSkill1->Location = System::Drawing::Point(263, 271);
			this->Label_LevelSkill1->Name = L"Label_LevelSkill1";
			this->Label_LevelSkill1->Size = System::Drawing::Size(33, 13);
			this->Label_LevelSkill1->TabIndex = 19;
			this->Label_LevelSkill1->Text = L"Level";
			// 
			// Label_LevelSkill2
			// 
			this->Label_LevelSkill2->AutoSize = true;
			this->Label_LevelSkill2->Enabled = false;
			this->Label_LevelSkill2->Location = System::Drawing::Point(263, 298);
			this->Label_LevelSkill2->Name = L"Label_LevelSkill2";
			this->Label_LevelSkill2->Size = System::Drawing::Size(33, 13);
			this->Label_LevelSkill2->TabIndex = 20;
			this->Label_LevelSkill2->Text = L"Level";
			// 
			// AddNewCharm
			// 
			this->AddNewCharm->Enabled = false;
			this->AddNewCharm->Location = System::Drawing::Point(154, 171);
			this->AddNewCharm->Name = L"AddNewCharm";
			this->AddNewCharm->Size = System::Drawing::Size(85, 23);
			this->AddNewCharm->TabIndex = 21;
			this->AddNewCharm->Text = L"Add New";
			this->Tooltip->SetToolTip(this->AddNewCharm, L"Adds a new charm to the end of the item list");
			this->AddNewCharm->UseVisualStyleBackColor = true;
			this->AddNewCharm->Click += gcnew System::EventHandler(this, &Window::AddNewCharm_Click);
			// 
			// Slot1
			// 
			this->Slot1->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->Slot1->Enabled = false;
			this->Slot1->FormattingEnabled = true;
			this->Slot1->Items->AddRange(gcnew cli::array< System::Object^  >(4) { L"None", L"Level 1", L"Level 2", L"Level 3" });
			this->Slot1->Location = System::Drawing::Point(12, 351);
			this->Slot1->Name = L"Slot1";
			this->Slot1->Size = System::Drawing::Size(67, 21);
			this->Slot1->TabIndex = 22;
			this->Tooltip->SetToolTip(this->Slot1, L"Slots will always be ordered from largest to smallest in game");
			// 
			// Slot2
			// 
			this->Slot2->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->Slot2->Enabled = false;
			this->Slot2->FormattingEnabled = true;
			this->Slot2->Items->AddRange(gcnew cli::array< System::Object^  >(4) { L"None", L"Level 1", L"Level 2", L"Level 3" });
			this->Slot2->Location = System::Drawing::Point(85, 351);
			this->Slot2->Name = L"Slot2";
			this->Slot2->Size = System::Drawing::Size(67, 21);
			this->Slot2->TabIndex = 23;
			this->Tooltip->SetToolTip(this->Slot2, L"Slots will always be ordered from largest to smallest in game");
			// 
			// Slot3
			// 
			this->Slot3->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->Slot3->Enabled = false;
			this->Slot3->FormattingEnabled = true;
			this->Slot3->Items->AddRange(gcnew cli::array< System::Object^  >(4) { L"None", L"Level 1", L"Level 2", L"Level 3" });
			this->Slot3->Location = System::Drawing::Point(158, 351);
			this->Slot3->Name = L"Slot3";
			this->Slot3->Size = System::Drawing::Size(67, 21);
			this->Slot3->TabIndex = 24;
			this->Tooltip->SetToolTip(this->Slot3, L"Slots will always be ordered from largest to smallest in game");
			// 
			// Label_Slot1
			// 
			this->Label_Slot1->AutoSize = true;
			this->Label_Slot1->Enabled = false;
			this->Label_Slot1->Location = System::Drawing::Point(13, 335);
			this->Label_Slot1->Name = L"Label_Slot1";
			this->Label_Slot1->Size = System::Drawing::Size(34, 13);
			this->Label_Slot1->TabIndex = 25;
			this->Label_Slot1->Text = L"Slot 1";
			// 
			// Label_Slot2
			// 
			this->Label_Slot2->AutoSize = true;
			this->Label_Slot2->Enabled = false;
			this->Label_Slot2->Location = System::Drawing::Point(86, 335);
			this->Label_Slot2->Name = L"Label_Slot2";
			this->Label_Slot2->Size = System::Drawing::Size(34, 13);
			this->Label_Slot2->TabIndex = 26;
			this->Label_Slot2->Text = L"Slot 2";
			// 
			// Label_Slot3
			// 
			this->Label_Slot3->AutoSize = true;
			this->Label_Slot3->Enabled = false;
			this->Label_Slot3->Location = System::Drawing::Point(159, 335);
			this->Label_Slot3->Name = L"Label_Slot3";
			this->Label_Slot3->Size = System::Drawing::Size(34, 13);
			this->Label_Slot3->TabIndex = 27;
			this->Label_Slot3->Text = L"Slot 3";
			// 
			// SaveButton
			// 
			this->SaveButton->Enabled = false;
			this->SaveButton->Location = System::Drawing::Point(231, 349);
			this->SaveButton->Name = L"SaveButton";
			this->SaveButton->Size = System::Drawing::Size(65, 23);
			this->SaveButton->TabIndex = 28;
			this->SaveButton->Text = L"Save";
			this->Tooltip->SetToolTip(this->SaveButton, L"Save data and apply to game");
			this->SaveButton->UseVisualStyleBackColor = true;
			this->SaveButton->Click += gcnew System::EventHandler(this, &Window::SaveButton_Click);
			// 
			// ReadEquipmentBox
			// 
			this->ReadEquipmentBox->Location = System::Drawing::Point(12, 171);
			this->ReadEquipmentBox->Name = L"ReadEquipmentBox";
			this->ReadEquipmentBox->Size = System::Drawing::Size(136, 23);
			this->ReadEquipmentBox->TabIndex = 29;
			this->ReadEquipmentBox->Text = L"Read Equipment Box";
			this->Tooltip->SetToolTip(this->ReadEquipmentBox, L"Needs to be pressed once before all editing charms is possible");
			this->ReadEquipmentBox->UseVisualStyleBackColor = true;
			this->ReadEquipmentBox->Click += gcnew System::EventHandler(this, &Window::ReadEquipmentBox_Click);
			// 
			// Label_Other
			// 
			this->Label_Other->AutoSize = true;
			this->Label_Other->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold));
			this->Label_Other->Location = System::Drawing::Point(13, 384);
			this->Label_Other->Name = L"Label_Other";
			this->Label_Other->Size = System::Drawing::Size(38, 13);
			this->Label_Other->TabIndex = 30;
			this->Label_Other->Text = L"Other";
			// 
			// ZennyAmount
			// 
			this->ZennyAmount->Location = System::Drawing::Point(12, 408);
			this->ZennyAmount->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 99999999, 0, 0, 0 });
			this->ZennyAmount->Name = L"ZennyAmount";
			this->ZennyAmount->Size = System::Drawing::Size(120, 20);
			this->ZennyAmount->TabIndex = 31;
			// 
			// Label_Zenny
			// 
			this->Label_Zenny->AutoSize = true;
			this->Label_Zenny->Location = System::Drawing::Point(138, 410);
			this->Label_Zenny->Name = L"Label_Zenny";
			this->Label_Zenny->Size = System::Drawing::Size(37, 13);
			this->Label_Zenny->TabIndex = 32;
			this->Label_Zenny->Text = L"Zenny";
			// 
			// PointsAmount
			// 
			this->PointsAmount->Location = System::Drawing::Point(12, 435);
			this->PointsAmount->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 99999999, 0, 0, 0 });
			this->PointsAmount->Name = L"PointsAmount";
			this->PointsAmount->Size = System::Drawing::Size(120, 20);
			this->PointsAmount->TabIndex = 33;
			// 
			// Label_Points
			// 
			this->Label_Points->AutoSize = true;
			this->Label_Points->Location = System::Drawing::Point(138, 437);
			this->Label_Points->Name = L"Label_Points";
			this->Label_Points->Size = System::Drawing::Size(36, 13);
			this->Label_Points->TabIndex = 34;
			this->Label_Points->Text = L"Points";
			// 
			// SetZenny
			// 
			this->SetZenny->Location = System::Drawing::Point(181, 405);
			this->SetZenny->Name = L"SetZenny";
			this->SetZenny->Size = System::Drawing::Size(75, 23);
			this->SetZenny->TabIndex = 35;
			this->SetZenny->Text = L"Set";
			this->SetZenny->UseVisualStyleBackColor = true;
			this->SetZenny->Click += gcnew System::EventHandler(this, &Window::SetZenny_Click);
			// 
			// SetPoints
			// 
			this->SetPoints->Location = System::Drawing::Point(180, 432);
			this->SetPoints->Name = L"SetPoints";
			this->SetPoints->Size = System::Drawing::Size(75, 23);
			this->SetPoints->TabIndex = 36;
			this->SetPoints->Text = L"Set";
			this->SetPoints->UseVisualStyleBackColor = true;
			this->SetPoints->Click += gcnew System::EventHandler(this, &Window::SetPoints_Click);
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Location = System::Drawing::Point(190, 463);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(109, 13);
			this->label16->TabIndex = 37;
			this->label16->Text = L"Tool created by Fexty";
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->Tab_ItemboxEditor);
			this->tabControl1->Location = System::Drawing::Point(305, 12);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(530, 461);
			this->tabControl1->TabIndex = 41;
			// 
			// Tab_ItemboxEditor
			// 
			this->Tab_ItemboxEditor->Controls->Add(this->SaveItemBox);
			this->Tab_ItemboxEditor->Controls->Add(this->ReadItemBox);
			this->Tab_ItemboxEditor->Controls->Add(this->ItemboxCtrl);
			this->Tab_ItemboxEditor->Location = System::Drawing::Point(4, 22);
			this->Tab_ItemboxEditor->Name = L"Tab_ItemboxEditor";
			this->Tab_ItemboxEditor->Padding = System::Windows::Forms::Padding(3);
			this->Tab_ItemboxEditor->Size = System::Drawing::Size(522, 435);
			this->Tab_ItemboxEditor->TabIndex = 1;
			this->Tab_ItemboxEditor->Text = L"Itembox Editor";
			this->Tab_ItemboxEditor->UseVisualStyleBackColor = true;
			// 
			// SaveItemBox
			// 
			this->SaveItemBox->Location = System::Drawing::Point(437, 56);
			this->SaveItemBox->Name = L"SaveItemBox";
			this->SaveItemBox->Size = System::Drawing::Size(79, 43);
			this->SaveItemBox->TabIndex = 2;
			this->SaveItemBox->Text = L"Save Itembox";
			this->SaveItemBox->UseVisualStyleBackColor = true;
			this->SaveItemBox->Click += gcnew System::EventHandler(this, &Window::SaveItemBox_Click);
			// 
			// ReadItemBox
			// 
			this->ReadItemBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->ReadItemBox->Location = System::Drawing::Point(437, 6);
			this->ReadItemBox->Name = L"ReadItemBox";
			this->ReadItemBox->Size = System::Drawing::Size(79, 43);
			this->ReadItemBox->TabIndex = 1;
			this->ReadItemBox->Text = L"Read Itembox";
			this->ReadItemBox->UseVisualStyleBackColor = true;
			this->ReadItemBox->Click += gcnew System::EventHandler(this, &Window::ReadItemBox_Click);
			// 
			// ItemboxCtrl
			// 
			this->ItemboxCtrl->AllowUserToAddRows = false;
			this->ItemboxCtrl->AllowUserToDeleteRows = false;
			this->ItemboxCtrl->AllowUserToResizeRows = false;
			this->ItemboxCtrl->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->ItemboxCtrl->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(3) {
				this->Column_ID,
					this->Column_Item, this->Column_Amount
			});
			this->ItemboxCtrl->Dock = System::Windows::Forms::DockStyle::Left;
			this->ItemboxCtrl->Location = System::Drawing::Point(3, 3);
			this->ItemboxCtrl->Name = L"ItemboxCtrl";
			this->ItemboxCtrl->Size = System::Drawing::Size(428, 429);
			this->ItemboxCtrl->TabIndex = 0;
			this->ItemboxCtrl->EditingControlShowing += gcnew System::Windows::Forms::DataGridViewEditingControlShowingEventHandler(this, &Window::ItemboxCtrl_EditingControlShowing);
			// 
			// Column_ID
			// 
			this->Column_ID->HeaderText = L"ID";
			this->Column_ID->MaxInputLength = 10;
			this->Column_ID->Name = L"Column_ID";
			this->Column_ID->ReadOnly = true;
			this->Column_ID->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// Column_Item
			// 
			this->Column_Item->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			this->Column_Item->DisplayStyle = System::Windows::Forms::DataGridViewComboBoxDisplayStyle::ComboBox;
			this->Column_Item->FillWeight = 200;
			this->Column_Item->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->Column_Item->HeaderText = L"Item";
			this->Column_Item->Name = L"Column_Item";
			// 
			// Column_Amount
			// 
			this->Column_Amount->HeaderText = L"Amount";
			this->Column_Amount->MaxInputLength = 4;
			this->Column_Amount->Name = L"Column_Amount";
			this->Column_Amount->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// mainMenu1
			// 
			this->mainMenu1->MenuItems->AddRange(gcnew cli::array< System::Windows::Forms::MenuItem^  >(2) { this->FileMenu, this->LanguageMenu });
			// 
			// FileMenu
			// 
			this->FileMenu->Index = 0;
			this->FileMenu->MenuItems->AddRange(gcnew cli::array< System::Windows::Forms::MenuItem^  >(4) {
				this->ExportCharms, this->ExportItembox,
					this->ImportCharms, this->ImportItembox
			});
			this->FileMenu->Text = L"File";
			// 
			// ExportCharms
			// 
			this->ExportCharms->Index = 0;
			this->ExportCharms->Text = L"Export All Charms";
			this->ExportCharms->Click += gcnew System::EventHandler(this, &Window::ExportCharms_Click);
			// 
			// ExportItembox
			// 
			this->ExportItembox->Index = 1;
			this->ExportItembox->Text = L"Export Itembox";
			this->ExportItembox->Click += gcnew System::EventHandler(this, &Window::ExportItembox_Click);
			// 
			// ImportCharms
			// 
			this->ImportCharms->Index = 2;
			this->ImportCharms->Text = L"Import Charm(s)";
			this->ImportCharms->Click += gcnew System::EventHandler(this, &Window::ImportCharms_Click);
			// 
			// ImportItembox
			// 
			this->ImportItembox->Index = 3;
			this->ImportItembox->Text = L"Import Itembox";
			this->ImportItembox->Click += gcnew System::EventHandler(this, &Window::ImportItembox_Click);
			// 
			// LanguageMenu
			// 
			this->LanguageMenu->Index = 1;
			this->LanguageMenu->Text = L"Language";
			// 
			// RawCharms
			// 
			this->RawCharms->Location = System::Drawing::Point(245, 171);
			this->RawCharms->Name = L"RawCharms";
			this->RawCharms->Size = System::Drawing::Size(54, 23);
			this->RawCharms->TabIndex = 42;
			this->RawCharms->Text = L"Raw";
			this->RawCharms->UseVisualStyleBackColor = true;
			this->RawCharms->Click += gcnew System::EventHandler(this, &Window::RawCharms_Click);
			// 
			// Window
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(847, 485);
			this->Controls->Add(this->RawCharms);
			this->Controls->Add(this->label16);
			this->Controls->Add(this->SetPoints);
			this->Controls->Add(this->SetZenny);
			this->Controls->Add(this->Label_Points);
			this->Controls->Add(this->PointsAmount);
			this->Controls->Add(this->Label_Zenny);
			this->Controls->Add(this->ZennyAmount);
			this->Controls->Add(this->Label_Other);
			this->Controls->Add(this->ReadEquipmentBox);
			this->Controls->Add(this->SaveButton);
			this->Controls->Add(this->Label_Slot3);
			this->Controls->Add(this->Label_Slot2);
			this->Controls->Add(this->Label_Slot1);
			this->Controls->Add(this->Slot3);
			this->Controls->Add(this->Slot2);
			this->Controls->Add(this->Slot1);
			this->Controls->Add(this->AddNewCharm);
			this->Controls->Add(this->Label_LevelSkill2);
			this->Controls->Add(this->Label_LevelSkill1);
			this->Controls->Add(this->Skill2Level);
			this->Controls->Add(this->Skill1Level);
			this->Controls->Add(this->Label_Skill2);
			this->Controls->Add(this->Label_Skill1);
			this->Controls->Add(this->Skill2);
			this->Controls->Add(this->Skill1);
			this->Controls->Add(this->Label_Rarity);
			this->Controls->Add(this->RarityList);
			this->Controls->Add(this->Label_Charms);
			this->Controls->Add(this->CharmList);
			this->Controls->Add(this->ClearAllItems);
			this->Controls->Add(this->IncludeIllegalItems);
			this->Controls->Add(this->Label_CharmModifier);
			this->Controls->Add(this->RunScript);
			this->Controls->Add(this->Label_Amount);
			this->Controls->Add(this->ItemCount);
			this->Controls->Add(this->AddAllItems);
			this->Controls->Add(this->Label_ItemModifer);
			this->Controls->Add(this->tabControl1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->MaximizeBox = false;
			this->Menu = this->mainMenu1;
			this->MinimizeBox = false;
			this->Name = L"Window";
			this->Text = L"Monster Hunter Rise Charm Editor & More";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ItemCount))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Skill1Level))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Skill2Level))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ZennyAmount))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->PointsAmount))->EndInit();
			this->tabControl1->ResumeLayout(false);
			this->Tab_ItemboxEditor->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ItemboxCtrl))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: HANDLE ProcessHandle = nullptr;
	//private: static LPVOID DataManagerAddress = LPVOID(0x14C093638); // For Early Access game
	private: static LPVOID DataManagerAddress = LPVOID(0x14C0578C0);
	private: Generic::List<UINT32>^ ExcludeList;
	private: Generic::List<CharmData^>^ Charms;
	private: Generic::List<String^>^ SkillNames;
	private: Generic::Dictionary<UInt32, String^>^ ItemNames = nullptr;
	private: Generic::List<UInt32>^ FreeEqBoxIndices;
	private: Generic::List<ItemData^>^ Itembox;

	private:
		System::Void RunScript_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void ClearAllItems_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void ReadEquipmentBox_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void SaveButton_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void CharmList_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e);
		System::Void AddNewCharm_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void SetZenny_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void SetPoints_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void ItemboxCtrl_EditingControlShowing(System::Object^ sender, DataGridViewEditingControlShowingEventArgs^ e);
		System::Void ReadItemBox_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void SaveItemBox_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void ExportCharms_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void ExportItembox_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void ImportCharms_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void ImportItembox_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void RawCharms_Click(System::Object^ sender, System::EventArgs^ e);

		System::Void NumericTextBox_KeyPress(System::Object^ sender, KeyPressEventArgs^ e);
		System::Void NumericTextBox_ValueChanged(System::Object^ sender, EventArgs^ e);
		System::Void Dropdown_SelectedIndexChanged(System::Object^ sender, EventArgs^ e);

		System::Void AddItemsFunc(uintptr_t dataManager, UINT32 amount, bool clear);
		System::Void OpenGameProcess();
		System::Void OpenGameProcessByExeName(const wchar_t* name);
		System::UIntPtr GetDataManager();
		System::String^ FormatCharmName(CharmData^ charm);
		System::UInt32 GetFirstSlot(CharmData^ charm);
		System::UInt32 GetSecondSlot(CharmData^ charm);
		System::UInt32 GetThirdSlot(CharmData^ charm);
		System::Void WriteControlsToCharm(CharmData^ charm);
		System::Void WriteCharmToGame(CharmData^ charm);
		System::Void CollectItems();
		System::Void WriteItems();
		System::Void PopulateItembox();
		System::Void ApplyItembox();

		System::Void InitItemList(String^ langname);
		System::Void InitSkillNames(String^ langname);
		System::Void InitLanguages();
		System::Void ChangeMenuLanguage(String^ langname);
		System::Void Language_Click(Object^ sender, EventArgs^ e);
		System::String^ FormatCharmForFile(CharmData^ charm);
		Generic::List<CharmData^>^ GetCharmsFromEquipmentBox();
};
}

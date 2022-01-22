#pragma once

#include "Data.h"

namespace RisePCItemEditor {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for RawCharmEditor
	/// </summary>
	public ref class RawCharmEditor : public System::Windows::Forms::Form
	{
	public:
		RawCharmEditor(Generic::List<CharmData^>^ charms)
		{
			InitializeComponent();
			Charms = charms;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~RawCharmEditor()
		{
			if (components)
			{
				delete components;
			}

			SaveCharms();
		}
	private: System::Windows::Forms::DataGridView^ CharmEditorList;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ Skill1;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ Skill1Level;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ Skill2;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ Skill2Level;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ L1Slots;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ L2Slots;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ L3Slots;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ Rarity;
	protected:









	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(RawCharmEditor::typeid));
			this->CharmEditorList = (gcnew System::Windows::Forms::DataGridView());
			this->Skill1 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Skill1Level = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Skill2 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Skill2Level = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->L1Slots = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->L2Slots = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->L3Slots = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Rarity = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->CharmEditorList))->BeginInit();
			this->SuspendLayout();
			// 
			// CharmEditorList
			// 
			this->CharmEditorList->AllowUserToAddRows = false;
			this->CharmEditorList->AllowUserToDeleteRows = false;
			this->CharmEditorList->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::Fill;
			this->CharmEditorList->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->CharmEditorList->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(8) {
				this->Skill1,
					this->Skill1Level, this->Skill2, this->Skill2Level, this->L1Slots, this->L2Slots, this->L3Slots, this->Rarity
			});
			this->CharmEditorList->Dock = System::Windows::Forms::DockStyle::Fill;
			this->CharmEditorList->Location = System::Drawing::Point(0, 0);
			this->CharmEditorList->Name = L"CharmEditorList";
			this->CharmEditorList->Size = System::Drawing::Size(645, 400);
			this->CharmEditorList->TabIndex = 0;
			this->CharmEditorList->EditingControlShowing += gcnew System::Windows::Forms::DataGridViewEditingControlShowingEventHandler(this, &RawCharmEditor::CharmEditorList_EditingControlShowing);
			// 
			// Skill1
			// 
			this->Skill1->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			this->Skill1->HeaderText = L"Skill 1";
			this->Skill1->Name = L"Skill1";
			this->Skill1->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// Skill1Level
			// 
			this->Skill1Level->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			this->Skill1Level->HeaderText = L"Skill 1 Level";
			this->Skill1Level->Name = L"Skill1Level";
			this->Skill1Level->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// Skill2
			// 
			this->Skill2->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			this->Skill2->HeaderText = L"Skill 2";
			this->Skill2->Name = L"Skill2";
			this->Skill2->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// Skill2Level
			// 
			this->Skill2Level->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			this->Skill2Level->HeaderText = L"Skill 2 Level";
			this->Skill2Level->Name = L"Skill2Level";
			this->Skill2Level->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// L1Slots
			// 
			this->L1Slots->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			this->L1Slots->HeaderText = L"L1 Slots";
			this->L1Slots->Name = L"L1Slots";
			this->L1Slots->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// L2Slots
			// 
			this->L2Slots->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			this->L2Slots->HeaderText = L"L2 Slots";
			this->L2Slots->Name = L"L2Slots";
			this->L2Slots->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// L3Slots
			// 
			this->L3Slots->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			this->L3Slots->HeaderText = L"L3 Slots";
			this->L3Slots->Name = L"L3Slots";
			this->L3Slots->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// Rarity
			// 
			this->Rarity->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			this->Rarity->HeaderText = L"Rarity";
			this->Rarity->Name = L"Rarity";
			this->Rarity->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// RawCharmEditor
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(645, 400);
			this->Controls->Add(this->CharmEditorList);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Name = L"RawCharmEditor";
			this->Text = L"Raw Charm Value Editor";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &RawCharmEditor::RawCharmEditor_FormClosing);
			this->Load += gcnew System::EventHandler(this, &RawCharmEditor::RawCharmEditor_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->CharmEditorList))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
		Generic::List<CharmData^>^ Charms;

		System::Void CharmEditorList_EditingControlShowing(System::Object^ sender, System::Windows::Forms::DataGridViewEditingControlShowingEventArgs^ e);
		System::Void RawCharmEditor_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e);
		System::Void NumericTextBox_KeyPress(System::Object^ sender, KeyPressEventArgs^ e);
		System::Void RawCharmEditor_Load(System::Object^ sender, System::EventArgs^ e);

		System::Void SaveCharms();

	public:
		Generic::List<CharmData^>^ GetReturn();
};
}

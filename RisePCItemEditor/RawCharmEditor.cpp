#include "RawCharmEditor.h"

System::Void RisePCItemEditor::RawCharmEditor::CharmEditorList_EditingControlShowing(System::Object^ sender, System::Windows::Forms::DataGridViewEditingControlShowingEventArgs^ e)
{
	e->Control->KeyPress -= gcnew KeyPressEventHandler(this, &RawCharmEditor::NumericTextBox_KeyPress);

	TextBox^ tb = (TextBox^)e->Control;
	if (tb) tb->KeyPress += gcnew KeyPressEventHandler(this, &RawCharmEditor::NumericTextBox_KeyPress);
}

System::Void RisePCItemEditor::RawCharmEditor::RawCharmEditor_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e)
{
	SaveCharms();
}

System::Void RisePCItemEditor::RawCharmEditor::NumericTextBox_KeyPress(System::Object^ sender, KeyPressEventArgs^ e)
{
	if (!Char::IsControl(e->KeyChar) && !Char::IsDigit(e->KeyChar))
	{
		e->Handled = true;
	}
}

using System::Windows::Forms::DataGridViewTextBoxCell;
DataGridViewTextBoxCell^ GetCell(UInt32 val)
{
	DataGridViewTextBoxCell^ cell = gcnew DataGridViewTextBoxCell();
	cell->Value = val.ToString();
	return cell;
}

System::Void RisePCItemEditor::RawCharmEditor::RawCharmEditor_Load(System::Object^ sender, System::EventArgs^ e)
{
	CharmEditorList->Rows->Clear();

	for (int i = 0; i < Charms->Count; i++)
	{
		CharmData^ c = Charms[i];
		DataGridViewRow^ row = gcnew DataGridViewRow();
		
		row->Cells->Add(GetCell(c->Skill1));
		row->Cells->Add(GetCell(c->Skill1Level));
		row->Cells->Add(GetCell(c->Skill2));
		row->Cells->Add(GetCell(c->Skill2Level));
		row->Cells->Add(GetCell(c->Level1Slots));
		row->Cells->Add(GetCell(c->Level2Slots));
		row->Cells->Add(GetCell(c->Level3Slots));
		row->Cells->Add(GetCell(UInt32(c->Rarity)));
		CharmEditorList->Rows->Add(row);
	}
}

System::Void RisePCItemEditor::RawCharmEditor::SaveCharms()
{
	Charms->Clear();

	for (int i = 0; i < CharmEditorList->Rows->Count; i++)
	{
		DataGridViewRow^ row = CharmEditorList->Rows[i];

		CharmData^ c = gcnew CharmData();
		c->Skill1 = UInt32::Parse((String^)row->Cells[0]->Value);
		c->Skill1Level = UInt32::Parse((String^)row->Cells[1]->Value);
		c->Skill2 = UInt32::Parse((String^)row->Cells[2]->Value);
		c->Skill2Level = UInt32::Parse((String^)row->Cells[3]->Value);
		c->Level1Slots = UInt32::Parse((String^)row->Cells[4]->Value);
		c->Level2Slots = UInt32::Parse((String^)row->Cells[5]->Value);
		c->Level3Slots = UInt32::Parse((String^)row->Cells[6]->Value);
		c->Rarity = ::Rarity(UInt32::Parse((String^)row->Cells[7]->Value));
		c->Index = i;
		
		Charms->Add(c);
	}
}

System::Collections::Generic::List<CharmData^>^ RisePCItemEditor::RawCharmEditor::GetReturn()
{
	return Charms;
}

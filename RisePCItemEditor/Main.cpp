
#include "Window.h"

[STAThreadAttribute]
int main()
{
	RisePCItemEditor::Application::EnableVisualStyles();
	RisePCItemEditor::Application::SetCompatibleTextRenderingDefault(false);

	ShowWindow(GetConsoleWindow(), SW_HIDE);

	RisePCItemEditor::Window^ window = gcnew RisePCItemEditor::Window();
	RisePCItemEditor::Application::Run(window);
}

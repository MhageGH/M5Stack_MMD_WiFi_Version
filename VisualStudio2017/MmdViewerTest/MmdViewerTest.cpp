// MmdViewerTest.cpp : メイン プロジェクト ファイルです。

#include "stdafx.h"
#include "Form1.h"

using namespace MmdViewerTest;
using namespace MmdViewerCLR;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// コントロールが作成される前に、Windows XP ビジュアル効果を有効にします
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// メイン ウィンドウを作成して、実行します
	ControlVariableCLRWrapper^ controlVariableCLRWrapper = gcnew ControlVariableCLRWrapper();
	Form1^ form1 = gcnew Form1(controlVariableCLRWrapper);
	form1->Show();
	MyApplicationCLR^ myApplicationCLR = gcnew MyApplicationCLR(form1->Handle, controlVariableCLRWrapper);
	while(form1->Created) {
		try {
			myApplicationCLR->Run(); 
		}
		catch (Exception^ e) {
			MessageBox::Show(e->Message);
			form1->Close();
		}
		Application::DoEvents();
	}
	return 0;
}

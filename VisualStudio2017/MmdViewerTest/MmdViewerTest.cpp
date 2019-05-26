// MmdViewerTest.cpp : ���C�� �v���W�F�N�g �t�@�C���ł��B

#include "stdafx.h"
#include "Form1.h"

using namespace MmdViewerTest;
using namespace MmdViewerCLR;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// �R���g���[�����쐬�����O�ɁAWindows XP �r�W���A�����ʂ�L���ɂ��܂�
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// ���C�� �E�B���h�E���쐬���āA���s���܂�
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

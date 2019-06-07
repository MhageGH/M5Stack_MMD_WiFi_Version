// これは メイン DLL ファイルです。

#include "stdafx.h"
#include "MmdViewerCLR.h"
#include "../MmdViewerWin32/MyApplication.h"
#include "../MmdViewerWin32/ControlVariable.h"


using namespace MmdViewerCLR;

///// Vector3 /////
Vector3::Vector3() { X = 0; Y = 0; Z = 0; }

Vector3::Vector3(Vector3^ v) { X = v->X; Y = v->Y; Z = v->Z; }

Vector3::Vector3(float X, float Y, float Z) { this->X = X; this->Y = Y; this->Z = Z; }

Vector3^ Vector3::operator+(Vector3^v, Vector3^w) { return gcnew Vector3(v->X + w->X, v->Y + w->Y, v->Z + w->Z); }

Vector3^ Vector3::operator-(Vector3^v, Vector3^w) { return gcnew Vector3(v->X - w->X, v->Y - w->Y, v->Z - w->Z); }

Vector3^ Vector3::operator*(Vector3^v, float k) { return gcnew Vector3(k*v->X, k*v->Y, k*v->Z); }

Vector3^ Vector3::operator* (float k, Vector3^ v) { return gcnew Vector3(k*v->X, k*v->Y, k*v->Z); }

///// ControlVariableCLRWrapper /////
ControlVariableCLRWrapper::ControlVariableCLRWrapper() { controlVariable = new ControlVariable(); }

ControlVariableCLRWrapper::~ControlVariableCLRWrapper() { this->!ControlVariableCLRWrapper(); }

ControlVariableCLRWrapper::!ControlVariableCLRWrapper() { delete controlVariable; }

bool ControlVariableCLRWrapper::openPmdFileFlag::get() { return controlVariable->openPmdFileFlag; }

void ControlVariableCLRWrapper::openPmdFileFlag::set(bool f) { controlVariable->openPmdFileFlag = f; }

bool ControlVariableCLRWrapper::openVmdFileFlag::get() { return controlVariable->openVmdFileFlag; }

void ControlVariableCLRWrapper::openVmdFileFlag::set(bool f) { controlVariable->openVmdFileFlag = f; }

String^ ControlVariableCLRWrapper::pmdFileName::get() { return gcnew String(controlVariable->pmdFileName.c_str()); }

void ControlVariableCLRWrapper::pmdFileName::set(String^ s) {
	using namespace System::Runtime::InteropServices;
	IntPtr ptr = Marshal::StringToHGlobalUni(s);
	controlVariable->pmdFileName = (const wchar_t*)ptr.ToPointer();
	Marshal::FreeHGlobal(ptr);
}

String^ ControlVariableCLRWrapper::vmdFileName::get() { return gcnew String(controlVariable->vmdFileName.c_str()); }

void ControlVariableCLRWrapper::vmdFileName::set(String^ s) {
	using namespace System::Runtime::InteropServices;
	IntPtr ptr = Marshal::StringToHGlobalUni(s);
	controlVariable->vmdFileName = (const wchar_t*)ptr.ToPointer();
	Marshal::FreeHGlobal(ptr);
}

Vector3^ ControlVariableCLRWrapper::eyePoint::get() { return gcnew Vector3(controlVariable->eyePoint.x, controlVariable->eyePoint.y, controlVariable->eyePoint.z); }

void ControlVariableCLRWrapper::eyePoint::set(Vector3^ v) { controlVariable->eyePoint.x = v->X; controlVariable->eyePoint.y = v->Y; controlVariable->eyePoint.z = v->Z; }

Vector3^ ControlVariableCLRWrapper::lookAtPoint::get() { return gcnew Vector3(controlVariable->lookAtPoint.x, controlVariable->lookAtPoint.y, controlVariable->lookAtPoint.z); }

void ControlVariableCLRWrapper::lookAtPoint::set(Vector3^ v) { controlVariable->lookAtPoint.x = v->X; controlVariable->lookAtPoint.y = v->Y; controlVariable->lookAtPoint.z = v->Z; }

ControlVariable* ControlVariableCLRWrapper::GetControlVariable() { return controlVariable; }

bool ControlVariableCLRWrapper::physicsEnabled::get() { return controlVariable->physicsEnabled; }

void ControlVariableCLRWrapper::physicsEnabled::set(bool s) { controlVariable->physicsEnabled = s; }

bool ControlVariableCLRWrapper::boneMeshEnabled::get() { return controlVariable->boneMeshEnabled; }

void ControlVariableCLRWrapper::boneMeshEnabled::set(bool s) { controlVariable->boneMeshEnabled = s; }

bool ControlVariableCLRWrapper::rigidMeshEnabled::get() { return controlVariable->rigidMeshEnabled; }

void ControlVariableCLRWrapper::rigidMeshEnabled::set(bool s) { controlVariable->rigidMeshEnabled = s; }

bool ControlVariableCLRWrapper::jointMeshEnabled::get() { return controlVariable->jointMeshEnabled; }

void ControlVariableCLRWrapper::jointMeshEnabled::set(bool s) { controlVariable->jointMeshEnabled = s; }

bool ControlVariableCLRWrapper::bitmapRevising::get() { return controlVariable->bitmapRevising; }

void ControlVariableCLRWrapper::bitmapRevising::set(bool s) { controlVariable->bitmapRevising = s; }

void ControlVariableCLRWrapper::SetViewMatrix(cli::array<float,2>^ view) {
	for (int i = 0; i < 4; ++i) for (int j = 0; j < 4; ++j) controlVariable->view.m[i][j] = view[i, j];
}

cli::array<byte>^ ControlVariableCLRWrapper::GetBitmapBuffer() {
	if (bitmapBuffer == nullptr) bitmapBuffer = gcnew cli::array<byte>(controlVariable->bitmapBufferSize);
	if (controlVariable->bitmapBuffer) for (int i = 0; i < bitmapBuffer->Length; ++i) bitmapBuffer[i] = controlVariable->bitmapBuffer[i];
	return bitmapBuffer;
}

///// MyApplicationCLR //////
MyApplicationCLR::MyApplicationCLR(System::IntPtr hWnd, ControlVariableCLRWrapper^ controlVariableCLRWrapper, int width, int height) {
	ControlVariable* controlVariable = controlVariableCLRWrapper->GetControlVariable();
	myApplication = new MyApplication((HWND)(void*)hWnd, controlVariable, width, height);
}

MyApplicationCLR::~MyApplicationCLR() {	this->!MyApplicationCLR(); }

MyApplicationCLR::!MyApplicationCLR() {	delete myApplication; }

void MyApplicationCLR::Run() {
	try { myApplication->Run(); }
	catch (LPCTSTR str) { throw gcnew Exception(gcnew String(str));	}
}
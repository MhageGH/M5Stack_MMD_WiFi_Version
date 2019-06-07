#pragma once

using namespace System;

struct ControlVariable;
class MyApplication;
class BulletDemo;

namespace MmdViewerCLR {
	public ref class Vector3 {
	public:
		property float X;
		property float Y;
		property float Z;
		Vector3();
		Vector3(Vector3^);
		Vector3(float X, float Y, float Z);
		static Vector3^ operator+ (Vector3^, Vector3^);
		static Vector3^ operator- (Vector3^, Vector3^);
		static Vector3^ operator* (Vector3^, float);
		static Vector3^ operator* (float, Vector3^);
	};

	public ref class ControlVariableCLRWrapper {
		ControlVariable* controlVariable;
		cli::array<byte>^ bitmapBuffer = nullptr;
	public:
		ControlVariableCLRWrapper();
		~ControlVariableCLRWrapper();
		!ControlVariableCLRWrapper();
		property bool openPmdFileFlag { bool get(); void set(bool); }
		property bool openVmdFileFlag { bool get(); void set(bool); }
		property String^ pmdFileName { String^ get(); void set(String^); }
		property String^ vmdFileName { String^ get(); void set(String^); }
		property Vector3^ eyePoint { Vector3^ get(); void set(Vector3^); }
		property Vector3^ lookAtPoint { Vector3^ get(); void set(Vector3^); }
		property bool physicsEnabled { bool get(); void set(bool); }
		property bool boneMeshEnabled { bool get(); void set(bool); }
		property bool rigidMeshEnabled { bool get(); void set(bool); }
		property bool jointMeshEnabled { bool get(); void set(bool); }
		property bool bitmapRevising { bool get(); void set(bool); }
		void SetViewMatrix(cli::array<float, 2>^ view);
		cli::array<byte>^ GetBitmapBuffer();
	internal:
		ControlVariable* GetControlVariable();
	};

	public ref class MyApplicationCLR {
		MyApplication* myApplication;
	public:
		MyApplicationCLR(System::IntPtr hWnd, ControlVariableCLRWrapper^ controlVariableCLRWrapper, int width, int height);
		~MyApplicationCLR();
		!MyApplicationCLR();
		void Run();
	};
}

#pragma once


namespace MmdViewerTest {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace MmdViewerCLR;

	/// <summary>
	/// Form1 の概要
	///
	/// 警告: このクラスの名前を変更する場合、このクラスが依存するすべての .resx ファイルに関連付けられた
	///          マネージ リソース コンパイラ ツールに対して 'Resource File Name' プロパティを
	///          変更する必要があります。この変更を行わないと、
	///          デザイナと、このフォームに関連付けられたローカライズ済みリソースとが、
	///          正しく相互に利用できなくなります。
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(ControlVariableCLRWrapper^ controlVariableCLRWrapper) {
			InitializeComponent();
			this->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::Form1_MouseWheel);	// マウスホイールイベントはデザイナで定義できないため手書きする
			this->controlVariableCLRWrapper = controlVariableCLRWrapper;
			physicsEnabledToolStripMenuItem->Checked = controlVariableCLRWrapper->physicsEnabled;
			boneMeshEnabledToolStripMenuItem->Checked = controlVariableCLRWrapper->boneMeshEnabled;
			rigidMeshEnabledToolStripMenuItem->Checked = controlVariableCLRWrapper->rigidMeshEnabled;
			jointMeshEnabledToolStripMenuItem->Checked = controlVariableCLRWrapper->jointMeshEnabled;
		}

	protected:
		/// <summary>
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileFToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openOToolStripMenuItem;
	private: ControlVariableCLRWrapper^ controlVariableCLRWrapper;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::ToolStripMenuItem^  optionOToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  physicsEnabledToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  boneMeshEnabledToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  rigidMeshEnabledToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  jointMeshEnabledToolStripMenuItem;
	private: Point mousePos;
		/// <summary>
		/// 必要なデザイナ変数です。
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// デザイナ サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディタで変更しないでください。
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileFToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openOToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->optionOToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->physicsEnabledToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->boneMeshEnabledToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->rigidMeshEnabledToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->jointMeshEnabledToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->menuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->fileFToolStripMenuItem, 
				this->optionOToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(638, 24);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileFToolStripMenuItem
			// 
			this->fileFToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->openOToolStripMenuItem});
			this->fileFToolStripMenuItem->Name = L"fileFToolStripMenuItem";
			this->fileFToolStripMenuItem->Size = System::Drawing::Size(51, 20);
			this->fileFToolStripMenuItem->Text = L"File(&F)";
			// 
			// openOToolStripMenuItem
			// 
			this->openOToolStripMenuItem->Name = L"openOToolStripMenuItem";
			this->openOToolStripMenuItem->Size = System::Drawing::Size(112, 22);
			this->openOToolStripMenuItem->Text = L"Open(&O)";
			this->openOToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::openOToolStripMenuItem_Click);
			// 
			// optionOToolStripMenuItem
			// 
			this->optionOToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->physicsEnabledToolStripMenuItem, 
				this->boneMeshEnabledToolStripMenuItem, this->rigidMeshEnabledToolStripMenuItem, this->jointMeshEnabledToolStripMenuItem});
			this->optionOToolStripMenuItem->Name = L"optionOToolStripMenuItem";
			this->optionOToolStripMenuItem->Size = System::Drawing::Size(66, 20);
			this->optionOToolStripMenuItem->Text = L"Option(&O)";
			// 
			// physicsEnabledToolStripMenuItem
			// 
			this->physicsEnabledToolStripMenuItem->CheckOnClick = true;
			this->physicsEnabledToolStripMenuItem->Name = L"physicsEnabledToolStripMenuItem";
			this->physicsEnabledToolStripMenuItem->Size = System::Drawing::Size(171, 22);
			this->physicsEnabledToolStripMenuItem->Text = L"Physics Enabled";
			this->physicsEnabledToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::physicsEnabledToolStripMenuItem_Click);
			// 
			// boneMeshEnabledToolStripMenuItem
			// 
			this->boneMeshEnabledToolStripMenuItem->CheckOnClick = true;
			this->boneMeshEnabledToolStripMenuItem->Name = L"boneMeshEnabledToolStripMenuItem";
			this->boneMeshEnabledToolStripMenuItem->Size = System::Drawing::Size(171, 22);
			this->boneMeshEnabledToolStripMenuItem->Text = L"Bone Mesh Enabled";
			this->boneMeshEnabledToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::boneMeshEnabledToolStripMenuItem_Click);
			// 
			// rigidMeshEnabledToolStripMenuItem
			// 
			this->rigidMeshEnabledToolStripMenuItem->CheckOnClick = true;
			this->rigidMeshEnabledToolStripMenuItem->Name = L"rigidMeshEnabledToolStripMenuItem";
			this->rigidMeshEnabledToolStripMenuItem->Size = System::Drawing::Size(171, 22);
			this->rigidMeshEnabledToolStripMenuItem->Text = L"Rigid Mesh Enabled";
			this->rigidMeshEnabledToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::rigidMeshEnabledToolStripMenuItem_Click);
			// 
			// jointMeshEnabledToolStripMenuItem
			// 
			this->jointMeshEnabledToolStripMenuItem->CheckOnClick = true;
			this->jointMeshEnabledToolStripMenuItem->Name = L"jointMeshEnabledToolStripMenuItem";
			this->jointMeshEnabledToolStripMenuItem->Size = System::Drawing::Size(171, 22);
			this->jointMeshEnabledToolStripMenuItem->Text = L"Joint Mesh Enabled";
			this->jointMeshEnabledToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::jointMeshEnabledToolStripMenuItem_Click);
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(638, 478);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"Form1";
			this->Text = L"MMD Viewer";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
			///// フォーム開始イベント /////
			// 視点、注視点を設定
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
				 controlVariableCLRWrapper->eyePoint = gcnew Vector3(10.0f, 20.0f, -50.0f);
				 controlVariableCLRWrapper->lookAtPoint= gcnew Vector3(0, 10.0f, 0);
			}

			 ///// メニューopenボタンクリックイベント /////
			 // ファイルを開く
	private: System::Void openOToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenFileDialog^ openFileDialog = gcnew OpenFileDialog();
				 openFileDialog->Filter = "pmdファイル(*.pmd)|*.pmd|vmdファイル(*.vmd)|*.vmd";
				 openFileDialog->RestoreDirectory = true;	// これをtrueにしないと、カレントディレクトリがファイルを開いたディレクトリに変更されてしまう！
				 if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
					 String^ extension = System::IO::Path::GetExtension(openFileDialog->FileName);
					 if (extension == ".pmd") {
						 controlVariableCLRWrapper->pmdFileName = openFileDialog->FileName;
						 controlVariableCLRWrapper->openPmdFileFlag = true;
					 }
					 else if (extension == ".vmd") {
						 controlVariableCLRWrapper->vmdFileName = openFileDialog->FileName;
						 controlVariableCLRWrapper->openVmdFileFlag = true;
					 }
				 }
			 }
			 ///// タイマイベント /////
			 // 視線の回転、並行移動
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
				 Point mv(0, 0);
				 if (!mousePos.IsEmpty) mv = Point(Control::MousePosition.X - mousePos.X, Control::MousePosition.Y - mousePos.Y);
				 Vector3^ v = controlVariableCLRWrapper->lookAtPoint - controlVariableCLRWrapper->eyePoint;	// 視線ベクトル
				 if (Control::MouseButtons == System::Windows::Forms::MouseButtons::Right) {
					 controlVariableCLRWrapper->eyePoint = controlVariableCLRWrapper->lookAtPoint - RotateViewVector(v, mv);
					 mousePos = Control::MousePosition;
				 }
				 else if (Control::MouseButtons == System::Windows::Forms::MouseButtons::Middle) {
					 Vector3^ t = TranslateViewVector(v, mv);
					 controlVariableCLRWrapper->lookAtPoint += t;
					 controlVariableCLRWrapper->eyePoint += t;
					 mousePos = Control::MousePosition;
				 }
				 else mousePos = Point::Empty;
			 }

			 /// ベクトルを規格化
	private: Vector3^ NormalizeVector3(Vector3^ v) {
				 float r = (float)(Math::Sqrt(v->X*v->X + v->Y*v->Y + v->Z*v->Z));
				 return gcnew Vector3(v->X/r, v->Y/r, v->Z/r);
			 }

			 /// カーソルの移動量に応じて視線ベクトルを回転
			 /// @param v 視線ベクトル 
			 /// @param mv カーソルの移動量
			 /// @return 回転した視線ベクトル
	private: Vector3^ RotateViewVector(Vector3^ v, Point mv) {
				 const float speed = 0.01f;	// 視線ベクトルの回転スピード
				 double r = Math::Sqrt(v->X*v->X + v->Y*v->Y + v->Z*v->Z);
				 double a = Math::Atan2(v->Z, v->X);
				 double b = Math::Acos(v->Y/r);
				 a -= speed*mv.X;
				 b += speed*mv.Y;
				 return gcnew Vector3((float)(r*Math::Sin(b)*Math::Cos(a)), (float)(r*Math::Cos(b)), (float)(r*Math::Sin(b)*Math::Sin(a)));
			 }

			 /// カーソルの移動量に応じて視線ベクトルの並行移動量を取得
			 /// @param v 視線ベクトル
			 /// @param mv カーソルの移動量
			 /// @return 視線ベクトルの移動量
	private: Vector3^ TranslateViewVector(Vector3^ v, Point mv) {
				 const float speed = 0.02f;
				 Vector3^ x = gcnew Vector3(-v->Z, 0, v->X);	// 視線とY軸に垂直な方向
				 x = speed*mv.X*NormalizeVector3(x);
				 Vector3^ y = gcnew Vector3(-v->Y*v->X, v->Z*v->Z + v->X*v->X, -v->Y*v->Z); // 視線とxに垂直な方向
				 y = speed*mv.Y*NormalizeVector3(y);
				 return x + y;
			 }

			 ///// マウスホイールイベント /////
			 // 視線距離を変更
	private: void Form1_MouseWheel(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
				 Vector3^ v = controlVariableCLRWrapper->lookAtPoint - controlVariableCLRWrapper->eyePoint;	// 視線ベクトル
				 v *= 1 - e->Delta/1200.0f;
				 controlVariableCLRWrapper->eyePoint = controlVariableCLRWrapper->lookAtPoint - v;
			 }

			 ///// メニューPhysics Enabledボタンクリックイベント /////
			 // 物理演算使用可能フラグを切り替える
	private: System::Void physicsEnabledToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 controlVariableCLRWrapper->physicsEnabled = ((ToolStripMenuItem^)sender)->Checked;
			 }

			 ///// メニューBone Mesh Enabledボタンクリックイベント /////
			 // ボーンメッシュ使用可能フラグを切り替える
	private: System::Void boneMeshEnabledToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 controlVariableCLRWrapper->boneMeshEnabled = ((ToolStripMenuItem^)sender)->Checked;
			 }

			 ///// メニューRigid Mesh Enabledボタンクリックイベント /////
			 // 剛体メッシュ使用可能フラグを切り替える
	private: System::Void rigidMeshEnabledToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 controlVariableCLRWrapper->rigidMeshEnabled = ((ToolStripMenuItem^)sender)->Checked;
			 }
		
			 ///// メニューJoint Mesh Enabledボタンクリックイベント /////
			 // ジョイントメッシュ使用可能フラグを切り替える
	private: System::Void jointMeshEnabledToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 controlVariableCLRWrapper->jointMeshEnabled = ((ToolStripMenuItem^)sender)->Checked;
			 }
};
}


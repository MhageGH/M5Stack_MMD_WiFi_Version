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
	/// Form1 �̊T�v
	///
	/// �x��: ���̃N���X�̖��O��ύX����ꍇ�A���̃N���X���ˑ����邷�ׂĂ� .resx �t�@�C���Ɋ֘A�t����ꂽ
	///          �}�l�[�W ���\�[�X �R���p�C�� �c�[���ɑ΂��� 'Resource File Name' �v���p�e�B��
	///          �ύX����K�v������܂��B���̕ύX���s��Ȃ��ƁA
	///          �f�U�C�i�ƁA���̃t�H�[���Ɋ֘A�t����ꂽ���[�J���C�Y�ς݃��\�[�X�Ƃ��A
	///          ���������݂ɗ��p�ł��Ȃ��Ȃ�܂��B
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(ControlVariableCLRWrapper^ controlVariableCLRWrapper) {
			InitializeComponent();
			this->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::Form1_MouseWheel);	// �}�E�X�z�C�[���C�x���g�̓f�U�C�i�Œ�`�ł��Ȃ����ߎ菑������
			this->controlVariableCLRWrapper = controlVariableCLRWrapper;
			physicsEnabledToolStripMenuItem->Checked = controlVariableCLRWrapper->physicsEnabled;
			boneMeshEnabledToolStripMenuItem->Checked = controlVariableCLRWrapper->boneMeshEnabled;
			rigidMeshEnabledToolStripMenuItem->Checked = controlVariableCLRWrapper->rigidMeshEnabled;
			jointMeshEnabledToolStripMenuItem->Checked = controlVariableCLRWrapper->jointMeshEnabled;
		}

	protected:
		/// <summary>
		/// �g�p���̃��\�[�X�����ׂăN���[���A�b�v���܂��B
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
		/// �K�v�ȃf�U�C�i�ϐ��ł��B
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// �f�U�C�i �T�|�[�g�ɕK�v�ȃ��\�b�h�ł��B���̃��\�b�h�̓��e��
		/// �R�[�h �G�f�B�^�ŕύX���Ȃ��ł��������B
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
			///// �t�H�[���J�n�C�x���g /////
			// ���_�A�����_��ݒ�
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
				 controlVariableCLRWrapper->eyePoint = gcnew Vector3(10.0f, 20.0f, -50.0f);
				 controlVariableCLRWrapper->lookAtPoint= gcnew Vector3(0, 10.0f, 0);
			}

			 ///// ���j���[open�{�^���N���b�N�C�x���g /////
			 // �t�@�C�����J��
	private: System::Void openOToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 OpenFileDialog^ openFileDialog = gcnew OpenFileDialog();
				 openFileDialog->Filter = "pmd�t�@�C��(*.pmd)|*.pmd|vmd�t�@�C��(*.vmd)|*.vmd";
				 openFileDialog->RestoreDirectory = true;	// �����true�ɂ��Ȃ��ƁA�J�����g�f�B���N�g�����t�@�C�����J�����f�B���N�g���ɕύX����Ă��܂��I
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
			 ///// �^�C�}�C�x���g /////
			 // �����̉�]�A���s�ړ�
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
				 Point mv(0, 0);
				 if (!mousePos.IsEmpty) mv = Point(Control::MousePosition.X - mousePos.X, Control::MousePosition.Y - mousePos.Y);
				 Vector3^ v = controlVariableCLRWrapper->lookAtPoint - controlVariableCLRWrapper->eyePoint;	// �����x�N�g��
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

			 /// �x�N�g�����K�i��
	private: Vector3^ NormalizeVector3(Vector3^ v) {
				 float r = (float)(Math::Sqrt(v->X*v->X + v->Y*v->Y + v->Z*v->Z));
				 return gcnew Vector3(v->X/r, v->Y/r, v->Z/r);
			 }

			 /// �J�[�\���̈ړ��ʂɉ����Ď����x�N�g������]
			 /// @param v �����x�N�g�� 
			 /// @param mv �J�[�\���̈ړ���
			 /// @return ��]���������x�N�g��
	private: Vector3^ RotateViewVector(Vector3^ v, Point mv) {
				 const float speed = 0.01f;	// �����x�N�g���̉�]�X�s�[�h
				 double r = Math::Sqrt(v->X*v->X + v->Y*v->Y + v->Z*v->Z);
				 double a = Math::Atan2(v->Z, v->X);
				 double b = Math::Acos(v->Y/r);
				 a -= speed*mv.X;
				 b += speed*mv.Y;
				 return gcnew Vector3((float)(r*Math::Sin(b)*Math::Cos(a)), (float)(r*Math::Cos(b)), (float)(r*Math::Sin(b)*Math::Sin(a)));
			 }

			 /// �J�[�\���̈ړ��ʂɉ����Ď����x�N�g���̕��s�ړ��ʂ��擾
			 /// @param v �����x�N�g��
			 /// @param mv �J�[�\���̈ړ���
			 /// @return �����x�N�g���̈ړ���
	private: Vector3^ TranslateViewVector(Vector3^ v, Point mv) {
				 const float speed = 0.02f;
				 Vector3^ x = gcnew Vector3(-v->Z, 0, v->X);	// ������Y���ɐ����ȕ���
				 x = speed*mv.X*NormalizeVector3(x);
				 Vector3^ y = gcnew Vector3(-v->Y*v->X, v->Z*v->Z + v->X*v->X, -v->Y*v->Z); // ������x�ɐ����ȕ���
				 y = speed*mv.Y*NormalizeVector3(y);
				 return x + y;
			 }

			 ///// �}�E�X�z�C�[���C�x���g /////
			 // ����������ύX
	private: void Form1_MouseWheel(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
				 Vector3^ v = controlVariableCLRWrapper->lookAtPoint - controlVariableCLRWrapper->eyePoint;	// �����x�N�g��
				 v *= 1 - e->Delta/1200.0f;
				 controlVariableCLRWrapper->eyePoint = controlVariableCLRWrapper->lookAtPoint - v;
			 }

			 ///// ���j���[Physics Enabled�{�^���N���b�N�C�x���g /////
			 // �������Z�g�p�\�t���O��؂�ւ���
	private: System::Void physicsEnabledToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 controlVariableCLRWrapper->physicsEnabled = ((ToolStripMenuItem^)sender)->Checked;
			 }

			 ///// ���j���[Bone Mesh Enabled�{�^���N���b�N�C�x���g /////
			 // �{�[�����b�V���g�p�\�t���O��؂�ւ���
	private: System::Void boneMeshEnabledToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 controlVariableCLRWrapper->boneMeshEnabled = ((ToolStripMenuItem^)sender)->Checked;
			 }

			 ///// ���j���[Rigid Mesh Enabled�{�^���N���b�N�C�x���g /////
			 // ���̃��b�V���g�p�\�t���O��؂�ւ���
	private: System::Void rigidMeshEnabledToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 controlVariableCLRWrapper->rigidMeshEnabled = ((ToolStripMenuItem^)sender)->Checked;
			 }
		
			 ///// ���j���[Joint Mesh Enabled�{�^���N���b�N�C�x���g /////
			 // �W���C���g���b�V���g�p�\�t���O��؂�ւ���
	private: System::Void jointMeshEnabledToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 controlVariableCLRWrapper->jointMeshEnabled = ((ToolStripMenuItem^)sender)->Checked;
			 }
};
}


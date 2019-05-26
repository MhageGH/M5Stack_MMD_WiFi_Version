#pragma once

/// DirectX��{�N���X
/// ���̃N���X�̓V�[���̊J�n�E�I�����s�Ȃ��B �`��̓V�[�����ɍs���B
class DirectXFramework {
private:
	LPDIRECT3D9	pD3d;
	LPDIRECT3DDEVICE9 pDevice;					// Direct3D�f�o�C�X�I�u�W�F�N�g

private: // �������⏕�֐�
	void CreateDirect3DDeviceObject(HWND hWnd);	// DIRECT3D�f�o�C�X�I�u�W�F�N�g�̍쐬
	void SetRenderingConfiguration();			// �����_�����O�ݒ�
	void SetFogConfiguration();					// �t�H�O�̐ݒ�

public:
	/// �R���X�g���N�^
	DirectXFramework(HWND hWnd);

	/// �f�X�g���N�^
	~DirectXFramework();

	/// Direct3D�f�o�C�X�I�u�W�F�N�g���擾
	LPDIRECT3DDEVICE9 GetD3DDevice();

	/// ��ʂ�w�i�F�ŃN���A���A�V�[�����J�n����B�`��֐��̓V�[�����ɌĂяo��
	/// �����F�w�i�F��RGB�����B0�`255
	void BeginScene(int red, int green, int blue);

	/// �V�[�����I������
	void EndScene();
};

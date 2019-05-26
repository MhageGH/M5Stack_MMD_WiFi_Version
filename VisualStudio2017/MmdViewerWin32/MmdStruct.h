#pragma once


/// MMD��p�\���̒�`
namespace MmdStruct {
#pragma pack(push,1)	//�A���C�������g������I�t

	/// PMD�\���̒�`
	struct PmdHeader {
		unsigned char magic[3];
		float version;
		unsigned char model_name[20];
		unsigned char comment[256];
	};
	struct PmdVertex{
		float pos[3];
		float normal_vec[3];
		float uv[2];
		unsigned short bone_num[2];
		unsigned char bone_weight;
		unsigned char edge_flag;
	};
	struct PmdMaterial{
		float diffuse_color[3];
		float alpha;
		float specularity;
		float specular_color[3];
		float mirror_color[3];
		unsigned char toon_index;
		unsigned char edge_flag;
		unsigned long face_vert_count;	// ���̍ޗ��̖ʒ��_�� �� �ޗ��ԍ�i�̃|���S���ԍ��F pmdMaterial[i - 1].face_vert_count/3 �` pmdMaterial[i].face_vert_count/3 - 1
		char texture_file_name[20];
	};
	struct PmdBone {
		char bone_name[20];
		unsigned short parent_bone_index;	// �Ȃ��ꍇ��0xFFFF
		unsigned short tail_pos_bone_index;	// �Ȃ��ꍇ��0xFFFF
		unsigned char bone_type;			// 0:��] 1:��]�ƈړ� 2:IK 3:�s�� 4:IK�e���� 5:��]�e���� 6:IK�ڑ��� 7:��\�� 8:�P�� 9:��]�^�� (8, 9��MMD4.0�ȍ~)
		unsigned short ik_parent_bone_index;// �Ȃ��ꍇ��0
		float bone_head_pos[3];
	};
	struct PmdIkDataWithoutArray {
		unsigned short ik_bone_index;			// IK�{�[���ԍ�
		unsigned short ik_target_bone_index;	// IK�{�[���̈ʒu�ɂ��̃{�[������v������悤��IK�������s����
		unsigned char ik_chain_length;			// IK�`�F�[���̒���
		unsigned short iterations;				// �ċA���Z��
		float control_weight;					// �{�[���̒P�ʐ����p 1.0 �� 4.0[rad]�B�܂��u�Ђ��v���܂ރ{�[���������{�[����X�������ɂ��������Ȃ�����������B
		// unsigned short ik_child_bone_index[ik_chain_length]; // IK�e�����̃{�[���ԍ� (�z��̑傫�����ω�)
	};
	struct PmdIkData : public PmdIkDataWithoutArray {
		vector<unsigned short> ik_child_bone_index;
	};
	struct PmdSkinDataWithoutArray {
		char skin_name[20];						// �\�
		unsigned long skin_vert_count;			// �\��p�̒��_��
		unsigned char skin_type;				// �\��̎�� �� 0:base�A1:�܂�A2:�ځA3:���b�v�A4:���̑�
		// PmdSkinVertexData skin_vert_data[skin_vert_count];	// (�z��̑傫�����ω�)
	};
	struct PmdSkinVertexData {
		unsigned long skin_vert_index;			// �\��p�̒��_�̔ԍ�(���_���X�g�ɂ���ԍ�)
		float skin_vert_pos[3];
	};
	struct PmdSkinData : public PmdSkinDataWithoutArray {
		vector<PmdSkinVertexData> skin_vert_data;//�\��p���_�f�[�^
	};
	struct PmdBoneDisp {
		unsigned short bone_index;				// �g�p�{�[���ԍ�
		unsigned char bone_disp_frame_index;	// �\���g�ԍ�
	};
	struct PmdRigidBody {
		char rigidbody_name[20];				// ����
		unsigned short rigidbody_rel_bone_index;// �֘A�{�[���ԍ� �i�Ȃ���0xFFFF�j
		unsigned char rigidbody_group_index;	// �O���[�v(1�`16)
		unsigned short rigidbody_group_target;	// ��Փ˃O���[�v(�r�b�g�P��)
		unsigned char shape_type;				// �`��^�C�v 0:�� 1:�� 2:�J�v�Z��
		float shape_w;							// ��/2(���͔��a)
		float shape_h;							// ����/2(�J�v�Z���̏ꍇ�͋����S�Ԃ̋���)
		float shape_d;							// ���s/2
		float pos_pos[3];						// �ʒu (x, y, z)  : �֘A�{�[��������ꍇ�́A���̃^�C�v�ɂ�炸�֘A�{�[���ʒu����̑��Έʒu(PMD�G�f�B�^�ł̓��f���̃��[�J�����W�n�ł���A�l���Ⴄ�̂Œ��ӁI)
		float pos_rot[3];						// ��] : ��]�s��� D3DXMatrixRotationYawPitchRoll(&rotation, pos_rot[1], pos_rot[0], pos_rot[2])
		float rigidbody_weight;					// ����
		float rigidbody_pos_dim;				// �ړ�����
		float rigidbody_rot_dim;				// ��]����
		float rigidbody_recoil;					// ������
		float rigidbody_friction;				// ���C��
		unsigned char rigidbody_type;			// ���̃^�C�v 0:Bone�Ǐ] 1:�������Z 2:�������Z(Bone�ʒu���킹)
	};
	struct PmdJoint {
		char joint_name[20];					// ����
		unsigned long joint_rigidbody_a;		// ����A
		unsigned long joint_rigidbody_b;		// ����B
		float joint_pos[3];						// �ʒu (x, y, z) : ���f�����[�J�����W�n
		float joint_rot[3];						// ��] (rad(x), rad(y), rad(z))
		float constrain_pos_1[3];				// �ړ��������� (x, y, z)
		float constrain_pos_2[3];				// �ړ�������� (x, y, z)
		float constrain_rot_1[3];				// ��]�������� (rad(x), rad(y), rad(z))
		float constrain_rot_2[3];				// ��]������� (rad(x), rad(y), rad(z))
		float spring_pos[3];					// �ړ��΂� (x, y, z)
		float spring_rot[3];					// ��]�΂� (rad(x), rad(y), rad(z))
	};


	/// VMD�\���̒�`
	struct VmdHeader {
		char vmdHeader[30];
		char vmdModelName[20];
	};
	struct VmdMotion {
		char boneName[15];
		unsigned long frameNo;
		float location[3];	// �ړ���
		float rotation[4];	// ���f�����[�J�����W�n
		unsigned char interpolation[64];
	};
#pragma pack(pop)
}
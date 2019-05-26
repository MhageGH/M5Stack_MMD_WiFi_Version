#pragma once


/// MMD専用構造体定義
namespace MmdStruct {
#pragma pack(push,1)	//アラインメント制御をオフ

	/// PMD構造体定義
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
		unsigned long face_vert_count;	// この材料の面頂点数 → 材料番号iのポリゴン番号： pmdMaterial[i - 1].face_vert_count/3 ～ pmdMaterial[i].face_vert_count/3 - 1
		char texture_file_name[20];
	};
	struct PmdBone {
		char bone_name[20];
		unsigned short parent_bone_index;	// ない場合は0xFFFF
		unsigned short tail_pos_bone_index;	// ない場合は0xFFFF
		unsigned char bone_type;			// 0:回転 1:回転と移動 2:IK 3:不明 4:IK影響下 5:回転影響下 6:IK接続先 7:非表示 8:捻り 9:回転運動 (8, 9はMMD4.0以降)
		unsigned short ik_parent_bone_index;// ない場合は0
		float bone_head_pos[3];
	};
	struct PmdIkDataWithoutArray {
		unsigned short ik_bone_index;			// IKボーン番号
		unsigned short ik_target_bone_index;	// IKボーンの位置にこのボーンを一致させるようにIK処理が行われる
		unsigned char ik_chain_length;			// IKチェーンの長さ
		unsigned short iterations;				// 再帰演算回数
		float control_weight;					// ボーンの単位制限角 1.0 → 4.0[rad]。また「ひざ」を含むボーン名をもつボーンはX軸方向にしか動かない制限がある。
		// unsigned short ik_child_bone_index[ik_chain_length]; // IK影響下のボーン番号 (配列の大きさが変化)
	};
	struct PmdIkData : public PmdIkDataWithoutArray {
		vector<unsigned short> ik_child_bone_index;
	};
	struct PmdSkinDataWithoutArray {
		char skin_name[20];						// 表情名
		unsigned long skin_vert_count;			// 表情用の頂点数
		unsigned char skin_type;				// 表情の種類 → 0:base、1:まゆ、2:目、3:リップ、4:その他
		// PmdSkinVertexData skin_vert_data[skin_vert_count];	// (配列の大きさが変化)
	};
	struct PmdSkinVertexData {
		unsigned long skin_vert_index;			// 表情用の頂点の番号(頂点リストにある番号)
		float skin_vert_pos[3];
	};
	struct PmdSkinData : public PmdSkinDataWithoutArray {
		vector<PmdSkinVertexData> skin_vert_data;//表情用頂点データ
	};
	struct PmdBoneDisp {
		unsigned short bone_index;				// 枠用ボーン番号
		unsigned char bone_disp_frame_index;	// 表示枠番号
	};
	struct PmdRigidBody {
		char rigidbody_name[20];				// 名称
		unsigned short rigidbody_rel_bone_index;// 関連ボーン番号 （なし→0xFFFF）
		unsigned char rigidbody_group_index;	// グループ(1～16)
		unsigned short rigidbody_group_target;	// 非衝突グループ(ビット単位)
		unsigned char shape_type;				// 形状タイプ 0:球 1:箱 2:カプセル
		float shape_w;							// 幅/2(又は半径)
		float shape_h;							// 高さ/2(カプセルの場合は球中心間の距離)
		float shape_d;							// 奥行/2
		float pos_pos[3];						// 位置 (x, y, z)  : 関連ボーンがある場合は、剛体タイプによらず関連ボーン位置からの相対位置(PMDエディタではモデルのローカル座標系であり、値が違うので注意！)
		float pos_rot[3];						// 回転 : 回転行列は D3DXMatrixRotationYawPitchRoll(&rotation, pos_rot[1], pos_rot[0], pos_rot[2])
		float rigidbody_weight;					// 質量
		float rigidbody_pos_dim;				// 移動減衰
		float rigidbody_rot_dim;				// 回転減衰
		float rigidbody_recoil;					// 反発力
		float rigidbody_friction;				// 摩擦力
		unsigned char rigidbody_type;			// 剛体タイプ 0:Bone追従 1:物理演算 2:物理演算(Bone位置あわせ)
	};
	struct PmdJoint {
		char joint_name[20];					// 名称
		unsigned long joint_rigidbody_a;		// 剛体A
		unsigned long joint_rigidbody_b;		// 剛体B
		float joint_pos[3];						// 位置 (x, y, z) : モデルローカル座標系
		float joint_rot[3];						// 回転 (rad(x), rad(y), rad(z))
		float constrain_pos_1[3];				// 移動制限下限 (x, y, z)
		float constrain_pos_2[3];				// 移動制限上限 (x, y, z)
		float constrain_rot_1[3];				// 回転制限下限 (rad(x), rad(y), rad(z))
		float constrain_rot_2[3];				// 回転制限上限 (rad(x), rad(y), rad(z))
		float spring_pos[3];					// 移動ばね (x, y, z)
		float spring_rot[3];					// 回転ばね (rad(x), rad(y), rad(z))
	};


	/// VMD構造体定義
	struct VmdHeader {
		char vmdHeader[30];
		char vmdModelName[20];
	};
	struct VmdMotion {
		char boneName[15];
		unsigned long frameNo;
		float location[3];	// 移動量
		float rotation[4];	// モデルローカル座標系
		unsigned char interpolation[64];
	};
#pragma pack(pop)
}
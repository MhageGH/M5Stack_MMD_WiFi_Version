float4x4 world[61];	// 定数レジスタは256個 → (256 - 4 - 4- 1 - 1)/4 = 61のボーン行列がMAX
float4x4 view;
float4x4 proj;
float4 ambient;
float4 diffuse;
texture tex;

sampler TextureSampler = sampler_state {
	Texture = <tex>;
};

// ライト方向
float3 lightDir;

struct VS_IN {
	float3 pos : POSITION;
	float3 blend : BLENDWEIGHT; 	
	int4 idx : BLENDINDICES;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD0;
};

struct VS_OUT {
	float4 pos : POSITION;
	float4 color : COLOR0;
	float2 uv : TEXCOORD0;
};

// 頂点シェーダ
VS_OUT BlendVS(VS_IN In) {
	VS_OUT Out = (VS_OUT)0;
	float w[3] = (float[3])In.blend;
	float4x4 comb = (float4x4)0;
	for (int i = 0; i < 3; ++i) comb += world[In.idx[i]]*w[i];
	comb += world[In.idx[3]] * (1.0f - w[0] - w[1] - w[2]);
	float4 pos = mul( float4(In.pos, 1.0), comb );
	float4 normal_head = mul(float4(In.pos + In.normal, 1), comb);
	float3 normal = normalize(normal_head.xyz - pos.xyz);
	float4 color = ambient + diffuse*max(0, dot(normal, -normalize(lightDir)));
	Out.pos = pos;
	Out.pos = mul(Out.pos, view);
	Out.pos = mul(Out.pos, proj);
	Out.color = color;
	Out.uv = In.uv;
	return Out;
}

// ピクセルシェーダ
float4 BlendPS(VS_OUT In, uniform bool b_tex) : COLOR {
	if (b_tex) return In.color*tex2D(TextureSampler, In.uv);
	else return In.color;	
}

// テクニック
technique BlendTech {
	pass P0	{
		VertexShader = compile vs_3_0 BlendVS();
		PixelShader = compile ps_3_0 BlendPS(true);
	}
	pass P1	{
		VertexShader = compile vs_3_0 BlendVS();
		PixelShader = compile ps_3_0 BlendPS(false);
	}
}
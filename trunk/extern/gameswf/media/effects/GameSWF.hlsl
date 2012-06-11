float4x4 WorldViewProjectionMatrix : WorldViewProjection;

float4 DiffuseColor;

struct v2f 
{
    float4 Position:    POSITION;
    float2 TexCoord0:   TEXCOORD0;
    float4 Color:       COLOR;
};

v2f VS(float3 Position: POSITION,
	   float2 TexCoord0: TEXCOORD0,
	   float4 color:COLOR
	   ) 
{
    v2f output;
    output.Position = mul(WorldViewProjectionMatrix, float4(Position.xyz,1));
    output.TexCoord0 = TexCoord0;
    output.Color = color;
    //output.Color *= DiffuseColor;
    return output;
}

sampler2D TextureSampler;

float4 FS(v2f input) : COLOR 
{
	float4 color *= tex2D(TextureSampler, input.TexCoord0) + DiffuseColor;
	color *= input.Color;
    return color;
}

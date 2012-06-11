uniform lowp sampler2D TextureSampler;
uniform lowp vec4 DiffuseColor;

varying mediump vec2 vTexCoord0;
varying lowp vec4 vColor0;

void main()
{
	lowp vec4 color = texture2D(TextureSampler, vTexCoord0) + DiffuseColor;
	color *= vColor0;
	color.rgb *= color.a;

	gl_FragColor = color;
}
uniform lowp sampler2D uTextureUnit0;

varying mediump vec2 varTexCoord0;
varying mediump vec4 varVertexColor;

void main (void)
{	
	mediump vec4 texel = texture2D(uTextureUnit0, varTexCoord0.xy);	
	gl_FragColor = texel * varVertexColor;
}
uniform lowp sampler2D uTextureUnit0;

varying highp vec2 varTexCoord0;
varying highp vec4 varVertexColor;

void main (void)
{	
	highp vec4 texel = texture2D(uTextureUnit0, varTexCoord0.xy);	
	gl_FragColor = texel * varVertexColor;
}
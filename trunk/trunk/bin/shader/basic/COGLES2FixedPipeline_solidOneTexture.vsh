attribute mediump vec4 inVertexPosition;
attribute mediump vec4 inVertexColor;
attribute mediump vec2 inTexCoord0;

uniform mediump mat4 uMvpMatrix;

varying mediump vec4 varVertexColor;
varying mediump vec2 varTexCoord0;

void main(void)
{
	varVertexColor = inVertexColor;
	varTexCoord0 = inTexCoord0;
	gl_Position = uMvpMatrix * inVertexPosition;	
}
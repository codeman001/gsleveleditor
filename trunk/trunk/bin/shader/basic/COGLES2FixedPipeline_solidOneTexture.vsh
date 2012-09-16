attribute highp vec4 inVertexPosition;
attribute highp vec4 inVertexColor;
attribute highp vec2 inTexCoord0;

uniform highp mat4 uMvpMatrix;

varying highp vec4 varVertexColor;
varying highp vec2 varTexCoord0;

void main(void)
{
	varVertexColor = inVertexColor;
	varTexCoord0 = inTexCoord0;
	gl_Position = uMvpMatrix * inVertexPosition;	
}
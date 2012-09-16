attribute highp vec4 inVertexPosition;
attribute highp vec4 inVertexColor;

uniform highp mat4 uMvpMatrix;

varying highp vec4 varVertexColor;

void main(void)
{
	varVertexColor = inVertexColor;	
	gl_Position = uMvpMatrix * inVertexPosition;	
}
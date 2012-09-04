attribute vec4 inVertexPosition;
attribute vec2 inTexCoord0;
	
uniform mat4 uMvpMatrix;
uniform mat4 uPrjMatrix;	

varying vec2 originalCoord;

void main() 
{
	gl_Position = uPrjMatrix * (uMvpMatrix * vec4(0.0, 0.0, 0.0, 1.0) + vec4(inVertexPosition.x, inVertexPosition.y, 0.0, 0.0));
	originalCoord = inTexCoord0;
}
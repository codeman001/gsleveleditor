void main() 
{
	gl_Position = gl_ProjectionMatrix * (gl_ModelViewMatrix * vec4(0.0, 0.0, 0.0, 1.0) + vec4(gl_Vertex.x, gl_Vertex.y, 0.0, 0.0));
	gl_TexCoord[0] = gl_MultiTexCoord0;
}
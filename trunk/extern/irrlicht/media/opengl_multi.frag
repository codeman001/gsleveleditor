
uniform sampler2D myTexture;
uniform sampler2D myTexture1;

void main (void)
{
    vec4 col 	= texture2D(myTexture,  vec2(gl_TexCoord[0]));
	vec4 col2 	= texture2D(myTexture1, vec2(gl_TexCoord[0]));
    
	col += (col2*0.5);
		
    gl_FragColor = col;
}

uniform sampler2D grassTexture;

void main (void)
{
	gl_FragColor = texture2D( grassTexture, gl_TexCoord[0].st) ;
}

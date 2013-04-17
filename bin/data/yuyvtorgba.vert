// vertex
void main(void){
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1] = gl_MultiTexCoord0 + vec4(1,0,0,0);
	gl_Position = ftransform();
}
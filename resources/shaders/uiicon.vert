void main()
{
	// pass through vertex position
	gl_Position = ftransform();
	
	// pass through texture coords
	gl_TexCoord[0] = gl_MultiTexCoord0;
}

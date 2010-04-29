varying vec3 N;
varying vec3 v;

void main(void)
{    
    v = vec3(gl_ModelViewMatrix * gl_Vertex);
    N = normalize(gl_NormalMatrix * gl_Normal);
    gl_ClipVertex = gl_ModelViewMatrix * gl_Vertex;
    
    gl_Position = ftransform();
    gl_TexCoord[0] = gl_MultiTexCoord0;
}


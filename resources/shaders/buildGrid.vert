uniform vec3 light_position;


varying vec3 lightPos;
varying vec3 N;
varying vec3 v;
varying vec3 pos;

void main(void)
{    
    v = vec3(gl_ModelViewMatrix * gl_Vertex);
    pos = vec3(gl_ModelViewProjectionMatrix * gl_Vertex);
    lightPos = light_position - v;
    N = gl_NormalMatrix * gl_Normal;
    gl_ClipVertex = gl_ModelViewMatrix * gl_Vertex;
    
    gl_Position = ftransform();
    gl_TexCoord[0] = gl_MultiTexCoord0;
}


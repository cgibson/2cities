//test
struct gl_Light {
		vec3 position;    
		vec4 diffuse;    
		vec4 ambient;   
};

uniform gl_Light light;

varying vec3 lightPos;
varying vec3 N;
varying vec3 v;

void main(void)
{    
    v = vec3(gl_ModelViewMatrix * gl_Vertex);
    lightPos = light.position - v;
    N = gl_NormalMatrix * gl_Normal;
    gl_ClipVertex = gl_ModelViewMatrix * gl_Vertex;
    
    gl_Position = ftransform();
    gl_TexCoord[0] = gl_MultiTexCoord0;
}


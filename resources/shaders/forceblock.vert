// not used in the vertex shader
uniform float force;
uniform float shock;
uniform vec3 light_position;

varying vec3 normal;
varying vec3 v;
varying vec3 lightPos;

void main()
{
	vec3 pos = vec3(gl_ModelViewMatrix * gl_Vertex);
	v = vec3(gl_ModelViewProjectionMatrix * gl_Vertex);
    // just pass through the vertex position
    gl_Position = ftransform();
    lightPos = light_position - pos;
    
    normal = normalize(gl_NormalMatrix * gl_Normal);
    
    // pass through texture coordinates
    gl_TexCoord[0] = gl_MultiTexCoord0;
}

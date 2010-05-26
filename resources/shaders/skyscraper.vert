// passed in from draw call
uniform vec3 light_pos;
uniform vec3 eye_pos;

// passed to fragment shader
varying vec3 view_vec;
varying vec3 light_vec;
varying vec3 normal_vec;
   
void main()
{
    // just pass through the vertex position
    gl_Position = ftransform();
    
    // calculate the view and light vectors based on its world position
    vec4 world_pos = gl_ModelViewMatrix * gl_Vertex;
    view_vec = eye_pos - world_pos.xyz;
    light_vec = light_pos - world_pos.xyz;
    
    // pass through texture coordinates
    gl_TexCoord[0] = gl_MultiTexCoord0;
    
    // transform the normal
    normal_vec = gl_NormalMatrix * gl_Normal;
}

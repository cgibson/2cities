varying vec3 normal;
varying vec3 v_eyespace;

void main(void)
{    
    v_eyespace = (gl_ModelViewMatrix * gl_Vertex).xyz;
    
    normal = normalize(gl_NormalMatrix * gl_Normal);
    
    gl_Position = ftransform();
}


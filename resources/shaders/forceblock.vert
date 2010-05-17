uniform float force;
uniform float edge_strength;

void main()
{
    // just pass through the vertex position
    gl_Position = ftransform();
    
    // pass through texture coordinates
    gl_TexCoord[0] = gl_MultiTexCoord0;
}

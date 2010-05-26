/*
 * This is the fragment half of the per-pixel lighting shader code.  This code 
 * takes the N and L vector values and interpolates the lighting values
 */

varying vec3 N;
varying vec3 v; 
  
void main (void) 
{

    N = normalize(N);
    vec4 finalColor = vec4(0.0, 0.0, 0.0, 0.0);
    
    vec3 aux = (gl_LightSource[0].position.xyz-v);
    vec3 L = normalize(aux);
    vec3 E = normalize(-v);
    vec3 R = normalize(-reflect(L,N));
    float dist = length(aux);
    vec4 Ispec = vec4(0);
    float NdotL = max(0.0, dot(N,L));

    vec4 diffuse = gl_Color;
    
    vec4 Iamb = max(gl_FrontLightProduct[0].ambient, gl_FrontMaterial.ambient);
    
    vec4 Idiff = (diffuse * max(dot(N,L), 0.0));
    Idiff = clamp(Idiff, 0.0, 1.0);
    
    if(NdotL > 0.0)
    {
    Ispec = gl_FrontMaterial.specular
    * pow(max(dot(R,E),0.0),0.3*gl_FrontMaterial.shininess);
    Ispec *= max(dot(N,L) , 0.0);
    Ispec = clamp(Ispec, 0.0, 1.0);
    }
    
    finalColor += (Iamb + Idiff + Ispec);
    
    gl_FragColor = gl_FrontLightModelProduct.sceneColor + finalColor;
  }

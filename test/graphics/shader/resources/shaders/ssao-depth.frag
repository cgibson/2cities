varying vec3 normal; 
varying vec3 v_eyespace;

float linearize(float z)
{
  float n = 1;
  float f = 400.0;
  return z / 400;//(z - n) / (f - n);
}
  
void main (void) 
{

    vec3 normalVec = normalize(normal);
    vec4 phong;
    

    vec3 N = normalize(normal);
    vec4 finalColor = vec4(0.0, 0.0, 0.0, 0.0);
    
    vec3 aux = (gl_LightSource[0].position.xyz- v_eyespace);
    vec3 L = normalize(aux);
    vec3 E = normalize(-v_eyespace);
    vec3 R = normalize(-reflect(L,N));
    float dist = length(aux);
    vec4 Ispec = vec4(0);
    float NdotL = max(0.0, dot(N,L));

    vec4 diffuse = gl_FrontMaterial.diffuse * gl_FrontLightProduct[0].diffuse;
    
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
    
    //finalColor += (Iamb + Idiff + Ispec);
    finalColor = diffuse;

    
    gl_FragData[0] = vec4(normal, 1);
    gl_FragData[1] = finalColor;
    gl_FragData[2] = vec4(linearize(length(v_eyespace)));
  }

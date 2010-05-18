struct Material {
		vec4 ambient;    
		vec4 diffuse;    
		vec4 specular;   
		vec4 shininess; 
};

struct Light {
		vec3 position;    
		vec3 diffuse;    
		vec3 ambient;   
};

uniform Material frontMat;
uniform Light light;

varying vec3 N;
varying vec3 v; 
  
void main (void) 
{
    vec4 ambient = vec4(0.1,0.1,0.1,1);
    vec4 diffuse = vec4(1.0,0,0,1);
    vec3 norm = normalize(N);
    vec4 finalColor = vec4(0.0, 0.0, 0.0, 0.0);
    
    vec3 aux = (light.position-v);
    vec3 L = normalize(aux);
    vec3 E = normalize(-v);
    vec3 R = normalize(-reflect(L,norm));
    float dist = length(aux);
    vec4 Ispec = vec4(0);
    float NdotL = max(0.0, dot(norm,L));

    //vec4 diffuse = diffuse;// * vec4(light.diffuse,1);
    
    vec4 Iamb = ambient;
    
    vec4 Idiff = (diffuse * max(dot(norm,L), 0.0));
    Idiff = clamp(Idiff, 0.0, 1.0);
    
    if(NdotL > 0.0)
    {
    Ispec = gl_FrontMaterial.specular
    * pow(max(dot(R,E),0.0),0.3*frontMat.shininess[0]);
    Ispec *= max(dot(norm,L) , 0.0);
    Ispec = clamp(Ispec, 0.0, 1.0);
    }
    
    finalColor += (Iamb + Idiff + Ispec);
    
    gl_FragColor = finalColor;
  }

//uniform gl_Test material;

struct gl_Material {
		vec4 ambient;    
		vec4 diffuse;    
		vec4 specular;   
		vec4 shininess; 
};

struct gl_Light {
		vec3 position;    
		vec4 diffuse;    
		vec4 ambient;   
};

uniform gl_Material material;
uniform gl_Light light;

varying vec3 N;
varying vec3 v; 
  
void main (void) 
{
    vec3 norm = normalize(N);
    vec4 finalColor = vec4(0.0, 0.0, 0.0, 0.0);
    
    vec3 aux = (vec3(50,50,50)-v);
    vec3 L = normalize(aux);
    vec3 E = normalize(-v);
    vec3 R = normalize(-reflect(L,norm));
    float dist = length(aux);
    vec4 Ispec = vec4(0);
    float NdotL = max(0.0, dot(norm,L));

    vec4 diffuse = material.diffuse * light.diffuse;
    
    vec4 Iamb = material.ambient;
    
    vec4 Idiff = (material.diffuse * max(dot(norm,L), 0.0));
    Idiff = clamp(Idiff, 0.0, 1.0);
    
    if(NdotL > 0.0)
    {
    Ispec = material.specular
    * pow(max(dot(R,E),0.0),0.3*material.shininess[0]);
    Ispec *= max(dot(norm,L) , 0.0);
    Ispec = clamp(Ispec, 0.0, 1.0);
    }
    
    finalColor += (Iamb + Idiff);// + Ispec);
    
    gl_FragColor = finalColor;
  }

//uniform gl_Test material;

//TODO: change to include structures
/*struct gl_Material {
		vec4 ambient;    
		vec4 diffuse;    
		vec4 specular;   
		vec4 shininess; 
};*/
uniform vec4 material_ambient;
uniform vec4 material_diffuse;
uniform vec4 material_specular;
uniform float material_shininess;
/*
struct gl_Light {
		vec3 position;    
		vec4 diffuse;    
		vec4 ambient;   
};*/
uniform vec3 light_position;
uniform vec4 light_diffuse;
uniform vec4 light_ambient;
//uniform gl_Material material;
//uniform gl_Light light;

varying vec3 lightPos;
varying vec3 N;
varying vec3 v;
  
void main (void) 
{
    vec3 view_vec = normalize(v);
    vec3 light_vec = normalize(lightPos);
    vec3 normal = normalize(N);
    
    vec3 reflection = normalize( -reflect(view_vec,normal) );

    vec4 finalColor = vec4(0.0, 0.0, 0.0, 1.0);

    float NdotL = clamp(dot(normal,light_vec), 0.0, 1.0);

    vec4 diffuse = material_diffuse * NdotL;//* light.diffuse * NdotL;
    vec4 ambient = material_ambient;// * light.ambient;
    vec4 phong = ambient + diffuse;
    
    /*if(NdotL > 0.0)
    {
    Ispec = material.specular
    * pow(max(dot(R,E),0.0),0.3*material.shininess[0]);
    Ispec *= max(dot(norm,L) , 0.0);
    Ispec = clamp(Ispec, 0.0, 1.0);
    }*/
    
    finalColor += phong;// + Ispec);
    
    gl_FragData[0] = finalColor;
    gl_FragData[1] = vec4(0,0,0,1);
    gl_FragData[2] = vec4(0,0,0,1);
  }

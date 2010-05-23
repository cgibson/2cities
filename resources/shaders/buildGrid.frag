//
uniform vec4 material_ambient;
uniform vec4 material_diffuse;
uniform vec4 grid_diffuse;
uniform vec4 material_specular;
uniform float material_shininess;
uniform float grid_size;
uniform float line_pct;

uniform vec3 light_position;
uniform vec4 light_diffuse;
uniform vec4 light_ambient;

varying vec3 lightPos;
varying vec3 N;
varying vec3 v; 

#define Integral(x, p, notp) ((floor(x)*(p)) + max(fract(x)-(notp), 0.0))
  
void main (void) 
{
    vec3 view_vec = normalize(v);
    vec3 light_vec = normalize(lightPos);
    vec3 normal = normalize(N);
    
    vec2 position = v.xz / grid_size;
    vec2 useBrick;
    vec2 fw;
    
    fw = fwidth(position);
    
    position = fract(position);
    
    //useBrick = step(position, vec2(0.95, 0.95));
    
    vec2 pct = vec2(1.0 - line_pct, 1.0 - line_pct);
    vec2 upct = vec2(line_pct, line_pct);
    
    useBrick = (Integral(position + fw, pct, upct) -
                Integral(position, pct, upct)) / fw;
    
    vec4 diffuse = mix(grid_diffuse, material_diffuse, useBrick.x * useBrick.y);
    
    vec3 reflection = normalize( -reflect(view_vec,normal) );

    float NdotL = clamp(dot(normal,light_vec), 0.0, 1.0);

    diffuse = diffuse * NdotL;//* light.diffuse * NdotL;
    
    vec4 ambient = material_ambient;
    vec4 phong = ambient + diffuse;
    
    gl_FragColor = phong;
  }

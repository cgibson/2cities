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

#define WIDTH 40
#define HEIGHT 40
#define EDGE 1

#define Integral(x, p, notp) ((floor(x)*(p)) + max(fract(x)-(notp), 0.0))
  
void main (void) 
{
    float thick_size = 5.0;
    vec3 view_vec = normalize(v);
    vec3 light_vec = normalize(lightPos);
    vec3 normal = normalize(N);
    
    vec2 position1 = v.xz / grid_size;
    vec2 position2 = v.xz / (grid_size * thick_size);
    
    vec2 useBrick1;
    vec2 fw1;
    vec2 useBrick2;
    vec2 fw2;
    
    fw1 = fwidth(position1);
    fw2 = fwidth(position2);
    
    position1 = fract(position1);
    position2 = fract(position2);
    
    vec2 pct1 = vec2(1.0 - line_pct, 1.0 - line_pct);
    vec2 upct1 = vec2(line_pct, line_pct);
    
    vec2 pct2 = vec2(1.0 - (line_pct / 2.0), 1.0 - (line_pct / 2.0));
    vec2 upct2 = vec2(line_pct / 2.0, line_pct / 2.0);
    
    useBrick1 = (Integral(position1 + fw1, pct1, upct1) -
                Integral(position1, pct1, upct1)) / fw1;
    useBrick2 = (Integral(position2 + fw2, pct2, upct2) -
                Integral(position2, pct2, upct2)) / fw2;
    
    vec4 diffuse1 = mix(grid_diffuse, material_diffuse, clamp( (useBrick1.x * useBrick1.y), 0.0, 1.0));
    vec4 diffuse2 = mix(grid_diffuse, material_diffuse, clamp( (useBrick2.x * useBrick2.y), 0.0, 1.0));
    
    float balance = 0.2;
    
    vec4 diffuse = vec4(0,0,0,1);
    
    vec3 reflection = normalize( -reflect(view_vec,normal) );

    float NdotL = clamp(dot(normal,light_vec), 0.0, 1.0);

    if((v.x < WIDTH) && (v.x > -WIDTH) &&
       (v.z < HEIGHT) && (v.z > -HEIGHT))
    {
      diffuse = (diffuse1 * balance) + (diffuse2 * (1.0 - balance));
    }else if((v.x < (WIDTH + EDGE)) && (v.x > -(WIDTH + EDGE)) &&
             (v.z < (HEIGHT + EDGE)) && (v.z > -(HEIGHT + EDGE)))
    {
      float colorflip = sin((v.x + v.z) * 2.0);
      if(colorflip > 0.0)
        diffuse = vec4(1,1,0,1);
      else
        diffuse = vec4(0,0,0,1);
    }else{
      discard;
    }
    
    diffuse = diffuse * NdotL;
    
    vec4 ambient = material_ambient;
    vec4 phong = ambient + diffuse;
    
    gl_FragColor = phong;
  }

#ifndef LIGHTING_INC
#define LIGHTING_INC

#define MAX_LIGHTS 3
 
class LightData {
  public:
    float pos[4];
    float diffuse[4];
    float ambient[4];
    float specular;
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
    

    LightData();
    virtual ~LightData();
};

class Lighting {
  public:
    void doLighting();
    void setLightPos(int index, float x, float y, float z);
    void createLight(float x, float y, float z,
 		           float Dr, float Dg, float Db, float spec,
 		           float Ar, float Ag, float Ab,
 		           float cA, float lA, float qA);
};

#endif

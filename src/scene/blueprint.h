#ifndef _BLUEPRINT_H_
#define _BLUEPRINT_H_
#include "../helper/Vector.h"
#include "../system/enum.h"

using namespace enumeration;

class Blueprint {

public:
  Blueprint(){}
  Blueprint(ObjectType type, float mass, MaterialType material, Shape shape, Vector size){
    m_type = type;
    m_mass = mass;
    m_material = material;
    m_shape = shape;
    m_size = size;
  }
  
  ObjectType getType(){ return m_type; }
  float getMass(){ return m_mass; }
  MaterialType getMaterial(){ return m_material; }
  Shape getShape(){ return m_shape; }
  Vector getSize(){ return m_size; }
  
private:
  ObjectType m_type;
  float m_mass;
  MaterialType m_material;
  Shape m_shape;
  Vector m_size;
  
};

#endif

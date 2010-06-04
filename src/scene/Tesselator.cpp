#include "Tesselator.h"
#include "../system/global.h"

using namespace std;
namespace Tesselator
{
  void dumpFileIntoPhysics(const char * fileName)
  {
    FILE * inFile = fopen(fileName, "r");
    if (inFile == NULL)
    {
      printf("Unable to open file.\n");
      return;
    }
    
    int blockSize, count, x1, y1, z1, x2, y2, z2, type;
    unsigned int i;
    float sz;
    vector<WorldObject *> objects;
    if ((count = fscanf(inFile, "blocksize: %d\n", &blockSize)) == 1)
    {
      while((count = fscanf(inFile, "%d %d %d %d %d %d",
        &x1, &y1, &z1, &x2, &y2, &z2)) == 6)
      {
        if (blockSize == 0)
        {
          sz = 1;
        }
        else
        {
          sz = 1.0 / blockSize;
        }
        objects = placeBuilding(Vector(x1, y1, z1) * sz, Vector(x2, y2, z2) * sz,
                      TESS_SIMPLE);
        for (i = 0; i < objects.size(); i++)
          networkManager->network->addObject(objects[i]);
      }
    }
    else
    {
      while((count = fscanf(inFile, "%d %d %d %d %d %d %d",
        &x1, &y1, &z1, &x2, &y2, &z2, &type)) == 7)
      {
        objects = placeBuilding(Vector(x1, y1, z1), Vector(x2, y2, z2), (Tesselation)type);
        for (i = 0; i < objects.size(); i++)
          networkManager->network->addObject(objects[i]);
      }
    }
  }

  WorldObject * placeBlock(ObjectType type, Vector lowerBound)
  {
    WorldObject * obj = new WorldObject(0, 0, type);
    obj->setPosition(lowerBound +
      global::factory->getBlueprint(type).getSize() * 0.5);
    //networkManager->network->addObject(obj);
    return obj;
  }
  
/*  void placeSimpleEdge(Vector spot, Quaternion quat)
  {
    
  }
  void placeSimpleCorner(Vector spot, Quaternion quat)
  {
    placeSimpleEdge(spot, quat);
    for (int i = 1);
  }
  void placeSimpleColumn(Vector spot)*/
  vector<WorldObject *> placeSolidSegment(Vector spot, int bits)
  {
    vector<WorldObject *> result;
    Vector sc = global::factory->getBlueprint(BLOCK_5_1_5).getSize();
    for (int i = 0; i < TESS_BLOCKSIZE; i++)
      result.push_back(placeBlock(BLOCK_5_1_5, spot + sc.yVector() * i));
    return result;
  }
  
  vector<WorldObject *> placeStoneHengeSegment(Vector spot, int bits)
  {
    vector<WorldObject *> result;
    Vector sg = Vector(TESS_BLOCKSIZE, TESS_BLOCKSIZE,
                       TESS_BLOCKSIZE); // size of a segment
    Vector sb = global::factory->getBlueprint(BLOCK_2_4_2).getSize();
    Vector sc = global::factory->getBlueprint(BLOCK_5_1_5).getSize();
    Vector one = Vector(1, 1, 1);
    if ((bits & TESS_MIN_X) && (bits & TESS_MIN_Z))
      result.push_back(placeBlock(BLOCK_2_4_2, spot));
    if ((bits & TESS_MIN_X) && (bits & TESS_MAX_Z))
      result.push_back(placeBlock(BLOCK_2_4_2, spot + sg.zVector() - sb.zVector()));
    if ((bits & TESS_MAX_X) && (bits & TESS_MIN_Z))
      result.push_back(placeBlock(BLOCK_2_4_2, spot + sg.xVector() - sb.xVector()));
    if ((bits & TESS_MAX_X) && (bits & TESS_MAX_Z))
      result.push_back(placeBlock(BLOCK_2_4_2, spot + sg.xVector() - sb.xVector() + sg.zVector() - sb.zVector()));
    if ((bits & TESS_MIN_X) && (!(bits & (TESS_MIN_Z ))))//| TESS_MAX_Z))))
      result.push_back(placeBlock(BLOCK_2_4_2, spot - sb.zVector() * 0.5));
    if ((bits & TESS_MAX_X) && (!(bits & (TESS_MIN_Z ))))//| TESS_MAX_Z))))
      result.push_back(placeBlock(BLOCK_2_4_2, spot + sg.xVector() - sb.xVector() - sb.zVector() * 0.5));
    if ((bits & TESS_MIN_Z) && (!(bits & (TESS_MIN_X ))))//| TESS_MAX_X))))
      result.push_back(placeBlock(BLOCK_2_4_2, spot - sb.xVector() * 0.5));
    if ((bits & TESS_MAX_Z) && (!(bits & (TESS_MIN_X ))))//| TESS_MAX_X))))
      result.push_back(placeBlock(BLOCK_2_4_2, spot + sg.zVector() - sb.zVector() - sb.xVector() * 0.5));
//      
    if (!(bits & (TESS_MIN_X | TESS_MIN_Z)))
      result.push_back(placeBlock(BLOCK_2_4_2, spot- sb.xVector() * 0.5 - sb.zVector() * 0.5));
    result.push_back(placeBlock(BLOCK_5_1_5, spot + sg.yVector() - sc.yVector()));
    return result;
  }
  
  vector<WorldObject *> placeSimpleSegment(Vector spot, int bits)
  {
    vector<WorldObject *> result;
    // Tesselation segment size: (5, 5, 5), hardcoded
    Vector sg = Vector(TESS_BLOCKSIZE, TESS_BLOCKSIZE,
                       TESS_BLOCKSIZE); // size of a segment
    Vector sa = global::factory->getBlueprint(BLOCK_1_2_1).getSize();
    Vector sb = global::factory->getBlueprint(BLOCK_2_4_2).getSize();
    Vector sc = global::factory->getBlueprint(BLOCK_5_1_5).getSize();
    Vector one = Vector(1, 1, 1);
    if ((bits & TESS_MIN_X) || (bits & TESS_MIN_Z))
    {
      result.push_back(placeBlock(BLOCK_1_2_1, spot));
      result.push_back(placeBlock(BLOCK_1_2_1, spot + sa.yVector()));
    }
    if ((bits & TESS_MIN_X) || (bits & TESS_MAX_Z))
    {
      result.push_back(placeBlock(BLOCK_1_2_1, spot + sg.zVector() - sa.zVector()));
      result.push_back(placeBlock(BLOCK_1_2_1, spot + sa.yVector() + sg.zVector() - sa.zVector()));
    }
    if ((bits & TESS_MAX_X) || (bits & TESS_MIN_Z))
    {
      result.push_back(placeBlock(BLOCK_1_2_1, spot + sg.xVector() - sa.xVector()));
      result.push_back(placeBlock(BLOCK_1_2_1, spot + sa.yVector() + sg.xVector() - sa.xVector()));
    }
    if ((bits & TESS_MAX_X) || (bits & TESS_MAX_Z))
    {
      result.push_back(placeBlock(BLOCK_1_2_1, spot + sg.xVector() - sa.xVector() + sg.zVector() - sa.zVector()));
      result.push_back(placeBlock(BLOCK_1_2_1, spot + sa.yVector() + sg.xVector() - sa.xVector() + sg.zVector() - sa.zVector()));
    }
    if (!(bits & (TESS_MIN_X | TESS_MIN_Z)))
      result.push_back(placeBlock(BLOCK_2_4_2, spot- sb.xVector() * 0.5 - sb.zVector() * 0.5));
    for (int i = 1; i < 4; i++)
    {
      if ((bits & TESS_MIN_X))
      {
        result.push_back(placeBlock(BLOCK_1_2_1, spot + one.zVector() * i));
        result.push_back(placeBlock(BLOCK_1_2_1, spot + one.zVector() * i + sa.yVector()));
      }
      if ((bits & TESS_MIN_Z))
      {
        result.push_back(placeBlock(BLOCK_1_2_1, spot + one.xVector() * i));
        result.push_back(placeBlock(BLOCK_1_2_1, spot + one.xVector() * i + sa.yVector()));
      }
      if ((bits & TESS_MAX_X))
      {
        result.push_back(placeBlock(BLOCK_1_2_1, spot + one.zVector() * i + sg.xVector() - sa.xVector()));
        result.push_back(placeBlock(BLOCK_1_2_1, spot + one.zVector() * i + sg.xVector() - sa.xVector() + sa.yVector()));
      }
      if ((bits & TESS_MAX_Z))
      {
        result.push_back(placeBlock(BLOCK_1_2_1, spot + one.xVector() * i + sg.zVector() - sa.zVector()));
        result.push_back(placeBlock(BLOCK_1_2_1, spot + one.xVector() * i + sg.zVector() - sa.zVector() + sa.yVector()));
      }
    }
    result.push_back(placeBlock(BLOCK_5_1_5, spot + sg.yVector() - sc.yVector()));
    return result;
  }

  vector<WorldObject *> placeSegment(Vector spot, Tesselation type,
      int bits)
  {
    vector<WorldObject *> result;
    switch (type)
    {
      case TESS_NONE:
        for (int x = 0; x < TESS_BLOCKSIZE; x++)
          for (int y = 0; y < TESS_BLOCKSIZE; y++)
            for (int z = 0; z < TESS_BLOCKSIZE; z++)
              result.push_back(placeBlock(DUMMY_BLOCK, spot + Vector(x, y, z)));
        return result;
      case TESS_SIMPLE:
        return placeSimpleSegment(spot, bits);
      case TESS_STONEHENGE:
        return placeStoneHengeSegment(spot, bits);
      case TESS_SOLID:
        return placeSolidSegment(spot, bits);
      default:
      result.push_back(placeBlock(DUMMY_BLOCK, spot));
      result.push_back(placeBlock(DUMMY_BLOCK, spot));
      result.push_back(placeBlock(DUMMY_BLOCK, spot));
      printf("This tesselation number (%d) is not implemented yet.\n",
             type);
      return result;
    }
  }

  vector <WorldObject *> placeBuilding(Vector lowerBound, Vector upperBound,
        Tesselation type)
  {
    vector<WorldObject *> result;
    vector<WorldObject *> subResult;
    Vector here;
    Vector sg = Vector(5,5,5);// TODO initialize this properly
    int bits;
    // Loop through each segment in turn.
    // Note: The order here is selected as such
    // in order to facilitate generation over time
    for (here.y(lowerBound.y()); here.y() < upperBound.y();
                                here = here + sg.yVector())
      for (here.x(lowerBound.x()); here.x() < upperBound.x();
                                  here = here + sg.xVector())
        for (here.z(lowerBound.z()); here.z() < upperBound.z();
                                    here = here + sg.zVector())
        {
          // Generate the value for bits
          bits = 0;
          if (here.x() == lowerBound.x()) bits += TESS_MIN_X;
          if (here.y() == lowerBound.y()) bits += TESS_MIN_Y;
          if (here.z() == lowerBound.z()) bits += TESS_MIN_Z;
          if ((here.x() + sg.x()) >= upperBound.x()) bits += TESS_MAX_X;
          if ((here.y() + sg.y()) >= upperBound.y()) bits += TESS_MAX_Y;
          if ((here.z() + sg.z()) >= upperBound.z()) bits += TESS_MAX_Z;
          subResult = placeSegment(here, type, bits);
          for (unsigned int i = 0; i < subResult.size(); i++)
            result.push_back(subResult[i]);
        }
    return result;
  }
}

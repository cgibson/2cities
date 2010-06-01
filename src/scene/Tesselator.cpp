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
    float sz;
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
        placeBuilding(Vector(x1, y1, z1) * sz, Vector(x2, y2, z2) * sz,
                      TESS_NONE);
      }
    }
    else
    {
      printf("starting\n");
      while((count = fscanf(inFile, "%d %d %d %d %d %d %d",
        &x1, &y1, &z1, &x2, &y2, &z2, &type)) == 7)
      {
        placeBuilding(Vector(x1, y1, z1), Vector(x2, y2, z2), (Tesselation)type);
        printf("placeBuilding being called\n");
      }
      printf("count %d\n", count);
    }
  }

  void placeBlock(ObjectType type, Vector lowerBound)
  {
    WorldObject obj = WorldObject(0, 0, type);
    obj.setPosition(lowerBound +
      global::factory->getBlueprint(type).getSize() * 0.5);
    networkManager->network->addObject(&obj);
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
  
  void placeSimpleSegment(Vector spot, int bits)
  {
    printf("simpleSegment %s, bits: %d", spot.str(), bits);
    // Tesselation segment size: (5, 5, 5), hardcoded
    Vector sg = Vector(TESS_BLOCKSIZE, TESS_BLOCKSIZE,
                       TESS_BLOCKSIZE); // size of a segment
    Vector sa = global::factory->getBlueprint(BLOCK_1_2_1).getSize();
    Vector sb = global::factory->getBlueprint(BLOCK_2_4_2).getSize();
    Vector sc = global::factory->getBlueprint(BLOCK_5_1_5).getSize();
    Vector one = Vector(1, 1, 1);
    if ((bits & TESS_MIN_X) || (bits & TESS_MIN_Z))
    {
      placeBlock(BLOCK_1_2_1, spot);
      placeBlock(BLOCK_1_2_1, spot + sa.yVector());
    }
    if ((bits & TESS_MIN_X) || (bits & TESS_MAX_Z))
    {
      placeBlock(BLOCK_1_2_1, spot + sg.zVector() - sa.zVector());
      placeBlock(BLOCK_1_2_1, spot + sa.yVector() + sg.zVector() - sa.zVector());
    }
    if ((bits & TESS_MAX_X) || (bits & TESS_MIN_Z))
    {
      placeBlock(BLOCK_1_2_1, spot + sg.xVector() - sa.xVector());
      placeBlock(BLOCK_1_2_1, spot + sa.yVector() + sg.xVector() - sa.xVector());
    }
    if ((bits & TESS_MAX_X) || (bits & TESS_MAX_Z))
    {
      placeBlock(BLOCK_1_2_1, spot + sg.xVector() - sa.xVector() + sg.zVector() - sa.zVector());
      placeBlock(BLOCK_1_2_1, spot + sa.yVector() + sg.xVector() - sa.xVector() + sg.zVector() - sa.zVector());
    }
    if (!(bits & (TESS_MIN_X | TESS_MIN_Z)))
      placeBlock(BLOCK_2_4_2, spot- sb.xVector() * 0.5 - sb.zVector() * 0.5);
    for (int i = 1; i < 4; i++)
    {
      if ((bits & TESS_MIN_X))
      {
        placeBlock(BLOCK_1_2_1, spot + one.zVector() * i);
        placeBlock(BLOCK_1_2_1, spot + one.zVector() * i + sa.yVector());
      }
      if ((bits & TESS_MIN_Z))
      {
        placeBlock(BLOCK_1_2_1, spot + one.xVector() * i);
        placeBlock(BLOCK_1_2_1, spot + one.xVector() * i + sa.yVector());
      }
      if ((bits & TESS_MAX_X))
      {
        placeBlock(BLOCK_1_2_1, spot + one.zVector() * i + sg.xVector() - sa.xVector());
        placeBlock(BLOCK_1_2_1, spot + one.zVector() * i + sg.xVector() - sa.xVector() + sa.yVector());
      }
      if ((bits & TESS_MAX_Z))
      {
        placeBlock(BLOCK_1_2_1, spot + one.xVector() * i + sg.zVector() - sa.zVector());
        placeBlock(BLOCK_1_2_1, spot + one.xVector() * i + sg.zVector() - sa.zVector() + sa.yVector());
      }
    }
    placeBlock(BLOCK_5_1_5, spot + sg.yVector() - sc.yVector());
    printf("sg %s, sc %s, sp%s\n", sg.str(), sc.str(), spot.str());
  }

  void placeSegment(Vector spot, Tesselation type,
      int bits)
  {
    printf("placeSegment\n");
    switch (type)
    {
      case TESS_NONE:
        placeBlock(DUMMY_BLOCK, spot);
      break;
      case TESS_SIMPLE:
        placeSimpleSegment(spot, bits);
        break;
      case TESS_STONEHENGE:
      case TESS_SOLID:
      default:
      placeBlock(DUMMY_BLOCK, spot);
      placeBlock(DUMMY_BLOCK, spot);
      placeBlock(DUMMY_BLOCK, spot);
      printf("This tesselation number (%d) is not implemented yet.\n",
             type);
      break;
    }
  }

  void placeBuilding(Vector lowerBound, Vector upperBound,
        Tesselation type)
  {
    printf("PlaceBuilding\n");
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
          placeSegment(here, type, bits);
        }
  }
}

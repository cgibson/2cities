#include "model.h"

namespace mdl
{
    //stl vector to store all the triangles for the mesh
    vector<Vector3 *> Vertices;

    //stl vector to store all the vertices in the mesh
    vector<Vector3 *> Normals;

    //vertex array
    float *vertexarray;

    //normal array (same size as vertex array)
    float *normalarray;

    float *colorarray;

    int totalfaces;
}

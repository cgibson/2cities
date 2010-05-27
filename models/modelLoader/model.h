#ifndef MODEL_H_
#define MODEL_H_

#include <stdlib.h>
#include <vector>

using namespace std;

namespace model
{
    typedef struct Vector3
    {
        float x;
        float y;
        float z;
        Vector3(float in_x, float in_y, float in_z) : x(in_x), y(in_y), z(in_z) {}
        Vector3() {}
    } Vector3;

    //data structure to store triangle -
    //note that v1, v2, and v3 are indexes into the vertex array
    typedef struct Triangle
    {
        int v1;
        int v2;
        int v3;

        Vector3 normal;
        Vector3 center;
        Vector3 color;
        Triangle(int in_v1, int in_v2, int in_v3) : v1(in_v1), v2(in_v2), v3(in_v3),
            normal(0, 1, 0), center(0, 0, 0){}
        Triangle() : normal(0, 1, 0) {}
    } Triangle;

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

#endif

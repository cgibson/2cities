#ifndef _LOADER_H_
#define _LOADER_H_

#include "model.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

//Toggle for debug statements
//#define MODEL_DEBUG

using namespace std;
using namespace model;

//Definitions
#define MAX_MODEL_LINE_LENGTH 255

namespace loader
{

    char line[MAX_MODEL_LINE_LENGTH];

    //Function taken from stackoverflow.com
    //Checks a string for a specified ending string
    bool hasEnding(std::string const &modelFile,
        std::string const &extension)
    {
        if (modelFile.length() > extension.length())
        {
            return (modelFile.compare(modelFile.length() -
            extension.length(), extension.length(), extension) == 0);
        }
        return false;
    }

    //Reads in triangle data from a .obj file.
    void readFile(char* filename)
    {
        model::totalfaces = 0;
        int lineNum = 1;
        float v1, v2, v3;
        float n1, n2, n3;
        vector<int> faceVertices;
        vector<int> faceNormals;

        //check for correct file extension first
        if(!hasEnding(filename, ".obj"))
        {
            cerr << "<mesh_obj_file> must have .obj extension" << endl;
            exit(EXIT_FAILURE);
        }
        std::string line;
        std::string buffer;
        std::ifstream modelFile(filename);
        if(modelFile.is_open())
        {
            while(std::getline(modelFile, line))
            {
    #ifdef MODEL_DEBUG
                cout << "Line read in: " << line << endl;
    #endif
                std::stringstream lineStream(line);
                vector<string> lineTokens;
                //Split line into tokens, add to vector
                while(lineStream >> buffer)
                {
                    lineTokens.push_back(buffer);
                }
                try
                {
                    //reusing buffer to iterate through tokens
                    buffer = lineTokens.at(0);

                    //Handles vertex trio: "v x y z"
                    if(!buffer.compare("v") && lineTokens.size() == 4)
                    {
    #ifdef MODEL_DEBUG
                        cout << "Found vertex trio" << endl;
    #endif

                        v1 = atof(lineTokens.at(1).c_str());
                        v2 = atof(lineTokens.at(2).c_str());
                        v3 = atof(lineTokens.at(3).c_str());
                        model::Vector3 *vertextrio = new Vector3(v1, v2, v3);
                        model::Vertices.push_back(vertextrio);
                    }
                    //Handles vertex normal: "vn x y z"
                    else if(!buffer.compare("vn") && lineTokens.size() == 4)
                    {
    #ifdef MODEL_DEBUG
                        cout << "Found a vertex normal" << endl;
    #endif
                        n1 = atof(lineTokens.at(1).c_str());
                        n2 = atof(lineTokens.at(2).c_str());
                        n3 = atof(lineTokens.at(3).c_str());
                        model::Vector3 *vertexnormal = new Vector3(n1, n2, n3);
                        model::Normals.push_back(vertexnormal);
                    }
                    //Handles face-normal trio: "f 1//1 2//2 3//3"
                    else if(!buffer.compare("f") && lineTokens.size() == 4)
                    {
    #ifdef MODEL_DEBUG
                        cout << "Found a face-normal trio" << endl;
    #endif
                        stringstream facenormal1(lineTokens.at(1));
                        getline(facenormal1, buffer, '/');
                        faceVertices.push_back(atoi(buffer.c_str()));

                        //skip over the empty string between the two '//'
                        getline(facenormal1, buffer, '/');

                        getline(facenormal1, buffer, '/');
                        faceNormals.push_back(atoi(buffer.c_str()));

                        stringstream facenormal2(lineTokens.at(2));
                        getline(facenormal2, buffer, '/');
                        faceVertices.push_back(atoi(buffer.c_str()));

                        //skip over the empty string between the two '//'
                        getline(facenormal2, buffer, '/');

                        getline(facenormal2, buffer, '/');
                        faceNormals.push_back(atoi(buffer.c_str()));

                        stringstream facenormal3(lineTokens.at(3));
                        getline(facenormal3, buffer, '/');
                        faceVertices.push_back(atoi(buffer.c_str()));

                        //skip over the empty string between the two '//'
                        getline(facenormal3, buffer, '/');

                        getline(facenormal3, buffer, '/');
                        faceNormals.push_back(atoi(buffer.c_str()));
                        model::totalfaces++;
                    }
                }
                catch(exception& ex)
                {
                    //line number is 1-based
                    cerr << "Error on line #" << lineNum << ": " << ex.what() << endl;
                    exit(EXIT_FAILURE);
                }
                lineNum++;
            }
            modelFile.close();
    #ifdef MODEL_DEBUG
            cout << "totalfaces: " << model::totalfaces << endl;
            cout << "size of Vertices: " << model::Vertices.size() << endl;
            cout << "size of Normals: " << model::Normals.size() << endl;
            for(int i = 0; i < faceVertices.size(); i++)
            {
                cout << "faceVertices[" << i << "]: " << faceVertices[i] << endl;
            }
            for(int i = 0; i < faceNormals.size(); i++)
            {
                cout << "faceNormals[" << i << "]: " << faceNormals[i] << endl;
            }
    #endif
            model::vertexarray = (float*)malloc(sizeof(float) * faceVertices.size() * 3);
            model::normalarray = (float*)malloc(sizeof(float) * faceNormals.size() * 3);
            model::colorarray = (float*)malloc(sizeof(float) * faceVertices.size() * 3);

            for(int i = 0; i < faceVertices.size(); i++)
            {
    #ifdef MODEL_DEBUG
                cout << "face #" << i << " faceVertices[i]: " << faceVertices[i] << " faceNormals[i]: " << faceNormals[i] << endl;
    #endif
                model::vertexarray[i*3] = model::Vertices[faceVertices[i] - 1]->x;
                model::vertexarray[i*3+1] = model::Vertices[faceVertices[i] - 1]->y;
                model::vertexarray[i*3+2] = model::Vertices[faceVertices[i] - 1]->z;
                model::normalarray[i*3] = model::Normals[faceNormals[i] - 1]->x;
                model::normalarray[i*3+1] = model::Normals[faceNormals[i] - 1]->y;
                model::normalarray[i*3+2] = model::Normals[faceNormals[i] - 1]->z;
                model::colorarray[i*3] = drand48();
                model::colorarray[i*3+1] = drand48();
                model::colorarray[i*3+2] = drand48();
            }
        }
        else
        {
            cerr << "Unable to open mesh file: " << filename << endl;
            exit(EXIT_FAILURE);
        }
    }
}

#endif

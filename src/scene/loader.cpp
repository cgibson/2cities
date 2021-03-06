#include "loader.h"

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
    void readFile(const char* filename)
    {
        mdl::totalfaces = 0;
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
                        mdl::Vector3 *vertextrio = new Vector3(v1, v2, v3);
                        mdl::Vertices.push_back(vertextrio);
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
                        mdl::Vector3 *vertexnormal = new Vector3(n1, n2, n3);
                        mdl::Normals.push_back(vertexnormal);
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
                        mdl::totalfaces++;
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
            cout << "totalfaces: " << mdl::totalfaces << endl;
            cout << "size of Vertices: " << mdl::Vertices.size() << endl;
            cout << "size of Normals: " << mdl::Normals.size() << endl;
            for(int i = 0; i < faceVertices.size(); i++)
            {
                cout << "faceVertices[" << i << "]: " << faceVertices[i] << endl;
            }
            for(int i = 0; i < faceNormals.size(); i++)
            {
                cout << "faceNormals[" << i << "]: " << faceNormals[i] << endl;
            }
#endif
            mdl::vertexarray = (float*)malloc(sizeof(float) * faceVertices.size() * 3);
            mdl::normalarray = (float*)malloc(sizeof(float) * faceNormals.size() * 3);
            mdl::colorarray = (float*)malloc(sizeof(float) * faceVertices.size() * 3);

            for(unsigned int i = 0; i < faceVertices.size(); i++)
            {
#ifdef MODEL_DEBUG
                cout << "face #" << i << " faceVertices[i]: " << faceVertices[i] << " faceNormals[i]: " << faceNormals[i] << endl;
#endif
                mdl::vertexarray[i*3] = mdl::Vertices[faceVertices[i] - 1]->x;
                mdl::vertexarray[i*3+1] = mdl::Vertices[faceVertices[i] - 1]->y;
                mdl::vertexarray[i*3+2] = mdl::Vertices[faceVertices[i] - 1]->z;
                mdl::normalarray[i*3] = mdl::Normals[faceNormals[i] - 1]->x;
                mdl::normalarray[i*3+1] = mdl::Normals[faceNormals[i] - 1]->y;
                mdl::normalarray[i*3+2] = mdl::Normals[faceNormals[i] - 1]->z;
                mdl::colorarray[i*3] = drand48();
                mdl::colorarray[i*3+1] = drand48();
                mdl::colorarray[i*3+2] = drand48();
            }
        }
        else
        {
            cerr << "Unable to open mesh file: " << filename << endl;
            exit(EXIT_FAILURE);
        }
    }
}

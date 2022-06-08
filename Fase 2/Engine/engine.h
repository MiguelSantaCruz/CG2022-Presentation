#ifndef CG2022_ENGINE_H
#define CG2022_ENGINE_H
#endif //CG2022_ENGINE_H

#include <list>
#include "tinyxml.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cmath>


//Classe de configuração da camera
class CameraConfig {
public:
    float cameraX;
    float cameraY;
    float cameraZ;
    float lookAtX;
    float lookAtY;
    float lookAtZ;
    float upX;
    float upY;
    float upZ;
    float fov;
    float near;
    float far;
public:
    void printOut();
};

//Ponto no espaço
class Point{
public:
    float x;
    float y;
    float z;
};

class Transformation{
public:
    /* 0 - Transform
     * 1 - Rotate
     * 2 - Scale
     * 3 - Color
     * */
    float color_r = 1;
    float color_g = 1;
    float color_b = 1;
    int type;
    float x;
    float y;
    float z;
    float angle;
public:
    void applyTransformation();
    std::string toString();
};

//Modelo carregado de ficheiros .3d
class Model {
public:
    std::string filename = "NaN";
    std::list<Point> points;
    std::list<Transformation> transformations{};
    void drawModel() const;
    void printOut();
};

//Opens a 3d file and loads it to memory
Model openFileAndLoadModel (const std::string& filename);

/* Read camera configurations (Position FOV near far)*/
void readCameraXMLConfigurations(TiXmlElement* root);

/* Read transformations from o group */
std::list<Transformation> readTransformation(TiXmlElement* element);

/* Read a group from the XML file*/
void readGroupXMLConfigurationFile(TiXmlElement* element,std::list<Transformation> transformations);

//Ler o ficheiro XML e carregar os modelos
bool readXMLConfigurationFile(char* filename);

//Draw XYZ Axis
void drawAxis(float size);
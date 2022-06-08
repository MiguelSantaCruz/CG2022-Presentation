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
#include "CatmullRom.h"

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
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include "tinyxml.h"
#endif
#include "engine.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#define _USE_MATH_DEFINES
#include <math.h>
#include <list>

using namespace std;

//Angulo da camera
float cameraAngle = 0;
//Escala do viewport
float scale = 0;
//distância da camera à origem
float cameraDistance = 0;
//Ficheiro XML aberto no momento
fstream openedFile;

//Classe de configuração da camera
class CameraConfig{
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
};

//Ponto no espaço
class Point{
    public:
    float x;
    float y;
    float z;
};

//Modo de desenho
bool drawModeFill = false;

//Modelo carregado de ficheiros .3d
class Model {
    public:
        list<Point> points;
    void drawModel(){
        int i = 0;
        float color = 0;
        for(Point point: points){
            string line;
            //Togle between wireframe and fill
            if(drawModeFill) glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            else glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            //Draw model
            glBegin(GL_TRIANGLES);
            //Define color if in fill mode or white if wireframe
            if(drawModeFill){
               if(i % 3 == 0) {
                color == 0? color = 1: color = 0;
                }
                i++;
                glColor3f(color, 1-color, 1-color); 
            } else glColor3f(1, 1, 1);
            //Draw vertex 
            glVertex3f(point.x,point.y,point.z);
        }
        glEnd();
    }
};

//Modelos lidos do ficheiro XML
map<int,Model> modelsToDraw;

//Configuração da camera atual
CameraConfig cameraConfig;


//Abre um ficheiro .3d e carrega-o para memória
Model openFileAndLoadModel (string filename){
    fstream file;
    file.open(filename,ios::in);
    Model model;
    if(file.is_open()){
        string line;
        float point_x,point_y,point_z;
        int i = 0;
        float color = 0;
        while (getline(file,line)) {
            sscanf(line.c_str(),"%f %f %f",&point_x,&point_y,&point_z);
            Point point;
            point.x = point_x;
            point.y = point_y;
            point.z = point_z;
            model.points.insert(model.points.end(),point);
        }
        cout << "File drawn: [" << filename << "]\n";
    } else cout << "File not found: [" << filename << "]\n";
    return model;
}

//Ler o ficheiro XML e carregar os modelos
void readXMLConfigurationFile(char* filename) {
    TiXmlDocument document;
    bool fileOpened = document.LoadFile(filename);
    if(fileOpened){
        TiXmlElement* root = document.RootElement();
        int i = 0;
        for (TiXmlElement* e = root->FirstChildElement("camera")->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
        {
            if (e)
            {
                const char *a1 = e->Attribute("x");
                const char *a2 = e->Attribute("y");
                const char *a3 = e->Attribute("z");
                const char *a4 = e->Attribute("fov");
                const char *a5 = e->Attribute("near");
                const char *a6 = e->Attribute("far");

                //DEBUG
                if (a1 && i == 0) {
                    cout << "Position x: " << a1 << "\n";
                    cameraConfig.cameraX = atof(a1);
                }
                if (a2 && i == 0) {
                    cout << "Position y: " << a2 << "\n";
                    cameraConfig.cameraY = atof(a2);

                }
                if (a3 && i == 0) {
                    cout << "Position z: " << a3 << "\n";
                    cameraConfig.cameraZ = atof(a3);
                }
                if (a1 && i == 1) {
                    cout << "LookAt x: " << a1 << "\n";
                    cameraConfig.lookAtX = atof(a1);
                }
                if (a2 && i == 1) {
                    cout << "LookAt y: " << a2 << "\n";
                    cameraConfig.lookAtY = atof(a2);

                }
                if (a3 && i == 1) {
                    cout << "LookAt z: " << a3 << "\n";
                    cameraConfig.lookAtZ = atof(a3);
                }
                if (a1 && i == 2) {
                    cout << "Up x: " << a1 << "\n";
                    cameraConfig.upX = atof(a1);
                }
                if (a2 && i == 2) {
                    cout << "Up y: " << a2 << "\n";
                    cameraConfig.upY = atof(a2);
                }
                if (a3 && i == 2) {
                    cout << "Up z: " << a3 << "\n";
                    cameraConfig.upZ = atof(a3);
                }
                if (a4){
                    cout << "FOV: " << a4 << "\n";
                    cameraConfig.fov = atof(a4);

                }
                if (a5) {
                    cout << "Near: " << a5 << "\n";
                    cameraConfig.near = atof(a5);
                }
                if (a6) {
                    cout << "Far: " << a6 << "\n";
                    cameraConfig.far = atof(a6);
                }
            }
            i++;
        }
        i = 0;
        for (TiXmlElement* e = root->FirstChildElement("group")->FirstChildElement("models")->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
        {
            if (e)
            {
                const char *a1 = e->Attribute("file");

                if (a1)
                    cout << "Model file: " << a1 << "\n";
                    Model model = openFileAndLoadModel(a1);
                    modelsToDraw.insert(pair<int,Model>(i,model));
            }
            i++;
        }
    } else cout << "Error: Can't open XML File: " << filename << "\n";

}

//Redimensionar janela
void changeSize(int w, int h) {

    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if(h == 0)
        h = 1;

    // compute window's aspect ratio
    float ratio = w * 1.0 / h;

    // Set the projection matrix as current
    glMatrixMode(GL_PROJECTION);
    // Load Identity Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set perspective
    gluPerspective(cameraConfig.fov ,ratio, cameraConfig.near,cameraConfig.far);

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}


//Desenhar os eixos XYZ
void drawAxis(){
    glBegin(GL_LINES);
    //X Axis
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-100.0f, 0.0f, 0.0f);
    glVertex3f( 10.0f, 0.0f, 0.0f);
    //Y Axis
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -100.0f, 0.0f);
    glVertex3f(0.0f, 10.0f, 0.0f);
    // Z Axis
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -100.0f);
    glVertex3f(0.0f, 0.0f, 10.0f);
    glEnd();

}

void renderScene(void) {

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    glLoadIdentity();
   
    gluLookAt(cameraConfig.cameraX,cameraConfig.cameraY,cameraConfig.cameraZ,
              cameraConfig.lookAtX,cameraConfig.lookAtY,cameraConfig.lookAtZ,
              cameraConfig.upX,cameraConfig.upY,cameraConfig.upZ);
    
    glTranslatef(scale,scale,scale); 
    glRotatef(cameraAngle,0,1,0);
    //gluPerspective(cameraConfig.fov,400/400,cameraConfig.near,cameraConfig.far);

    for(auto &ent1 :  modelsToDraw) {
        ent1.second.drawModel();
    }
    drawAxis();
    // End of frame
    glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

// put code to process regular keys in here

}

void processSpecialKeys(int key, int xx, int yy) {

    switch (key) {
        case GLUT_KEY_UP:
            scale+= 0.5;
            break;
        case GLUT_KEY_DOWN:
            scale-= 0.5;
            break;
        case GLUT_KEY_RIGHT:
            cameraAngle += 5;
            break;
        case GLUT_KEY_LEFT:
            cameraAngle -= 5;
            break;
        case GLUT_KEY_F1:
            glCullFace(GL_FRONT);
            break;
        case GLUT_KEY_F2:
            glCullFace(GL_BACK);
            break;
        case GLUT_KEY_F3:
            drawModeFill = false;
            break;
        case GLUT_KEY_F4:
            drawModeFill = true;
            break;
    }
    glutPostRedisplay();

}

int main(int argc, char **argv) {
    if(argc != 2){
        cout << "Sintax error: \n";
        cout << "   Usage: ./Engine [XML configuration file]\n";
        exit(-1);
    }
    string xmlFile = argv[1];
    printf("XML File: %s\n",argv[1]);
    readXMLConfigurationFile(argv[1]);
    //distância da camera à origem
    //cameraDistance = sqrt((cameraConfig.cameraX)*(cameraConfig.cameraX) + (cameraConfig.cameraY)*(cameraConfig.cameraY) + (cameraConfig.cameraZ)*(cameraConfig.cameraZ));
    
// init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,800);
    char* windowName = static_cast<char *>(malloc(sizeof(char) * 100));
    sprintf(windowName,"File: [%s]",argv[1]);
    glutCreateWindow(windowName);

// Required callback registry
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);

// Callback registration for keyboard processing
    glutKeyboardFunc(processKeys);
    glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);

// enter GLUT's main cycle
    glutMainLoop();

    return 1;
}


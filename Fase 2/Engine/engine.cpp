#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>

#include <utility>
#endif
#define _USE_MATH_DEFINES
#include "engine.h"

using namespace std;

// Ângulos da camera
float alpha_angle,beta_angle = 0.5;

// Distância da camera à origem
float distance_Origin = 0;

/* Modo de desenho True -> GL_FILL False -> GL_LINE */
bool drawModeFill = false;

//Desenhar os eixos
bool drawAxisBool = true;

/* Culling Mode True -> GL_FRONT False -> GL_BACK*/
bool frontCull = false;

/* Change the way color is displayed True-> Use 2 alternating colors for faces False-> Use 1 color for faces*/
bool alternatingColorFaces = false;

/* XML file opened*/
string xmlFile;

//Modelos lidos do ficheiro XML
list<Model> modelsToDraw;

//Configuração da camera atual
CameraConfig cameraConfig;

/* Desenhar um determinado modelo */
void Model::drawModel() const {
    int i = 0;
    float color = 0;
    glPushMatrix();
    for( Transformation transformation : this->transformations){
        transformation.applyTransformation();
    }
    for(Point point: points){
        string line;
        //Togle between wireframe and fill
        if(drawModeFill) glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        else glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        //Draw model
        glBegin(GL_TRIANGLES);
        //Draw faces using 2 alternating colors ignoring color info
        if(drawModeFill && alternatingColorFaces){
            if(i % 3 == 0) {
                color == 0? color = 1: color = 0;
            }
            i++;
            glColor3f(color, 1-color, 1-color);
        }
        //Draw vertex
        glVertex3f(point.x,point.y,point.z);
    }
    glEnd();
    glPopMatrix();
}

/* Print Model to stdout */
void Model::printOut() {
    // Print file
    cout << "----------------------------------\n";
    cout << "Model Filename: " + this->filename + "\n";
    cout << "Transformations List:\n";
    // Print Transformation list
    for( Transformation transformation : this->transformations){
        cout << transformation.toString();
    }
}

void CameraConfig::printOut() {
    cout << "----------------------------------\n";
    cout << "Camera Configurations:\n";
    cout << "Position x: " << this->cameraX;
    cout << " y: " << this->cameraY;
    cout << " z :" << this->cameraZ << "\n";
    cout << "LookAt x: " << this->lookAtX;
    cout << " y: " << this->lookAtY;
    cout << " z :" << this->lookAtZ << "\n";
    cout << "Up x: " << this->upX;
    cout << " y: " << this->upY;
    cout << " z :" << this->upZ << "\n";
    cout << "FOV: " << this->fov<<"\n";
    cout << "Near: " << this->near<<"\n";
    cout << "Far: " << this->far<<"\n";
}

/* Apply tranformation to model */
void Transformation::applyTransformation() {
    switch (type) {
        case(0):
            glTranslatef(this->x,this->y,this->z);
            break;
        case(1):
            glRotatef(this->angle,this->x,this->y,this->z);
            break;
        case(2):
            glScalef(this->x,this->y,this->z);
            break;
        case(3):
            glColor3f(this->color_r/255,this->color_g/255,this->color_b/255);
            break;
        default:
            cout << "Unknown transformation type\n";
            break;
    }
}

/* Returns string with transformation's details  */
std::string Transformation::toString(){
    string output_string;
    switch (type) {
        case(0):
            output_string = "Translate x: " + to_string(this->x) + " y: " + to_string(this->y) + " z: " + to_string(this->z) + "\n";
            break;
        case(1):
            output_string = "Rotate angle: " + to_string(this->angle) + " x: " + to_string(this->x) + " y: " + to_string(this->y) + " z: " + to_string(this->z) + "\n";
            break;
        case(2):
            output_string = "Scale x: " + to_string(this->x) + " y: " + to_string(this->y) + " z: " + to_string(this->z) + "\n";
            break;
        case(3):
            output_string = "Color x: " + to_string(this->color_r) + " y: " + to_string(this->color_g) + " z: " + to_string(this->color_b) + "\n";
            break;
        default:
            output_string ="Unknown transformation type\n";
            break;
    }
    return output_string;
}

//Opens a 3d file and loads it to memory
Model openFileAndLoadModel (const string& filename){
    fstream file;
    file.open(filename,ios::in);
    Model model;
    model.filename = filename;
    if(file.is_open()){
        string line;
        float point_x,point_y,point_z;
        int i = 0;
        float color = 0;
        while (getline(file,line)) {
            sscanf(line.c_str(),"%f %f %f",&point_x,&point_y,&point_z);
            Point point{};
            point.x = point_x;
            point.y = point_y;
            point.z = point_z;
            model.points.insert(model.points.end(),point);
        }
    } else cout << "File not found: [" << filename << "]\n";
    return model;
}

/* Read camera configurations (Position FOV near far)*/
void readCameraXMLConfigurations(TiXmlElement* root){
    int i = 0;
    for (TiXmlElement* e = root->FirstChildElement("camera")->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
    {
        const char *x = e->Attribute("x");
        const char *y = e->Attribute("y");
        const char *z = e->Attribute("z");
        const char *fov = e->Attribute("fov");
        const char *near = e->Attribute("near");
        const char *far = e->Attribute("far");
        if (x && i == 0) cameraConfig.cameraX = atof(x);
        if (y && i == 0) cameraConfig.cameraY = atof(y);
        if (z && i == 0) cameraConfig.cameraZ = atof(z);
        if (x && i == 1) cameraConfig.lookAtX = atof(x);
        if (y && i == 1) cameraConfig.lookAtY = atof(y);
        if (z && i == 1) cameraConfig.lookAtZ = atof(z);
        if (x && i == 2) cameraConfig.upX = atof(x);
        if (y && i == 2) cameraConfig.upY = atof(y);
        if (z && i == 2) cameraConfig.upZ = atof(z);
        if (fov) cameraConfig.fov = atof(fov);
        if (near) cameraConfig.near = atof(near);
        if (far) cameraConfig.far = atof(far);
        i++;
    }
}

/* Read transformations from o group */
list<Transformation> readTransformation(TiXmlElement* element){
    list<Transformation> transformationList{};
    for(TiXmlElement* element2 = element -> FirstChildElement("transform")->FirstChildElement(); element2 != nullptr ; element2 = element2->NextSiblingElement()){
        Transformation transformation{};
        if(strcmp(element2->Value(),"color") == 0){
            transformation.type = 3;
            transformation.angle = 0;
            transformation.color_r = atof(element2->Attribute("r"));
            transformation.color_g = atof(element2->Attribute("g"));
            transformation.color_b = atof(element2->Attribute("b"));
            transformationList.push_back(transformation);
        }
        if(strcmp(element2->Value(),"translate") == 0){
            transformation.type = 0;
            transformation.angle = 0;
            transformation.x = atof(element2->Attribute("x"));
            transformation.y = atof(element2->Attribute("y"));
            transformation.z = atof(element2->Attribute("z"));
            transformationList.push_back(transformation);
        }
        if(strcmp(element2->Value(),"rotate") == 0){
            transformation.type = 1;
            transformation.angle = atof(element2->Attribute("angle"));
            transformation.x = atof(element2->Attribute("x"));
            transformation.y = atof(element2->Attribute("y"));
            transformation.z = atof(element2->Attribute("z"));
            transformationList.push_back(transformation);
        }
        if(strcmp(element2->Value(),"scale") == 0){
            transformation.type = 2;
            transformation.angle = 0;
            transformation.x = atof(element2->Attribute("x"));
            transformation.y = atof(element2->Attribute("y"));
            transformation.z = atof(element2->Attribute("z"));
            transformationList.push_back(transformation);
        }
    }
    return transformationList;
}

/* Read model file from group */
list<Model> readModel(TiXmlElement* element){
    list<Model> modelList{};
    if(element ->FirstChildElement("models") != NULL){
        for(TiXmlElement* element2 = element -> FirstChildElement("models")->FirstChildElement("model"); element2 != nullptr ; element2 = element2->NextSiblingElement()) {
            Model model{};
            const char *modelFile = element2->Attribute("file");
            model = openFileAndLoadModel(modelFile);
            modelList.push_back(model);
        }
    }
    return modelList;
}

/* Read a group from the XML file*/
void readGroupXMLConfigurationFile(TiXmlElement* element,list<Transformation> transformations){
    list<Transformation> transformationList = readTransformation(element);
    for(Transformation transformation: transformationList){
             transformations.push_back(transformation);
    }
    list<Model> modelList = readModel(element);
    for (Model m: modelList) {
        m.transformations = transformations;
        modelsToDraw.push_back(m);
    }
    for(TiXmlElement* subgroup = element -> FirstChildElement("group"); subgroup != nullptr ; subgroup = subgroup->NextSiblingElement()){
        readGroupXMLConfigurationFile(subgroup,transformationList);
    }
}

//Ler o ficheiro XML e carregar os modelos
bool readXMLConfigurationFile(char* filename) {
    TiXmlDocument document;
    bool fileOpened = document.LoadFile(filename);
    int i = 0;
    if(fileOpened){
        TiXmlElement* root = document.RootElement();
        readCameraXMLConfigurations(root);
        for(TiXmlElement* element2 = root -> FirstChildElement("group"); element2 != nullptr ; element2 = element2->NextSiblingElement()){
            readGroupXMLConfigurationFile(element2,{});
        }
    } else cout << "Error: Can't open XML File: " << filename << "\n";
    return fileOpened;
}

//Redimensionar janela
void changeSize(int w, int h) {

    // Prevent a divide by zero, when window is too short
    // (you can't make a window with zero width).
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
    gluPerspective(cameraConfig.fov ,ratio, cameraConfig.near ,cameraConfig.far);

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}

//Draw XYZ Axis
void drawAxis(float size){
    glBegin(GL_LINES);

    //X Axis
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-size, 0.0f, 0.0f);
    glVertex3f( size, 0.0f, 0.0f);
    //Y Axis
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -size, 0.0f);
    glVertex3f(0.0f, size, 0.0f);
    // Z Axis
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -size);
    glVertex3f(0.0f, 0.0f, size);
    glColor3f(1.0f,1.0f,1.0f);
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

    // For each pair model list<transformation> apply transformations and draw model
    for(const Model& model:  modelsToDraw) {
        model.drawModel();
    }
    //draw axis if enabled
    if(drawAxisBool) drawAxis(1000);
    // End of frame
    glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

// put code to process regular keys in here

}

void processSpecialKeys(int key, int xx, int yy) {
    switch (key) {
        case GLUT_KEY_RIGHT:
            alpha_angle -= 0.1; break;
        case GLUT_KEY_LEFT:
            alpha_angle += 0.1; break;
        case GLUT_KEY_UP:
            beta_angle += 0.01f;
            if (beta_angle > 1.5f)
                beta_angle = 1.5f;
            break;
        case GLUT_KEY_DOWN:
            beta_angle -= 0.01f;
            if (beta_angle < -1.5f)
                beta_angle = -1.5f;
            break;
        case GLUT_KEY_PAGE_DOWN: distance_Origin -= 5.0f;
            if (distance_Origin < 1.0f)
                distance_Origin = 1.0f;
            break;
        case GLUT_KEY_PAGE_UP: distance_Origin += 5.0f; break;
        case GLUT_KEY_F1:
            drawAxisBool ? drawAxisBool = false: drawAxisBool = true;
            break;
        case GLUT_KEY_F2:
            frontCull ? frontCull = false: frontCull = true;
            frontCull ? glCullFace(GL_FRONT) : glCullFace(GL_BACK);
            break;
        case GLUT_KEY_F3:
            drawModeFill ? drawModeFill = false : drawModeFill = true;
            break;
        case GLUT_KEY_F4:
            alternatingColorFaces ? alternatingColorFaces = false : alternatingColorFaces = true;
            break;
    }
    cameraConfig.cameraX = distance_Origin * cos(beta_angle) * sin(alpha_angle);
    cameraConfig.cameraY = distance_Origin * sin(beta_angle);
    cameraConfig.cameraZ = distance_Origin * cos(beta_angle) * cos(alpha_angle);
    string windowTitle = "File: [ " + xmlFile + "]   ";
    drawAxisBool? windowTitle = windowTitle + "AXIS: TRUE |" :windowTitle = windowTitle + "AXIS: FALSE|";
    frontCull? windowTitle = windowTitle + " CULL: BACK |" :windowTitle = windowTitle + " CULL: FRONT|";
    drawModeFill? windowTitle = windowTitle + " DRAW: FILL|" :windowTitle = windowTitle + " DRAW: LINE|";
    alternatingColorFaces? windowTitle = windowTitle + " COLOR: 2  " :windowTitle = windowTitle + " COLOR: ANY";
    glutSetWindowTitle(windowTitle.c_str());
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    /* Verify the number of arguments */
    if(argc != 2){
        cout << "Syntax error: \n";
        cout << "   Usage: ./Engine [XML configuration file]\n";
        exit(-1);
    }
    /* Open XML file and load models and camera configuration */
    xmlFile = argv[1];
    cout << "XML File: " << xmlFile << "\n";
    bool fileExists = readXMLConfigurationFile(argv[1]);
    if(!fileExists) exit(-1);
    /* Print Camera information read */
    cameraConfig.printOut();
    /* Print Models read */
    for(Model model:  modelsToDraw) {
        model.printOut();
    }
    //Calculate distance to origin
    distance_Origin = sqrt((cameraConfig.cameraX)*(cameraConfig.cameraX) + (cameraConfig.cameraY)*(cameraConfig.cameraY) + (cameraConfig.cameraZ)*(cameraConfig.cameraZ));
    
// init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,800);
    string windowTitle = "File: [ " + xmlFile + "]   ";
    drawAxisBool? windowTitle = windowTitle + "AXIS: TRUE |" :windowTitle = windowTitle + "AXIS: FALSE|";
    frontCull? windowTitle = windowTitle + " CULL: BACK |" :windowTitle = windowTitle + " CULL: FRONT|";
    drawModeFill? windowTitle = windowTitle + " DRAW: FILL|" :windowTitle = windowTitle + " DRAW: LINE|";
    alternatingColorFaces? windowTitle = windowTitle + " COLOR: 2  " :windowTitle = windowTitle + " COLOR: ANY";
    glutCreateWindow(windowTitle.c_str());
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


#include <list>
#include <string>

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
    float x = 0;
    float y = 0;
    float z = 0;
public:
    float getCoordinate(int index){
        float returnCoordinate;
        switch (index) {
            case 0:
                returnCoordinate = this->x;
                break;
            case 1:
                returnCoordinate = this->y;
                break;
            case 2:
                returnCoordinate = this->z;
                break;
            default:
                returnCoordinate = this->x;
                break;
        }
        return returnCoordinate;
    }
};

class Transformation{
public:
    /* 0 - Transform
     * 1 - Rotate
     * 2 - Scale
     * 3 - Color
     * */
    int type;
    float x;
    float y;
    float z;
    float angle;
    /* Catmull-Rom curves */
    float time = 0; // Time to move in the curve
    std::list<Point> catmullRomPoints; // List of points for catmull-rom curve
    bool align = true; // Align with the curve
    float prev_y[3] = {0,1,0};
public:
    void applyTransformation();
    std::string toString();
};

//Modelo carregado de ficheiros .3d
class Model {
public:
    std::string filename = "NaN";
    std::list<Point> points;
    std::list<Point> normals;
    std::list<Transformation> transformations{};
    std::string texture = "NaN";
    float difuse_color[4] = {50,50,50,1};
    float ambient_color[4] = {50,50,50,1};
    float specular_color[4] = {0,0,0,1};
    float emissive_color[4] = {0,0,0,1};
    float shininess = 0;
    void drawModel() const;
    void printOut();
};

class Light {
public:
    std::string type;
    float posicao[3] = {0,10,0};
    float direcao[3] = {1,1,1};
    float cutoff = 45;
    void drawLight();
    void printOut();
};


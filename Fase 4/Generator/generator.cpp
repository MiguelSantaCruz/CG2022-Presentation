#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "generator.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

string filename;

//Classe Point
class Point{
public: float x,y,z;
};

void drawBox(float x, float y, float z, int slices)
{
    float x1 = x / 2;
    float z1 = z / 2;
    float slicesX = x / slices;
    float slicesY = y / slices;
    float slicesZ = z / slices;

    fstream file;
    file.open(filename, ios::out);

    string normal = "0 -1 0\n";
    // Base
    float auxX = -x1;
    float auxY = 0-(y/2);
    float auxZ = -z1;
    for (int i = 0; i < slices; i++){
        for (int j = 0; j < slices; j++){
            // 1ºtriangulo
            file << auxX << " " << auxY << " " << auxZ << " " << normal;
            file << auxX + slicesX << " " << auxY << " " << auxZ << " " << normal;
            file << auxX + slicesX << " " << auxY << " " << auxZ + slicesZ << " " << normal;

            // 2ºtriangulo
            file << auxX + slicesX << " " << auxY << " " << auxZ + slicesZ << " " << normal;
            file << auxX << " " << auxY << " " << auxZ + slicesZ << " " << normal;
            file << auxX << " " << auxY << " " << auxZ << " " << normal;

            auxZ += slicesZ;
        }
        auxZ = -z1;
        auxX += slicesX;
    }
    // Cima
    auxX = -x1;
    auxZ = -z1;
    auxY = y-(y/2);
    normal = "0 1 0\n";
    for (int i = 0; i < slices; i++){
        for (int j = 0; j < slices; j++){

            file << auxX + slicesX << " " << auxY << " " << auxZ + slicesZ << " " << normal;
            file << auxX + slicesX << " " << auxY << " " << auxZ << " " << normal;
            file << auxX << " " << auxY << " " << auxZ << " " << normal;

            file << auxX << " " << auxY << " " << auxZ << " " << normal;
            file << auxX << " " << auxY << " " << auxZ + slicesZ << " " << normal;
            file << auxX + slicesX << " " << auxY << " " << auxZ + slicesZ << " " << normal;

            auxZ += slicesZ;
        }
        auxZ = -z1;
        auxX += slicesX;
    }

    // Lado direito
    auxX = x1;
    auxZ = z1;
    auxY = y -(y/2);
    normal = "1 0 0\n";
    for (int i = 0; i < slices; i++){
        for (int j = 0; j < slices; j++){

            file << auxX << " " << auxY << " " << auxZ << " " << normal;
            file << auxX << " " << auxY - slicesY << " " << auxZ << " " << normal;
            file << auxX << " " << auxY - slicesY << " " << auxZ - slicesZ << " " << normal;

            file << auxX << " " << auxY - slicesY << " " << auxZ - slicesZ << " " << normal;
            file << auxX << " " << auxY << " " << auxZ - slicesZ << " " << normal;
            file << auxX << " " << auxY << " " << auxZ << " " << normal;
            auxZ -= slicesZ;
        }
        auxZ = z1;
        auxY -= slicesY;
    }

    // Lado Esquerdo
    auxX = -x1;
    auxZ = -z1;
    auxY = y-(y/2);
    normal = "-1 0 0\n";
    for (int i = 0; i < slices; i++){
        for (int j = 0; j < slices; j++){

            file << auxX << " " << auxY << " " << auxZ << " " << normal;
            file << auxX << " " << auxY - slicesY << " " << auxZ << " " << normal;
            file << auxX << " " << auxY - slicesY << " " << auxZ + slicesZ << " " << normal;

            file << auxX << " " << auxY - slicesY << " " << auxZ + slicesZ << " " << normal;
            file << auxX << " " << auxY << " " << auxZ + slicesZ << " " << normal;
            file << auxX << " " << auxY << " " << auxZ << " " << normal;

            auxZ += slicesZ;
        }
        auxZ = -z1;
        auxY -= slicesY;
    }

    // Frente
    auxX = -x1;
    auxZ = z1;
    auxY = y-(y/2);
    normal = "0 0 1\n";
    for (int i = 0; i < slices; i++){
        for (int j = 0; j < slices; j++){
            file << auxX << " " << auxY << " " << auxZ << " " << normal;
            file << auxX << " " << auxY - slicesY << " " << auxZ << " " << normal;
            file << auxX + slicesX << " " << auxY - slicesY << " " << auxZ << " " << normal;

            file << auxX + slicesX << " " << auxY - slicesY << " " << auxZ << " " << normal;
            file << auxX + slicesX << " " << auxY << " " << auxZ << " " << normal;
            file << auxX << " " << auxY << " " << auxZ << " " << normal;

            auxX += slicesX;
        }
        auxX = -x1;
        auxY -= slicesY;
    }

    // Trás
    auxX = x1;
    auxZ = -z1;
    auxY = y-(y/2);
    normal = "0 0 -1\n";
    for (int i = 0; i < slices; i++){
        for (int j = 0; j < slices; j++){

            file << auxX << " " << auxY << " "<< auxZ << " " << normal;
            file << auxX << " " << auxY - slicesY << " " << auxZ << " " << normal;
            file << auxX - slicesX << " " << auxY - slicesY << " " << auxZ << " " << normal;

            file << auxX - slicesX << " " << auxY - slicesY << " " << auxZ << " " << normal;
            file << auxX - slicesX << " " << auxY << " " << auxZ << " " << normal;
            file << auxX << " " << auxY << " " << auxZ << " " << normal;
            auxX -= slicesX;
        }
        auxX = x1;
        auxY -= slicesY;
    }
}

void drawPlane(float length, int divisions){
    float x1 = length / 2;
    float z1 = length / 2;
    float div = length / divisions;

    int i, j;

    float x = -x1;
    float z = -z1;

    fstream file;
    file.open(filename, ios::out);
    string normal = "0 1 0\n";
    for (i = 0; i < divisions; i++){
        for (j = 0; j < divisions; j++){
            // 1º triangulo
            file << x << " 0 " << z << " " << normal;
            file << x + div << " 0 " << z + div << " " << normal;
            file << x + div << " 0 " << z << " " << normal;

            // 2º triangulo
            file << x + div << " 0 " << z + div << " " << normal;
            file << x << " 0 " << z << " " << normal;
            file << x << " 0 " << z + div << " " << normal;
            z += div;
        }
        z = -z1;
        x += div;
    }
}

void createSphere(float raio, int stacks, int slices)
{

    float delta1 = 2 * M_PI / stacks;
    float delta2 = M_PI / slices;

    float theta1 = -M_PI / 2;
    float theta2 = 0.0f;

    fstream file;
    file.open(filename, ios::out);
    float x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4;
    float a, aux = 0;

    for (int i = 0; i < slices; i++)
    {

        a = 0;

        for (int j = 0; j < stacks; j++)
        {

            x1 = raio * sin(aux) * sin(a);
            y1 = raio * cos(aux);
            z1 = raio * sin(aux) * cos(a);

            x2 = raio * sin(aux + delta2) * sin(a + delta1);
            y2 = raio * cos(aux + delta2);
            z2 = raio * sin(aux + delta2) * cos(a + delta1);

            x3 = raio * sin(aux + delta2) * sin(a);
            y3 = raio * cos(aux + delta2);
            z3 = raio * sin(aux + delta2) * cos(a);

            x4 = raio * sin(aux) * sin(a + delta1);
            y4 = raio * cos(aux);
            z4 = raio * sin(aux) * cos(a + delta1);

            file << x1 << " " << y1 << " " << z1 << " ";
            double N = sqrt(pow(x1,2)+pow(y1,2)+pow(z1,2));
            if (N == 0)
                file << x1 << " " << y1 << " " << z1<<"\n";
            else file << x1/N << " " << y1/N << " " << z1/N <<"\n";

            file << x3 << " " << y3 << " " << z3 << " ";
            N = sqrt(pow(x3,2)+pow(y3,2)+pow(z3,2));
            if (N == 0)
                file << x3 << " " << y3 << " " << z3<<"\n";
            else file << x3/N << " " << y3/N << " " << z3/N <<"\n";


            file << x2 << " " << y2 << " " << z2 << " ";
            N = sqrt(pow(x2,2)+pow(y2,2)+pow(z2,2));
            if (N == 0)
                file << x2 << " " << y2 << " " << z2<<"\n";
            else file << x2/N << " " << y2/N << " " << z2/N <<"\n";


            file << x1 << " " << y1 << " " << z1 << " ";
            N = sqrt(pow(x1,2)+pow(y1,2)+pow(z1,2));
            if (N == 0)
                file << x1 << " " << y1 << " " << z1<<"\n";
            else file << x1/N << " " << y1/N << " " << z1/N <<"\n";


            file << x2 << " " << y2 << " " << z2 << " ";
            N = sqrt(pow(x2,2)+pow(y2,2)+pow(z2,2));
            if (N == 0)
                file << x2 << " " << y2 << " " << z2<<"\n";
            else file << x2/N << " " << y2/N << " " << z2/N <<"\n";


            file << x4 << " " << y4 << " " << z4 << " ";
            N = sqrt(pow(x4,2)+pow(y4,2)+pow(z4,2));
            if (N == 0)
                file << x4 << " " << y4 << " " << z4<<"\n";
            else file << x4/N << " " << y4/N << " " << z4/N <<"\n";

            a += delta1;
        }

	theta1 += delta1;
        aux += delta2;
    }
}


void createCone(float radius, float height, int slices, int stacks)
{
    fstream file;
    file.open(filename, ios::out);

    float theta = 0;
    float nextTheta = 0;
    float delta = (2 * M_PI) / slices;
    float raio = radius / stacks;
    float altura = height / stacks;
    int i, j;

    // fazer a circunferência da base
    for (i = 0; i < slices; i++)
    {
        nextTheta = theta + delta;

        file << "0 0 0 0 -1 0\n";
        file << radius * sin(nextTheta) << " 0 " << radius * cos(nextTheta) << "0 -1 0\n";
        file << radius * sin(theta) << " 0 " << radius * cos(theta) << "0 -1 0\n";


        theta = nextTheta;
    } // Fazer as laterais
    float r1 = radius;
    float r2 = radius - raio;
    float alt1 = 0;
    float alt2 = altura;
    theta = 0;
    nextTheta = 0;

    for (i = 0; i < slices; i++)
    {
        nextTheta += delta;

        for (j = 0; j < stacks; j++)
        {
            file << r1 * sin(nextTheta) << " " << alt1 << " " << r1 * cos(nextTheta) << " ";

            //Normal
            double dis = sqrt(pow(r1*sin(nextTheta),2)+pow(alt1-height,2)+pow(r1 * cos(nextTheta), 2));
            double D = dis / cos(atan(radius/height));
            double N = sqrt(pow(r1*sin(nextTheta),2)+pow(alt1-(height-D),2)+pow(r1 * cos(nextTheta),2));
            if (N == 0)
                file <<r1 * sin(nextTheta) << " " << alt1-(height-D) << " " << r1 * cos(nextTheta) << "\n";
            else file <<(r1 * sin(nextTheta))/N << " " << (alt1-(height-D))/N << " " << (r1 * cos(nextTheta))/N << "\n";

            file << r2 * sin(nextTheta) << " " << alt2 << " " << r2 * cos(nextTheta) << " ";
            // normal do ponto
            dis= sqrt(pow(r2*sin(nextTheta),2)+pow(alt2-height,2)+pow(r2 * cos(nextTheta), 2));
            D = dis / cos(atan(radius/height));

            N = sqrt(pow(r1*sin(nextTheta),2)+pow(alt2-(height-D),2)+pow(r2 * cos(nextTheta),2));
            if (N == 0)
                file << r2 * sin(nextTheta) << " " << alt2-(height-D) << " " << r2 * cos(nextTheta) << "\n";
            else file <<(r2 * sin(nextTheta))/N << " " << (alt2-(height-D))/N << " " << (r2 * cos(nextTheta))/N << "\n";

            file << r1 * sin(theta) << " " << alt1 << " " << r1 * cos(theta) << " ";

            // normal do ponto
            dis = sqrt(pow(r1*sin(theta),2)+pow(alt1-height,2)+pow( r1 * cos(theta), 2));
            D = dis / cos(atan(radius/height));

            N = sqrt(pow(r1*sin(theta),2)+pow(alt1-(height-D),2)+pow(r1 * cos(theta),2));
            if (N == 0)
                file <<r1 * sin(theta) << " " << alt1-(height-D) << " " << r1 * cos(theta) << "\n";
            else file <<(r1 * sin(theta))/N << " " << (alt1-(height-D))/N << " " << (r1 * cos(theta))/N << "\n";




            file << r2 * sin(nextTheta) << " " << alt2 << " " << r2 * cos(nextTheta) << " ";

            N = sqrt(pow(r2*sin(nextTheta),2)+pow(alt2-(height-D),2)+pow(r2 * cos(nextTheta),2));
            if (N == 0)
                file <<r2 * sin(nextTheta) << " " << alt2-(height-D) << " " << r2 * cos(nextTheta) << "\n";
            else file <<(r2 * sin(nextTheta))/N << " " << (alt2-(height-D))/N << " " << (r2 * cos(nextTheta))/N << "\n";

            file << r2 * sin(theta) << " " << alt2 << " " << r2 * cos(theta) << " ";

            // normal do ponto
            dis = sqrt(pow(r2*sin(theta),2)+pow(alt2-height,2)+pow(r2 * cos(theta), 2));
            D = dis / cos(atan(radius/height));

            N = sqrt(pow(r2*sin(theta),2)+pow(alt2-(height-D),2)+pow(r2 * cos(theta),2));
            if (N == 0)
                file <<r2 * sin(theta) << " " << alt2-(height-D) << " " << r2 * cos(theta) << "\n";
            else file <<(r2 * sin(theta))/N << " " << (alt2-(height-D))/N << " " << (r2 * cos(theta))/N << "\n";

            file << r1 * sin(theta) << " " << alt1 << " " << r1 * cos(theta) << " ";

            // normal do ponto
            dis = sqrt(pow(r1*sin(theta),2)+pow(alt1-height,2)+pow( r1 * cos(theta), 2));
            D = dis / cos(atan(radius/height));

            N = sqrt(pow(r1*sin(theta),2)+pow(alt1-(height-D),2)+pow(r1 * cos(theta),2));
            if (N == 0)
                file <<r1 * sin(theta) << " " << alt1-(height-D) << " " << r1 * cos(theta) << "\n";
            else file <<(r1 * sin(theta))/N << " " << (alt1-(height-D))/N << " " << (r1 * cos(theta))/N << "\n";

            r1 -= raio;
            r2 -= raio;
            alt1 += altura;
            alt2 += altura;
        }
        r1 = radius;
        r2 = radius - raio;
        alt1 = 0;
        alt2 = altura;
        theta = nextTheta;
    }
}


void showSintaxError()
{
    printf("Sintax error:\n");
    printf("   Usage: ./generator [Shape] [Args] [Output File]\n");
    printf(" Shapes available: \n");
    printf("    plane: [length] [divisions]\n");
    printf("    box: [size] [divisions]\n");
    printf("    sphere: [radious] [slices] [height]\n");
    printf("    cone: [radious] [heigth] [Slices] [stacks]\n");
    printf("    bezier: [.patch file] [Tesselation]\n");
    exit(-1);
}

void createBezierPatch(const char* patch, int tesselation, const char* savefilename){
    std::fstream f;
    f.open(patch,std::ios::in);
    int patches=0;
    int vertices=0;
    std::vector<std::vector<int> > indicesPatch;
    std::vector<Point> vertex;

    if(f.is_open()){
        std::string line;
        if(getline(f,line)) sscanf(line.c_str(),"%d\n",&patches);
        for(int i=0; i<patches ;i++){
            std::vector<int> aux;
            if(getline(f,line)){
                int n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16=0;
                sscanf(line.c_str(),"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n",&n1,&n2,&n3,&n4,&n5,&n6,&n7,&n8,&n9,&n10,&n11,&n12,&n13,&n14,&n15,&n16);
                aux.push_back(n1);
                aux.push_back(n2);
                aux.push_back(n3);
                aux.push_back(n4);
                aux.push_back(n5);
                aux.push_back(n6);
                aux.push_back(n7);
                aux.push_back(n8);
                aux.push_back(n9);
                aux.push_back(n10);
                aux.push_back(n11);
                aux.push_back(n12);
                aux.push_back(n13);
                aux.push_back(n14);
                aux.push_back(n15);
                aux.push_back(n16);
            }

            indicesPatch.push_back(aux);
        }
        if(getline(f,line)) sscanf(line.c_str(),"%d\n",&vertices);
        for(int i=0; i<vertices ;i++){
            float x=0,y=0,z=0;
            if(getline(f,line)) sscanf(line.c_str(),"%f, %f, %f\n",&x,&y,&z);

            Point p;
            p.x = x; p.y = y; p.z = z;

            vertex.push_back(p);
        }

        f.close();
    }
    else { printf("Error: can't open .patch file\n"); exit(0); }

    float res[3];
    float t;
    int index, indices[4];
    float q[4][tesselation][3],r[tesselation][tesselation][3],tess = 1/((float)tesselation-1);
    float pontos = patches*(tesselation)*2*(tesselation)*3*3;

    std::fstream g;
    g.open(savefilename,std::ios::out);

    if(g.is_open()){
        for(int n=0; n<patches; n++){
            float p[16][3];
            for(int m=0; m<16; m++){
                p[m][0] = vertex[indicesPatch[n][m]].x;
                p[m][1] = vertex[indicesPatch[n][m]].y;
                p[m][2] = vertex[indicesPatch[n][m]].z;
            }
            int j=0,k=0;
            for(int i=0; i<15; i+=4){
                indices[0] = i;
                indices[1] = i+1;
                indices[2] = i+2;
                indices[3] = i+3;
                for(int a=0; a<tesselation-1; a++){
                    t = a*tess;
                    index = floor(t);
                    t = t - index;
                    res[0] = (-p[indices[0]][0] +3*p[indices[1]][0] -3*p[indices[2]][0] +p[indices[3]][0])*t*t*t + (3*p[indices[0]][0] -6*p[indices[1]][0] +3*p[indices[2]][0])*t*t + (-3*p[indices[0]][0] +3*p[indices[1]][0])*t + p[indices[0]][0];
                    res[1] = (-p[indices[0]][1] +3*p[indices[1]][1] -3*p[indices[2]][1] +p[indices[3]][1])*t*t*t + (3*p[indices[0]][1] -6*p[indices[1]][1] +3*p[indices[2]][1])*t*t + (-3*p[indices[0]][1] +3*p[indices[1]][1])*t + p[indices[0]][1];
                    res[2] = (-p[indices[0]][2] +3*p[indices[1]][2] -3*p[indices[2]][2] +p[indices[3]][2])*t*t*t + (3*p[indices[0]][2] -6*p[indices[1]][2] +3*p[indices[2]][2])*t*t + (-3*p[indices[0]][2] +3*p[indices[1]][2])*t + p[indices[0]][2];
                    q[j][k][0] = res[0];
                    q[j][k][1] = res[1];
                    q[j][k][2] = res[2];
                    k++;
                }

                t = 1;

                res[0] = (-p[indices[0]][0] +3*p[indices[1]][0] -3*p[indices[2]][0] +p[indices[3]][0])*t*t*t + (3*p[indices[0]][0] -6*p[indices[1]][0] +3*p[indices[2]][0])*t*t + (-3*p[indices[0]][0] +3*p[indices[1]][0])*t + p[indices[0]][0];
                res[1] = (-p[indices[0]][1] +3*p[indices[1]][1] -3*p[indices[2]][1] +p[indices[3]][1])*t*t*t + (3*p[indices[0]][1] -6*p[indices[1]][1] +3*p[indices[2]][1])*t*t + (-3*p[indices[0]][1] +3*p[indices[1]][1])*t + p[indices[0]][1];
                res[2] = (-p[indices[0]][2] +3*p[indices[1]][2] -3*p[indices[2]][2] +p[indices[3]][2])*t*t*t + (3*p[indices[0]][2] -6*p[indices[1]][2] +3*p[indices[2]][2])*t*t + (-3*p[indices[0]][2] +3*p[indices[1]][2])*t + p[indices[0]][2];

                q[j][k][0] = res[0];
                q[j][k][1] = res[1];
                q[j][k][2] = res[2];
                j++;
                k=0;
            }

            for(int j=0; j<tesselation; j++){
                for(int a=0; a<tesselation-1; a++){
                    t = a*tess;;
                    index = floor(t);
                    t = t - index;

                    res[0] = (-q[0][j][0] +3*q[1][j][0] -3*q[2][j][0] +q[3][j][0])*t*t*t + (3*q[0][j][0] -6*q[1][j][0] +3*q[2][j][0])*t*t + (-3*q[0][j][0] +3*q[1][j][0])*t + q[0][j][0];
                    res[1] = (-q[0][j][1] +3*q[1][j][1] -3*q[2][j][1] +q[3][j][1])*t*t*t + (3*q[0][j][1] -6*q[1][j][1] +3*q[2][j][1])*t*t + (-3*q[0][j][1] +3*q[1][j][1])*t + q[0][j][1];
                    res[2] = (-q[0][j][2] +3*q[1][j][2] -3*q[2][j][2] +q[3][j][2])*t*t*t + (3*q[0][j][2] -6*q[1][j][2] +3*q[2][j][2])*t*t + (-3*q[0][j][2] +3*q[1][j][2])*t + q[0][j][2];
                    r[j][k][0] = res[0];
                    r[j][k][1] = res[1];
                    r[j][k][2] = res[2];
                    k++;
                }

                t = 1;

                res[0] = (-q[0][j][0] +3*q[1][j][0] -3*q[2][j][0] +q[3][j][0])*t*t*t + (3*q[0][j][0] -6*q[1][j][0] +3*q[2][j][0])*t*t + (-3*q[0][j][0] +3*q[1][j][0])*t + q[0][j][0];
                res[1] = (-q[0][j][1] +3*q[1][j][1] -3*q[2][j][1] +q[3][j][1])*t*t*t + (3*q[0][j][1] -6*q[1][j][1] +3*q[2][j][1])*t*t + (-3*q[0][j][1] +3*q[1][j][1])*t + q[0][j][1];
                res[2] = (-q[0][j][2] +3*q[1][j][2] -3*q[2][j][2] +q[3][j][2])*t*t*t + (3*q[0][j][2] -6*q[1][j][2] +3*q[2][j][2])*t*t + (-3*q[0][j][2] +3*q[1][j][2])*t + q[0][j][2];

                r[j][k][0] = res[0];
                r[j][k][1] = res[1];
                r[j][k][2] = res[2];
                k=0;
            }

            for(int i=0; i<tesselation-1; i++)
                for(int j=0; j<tesselation-1; j++){
                    g << r[i][j][0];   g << ' '; g << r[i][j][1];   g << ' '; g << r[i][j][2];   g << '\n';
                    g << r[i+1][j][0]; g << ' '; g << r[i+1][j][1]; g << ' '; g <<r[i+1][j][2];  g << '\n';
                    g << r[i][j+1][0]; g << ' '; g << r[i][j+1][1]; g << ' '; g << r[i][j+1][2]; g << '\n';

                    g << r[i+1][j][0];   g << ' '; g << r[i+1][j][1];   g << ' '; g << r[i+1][j][2];   g << '\n';
                    g << r[i+1][j+1][0]; g << ' '; g << r[i+1][j+1][1]; g << ' '; g << r[i+1][j+1][2]; g << '\n';
                    g << r[i][j+1][0];   g << ' '; g << r[i][j+1][1];   g << ' '; g << r[i][j+1][2];   g << '\n';
                }
        }
        g.close();
    }
    else { printf("Error: Cannot open file...\n"); exit(0); }
}



int main(int argc, char **argv) {
    if (argc == 1)
        showSintaxError();

    if (strcmp(argv[1], "plane") == 0) {
        filename = argv[4];
        if (argc != 5)
            showSintaxError();
        drawPlane(atof(argv[2]), atof(argv[3]));
    } else if (strcmp(argv[1], "sphere") == 0) {
        if (argc != 6)
            showSintaxError();
        filename = argv[5];
        createSphere(atof(argv[2]), atof(argv[3]), atof(argv[4]));
    } else if ((strcmp(argv[1], "cone") == 0)) {
        if (argc != 7)
            showSintaxError();
        filename = argv[6];
        createCone(atof(argv[2]), atof(argv[3]), atoi(argv[4]), atoi(argv[5]));
    } else if (strcmp(argv[1], "box") == 0) {
        if (argc != 5)
            showSintaxError();
        filename = argv[4];
        drawBox(atof(argv[2]), atof(argv[2]), atof(argv[2]), atoi(argv[3]));
    } else if (strcmp(argv[1], "bezier") == 0) {
        if (argc != 5)
            showSintaxError();
        char *file = argv[2];
        filename = argv[4];
        createBezierPatch(file, atoi(argv[3]), filename.c_str());
    } else
        showSintaxError();
    return 1;
}

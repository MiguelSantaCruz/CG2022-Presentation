#include "CatmullRom.h"
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>

/* Given an index get the point at that index in the list */
Point listGet(std::list<Point> _list, int _i){
    std::list<Point>::iterator it = _list.begin();
    for(int i=0; i<_i; i++){
        ++it;
    }
    return *it;
}

//Build rotation matrix to apply to object being translated
void buildRotMatrix(float *x, float *y, float *z, float *m) {

    m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
    m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
    m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
    m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}

// Cross product between two vectors
void cross(float *a, float *b, float *res) {
    res[0] = a[1]*b[2] - a[2]*b[1];
    res[1] = a[2]*b[0] - a[0]*b[2];
    res[2] = a[0]*b[1] - a[1]*b[0];
}

// Normalize a vector
void normalize(float *a) {
    float l = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
    a[0] = a[0]/l;
    a[1] = a[1]/l;
    a[2] = a[2]/l;
}

// Returns the norm of a vector
float length(float *v) {

    float res = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    return res;

}

// Multiply two matrices
void multMatrixVector(float *m, float *v, float *res) {

    for (int j = 0; j < 4; ++j) {
        res[j] = 0;
        for (int k = 0; k < 4; ++k) {
            res[j] += v[k] * m[j * 4 + k];
        }
    }

}

// Calculate Catmull-Rom point and derivative given 4 vectors representing x,y,z coordinates of points
void getCatmullRomPoint(float t, Point p0, Point p1, Point p2, Point p3, float *pos, float *deriv) {

    // Catmull-Rom matrix
    float catmulRomMatrix[4][4] = {	{-0.5f,  1.5f, -1.5f,  0.5f},
                                       { 1.0f, -2.5f,  2.0f, -0.5f},
                                       {-0.5f,  0.0f,  0.5f,  0.0f},
                                       { 0.0f,  1.0f,  0.0f,  0.0f}};

    for (int i = 0; i < 3; i++) {
        float points[4] = { p0.getCoordinate(i),p1.getCoordinate(i),p2.getCoordinate(i),p3.getCoordinate(i)};
        // Make the product of Points and Catmull-Rom matrix
        float a[4];
        multMatrixVector(*catmulRomMatrix, points, a);
        // Compute pos = T * A to get point
        pos[i] = powf(t,3) * a[0] + powf(t,2)*a[1]+t*a[2]+a[3];
        // Compute deriv = T' * A to get derivative at point
        deriv[i] = 3*powf(t,2) * a[0] + 2* t*a[1]+ a[2];
    }

}


// given  global t, returns the point in the curve
void getGlobalCatmullRomPoint(float gt, float *pos, float *deriv,std::list<Point> points) {

    int point_count = points.size();
    float t = gt * point_count; // this is the real global t
    int index = floor(t);  // which segment
    t = t - index; // where within  the segment

    // indices store the points
    int indices[4];
    indices[0] = (index + point_count-1)%point_count;
    indices[1] = (indices[0]+1)%point_count;
    indices[2] = (indices[1]+1)%point_count;
    indices[3] = (indices[2]+1)%point_count;

    getCatmullRomPoint(t, listGet(points,indices[0]), listGet(points,indices[1]), listGet(points,indices[2]), listGet(points,indices[3]), pos, deriv);
}



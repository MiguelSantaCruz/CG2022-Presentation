#include "AuxiliaryStructures.h"

//Build rotation matrix to apply to object being translated
void buildRotMatrix(float *x, float *y, float *z, float *m);

// Cross product between two vectors
void cross(float *a, float *b, float *res);

// Normalize a vector
void normalize(float *a);

// Returns the norm of a vector
float length(float *v);

// Multiply two matrices
void multMatrixVector(float *m, float *v, float *res);

// Calculate Catmull-Rom point and derivative given 4 vectors representing x,y,z coordinates of points
void getCatmullRomPoint(float t, Point p0, Point p1, Point p2, Point p3, float *pos, float *deriv);

// given  global t, returns the point in the curve
void getGlobalCatmullRomPoint(float gt, float *pos, float *deriv,std::list<Point> points);




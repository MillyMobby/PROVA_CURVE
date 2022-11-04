#pragma once
#include "Vec3f.h"
struct CurvePoint
{
    Vec3f V; // Vertex
    Vec3f T; // Tangent  (unit)
    Vec3f N; // Normal   (unit)
    Vec3f B; // Binormal (unit)
};

class CurveMath
{
private:
    std::vector<CurvePoint> Curve;
    std::vector<double> knots;		
    std::vector<Vec3f> controlPoints;

    int degree;
    double t = 0.001;	

public:
    void generateKnots();
    std::vector<CurvePoint> BSpline(int u);
};


#pragma once
#include "Vec3d.h"
struct CurvePoint
{
    Vec3d V; // Vertex
    Vec3d T; // Tangent  (unit)
    Vec3d N; // Normal   (unit)
    Vec3d B; // Binormal (unit)
};

class CurveMath
{
private:
    std::vector<CurvePoint> Curve;
    std::vector<double> knots;		
    std::vector<Vec3d> controlPoints;

    int degree;
    double t = ((double)1 / 3);

public:
    CurveMath() {};
    ~CurveMath() {};

    int getKnotsInterval(int t);
    void generateKnots();
    void setControlPoints(std::vector<Vec3d> cp);
    void setDegree(int p);
    std::vector<CurvePoint> BSpline(int u);
    Vec3d deCasteljau(std::vector<Vec3d> controlPoints);
    std::vector<double> fullInsertion(int knotInterval);
    Vec3d deBoor(std::vector<Vec3d> controlPoints, double increment, int knotInterval);
   
};


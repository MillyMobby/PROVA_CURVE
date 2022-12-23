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
    std::vector<int> multiplicities;
    std::vector<double> knots;		
    std::vector<Vec3d> controlPoints; 

    int degree;
    bool bezier = false;
    
    /*double increment = 0.01;
    int steps = 1 / increment;*/

public:
    CurveMath() {};
    ~CurveMath() {};

    bool degenere(int pointsNum);
    const int getKnotsInterval(double t);
    const std::vector<double> getKnotsVector();

    std::vector<int> getMultiplicityVector();
    void editMultiplicity(int pointIndex);
    void resizeMultiplicities();
    void setMultiplicity(int i);
    inline int getPointMultiplicity(int i) const { return multiplicities[i]; }
    void deleteLastMultiplicity();
    void deleteFirstMultiplicity(int i);

    const double getKnot(int i);
    inline int getKnotsSize() const {  return knots.size(); }
    inline bool isBezier() const { return bezier; }
    inline void setBezier(bool isBezier) { bezier = isBezier; }
    inline int getDegree() const { return degree; }

    inline double getDomainLeft() const { return 0; }
    inline double getDomainRight() const { return 1; }
   // inline double getIncrement() const {   return increment;  }
    
    void generateKnots();
    void setControlPoints(std::vector<Vec3d> cp);
    void setDegree(int p);
    std::vector<CurvePoint> BSpline(int u);
    Vec3d deCasteljau(/*std::vector<Vec3d> controlPoints,*/ double t, std::vector<float> w);
    Vec3d deCas(double t, std::vector<float> w);
    std::vector<double> fullInsertion(int knotInterval, double t);   
    Vec3d deBoor(/*std::vector<Vec3d> controlPoints,*/ double t, std::vector<float> w);
    //Vec3d deBoorModify(std::vector<Vec3d> controlPoints, int knotInterval, double t);
    Vec3d deb(double t, std::vector<float> w);

    void curvePointsInSupport(std::vector<Vec3d> curvePointsVec);

    void addWeights(std::vector<float> w, std::vector<Vec3d> controlPol);
    void removeWeights(std::vector<float> w, std::vector<Vec3d> controlPol);
    void print();
};


#pragma once
#include "Vec3f.h"
#include "Shader.h"
#include "CurveMath.h"
class CurveGraphics
{
private:
	static const int MaxNumPoints = 100;    
	int _pointsNumber = 0;  
	
	//double _controlPolygon[MaxNumPoints][3];
	double controlPolygon[MaxNumPoints*3];
	int _selectedVert = -1;

	GLuint _VAO = -1; // SONO INT -> perché sono riferimenti// Vertex Array Object - holds info about an array of vertex data;
	GLuint _VBO = -1;// Vertex Buffer Object - holds an array of data
	GLuint _EBO = -1;

	CurveMath curveMath;
	
	unsigned int _stride = 3;
	const unsigned int _posLocation = 0;   // "location = 0"  vertex shader 
	const unsigned int _colorLocation = 1;
	
	Vec3f _pointsColor = Vec3f(0.0f, 1.0f, 1.0f); 
	Vec3f _lineColor = Vec3f(1.0f, 0.0f, 1.0f);
	
	bool example = true;

public:
	void setCurve();
	std::vector<Vec3d> controlPointsVector();
	double getControlPt_X(int i);
	double getControlPt_Y(int i);
	double getControlPt_Z(int i);

	//inline CurveGraphics& operator=(const CurveGraphics& m) {
	//	for (int i = 0; i < MaxNumPoints; i++)
	//	{
	//		//for (int j = 0; j < 2; j++) { _controlPolygon[i][j] = m._controlPolygon[i][j]; }
	//		_VAO = m._VAO;
	//		_VBO = m._VBO;
	//		
	//	}return *this;
	//};
	CurveGraphics() {};
	~CurveGraphics() { delete[] controlPolygon; };

	void initGL();
	void setupGL();
    void LoadPointsIntoVBO();
	void clipSpace();
    void AddPoint(double x, double y);
    void ChangePoint(int i, double x, double y);
    void RemoveFirstPoint();
	void RemoveLastPoint();
	void renderScene();

	int getSelectedVert();
	void setSelectedVert(int s);
	int getPointsNumber();

};


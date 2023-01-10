#pragma once
#pragma once
#include "Vec3f.h"
#include "Vec4d.h"
#include "Mat4d.h"
#include "Shader.h"
class LineStripGraphics
{
	static const int MaxNumPoints = 100;
	int _pointsNumber = 0, _curvePointsNumber = 0;
	//double* controlPolygon;// [MaxNumPoints * 3] ;
	double controlPolygon[MaxNumPoints * 2];
	int _selectedVert = -1;
	GLuint _VAO = -1;
	GLuint _VBO = -1;
	GLuint _EBO = -1;

	//VERTICES SETTINGS
	unsigned int _stride = 2;
	unsigned int _curveStride = 3;
	const unsigned int _posLocation = 0;   // "location = 0"  vertex shader 
	const unsigned int _colorLocation = 1;

	Vec3f _pointsColor = Vec3f(0.0f, 1.0f, 1.0f);
	Vec3f _lineColor = Vec3f(1.0f, 0.0f, 1.0f);

	bool isChanged = false;
public:
	LineStripGraphics() {
		//controlPolygon = new double[MaxNumPoints * 3];
		

	};
	~LineStripGraphics();

	const double& getControlPt_X(int& i);
	const double& getControlPt_Y(int& i);
	const double& getControlPt_Z(int& i);

	//RENDERING
	void initGL();
	void setupGL();
	void renderScene();
	void cleanGL(GLuint& shaderProgram);

	//POINTS MANIPULATION
	void LoadPointsIntoVBO();
	void AddPoint(double x, double y);
	void ChangePoint(int i, double x, double y, double z);
	void ChangePoint(int i, double x, double y);
	void setPointsVector(std::vector<float>  points);
	void initializeXcoordinates();

	const int getSelectedVert();
	void setSelectedVert(int s);

	void setLineColor(Vec3f col);

	const int getPointsNumber();

	void RemoveFirstPoint();
	void RemoveLastPoint();

};


#pragma once
#include "Vec3f.h"
#include "Shader.h"
#include "CurveMath.h"

class CurveGraphics
{
private:
	static const int MaxNumPoints = 100;    
	int _pointsNumber = 0;
	int _curvePointsNumber = 0;
	double* controlPolygon;// [MaxNumPoints * 3] ;
	static const int steps = 100;
	double curvePoints[steps * 10];
	int _selectedVert = -1;

	GLuint _VAO = -1; // SONO INT -> perché sono riferimenti// Vertex Array Object - holds info about an array of vertex data;
	GLuint _VBO = -1;// Vertex Buffer Object - holds an array of data
	GLuint _EBO = -1;

	GLuint _curveVAO = -1; // SONO INT -> perché sono riferimenti// Vertex Array Object - holds info about an array of vertex data;
	GLuint _curveVBO = -1;// Vertex Buffer Object - holds an array of data
	
	CurveMath curveMath;
	
	
	unsigned int _stride = 3;
	const unsigned int _posLocation = 0;   // "location = 0"  vertex shader 
	const unsigned int _colorLocation = 1;
	
	Vec3f _pointsColor = Vec3f(0.0f, 1.0f, 1.0f); 
	Vec3f _lineColor = Vec3f(1.0f, 0.0f, 1.0f);
	Vec3f _curveColor = Vec3f(1.0f, 0.7f, 1.0f);
	
	

public:
	
	void rendering(int &deg, bool makeBezier, bool makeNURBS, std::vector<float>& w);
	std::vector<Vec3d> controlPointsVector();
	std::vector<Vec3d>curvePointsVector();
	const double& getControlPt_X(int& i);
	const double& getControlPt_Y(int& i);
	const double& getControlPt_Z(int& i);

	//inline CurveGraphics& operator=(const CurveGraphics& m) {
	//	for (int i = 0; i < MaxNumPoints; i++)
	//	{
	//		//for (int j = 0; j < 2; j++) { _controlPolygon[i][j] = m._controlPolygon[i][j]; }
	//		_VAO = m._VAO;
	//		_VBO = m._VBO;
	//		
	//	}return *this;
	//};
	CurveGraphics() {
		controlPolygon = new double[MaxNumPoints * 3]; 
		//curvePoints = new double[steps * 3];
		curveMath.setDegree(2); // grado di default
	};
	~CurveGraphics();

	void initGL();
	void initCurveGL();
	void setupGL();
	void cleanGL(GLuint& shaderProgram);

    void LoadPointsIntoVBO();
	void LoadPointsIntoCurveVBO();
    void AddPoint(double x, double y);
	void AddCurvePoint(double x, double y);	
    void ChangePoint(int i, double x, double y);
	void modifyCurve(std::vector<float>& w/*int i, double x, double y*/);
	void ChangeCurvePoint(int i, double x, double y);

	void addWeights(std::vector<float> w);
	void removeWeights(std::vector<float> w);

    void RemoveFirstPoint(); 
	void RemoveLastPoint();

	void renderScene();
	void renderCurve();	

	const int getSelectedVert();
	void setSelectedVert(int s);
	const int getPointsNumber();

};


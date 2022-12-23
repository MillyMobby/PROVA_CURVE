#pragma once
#include "Vec3f.h"
#include "Vec4d.h"
#include "Mat4d.h"
#include "Shader.h"
#include "CurveMath.h"

class CurveGraphics
{
private:
	//POLIGONO DI CONTROLLO
	static const int MaxNumPoints = 100;    
	int _pointsNumber = 0, _curvePointsNumber = 0;
	double* controlPolygon;// [MaxNumPoints * 3] ;
	int _selectedVert = -1;
	GLuint _VAO = -1; 
	GLuint _VBO = -1;
	GLuint _EBO = -1;

	//CURVA
	static const int steps = 100;
	double curvePoints[steps * 10];
	GLuint _curveVAO = -1; 
	GLuint _curveVBO = -1;

	CurveMath curveMath;

	//VERTICES SETTINGS
	unsigned int _stride = 4;
	unsigned int _curveStride = 3;
	const unsigned int _posLocation = 0;   // "location = 0"  vertex shader 
	const unsigned int _colorLocation = 1;
	
	Vec3f _pointsColor = Vec3f(0.0f, 1.0f, 1.0f); 
	Vec3f _lineColor = Vec3f(1.0f, 0.0f, 1.0f);
	Vec3f _curveColor = Vec3f(1.0f, 0.7f, 1.0f);
	
	bool isChanged = false;
	Mat4d _transform;
public:	
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
		controlPolygon = new double[MaxNumPoints * 4]; 
		curveMath.generateKnots();
		curveMath.setDegree(3); // grado di default
	};
	~CurveGraphics();

	

	//ARRAYS CONVERTED IN Vec3d FORMAT
	std::vector<Vec3d> controlPointsVector();
	std::vector<Vec3d>curvePointsVector();

	const double& getControlPt_X(int& i);
	const double& getControlPt_Y(int& i);
	const double& getControlPt_Z(int& i);
	
	//RENDERING
	void initGL();
	void initCurveGL();
	void setupGL();
	void rendering(int &deg, bool makeBezier, bool makeNURBS, std::vector<float>& w);
	void renderScene();
	void renderCurve();	
	void cleanGL(GLuint& shaderProgram);

    //POINTS MANIPULATION
	void LoadPointsIntoVBO();
	void LoadPointsIntoCurveVBO();
    void AddPoint(double x, double y);
	void AddCurvePoint(double x, double y);	
    void ChangePoint(int i, double x, double y, double z);
	void ChangePoint(int i, double x, double y);
	void modifyCurve(std::vector<float>& w/*int i, double x, double y*/);
	void modifyControlPolygon(std::vector<float>& w);
	void ChangeCurvePoint(int i, double x, double y);

	const int getSelectedVert();
	void setSelectedVert(int s);
	
	const int getPointsNumber();

	//NURBS WEIGHTS (not used)
	void addWeights(std::vector<float> w);
	void removeWeights(std::vector<float> w);

    void RemoveFirstPoint(); 
	void RemoveLastPoint();
	void RemoveLastCurvePoint();

	void checkLastCurvePt(); //not used
	void example(double* v);
	void transformWithCamera(Camera& camera);
};


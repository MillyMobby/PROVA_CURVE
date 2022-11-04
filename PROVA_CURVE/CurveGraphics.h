#pragma once
#include "Vec3f.h"
#include "Shader.h"
class CurveGraphics
{
private:
	static const int MaxNumPoints = 100;    
	int _pointsNumber = 0;  
	
	float _controlPolygon[MaxNumPoints][3];
	int _selectedVert = -1;

	GLuint _VAO = -1; // SONO INT -> perché sono riferimenti// Vertex Array Object - holds info about an array of vertex data;
	GLuint _VBO = -1;// Vertex Buffer Object - holds an array of data
	GLuint _EBO = -1;

	/*unsigned int _vertex_mask = 0;
	unsigned int _stride = 3;*/
	
	const unsigned int _posLocation = 0;   // "location = 0"  vertex shader 
	const unsigned int _colorLocation = 1;
	
	Vec3f _pointsColor = Vec3f(0.0f, 1.0f, 1.0f); 
	Vec3f _lineColor = Vec3f(1.0f, 0.0f, 1.0f);

	/*bool hasPosition();
	bool hasNormal();
	bool hasColor();
	bool hasTexCoord();
	bool hasTangents();*/

	//const unsigned int _mesh_vertices_position = 1;
	//const unsigned int _mesh_vertices_texture_uv0 = 2;
	//const unsigned int _mesh_vertices_color = 4;
	//const unsigned int _mesh_vertices_normal = 8;
	//const unsigned int _mesh_vertices_tangent = 16;

	//const unsigned int _nElements_position = 3; // da quanti elementi è formata la posizione (x, y, z)
	//const unsigned int _nElements_texCoord = 2;
	//const unsigned int _nElements_color = 3;
	//const unsigned int _nElements_normal = 3;
	//const unsigned int _nElements_tangent = 3;

	

public:

	float getControlPt_X(int i);
	float getControlPt_Y(int i);
	float getControlPt_Z(int i);

	inline CurveGraphics& operator=(const CurveGraphics& m) {
		for (int i = 0; i < MaxNumPoints; i++)
		{
			for (int j = 0; j < 2; j++) { _controlPolygon[i][j] = m._controlPolygon[i][j]; }
			_VAO = m._VAO;
			_VBO = m._VBO;
			
		}return *this;
	};
	CurveGraphics() {};
	~CurveGraphics() {
	};

	void initGL();
	void setupGL();
    void LoadPointsIntoVBO();
	void insertPosition(const float& x, const float& y, const float& z);
    void AddPoint(float x, float y);
    void ChangePoint(int i, float x, float y);
    void RemoveFirstPoint();
	void RemoveLastPoint();
	void renderScene();

	int getSelectedVert();
	void setSelectedVert(int s);
	int getPointsNumber();


	/*void setHasPositions(bool hasPos);
	void setHasTexCoords(bool hasTexCoord);
	void setHasVertexColor(bool hasVColor);
	void setHasNormals(bool hasNormals);
	void setHasTangents(bool hasTangents);*/
	
};


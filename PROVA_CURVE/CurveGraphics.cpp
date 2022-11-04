#include "CurveGraphics.h"
int CurveGraphics::getSelectedVert() {
    return _selectedVert;
}
void CurveGraphics::setSelectedVert(int s) {
    _selectedVert = s;
}
int CurveGraphics::getPointsNumber() {
    return _pointsNumber;
}

float CurveGraphics::getControlPt_X(int i) {
    return _controlPolygon[i][0];
}

float CurveGraphics::getControlPt_Y(int i) {
    return _controlPolygon[i][1];
}

float CurveGraphics::getControlPt_Z(int i) {
    return _controlPolygon[i][2];
}

void CurveGraphics::initGL() {

    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);

    glBufferData(GL_ARRAY_BUFFER, MaxNumPoints * 3 * sizeof(float), (void*)0, GL_STATIC_DRAW);
    glVertexAttribPointer(_posLocation, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(_posLocation);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /*GLsizeiptr verticesSize = MaxNumDots * sizeof(_vertices[0]);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, _vertices.data(), GL_STATIC_DRAW);
    unsigned int position = 0;
    if (hasPosition()) {
        std::cout << "POSIZIONE ";
        glVertexAttribPointer(0, _nElements_position, GL_FLOAT, GL_FALSE, _stride * sizeof(GLfloat), (GLvoid*)(position * sizeof(GL_FLOAT)));
        glEnableVertexAttribArray(0);
        position += _nElements_position;
    }*/
    
    /*if (hasColor()) {
        std::cout << "COLOR ";
        glVertexAttribPointer(vertColor_loc, _nElements_color, GL_FLOAT, GL_FALSE, _stride * sizeof(GLfloat), (GLvoid*)(position * sizeof(GL_FLOAT)));
        glEnableVertexAttribArray(vertColor_loc);
        position += _nElements_color;
    }*/

   /* glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);*/

}

void CurveGraphics::setupGL() {

    glEnable(GL_DEPTH_TEST);	// depth buffering
    glDepthFunc(GL_LEQUAL);		// multipass shaders (?????????????)

    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);	// non vengono tondi
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);		// Antialias 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPointSize(30);
    glLineWidth(5);

}


void CurveGraphics::LoadPointsIntoVBO()
{
    glBindBuffer(GL_ARRAY_BUFFER, _VBO); 
    glBufferSubData(GL_ARRAY_BUFFER, 0, _pointsNumber * 3 * sizeof(float), _controlPolygon); //non e' un resize
   // glBufferSubData(GL_ARRAY_BUFFER, 0, _vertices.size()/3 * sizeof(_vertices[0]), _vertices.data());
}

void CurveGraphics::AddPoint(float x, float y)
{
    if (_pointsNumber < MaxNumPoints) {
        _controlPolygon[_pointsNumber][0] = x;
        _controlPolygon[_pointsNumber][1] = y;
        _controlPolygon[_pointsNumber][2] = 0;
        _pointsNumber++;
        LoadPointsIntoVBO();
    } 
}

void CurveGraphics::ChangePoint(int i, float x, float y)
{
    if (i >= 0 && i < _pointsNumber) {
        _controlPolygon[i][0] = x;
        _controlPolygon[i][1] = y;}
    LoadPointsIntoVBO();
}

void CurveGraphics::RemoveFirstPoint()
{
    if (_pointsNumber == 0) {
        return;
    }
    for (int i = 0; i < _pointsNumber - 1; i++) {
        _controlPolygon[i][0] = _controlPolygon[i + 1][0];
        _controlPolygon[i][1] = _controlPolygon[i + 1][1];
        _controlPolygon[i][2] = _controlPolygon[i + 1][2];
    }
    _pointsNumber--;
    if (_pointsNumber > 0) {
        LoadPointsIntoVBO();
    }
}

void CurveGraphics::RemoveLastPoint()
{
    _pointsNumber = (_pointsNumber > 0) ? _pointsNumber - 1 : 0;
}

//bool CurveGraphics::hasPosition() {
//    return true; //_vertex_mask & vertPos_loc;
//}
//
//bool CurveGraphics::hasTexCoord() {
//    return _vertex_mask & vertColor_loc;
//}
//
//bool CurveGraphics::hasColor() {
//
//    return true; //_vertex_mask & _mesh_vertices_color;
//}
//
void CurveGraphics::renderScene() {
    glBindVertexArray(_VAO);
    
    if (_pointsNumber > 0) {
        glVertexAttrib3f(_colorLocation, _lineColor.x, _lineColor.y, _lineColor.z);		// magenta
        glDrawArrays(GL_LINE_STRIP, 0, _pointsNumber);                //linee
    }

    // punti
    glVertexAttrib3f(_colorLocation, _pointsColor.x, _pointsColor.y, _pointsColor.z);		
    glDrawArrays(GL_POINTS, 0, _pointsNumber);

    glBindVertexArray(0);
    
}


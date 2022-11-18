#include "CurveGraphics.h"

CurveGraphics::~CurveGraphics() { std::cout << "DISTRUTTOREEEE"; delete[] controlPolygon; }

void CurveGraphics::setCurve() {
    std::vector<Vec3d> prova;
    prova = controlPointsVector();
    curveMath.setControlPoints(prova);
    curveMath.setDegree(3);
   // curveMath.setDegree(_pointsNumber - 1);
   ///curveMath.deCasteljau(prova); 
    curveMath.generateKnots(_pointsNumber);

    double x = double(0.4);
    if (curveMath.degenere(_pointsNumber) == false) { curveMath.deBoor(prova, x); }
}
const int CurveGraphics::getSelectedVert() {
    return _selectedVert;
}
void CurveGraphics::setSelectedVert(int s) {
    _selectedVert = s;
}
const int CurveGraphics::getPointsNumber() {
    return _pointsNumber;
}

const double& CurveGraphics::getControlPt_X(int& i) {
   // return _controlPolygon[i][0];
    return controlPolygon[i * _stride];
}

const double& CurveGraphics::getControlPt_Y(int& i) {
    //return _controlPolygon[i][1];
    return controlPolygon[i * _stride + 1];
}

const double& CurveGraphics::getControlPt_Z(int& i) {
   // return _controlPolygon[i][2];
    return controlPolygon[i * _stride + 2];
}

void CurveGraphics::initGL() {

    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);

    GLsizeiptr verticesSize = MaxNumPoints * sizeof(controlPolygon[0]);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, (void*)0, GL_STATIC_DRAW); 
    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, _stride * sizeof(GLdouble), (void*)0);
    glEnableVertexAttribArray(0);  
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
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

void CurveGraphics::cleanGL(GLuint &shaderProgram) {
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
    glDeleteProgram(shaderProgram);
}

void CurveGraphics::LoadPointsIntoVBO()
{
    glBindBuffer(GL_ARRAY_BUFFER, _VBO); 
    glBufferSubData(GL_ARRAY_BUFFER, 0, _pointsNumber * 3 * sizeof(double), controlPolygon); //non e' un resize
   // glBufferSubData(GL_ARRAY_BUFFER, 0, _vertices.size()/3 * sizeof(_vertices[0]), _vertices.data());    
}



void CurveGraphics::clipSpace() {
    for (int i = 0; i < _pointsNumber - 1; i++) {
        
        controlPolygon[i * _stride] = controlPolygon[(i + 1) * _stride];
        controlPolygon[i * _stride + 1] = controlPolygon[(i + 1) * _stride + 1];
        controlPolygon[i * _stride + 2] = controlPolygon[(i + 1) * _stride + 2];
    }
}

void CurveGraphics::AddPoint(double x, double y)
{
    if (_pointsNumber < MaxNumPoints && _pointsNumber!=0) {
       
        controlPolygon[_pointsNumber *_stride] = x;
        controlPolygon[_pointsNumber * _stride +1] = y;
        controlPolygon[_pointsNumber * _stride +2] = 0;
        
    } 
    else if (_pointsNumber == 0) {
        controlPolygon[0] = x;
        controlPolygon[1] = y;
        controlPolygon[2] = 0;
    }
        _pointsNumber++;
        LoadPointsIntoVBO(); 
        controlPointsVector();
}

void CurveGraphics::ChangePoint(int i, double x, double y)
{
    if (i >= 0 && i < _pointsNumber) {
        
        controlPolygon[i * _stride] = x;
        controlPolygon[i * _stride + 1] = y;
    }
    LoadPointsIntoVBO();
}

void CurveGraphics::RemoveFirstPoint()
{
    if (_pointsNumber == 0) {
        return;
    }
    for (int i = 0; i < _pointsNumber - 1; i++) {
        controlPolygon[i * _stride] = controlPolygon[(i + 1) * _stride ];
        controlPolygon[i * _stride + 1] = controlPolygon[(i + 1) * _stride + 1];
        controlPolygon[i * _stride + 2] = controlPolygon[(i + 1) * _stride + 2];
    }
    _pointsNumber--;
    if (_pointsNumber > 0) {
        LoadPointsIntoVBO();
    }
}

void CurveGraphics::RemoveLastPoint() {  _pointsNumber = (_pointsNumber > 0) ? _pointsNumber - 1 : 0; }

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

void CurveGraphics::renderCurve() {
    double uinc = 0.001;
    // draw curve
    glLineWidth(3.5);
    glBegin(GL_LINE_STRIP);

    std::vector<Vec3d> prova;
    prova = controlPointsVector();
    curveMath.setControlPoints(prova);
    curveMath.setDegree(3);
    int p = 3;
    // curveMath.setDegree(_pointsNumber - 1);
    ///curveMath.deCasteljau(prova); 
    curveMath.generateKnots(_pointsNumber);
    // populate (standard) knot sequence
    //generateKnots();

    // generate B-spline
    for (double u = curveMath.getKnot(p - 1) + uinc; u <= curveMath.getKnot(_pointsNumber); u += uinc) {
        // get delta value for this u
        if (curveMath.degenere(_pointsNumber) == false) { 
            Vec3d point = curveMath.deBoor(prova, u);glColor3f(1.0f, 0.0f, 0.0f);
            glVertex3f(point.x, point.y, 0); 
        }
   
    }
    glEnd();
}

std::vector<Vec3d> CurveGraphics::controlPointsVector() {
    
    Vec3d pt = Vec3d(0);
    std::vector<Vec3d> controlPt;
    //std::cout << "STAMPO I PUNTI DI CONTROLLO ->" << std::endl;
    for (int i = 0; i < _pointsNumber*3; i = i + 3) {
        
        controlPt.push_back(pt);
        
        controlPt[i/3].x = controlPolygon[i];
        controlPt[i/3].y = controlPolygon[i + 1];
        controlPt[i/3].z = controlPolygon[i + 2];
    }
    return controlPt;
}


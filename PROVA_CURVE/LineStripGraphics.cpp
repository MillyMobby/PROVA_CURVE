#include "LineStripGraphics.h"
LineStripGraphics::~LineStripGraphics() { /*delete[] controlPolygon;*/ }

const int LineStripGraphics::getSelectedVert() { return _selectedVert; }

void LineStripGraphics::setSelectedVert(int s) { _selectedVert = s; }

void LineStripGraphics::setLineColor(Vec3f col) {
    _lineColor = col;
}

const int LineStripGraphics::getPointsNumber() { return _pointsNumber; }

const double& LineStripGraphics::getControlPt_X(int& i) {
    return controlPolygon[i * _stride];
}

const double& LineStripGraphics::getControlPt_Y(int& i) {
    return controlPolygon[i * _stride + 1];
}

const double& LineStripGraphics::getControlPt_Z(int& i) {
    return controlPolygon[i * _stride + 2];
}

void LineStripGraphics::initGL() {
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    GLsizeiptr verticesSize = MaxNumPoints * sizeof(controlPolygon[0]);

    glBufferData(GL_ARRAY_BUFFER, verticesSize, (void*)0, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, _stride * sizeof(GLdouble), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}



void LineStripGraphics::setupGL() {

    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// depth buffering
    glDepthFunc(GL_LEQUAL);		// multipass shaders (?????????????)

    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);	// non vengono tondi
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);		// Antialias 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPointSize(10);
    glLineWidth(5);
}

void LineStripGraphics::cleanGL(GLuint& shaderProgram) {
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
    
    glDeleteProgram(shaderProgram);
}

void LineStripGraphics::LoadPointsIntoVBO()
{
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, _pointsNumber * 2 * sizeof(double), controlPolygon); //non e' un resize
   // glBufferSubData(GL_ARRAY_BUFFER, 0, _vertices.size()/3 * sizeof(_vertices[0]), _vertices.data());    
}




void LineStripGraphics::AddPoint(double x, double y)
{
    int preview;
    bool repeated = false;
    //_pointsNumber > 0 ? preview = _pointsNumber - 1 : preview = 0;

  /*  if (preview != 0) {
        if (controlPolygon[preview * _stride] == x && controlPolygon[preview * _stride + 1] == y) {
            std::cout << "il punto è ripetuto \n";
            curveMath.editMultiplicity(_pointsNumber - 1);
            repeated = true;
        }
    }*/
    if (_pointsNumber < MaxNumPoints /*&& repeated == false*/) {

        controlPolygon[_pointsNumber * _stride] = x;
        controlPolygon[_pointsNumber * _stride + 1] = y;
        //controlPolygon[_pointsNumber * _stride + 2] = 1;
        //controlPolygon[_pointsNumber * _stride + 3] = 1;
        _pointsNumber++; 
        //curveMath.editMultiplicity(_pointsNumber);
        LoadPointsIntoVBO();
    }
    //}      
    
    isChanged = true;
    /*if (_pointsNumber > curveMath.getDegree()) */


}



void LineStripGraphics::ChangePoint(int i, double x, double y, double z)
{
    if (i >= 0 && i < _pointsNumber) {
        controlPolygon[i * _stride] = x;
        controlPolygon[i * _stride + 1] = y;
        controlPolygon[i * _stride + 2] = z;
    }
    LoadPointsIntoVBO();

    
    isChanged = true;
}
void LineStripGraphics::ChangePoint(int i, double x, double y)
{
    if (i >= 0 && i < _pointsNumber) {
        controlPolygon[i * _stride] = x;
        controlPolygon[i * _stride + 1] = y;

    }
    LoadPointsIntoVBO();
    isChanged = true;
}

void LineStripGraphics::setPointsVector(std::vector<float> points) {
   
        for (int i = 0; i <= 10; i++) {
            ChangePoint(i,controlPolygon[i*_stride], points[i]);
            std::cout << "added pt " << controlPolygon[i * _stride] << ", " << points[i] << std::endl;
        }
    
}

void LineStripGraphics::initializeXcoordinates() {
    for (double t = 0; t <= 1 + 0.1; t = t + 0.1) {
        AddPoint(t,1);
    }
}
//void CurveGraphics::removeWeights(std::vector<float> w) {
//    for (int i = 0; i < _pointsNumber; i++) {
//        controlPolygon[i * _stride] = controlPolygon[i * _stride] / controlPolygon[i * _stride + 2];
//        controlPolygon[i * _stride + 1] = controlPolygon[i * _stride + 1] / controlPolygon[i * _stride + 2];
//        controlPolygon[i * _stride + 2] = w[i];
//    }
//}



void LineStripGraphics::RemoveFirstPoint()
{
    if (_pointsNumber == 0) { return; }

    for (int i = 0; i < _pointsNumber - 1; i++) {
        controlPolygon[i * _stride] = controlPolygon[(i + 1) * _stride];
        controlPolygon[i * _stride + 1] = controlPolygon[(i + 1) * _stride + 1];
        controlPolygon[i * _stride + 2] = controlPolygon[(i + 1) * _stride + 2];
        controlPolygon[i * _stride + 3] = controlPolygon[(i + 1) * _stride + 3];
        
    }
    _pointsNumber--;
    if (_pointsNumber > 0) {
        LoadPointsIntoVBO();
        
        isChanged = true;
    }
}

void LineStripGraphics::RemoveLastPoint() {
    if (_pointsNumber == 0) {
        return;
    }
    if (_pointsNumber > 0) {
        _pointsNumber--;
       
        isChanged = true;
    }
}


void LineStripGraphics::renderScene() {
    glBindVertexArray(_VAO);
    //linee
    if (_pointsNumber > 0)
    {
        glBegin(GL_LINE_STRIP);
        glVertexAttrib3f(_colorLocation, _lineColor.x, _lineColor.y, _lineColor.z);		// magenta
        glDrawArrays(GL_LINE_STRIP, 0, _pointsNumber);
        glEnd();
    }
    // punti
   /* glVertexAttrib3f(_colorLocation, _pointsColor.x, _pointsColor.y, _pointsColor.z);
    glDrawArrays(GL_POINTS, 0, _pointsNumber);*/

    glBindVertexArray(0);
}

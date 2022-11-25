#include "CurveGraphics.h"

CurveGraphics::~CurveGraphics() { std::cout << "DISTRUTTOREEEE"; delete[] controlPolygon; }

void CurveGraphics::setCurve() {
   //std::vector<Vec3d> prova;
   // prova = controlPointsVector();
   // curveMath.setControlPoints(prova);
   // curveMath.setDegree(3);
   ////// curveMath.setDegree(_pointsNumber - 1);
   ///////curveMath.deCasteljau(prova); 
   // curveMath.generateKnots(_pointsNumber);

   //// double x = double(0.4);
   // if (curveMath.degenere(_pointsNumber) == false) { curveMath.deBoor(prova, x); }
    //renderCurve();
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
 
    return controlPolygon[i * _stride];
}

const double& CurveGraphics::getControlPt_Y(int& i) {   
    return controlPolygon[i * _stride + 1];
}

const double& CurveGraphics::getControlPt_Z(int& i) {
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
    initCurveGL();
    
}

void CurveGraphics::initCurveGL() {
   
    glGenVertexArrays(1, &_curveVAO);
    glGenBuffers(1, &_curveVBO);
    glBindVertexArray(_curveVAO);
    glBindBuffer(GL_ARRAY_BUFFER, _curveVBO);
 GLsizeiptr verticesSize = (steps*3) * sizeof(controlPolygon[0]);
    
    glBufferData(GL_ARRAY_BUFFER, verticesSize, (void*)0, GL_STATIC_DRAW); 
    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, _stride * sizeof(GLdouble), (void*)0);
    glEnableVertexAttribArray(0);  
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void CurveGraphics::setupGL() {

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

void CurveGraphics::cleanGL(GLuint &shaderProgram) {
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
    glDeleteVertexArrays(1, &_curveVAO);
    glDeleteBuffers(1, &_curveVBO);
    glDeleteProgram(shaderProgram);
}

void CurveGraphics::LoadPointsIntoVBO()
{
    glBindBuffer(GL_ARRAY_BUFFER, _VBO); 
    glBufferSubData(GL_ARRAY_BUFFER, 0, _pointsNumber * 3 * sizeof(double), controlPolygon); //non e' un resize
   // glBufferSubData(GL_ARRAY_BUFFER, 0, _vertices.size()/3 * sizeof(_vertices[0]), _vertices.data());    
}

void CurveGraphics::LoadPointsIntoCurveVBO() {
    glBindBuffer(GL_ARRAY_BUFFER, _curveVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, _curvePointsNumber * 3 * sizeof(double), curvePoints);
}


void CurveGraphics::AddPoint(double x, double y)
{
    if (_pointsNumber < MaxNumPoints) {
       
        controlPolygon[_pointsNumber *_stride] = x;
        controlPolygon[_pointsNumber * _stride +1] = y;
        controlPolygon[_pointsNumber * _stride +2] = 0;       
    } 
  
        _pointsNumber++;
        LoadPointsIntoVBO(); 

       
      if (_pointsNumber > curveMath.getDegree()) curveMath.generateKnots(_pointsNumber);   
        
    
}

void CurveGraphics::check(int deg, bool makeBezier) {
    _lineColor = Vec3f(0.0f, 0.5f, 0.7f);
    
    if (makeBezier == true) { curveMath.setDegree(_pointsNumber-1); }
    else curveMath.setDegree(deg);   
 if (curveMath.degenere(_pointsNumber) == false) {
     //_lineColor =Vec3f(1.0f, 0.0f, 1.0f);
            std::cout << "INIZIO A GENERARE LA CURVA \n";
            // generateFullCurve();   
           modifyCurve();
        }
}
void CurveGraphics::AddCurvePoint(double x, double y) {
        curvePoints[_curvePointsNumber * _stride] = x;
        curvePoints[_curvePointsNumber * _stride + 1] = y;
        curvePoints[_curvePointsNumber * _stride + 2] = 0;
 
    _curvePointsNumber++;
    LoadPointsIntoCurveVBO();
}

void CurveGraphics::ChangePoint(int i, double x, double y)
{
    if (i >= 0 && i < _pointsNumber) {      
        controlPolygon[i * _stride] = x;
        controlPolygon[i * _stride + 1] = y;
    }
    LoadPointsIntoVBO();

    

}
void CurveGraphics::modifyCurve(/*int i, double x, double y*/) {

    double uinc = 0.01;
    std::vector<Vec3d> controlPoints = controlPointsVector();
   
    int c = 0;
    Vec3d point = Vec3d(0);
    curveMath.generateKnots(_pointsNumber);

    if (curveMath.degenere(_pointsNumber) == false) {
        // if (curveMath.getKnotsSize() == (curveMath.getDegree() + 1) * 2) {
        //std::cout << "curva di bezier \n";
        for (double u = curveMath.getKnot(curveMath.getDegree() - 1) + uinc; u <= curveMath.getKnot(_pointsNumber); u += uinc) {
            if (curveMath.getKnotsSize() == (curveMath.getDegree() + 1) * 2) {
                curveMath.setBezier(true);
                point = curveMath.deCasteljau(controlPoints, u);
                //ChangeCurvePoint(c, point.x, point.y);
                //AddCurvePoint(point.x, point.y);
            }
            else {
                point = curveMath.deBoor(controlPoints, double(u));
                curveMath.setBezier(false);
                
            }if (_curvePointsNumber > 0 && c < _curvePointsNumber) {
                     ChangeCurvePoint(c, point.x, point.y);
                }
                else if (_curvePointsNumber >= 0 && c >= _curvePointsNumber) {
                    AddCurvePoint(point.x, point.y);
                }
            c++;
            //std::cout << "punti della curva " << point << std::endl;
        }
    }

            
    
}
void CurveGraphics::ChangeCurvePoint(int i, double x, double y)
{
    if (i >= 0 && i < _curvePointsNumber) {
        curvePoints[i * _stride] = x;
        curvePoints[i * _stride + 1] = y;
    }
    LoadPointsIntoCurveVBO();
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
         glBegin(GL_LINE_STRIP);
        glVertexAttrib3f(_colorLocation, _lineColor.x, _lineColor.y, _lineColor.z);		// magenta
        glDrawArrays(GL_LINE_STRIP, 0, _pointsNumber); 
       glEnd();//linee
    }
    // renderCurve();
    // punti
    glVertexAttrib3f(_colorLocation, _pointsColor.x, _pointsColor.y, _pointsColor.z);		
    glDrawArrays(GL_POINTS, 0, _pointsNumber);
 
 glBindVertexArray(0);  
   
    
  
   
    // renderCurve();
   // lineStrip();
}

void CurveGraphics::renderCurve() { 
   // std::cout << " la curva ha " << _curvePointsNumber << " punti \n";
   
    if (_curvePointsNumber > 0) {
        glBindVertexArray(_curveVAO);
        glBegin(GL_LINE_STRIP);
        glLineWidth(2);
        glVertexAttrib3f(_colorLocation, _curveColor.x, _curveColor.y, _curveColor.z);		// magenta
        glDrawArrays(GL_LINE_STRIP, 0, _curvePointsNumber);  
       // glBindVertexArray(0);   
        glEnd();//linee
       /* glVertexAttrib3f(_colorLocation, _pointsColor.x, _pointsColor.y, _pointsColor.z);
        glDrawArrays(GL_POINTS, 0, _curvePointsNumber);*/
    }
    glBindVertexArray(0);

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

std::vector<Vec3d> CurveGraphics::curvePointsVector() {

    Vec3d pt = Vec3d(0);
    std::vector<Vec3d> curvept;
    //std::cout << "STAMPO I PUNTI DI CONTROLLO ->" << std::endl;
    for (int i = 0; i < _curvePointsNumber * 3; i = i + 3) {

        curvept.push_back(pt);

        curvept[i / 3].x = curvePoints[i];
        curvept[i / 3].y = curvePoints[i + 1];
        curvept[i / 3].z = curvePoints[i + 2];
        std::cout << curvept[i/3];
    }
    return curvept;
}



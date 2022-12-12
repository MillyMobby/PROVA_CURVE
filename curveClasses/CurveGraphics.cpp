#include "CurveGraphics.h"

CurveGraphics::~CurveGraphics() { std::cout << "DISTRUTTOREEEE"; delete[] controlPolygon; }

const int CurveGraphics::getSelectedVert() {  return _selectedVert; }

void CurveGraphics::setSelectedVert(int s) {  _selectedVert = s; }

const int CurveGraphics::getPointsNumber() {  return _pointsNumber; }

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
 GLsizeiptr verticesSize = (250*30/*steps*15*/) * sizeof(controlPolygon[0]);
    
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
    /*if (_pointsNumber < MaxNumPoints) {*/

        int preview;
        bool repeated = false;
        _pointsNumber > 0 ? preview = _pointsNumber - 1 : preview = 0;
        //
        if (preview !=0) { //int mul = curveMath.getMultiplicityVector().at(preview);   
            if (controlPolygon[preview * _stride] == x && controlPolygon[preview * _stride + 1] == y) { 
                std::cout << "il punto è ripetuto \n";
                curveMath.editMultiplicity(_pointsNumber - 1);
                repeated = true;
            }
        }
         if (_pointsNumber < MaxNumPoints && repeated ==false) {
           
            controlPolygon[_pointsNumber * _stride] = x;
            controlPolygon[_pointsNumber * _stride + 1] = y;
            controlPolygon[_pointsNumber * _stride + 2] = 0;
            _pointsNumber++; curveMath.editMultiplicity(_pointsNumber);
            LoadPointsIntoVBO(); 
        }
    //}      
        curveMath.setControlPoints(controlPointsVector());
        isChanged = true;
      /*if (_pointsNumber > curveMath.getDegree()) curveMath.generateKnots(_pointsNumber);*/   
        
    
}

void CurveGraphics::rendering(int &deg, bool makeBezier,  bool makeNURBS, std::vector<float>& w) {
    _lineColor = Vec3f(0.0f, 0.5f, 0.7f);
    //addWeights(w);
    if (makeBezier == true  && _pointsNumber> 1) 
    {
        if (deg != _pointsNumber - 1)isChanged = true;
        curveMath.setDegree(_pointsNumber - 1); 
        deg = curveMath.getDegree();       
    }
    else if (_pointsNumber >= deg + 1 && deg != curveMath.getDegree()) {
        curveMath.setDegree(deg);
        isChanged = true;
    }
    //else if (_pointsNumber <= 2 && _pointsNumber>0 && deg <=1) {}
    else deg = curveMath.getDegree();
    curveMath.resizeMultiplicities();

   if (isChanged == true || makeNURBS==true) { modifyCurve(w);}
    isChanged = false;
       if (curveMath.degenere(_pointsNumber) == false) {
           renderScene();
           if (deg == curveMath.getDegree() /*&& deg < _pointsNumber*/) renderCurve();
       }
       else renderScene();           
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

    curveMath.setControlPoints(controlPointsVector());
    isChanged = true;
}

void CurveGraphics::modifyCurve(std::vector<float>& w/*int i, double x, double y*/) 
{
    curveMath.resizeMultiplicities();
    
    double uinc = 0.004;
    std::vector<Vec3d> controlPoints = controlPointsVector(); 
    curveMath.setControlPoints(controlPoints);
    curveMath.generateKnots(_pointsNumber);
    int c = 0;
    Vec3d point = Vec3d(0);
    //curveMath.generateKnots(_pointsNumber);

    if (curveMath.degenere(_pointsNumber) == false) 
    {
        for (double u = curveMath.getKnot(curveMath.getDegree() - 1) ; u <= curveMath.getKnot/*sVector().size()*/(_pointsNumber + curveMath.getDegree())/*+uinc*/; u += uinc) {
            if (curveMath.getKnotsSize() == (curveMath.getDegree() + 1) * 2 /*&& u < 1*/)
            {
                curveMath.setBezier(true);
                point = curveMath.deCasteljau(u, w);
            }
            else
            {
                point = curveMath.deBoor(double(u), w);
                curveMath.setBezier(false);
            }
            //else if (u /*>*/>= 1) point = controlPoints.back()*w[_pointsNumber];
            if (_curvePointsNumber > 0 && c < _curvePointsNumber) { ChangeCurvePoint(c, point.x, point.y); }
            else if (_curvePointsNumber >= 0 && c >= _curvePointsNumber /*+1*/) { AddCurvePoint(point.x, point.y); }
            c++; 
            //if (_curvePointsNumber >= 0 && c/*+1*/ >= _curvePointsNumber-1) {AddCurvePoint(controlPolygon[(_pointsNumber) * _stride-1], controlPolygon[(_pointsNumber) * _stride+1-1]);  }         
        }// checkLastCurvePt(); AddCurvePoint(controlPolygon[(_pointsNumber)*_stride /*- 1*/], controlPolygon[(_pointsNumber)*_stride + 1 /*- 1*/]);
        //if (_curvePointsNumber > 0 && c+1 < _curvePointsNumber-1) { ChangeCurvePoint(c+1, controlPolygon[(_pointsNumber-1) * _stride], controlPolygon[(_pointsNumber-1 )* _stride + 1]); }         
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

void CurveGraphics::addWeights(std::vector<float> w) {
    for (int i = 0; i < _pointsNumber; i++) {
        controlPolygon[i * _stride] = controlPolygon[i * _stride] *w[i];
        controlPolygon[i * _stride + 1] = controlPolygon[i * _stride + 1]*w[i];
        controlPolygon[i * _stride + 2] = w[i];
    }
}

void CurveGraphics::removeWeights(std::vector<float> w) {
    for (int i = 0; i < _pointsNumber; i++) {
        controlPolygon[i * _stride] = controlPolygon[i * _stride] / controlPolygon[i * _stride + 2];
        controlPolygon[i * _stride + 1] = controlPolygon[i * _stride + 1] / controlPolygon[i * _stride + 2];
        controlPolygon[i * _stride + 2] = w[i];
    }
}

void CurveGraphics::RemoveFirstPoint()
{
    if (_pointsNumber == 0) {   return;  }
    
    for (int i = 0; i < _pointsNumber - 1; i++) {
        controlPolygon[i * _stride] = controlPolygon[(i + 1) * _stride ];
        controlPolygon[i * _stride + 1] = controlPolygon[(i + 1) * _stride + 1];
        controlPolygon[i * _stride + 2] = controlPolygon[(i + 1) * _stride + 2];
        curveMath.deleteFirstMultiplicity(i);
    }
    _pointsNumber--;
    if (_pointsNumber > 0) {
        LoadPointsIntoVBO();
        curveMath.setControlPoints(controlPointsVector());
    }
}

void CurveGraphics::RemoveLastPoint() {
    if (_pointsNumber == 0) {
        return;
    }
    if (_pointsNumber > 0) { 
        _pointsNumber--; 
        curveMath.deleteLastMultiplicity(); 
        curveMath.setControlPoints(controlPointsVector());
    }
}

void CurveGraphics::RemoveLastCurvePoint() { _curvePointsNumber = (_curvePointsNumber > 0) ? _curvePointsNumber - 1 : 0; }

void CurveGraphics::checkLastCurvePt() {
    for (int i = 2; i < _pointsNumber; i++) {
        if (curvePoints[_curvePointsNumber * _stride] == controlPolygon[i * _stride] && curvePoints[_curvePointsNumber * _stride + 1] == controlPolygon[i * _stride + 1]) {
            RemoveLastCurvePoint();
        }    
    }
}

void CurveGraphics::renderScene() {
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
    glVertexAttrib3f(_colorLocation, _pointsColor.x, _pointsColor.y, _pointsColor.z);		
    glDrawArrays(GL_POINTS, 0, _pointsNumber);
 
    glBindVertexArray(0);  
}

void CurveGraphics::renderCurve() 
{   
    if (_curvePointsNumber > 0) {
        glBindVertexArray(_curveVAO);
        glBegin(GL_LINE_STRIP);
        glLineWidth(2);
        glVertexAttrib3f(_colorLocation, _curveColor.x, _curveColor.y, _curveColor.z);		
        glDrawArrays(GL_LINE_STRIP, 0, _curvePointsNumber);         
        glEnd();
    }
    /*glVertexAttrib3f(_colorLocation, _pointsColor.x, _pointsColor.y, _pointsColor.z);
        glDrawArrays(GL_POINTS, 0, _curvePointsNumber);*/   
    glBindVertexArray(0);
}

std::vector<Vec3d> CurveGraphics::controlPointsVector() {   
    Vec3d pt = Vec3d(0);
    std::vector<Vec3d> controlPt;

    for (int i = 0; i < _pointsNumber*3; i = i + 3) {
        //std::cout << "molteplicità del pt " << i/3 << " = " << curveMath.getPointMultiplicity(i / 3);
        //if (curveMath.getPointMultiplicity(i / 3) > curveMath.getDegree()) curveMath.resizeMultiplicities(i / 3);
        for (int j = 0; j < curveMath.getPointMultiplicity(i/3); j++) {       
            controlPt.push_back(pt);
        
            controlPt.back().x = controlPolygon[i];
            controlPt.back().y = controlPolygon[i + 1];
            controlPt.back().z = controlPolygon[i + 2];
        }
    }    
    return controlPt;
}

std::vector<Vec3d> CurveGraphics::curvePointsVector() {

    Vec3d pt = Vec3d(0);
    std::vector<Vec3d> curvept;   
    for (int i = 0; i < _curvePointsNumber * 3; i = i + 3) {

        curvept.push_back(pt);

        curvept[i / 3].x = curvePoints[i];
        curvept[i / 3].y = curvePoints[i + 1];
        curvept[i / 3].z = curvePoints[i + 2];
        std::cout << curvept[i/3];
    }
    return curvept;
}



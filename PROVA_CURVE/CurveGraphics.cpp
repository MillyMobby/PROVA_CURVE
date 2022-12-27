#include "CurveGraphics.h"

CurveGraphics::~CurveGraphics() { delete[] controlPolygon; }

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
 GLsizeiptr verticesSize = (steps*30) * sizeof(controlPolygon[0]);
    
    glBufferData(GL_ARRAY_BUFFER, verticesSize, (void*)0, GL_STATIC_DRAW); 
    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, _curveStride * sizeof(GLdouble), (void*)0);
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
    glBufferSubData(GL_ARRAY_BUFFER, 0, _pointsNumber * 4 * sizeof(double), controlPolygon); //non e' un resize
   // glBufferSubData(GL_ARRAY_BUFFER, 0, _vertices.size()/3 * sizeof(_vertices[0]), _vertices.data());    
}

void CurveGraphics::LoadPointsIntoCurveVBO() {
    glBindBuffer(GL_ARRAY_BUFFER, _curveVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, _curvePointsNumber * 3 * sizeof(double)+1, curvePoints);
}


void CurveGraphics::AddPoint(double x, double y)
{
        int preview;
        bool repeated = false;
        _pointsNumber > 0 ? preview = _pointsNumber - 1 : preview = 0;
        
        if (preview !=0) { 
            if (controlPolygon[preview * _stride] == x && controlPolygon[preview * _stride + 1] == y) { 
                std::cout << "il punto � ripetuto \n";
                curveMath.editMultiplicity(_pointsNumber - 1);
                repeated = true;
            }
        }
         if (_pointsNumber < MaxNumPoints && repeated ==false) {
           
            controlPolygon[_pointsNumber * _stride] = x;
            controlPolygon[_pointsNumber * _stride + 1] = y;
            controlPolygon[_pointsNumber * _stride + 2] = 1;
            controlPolygon[_pointsNumber * _stride + 3] = 1;
            _pointsNumber++; curveMath.editMultiplicity(_pointsNumber);
            LoadPointsIntoVBO(); 
        }
    //}      
        curveMath.setControlPoints(controlPointsVector());
        curveMath.generateKnots();
       curveMath.print();
        isChanged = true;
      /*if (_pointsNumber > curveMath.getDegree()) */   
        
    
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
addWeights(w);
    if (isChanged == true || makeNURBS == true) {  modifyCurve(w); }
    isChanged = false;
       if (curveMath.degenere(_pointsNumber) == false) {
           renderScene();
           if (deg == curveMath.getDegree() /*&& deg < _pointsNumber*/) renderCurve();
       }
       else renderScene();         
}

void CurveGraphics::AddCurvePoint(double x, double y) {
        curvePoints[_curvePointsNumber * _curveStride] = x;
        curvePoints[_curvePointsNumber * _curveStride + 1] = y;
        curvePoints[_curvePointsNumber * _curveStride + 2] = 1;
 
    _curvePointsNumber++;
    LoadPointsIntoCurveVBO();
}

void CurveGraphics::ChangePoint(int i, double x, double y, double z)
{
    if (i >= 0 && i < _pointsNumber) { 
        controlPolygon[i * _stride] = x;
        controlPolygon[i * _stride + 1] = y;  
        controlPolygon[i * _stride + 2] = z;
    }
    LoadPointsIntoVBO();

    curveMath.setControlPoints(controlPointsVector());
    isChanged = true;
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

void CurveGraphics::modifyCurve(std::vector<float>& w) 
{
    curveMath.resizeMultiplicities();
    
    double uinc = 0.01;
    std::vector<Vec3d> controlPoints = controlPointsVector(); 
    curveMath.setControlPoints(controlPoints);
    curveMath.generateKnots();
    int c = 0;
    Vec3d point = Vec3d(0);

    if (curveMath.degenere(_pointsNumber) == false)  {
   
        for(double u = curveMath.getDomainLeft();u<curveMath.getDomainRight()+uinc; u = u+uinc) {
            if (curveMath.getKnotsSize() == (curveMath.getDegree() + 1) * 2)
            {
                curveMath.setBezier(true);
                point = curveMath.deCasteljau(u, w);
            }
            else
            {
                point = curveMath.deBoor(double(u), w);
                curveMath.setBezier(false);
            }
           
            if (_curvePointsNumber > 0 && c < _curvePointsNumber) { ChangeCurvePoint(c, point.x, point.y); }
            else if (_curvePointsNumber >= 0 && c >= _curvePointsNumber ) { AddCurvePoint(point.x, point.y); }
            c++; 
        }     
    }   
}



void CurveGraphics::ChangeCurvePoint(int i, double x, double y)
{
    if (i >= 0 && i < _curvePointsNumber) {
        curvePoints[i * _curveStride] = x;
        curvePoints[i * _curveStride + 1] = y;
    }
    LoadPointsIntoCurveVBO();
}

void CurveGraphics::addWeights(std::vector<float> w) {
    
    for (int i = 0; i < _pointsNumber; i++) {
        if (w[i]!= controlPolygon[i * _stride + 2]) { 
            ChangePoint(i,
                (controlPolygon[i * _stride] / controlPolygon[i * _stride + 2]) * w[i],
                (controlPolygon[i * _stride + 1] / controlPolygon[i * _stride + 2]) * w[i],
                w[i]);

        }
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
        controlPolygon[i * _stride + 3] = controlPolygon[(i + 1) * _stride + 3];
        curveMath.deleteFirstMultiplicity(i);
    }
    _pointsNumber--;
    if (_pointsNumber > 0) {
        LoadPointsIntoVBO();
        curveMath.setControlPoints(controlPointsVector());
        isChanged = true;
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
        isChanged = true;
    }
}

void CurveGraphics::RemoveLastCurvePoint() { _curvePointsNumber = (_curvePointsNumber > 0) ? _curvePointsNumber - 1 : 0; }

void CurveGraphics::checkLastCurvePt() {
    for (int i = 2; i < _pointsNumber; i++) {
        if (curvePoints[_curvePointsNumber * _curveStride] == controlPolygon[i * _stride] && curvePoints[_curvePointsNumber * _curveStride + 1] == controlPolygon[i * _curveStride + 1]) {
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

    for (int i = 0; i < _pointsNumber*4; i = i + 4) {
        //std::cout << "molteplicit� del pt " << i/3 << " = " << curveMath.getPointMultiplicity(i / 3);
        for (int j = 0; j < curveMath.getPointMultiplicity(i/4); j++) {       
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


void CurveGraphics::example(double* v) {curveMath.setDegree(3); 
    for (int i = 0; i < 18; i=i+3) {
        AddPoint(v[i], v[i+1]);
        // _pointsNumber++;
    }
 
 
}

void CurveGraphics::transformWithCamera(Camera& camera)
{
    Vec4d pt = Vec4d(0);
    Mat4d t = camera.getTransform();//_camera.updateViewMatrix(inverseT);
    Mat4d z = camera.getZoomMatrix();
    z = z.inverse();
    for (int i = 0; i < _pointsNumber * 4 ; i = i + 4) {
        pt.x = controlPolygon[i];
        pt.y = controlPolygon[i + 1];
        pt.z = controlPolygon[i + 2];
        pt.w = 1;
        

        Vec4d newPt = pt;
        newPt = z * t  * pt;
        
        ChangePoint(i / 4, newPt.x, newPt.y, newPt.z);
    }
    
}

float* CurveGraphics::BSplineBasisGraphic() {
   float value = -2;
    if (curveMath.degenere(_pointsNumber) == false) {
        
    
    for (int k = 0; k < curveMath.getKnotsVector().size(); k++) {
        curveMath.BSplineBasis(curveMath.getDegree(), k);
    }
    }
    return curveMath.getBasisValues();
}

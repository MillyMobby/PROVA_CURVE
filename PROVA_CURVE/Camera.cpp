#include "Camera.h"
void Camera::setEye(const Vec3f& eye) {
	_eye = eye;
	_isChanged = true;
}
const Vec3f& Camera::getEye() const {
	return _eye;
}

void Camera::setLookAt(const Vec3f& lookat) {
	_lookAt = lookat;
	_isChanged = true;
}

const Vec3f& Camera::getLookAt() const {
	return _lookAt;
}
void Camera::setup(const Vec3f& up) {
	_up = up;
	_isChanged = true;
}
const Vec3f& Camera::getUp() const {
	return _up;
}
void Camera::setFov(const float& fov) {
	_fov = fov;
	_isChanged = true;

}
const float Camera::getFov() const {
	return _fov * (180.0 / 3.141592653589793238463);
}
void Camera::setNear(const float& cameraNear) {
	_near = cameraNear;
	_isChanged = true;
}
const float& Camera::getNear() const {
	return _near;
}

void Camera::setFar(const float& cameraFar) {
	_far = cameraFar;
	_isChanged = true;
}
const float& Camera::getFar() const {
	return _far;
}

const float& Camera::getAspectRatio() const {
	return _aspectRatio;
}

void Camera::computeAspectRatio(int width, int height) {
	_aspectRatio = static_cast<double> (width) / static_cast<double> (height);
	_isChanged = true;
}

void Camera::ProcessKeyboard(std::string direction, float deltaTime, std::string scene)
{
    float velocity = 5.5f * deltaTime;
    if (direction == "FORWARD") {
        
            setEye(_eye - (_eye - _lookAt).normalize() * velocity);
            setLookAt(_lookAt - (_eye - _lookAt).normalize() * velocity);
            previousDirection = "FORWARD";
            updateViewMatrix();
        

    }
    if (direction == "BACKWARD") {
        
            setEye(_eye + (_eye - _lookAt).normalize() * velocity);
            setLookAt(_lookAt + (_eye - _lookAt).normalize() * velocity);
            previousDirection = "BACKWARD";
            updateViewMatrix();

       


    }
    if (direction == "ROTATION_SX") {
        Vec3f v = (_lookAt - _eye);
        Mat3f rot3 = Mat3f::rotateY(velocity / 4 * 3.141592653589793238463 / 8);
        v = rot3 * v;
        _lookAt = v + _eye;

        updateViewMatrix();

    }
    if (direction == "ROTATION_DX") {

        Mat3f rot3 = Mat3f::rotateY(velocity / 4 * -3.141592653589793238463 / 8);
        Vec3f v = (_lookAt - _eye);
        v = rot3 * v;
        _lookAt = v + _eye;


        updateViewMatrix();

    }
    if (direction == "LEFT") {
       
            Vec3f occhio = _eye, vede = _lookAt, asseVerticale = _up;
            Vec3f sguardo = (occhio - vede).normalize();

            Vec3f dir = (sguardo.cross(asseVerticale)).normalize();

            _eye.x = _eye.x + dir.x * velocity;
            _lookAt.x = _lookAt.x + dir.x * velocity;

            _eye.z = _eye.z + dir.z * velocity;
            _lookAt.z = _lookAt.z + dir.z * velocity;
            previousDirection = "LEFT";
            updateViewMatrix();
        

    }
    if (direction == "RIGHT") {
       
            Vec3f occhio = _eye, vede = _lookAt, asseVerticale = _up;
            Vec3f sguardo = (occhio - vede).normalize();
            Vec3f dir = (sguardo.cross(asseVerticale)).normalize();

            _eye.x = _eye.x - dir.x * velocity;
            _lookAt.x = _lookAt.x - dir.x * velocity;
            _eye.z = _eye.z - dir.z * velocity;
            _lookAt.z = _lookAt.z - dir.z * velocity;
            previousDirection = "RIGHT";
            updateViewMatrix();
        

    }
}

void Camera::updateProjectionMatrix(int w, int h) {
    float r = _near * tanf(getFov() * 0.5f); // r = n tan (fov/2) e il 2????'
    float t = r / _aspectRatio; // t = aspectRatio * r
    //float h = w / 1.5;
    // 0.78539 è il fov in radianti calcolato su internet
    // _projectionMatrix = Mat4f::ProjectionMatrix(r, t, 0, _aspectRatio, _near, _far);
    _projectionMatrix = Mat4f::orthographicProjection(w,h, _near, _far);

}

const Mat4f& Camera::getProjectionMatrix() const {
    return _projectionMatrix;
}

const Mat4f& Camera::getViewMatrix() const {
    return _viewMatrix;
}

void Camera::updateViewMatrix() {
   // Mat4f scale = Mat4f::scaleMatrix(5, 5, 5);
    _viewMatrix = Mat4f::ViewMatrix(_eye, _lookAt, _up);
    _viewMatrix = /*scale **/ _viewMatrix;
}

void Camera::update() {
    if (_isChanged) {
        updateViewMatrix();
        updateProjectionMatrix();

        _isChanged = false;
    }
}
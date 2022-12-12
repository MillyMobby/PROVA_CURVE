#pragma once 
#include "Vec3f.h" 
#include "Mat4f.h"
//#include "Vec2f.h"
#include <cmath> 

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera
{
private:
	Mat4f _viewMatrix;
	Mat4f _movementMatrix;
	Mat4f _projectionMatrix;
	Vec3f _eye;
	Vec3f _lookAt;
	Vec3f _up;
	Vec3f _horizontalRotation;
	float _fov = 0.0f;
	float _aspectRatio = 0.0;
	float _near = 0.0f;
	float _far = 0.0f;
	bool _isChanged = false;

	std::string previousDirection;
public:
	Camera() {}
	Camera(const Camera& camera) {
		_eye = camera._eye; _lookAt = camera._lookAt;
		_up = camera._up; _fov = camera._fov;
		_aspectRatio = camera._aspectRatio;
		_near = camera._near;
		_far = camera._far;
		_viewMatrix = camera._viewMatrix;
		_movementMatrix = camera._movementMatrix;
		_projectionMatrix = camera._projectionMatrix;
		_isChanged = camera._isChanged;

	}
	~Camera() {}
	void setEye(const Vec3f& eye);
	const Vec3f& getEye() const;
	void setFront(const Vec3f& front);


	int walk(std::string direction, std::string scene);
	void setLookAt(const Vec3f& lookat);
	const Vec3f& getLookAt() const;
	void setup(const Vec3f& up);
	const Vec3f& getUp() const;
	void setFov(const float& fov);
	const float getFov() const;
	void setNear(const float& cameraNear);
	const float& getNear() const;
	void setFar(const float& cameraFar);
	const float& getFar() const;
	void computeAspectRatio(int width, int height);
	void ProcessKeyboard(std::string direction, float deltaTime, std::string scene); 
	const float& getAspectRatio() const;
	const Mat4f& getViewMatrix() const;
	Mat4f getCubeMapViewMatrix() const;
	const Mat4f& getProjectionMatrix() const;
	void updateViewMatrix();
	void updateProjectionMatrix(int w, int h);
	void update();

	bool canMove(std::string currentDir, std::string scene);  


};
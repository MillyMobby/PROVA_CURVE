#pragma once
#include "CurveMath.h"
class NurbsMath :   public CurveMath
{
public:
	Vec3d deCasteljauNURBS(std::vector<Vec3d> controlPoints, double t, std::vector<float> w);
};


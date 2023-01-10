#include "PredefinedCurves.h"
const std::vector<float> PredefinedCurves::getCircleWeights() {
	//const std::vector<float> circleWeights = { 1, 0.707107, 1, 0.707107, 1, 0.707107, 1, 0.707107   ,1/*  , 0.707107, 1*/ };
	const std::vector<float> circleWeights = { 1, 1, 1, 1, 1, 1, 1, 1   ,1/*  , 0.707107, 1*/ };
	return circleWeights;
}

const std::vector<double> PredefinedCurves::getCircleControlPolygon() {
	const std::vector<double> circleControlPolygon = {
	 0.5, 0.0, //P1
	 0.5, 0.5, //P2
	 0.0, 0.5, //P3
	-0.5, 0.5, //P4
	-0.5, 0.0, //P5
	-0.5,-0.5, //P6
	 0.0,-0.5, //P7
	 0.5,-0.5, //P8
	 0.5, 0.0, //P1
	 //0.5, 0.5, //P2
	 //0.0, 0.5, //P3
	};

	return circleControlPolygon;
}
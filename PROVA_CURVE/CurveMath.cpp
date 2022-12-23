#include "CurveMath.h"
bool CurveMath::degenere(int pointsNum) {	
	if (pointsNum <= degree ) return true;
	else return false;
}

const int CurveMath::getKnotsInterval(double t) {
	int n = knots.size()-degree-2;
	
	for (int i = 0; i < n; i++) {
		double k = knots[i];
		if (t >= knots[i] && t < knots[i+1]) return i;
	}
	if (t >= knots[n] ) {  return n; }
	
	return -1;
}

const std::vector<double> CurveMath::getKnotsVector() { return knots; }

std::vector<int> CurveMath::getMultiplicityVector() { return multiplicities; }

void CurveMath::editMultiplicity(int pointIndex) {
	if (multiplicities.size()>=pointIndex) {
		if (multiplicities.back() < degree) multiplicities.back()++;
		else std::cout << "degenere\n";
	}
	else multiplicities.push_back(1);
	
}

void CurveMath::resizeMultiplicities(/*int i*/) {
	if (multiplicities.size() > 0) {
		
		for (int i = 0; i < multiplicities.size(); i++) {
			if (degree >=1 &&multiplicities[i] > degree) multiplicities[i] = degree;			
		}
	}
}

void CurveMath::setMultiplicity(int i) { multiplicities.push_back(i); }

void CurveMath::deleteLastMultiplicity() { multiplicities.pop_back(); }

void CurveMath::deleteFirstMultiplicity(int i) {	
		multiplicities[i] = multiplicities[(i + 1) ];
		if (i + 1 == multiplicities.size()) { multiplicities.pop_back(); }		
}

const double CurveMath::getKnot(int i) {
	if (i <= knots.size()-1) return knots[i];
	else return 1;
}

void CurveMath::generateKnots() {
	knots.clear();

	for (int i = 0; i <= degree; i++) { knots.push_back(0); }

	int middle = (controlPoints.size() -1)- degree;
	for (int j = 0; j < middle; j++) {
		double h = double(j + 1) / (middle + 1);
		knots.push_back(h);		
	}	
	for (int k = 0; k <=degree; k++) { knots.push_back(1);  }
}

void CurveMath::setDegree(int p) { degree = p; }


Vec3d CurveMath::deCasteljau(double t, std::vector<float> w) {	
	Vec3d result = Vec3d(0);
		
		std::vector<Vec3d> points = controlPoints;
		for (int i = degree; i > 0; i--) {
			for (int j = 0; j < i; j++) {
				points[j] = Vec3d::lerp(points[j]/**w[j]*/, points[j + 1]/**w[j+1]*/, t);
			}
		}
		result = points[0];
		points.clear();

	return result;
}

void CurveMath::setControlPoints(std::vector<Vec3d> cp) {
	controlPoints = cp;
	generateKnots();
}

std::vector<double> CurveMath::fullInsertion(int knotInterval, double t) {
	std::vector<double> newKnots;
	for (int i = 0; i < knotInterval; i++) {
		newKnots.push_back(knots[i]);
	}
	for (int i = 0; i < degree; i++) {
		newKnots.push_back(t);
	}
	for (int i = 0; i < knots.size() - (knotInterval + 1); i++) {
		newKnots.push_back(knots[knotInterval + 1 + i]);
	}
	return newKnots;
}


Vec3d CurveMath::deBoor(double t, std::vector<float> w) {
	Vec3d result = Vec3d(0);
	int m = getKnotsInterval(t);
	int n = controlPoints.size();

	if (m >= degree && m < n) {
		std::vector<Vec3d> points;
		for (int j = m - degree; j <= m; j++) {
			points.push_back(controlPoints[j] /** w[j]*/);
		}

		for (int i = degree; i > 0; i--) {
			for (int j = 0; j < i; j++) {
				double omega = (t - knots[j + m - i + 1]) / (knots[j + m + 1] - knots[j + m - i + 1]);
				points[j] = Vec3d::lerp(points[j], points[j + 1], omega);
			}
		}
		result = points[0]; // lo divido per Pz
		points.clear();
	}
	return result;
}

void CurveMath::print() {
	for (int i = 0; i < knots.size(); i++) { std::cout << " " << knots[i] << ", "; }
}














//void CurveMath::addWeights(std::vector<float> w, std::vector<Vec3d> controlPol) {
//	for (int i = 0; i < controlPol.size(); i++) {
//		controlPoints[i].x = controlPol[i].x * w[i];
//		controlPoints[i].y = controlPol[i].y * w[i];
//		controlPoints[i].z = w[i];
//	}
//}
//
//void CurveMath::removeWeights(std::vector<float> w, std::vector<Vec3d> controlPol) {
//	for (int i = 0; i < controlPol.size(); i++) {
//		controlPoints[i].x = controlPol[i].x / controlPoints[i].z;
//		controlPoints[i].y = controlPol[i].y / controlPoints[i].z;
//		controlPoints[i].z = w[i];
//	}
//}

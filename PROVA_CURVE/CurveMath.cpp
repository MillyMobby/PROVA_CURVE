#include "CurveMath.h"
bool CurveMath::degenere(int pointsNum) {	
	if (pointsNum <= degree /*+ 1*/) return true;
	else return false;
}

const int CurveMath::getKnotsInterval(double t) {
	int n = knots.size()-1;
	

	if (t >= knots.back()) { std::cout << "t è 1 aaaa\n"; return n; }
	else 
	{
		for (int nodeI = 0; nodeI < knots.size(); nodeI++) {
			if (t >= knots[nodeI] && t < knots[nodeI + 1]) {
				return nodeI;
			}
		}
	}
	return -1;
}

const std::vector<double> CurveMath::getKnotsVector() { return knots; }

std::vector<int> CurveMath::getMultiplicityVector() { return multiplicities; }

void CurveMath::editMultiplicity(int pointIndex) {
	if (multiplicities.size()>=pointIndex) {
		if (multiplicities.back() /*+ 1*/ < degree) multiplicities.back()++;
		else std::cout << "degenere\n";
	}
	else  multiplicities.push_back(1);
	//std::cout << "molteplicita = " << multiplicities[pointIndex] << std::endl;
}

void CurveMath::resizeMultiplicities(/*int i*/) {
	if (multiplicities.size() > 0) {
		for (int i = 0; i < multiplicities.size(); i++) {
			if (multiplicities[i] > degree) multiplicities[i] = degree;
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
	if (i < knots.size()) return knots[i];
	else return 1;
}

void CurveMath::generateKnots(int pointsNum) {
	knots.clear();

	for (int i = 0; i <= degree; i++) { knots.push_back(0); }

	int middle = (controlPoints.size() -1)- degree;
	for (int j = 0; j <= middle; j++) {
		double h = double(j + 1) / (middle + 1);
		knots.push_back(double(j + 1) / (middle + 1));
		
	}	
	for (int k = 0; k <degree; k++) { knots.push_back(1);  }
}

void CurveMath::setDegree(int p) { degree = p; }

Vec3d CurveMath::deCasteljau(/*std::vector<Vec3d> controlPoints,*/ double t, std::vector<float> w) {
	// p iterazioni
	std::vector<Vec3d> points = controlPoints;
	std::vector<Vec3d> temporaryPoints;
	Vec3d result;
	int segments;
	for (int i = degree + 1; i > 0; i--) {		
		segments = i - 1;
		if (segments == 0) { 
			//std::cout << "PUNTO DI DE CASTELJAU: " << controlPoints[0] << std::endl;
			return result; 
		}
		else {
			for (int j = 0; j < segments; j++) {				
				Vec3d lerp = Vec3d::lerp(/*controlPoints*/points[j]*w[j], /*controlPoints*/points[j + 1]*w[j+1], t);
				//std::cout << "LERP tra " << controlPoints[j] << " e "<< controlPoints[j + 1] << " ------> " << lerp << std::endl;
				temporaryPoints.push_back(lerp);			
			}
			/*controlPoints*/points = temporaryPoints;
			 result = temporaryPoints[0];
			temporaryPoints.clear(); 
		}
	}	
	return result;
}

void CurveMath::setControlPoints(std::vector<Vec3d> cp) {
	controlPoints = cp;
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

Vec3d CurveMath::deBoor(/*std::vector<Vec3d> controlPoints,*/ double t, std::vector<float> w) {
	int m = getKnotsInterval(t);
	std::vector<Vec3d> affectedPoints,newVec,points;
	Vec3d evaluatedPoint = Vec3d(0);
	int count = 0,c=0;

	/*
	std::cout << "INTERVALLO NODO DI INDICE -> " << m << std::endl;
	std::cout << "SUPPORTO NEI NODI [ " << m  << ", "<< m + degree + 1<< ") "<< std::endl;	*/
	//affectedPoints.push_back(controlPoints[m - degree]);
	
		for (int i = m - degree; i < m + 1; i++) {
			c++;
			affectedPoints.push_back(controlPoints[i] * w[i]);

		}

		for (int r = 1; r <= degree; r++) {
			for (int i = m - degree + r; i <= m; i++) {

				double omega = (t - knots[i]) / (knots[i + 1 + degree - r] - knots[i]);

				evaluatedPoint = Vec3d::lerp(affectedPoints[count], affectedPoints[count + 1], omega);
				newVec.push_back(evaluatedPoint);
				count++;
			}
			affectedPoints = newVec;
			evaluatedPoint = affectedPoints[0];
			newVec.clear();
			count = 0;
		}
		return affectedPoints[0];
	
}

void CurveMath::addWeights(std::vector<float> w, std::vector<Vec3d> controlPol) {
	for (int i = 0; i < controlPol.size(); i++) {
		controlPoints[i].x = controlPol[i].x * w[i];
		controlPoints[i].y = controlPol[i].y * w[i];
		controlPoints[i].z = w[i];
	}
}

void CurveMath::removeWeights(std::vector<float> w, std::vector<Vec3d> controlPol) {
	for (int i = 0; i < controlPol.size(); i++) {
		controlPoints[i].x = controlPol[i].x / controlPoints[i].z;
		controlPoints[i].y = controlPol[i].y / controlPoints[i].z;
		controlPoints[i].z = w[i];
	}
}

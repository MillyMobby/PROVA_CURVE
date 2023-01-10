#include "CurveMath.h"
bool CurveMath::degenere(int pointsNum) {	
	if (pointsNum <= degree ) return true;
	else return false;
}

const int CurveMath::getKnotsInterval(double t) {
	//if (knotsType == 0) {
	int n = knots.size()-degree-2;
	
	for (int i = 0; i < n; i++) {
		double k = knots[i];
		if (t >= knots[i] && t < knots[i+1]) return i;
	}
	if (t >= knots[n] ) {  return n; }
	
	
	//}
	/*else if (knotsType == 1) {
		int n = knots.size() - degree - 2;
		for (int i = 0; i < n; i++) {
			double k = knots[i];
			if (t >= knots[i] && t < knots[i + 1]) return i;
			else if (t == knots[i] && t == knots[i + 1]) return i;
		}
		if (t >= knots[n]) { return n; }
	}*/
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
	switch (knotsType) {
	case 0:
		openKnots();
		break;
	case 1:
		circle();
		break;
	default:
		break;


	}
	
}

void CurveMath::openKnots() {
for (int i = 0; i <= degree; i++) { knots.push_back(0); }

		int middle = (controlPoints.size() -1)- degree;
		for (int j = 0; j < middle; j++) {
			double h = double(j + 1) / (middle + 1);
			knots.push_back(h);		
		}	
		for (int k = 0; k <=degree; k++) { knots.push_back(1);  }
}

void CurveMath::circle() {
	//if (degree == 2) {
		for (int i = 0; i <= degree; i++) { knots.push_back(0); }

		/*int middle = (controlPoints.size() - 1) - degree;
		for (int j = 0; j < middle; j++) {
			double h = double(j + 1) / (middle + 1);
			for (int k = 0; k < degree; k++) {knots.push_back(h);}
			
		}*/
		knots.push_back(double(0.25));
		knots.push_back(double(0.25));

		knots.push_back(double(0.5));
		knots.push_back(double(0.5));

		knots.push_back(double(0.75));
		knots.push_back(double(0.75));

		for (int k = 0; k <= degree; k++) { knots.push_back(1); }
	
	
}

void CurveMath::setDegree(int p) { degree = p; }


Vec3d CurveMath::deCasteljau(double t, std::vector<float> w) {	
	Vec3d result = Vec3d(0);
		
		std::vector<Vec3d> points = controlPoints;
		for (int i = degree; i > 0; i--) {
			for (int j = 0; j < i; j++) {
				points[j] = Vec3d::lerp(points[j], points[j + 1], t);
			}
		}
		result = points[0];
		points.clear();

	return result;
}

void CurveMath::setControlPoints(std::vector<Vec3d> cp, int knotsType) {
	controlPoints = cp;
	generateKnots();
}


std::vector<double> CurveMath::fullInsertion(int knotInterval, double t) { //NON USATO
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
			points.push_back(controlPoints[j]);
		}

		for (int i = degree; i > 0; i--) {
			for (int j = 0; j < i; j++) {
				double omega = (t - knots[j + m - i + 1]) / (knots[j + m + 1] - knots[j + m - i + 1]);
				points[j] = Vec3d::lerp(points[j], points[j + 1], omega);
			}
		}
		result = points[0]; 
		points.clear();
	}
	return result;
}

void CurveMath::print() {
	for (int i = 0; i < knots.size(); i++) { std::cout << " " << knots[i] << ", "; }
}

float CurveMath::BSplineBasis( int p, int interval, double t) {
	double coordinate_Y = -1;
	
	double numerator1, denominator1, numerator2, denominator2, expr1 =0, expr2=0;
	int k = 0;
	//if (t == knots[0]) { return 1.0f; }
	
	if (p == 0) {
		
		if (t >= knots[interval] && t < knots[interval + 1]) coordinate_Y = 1;
		else coordinate_Y = 0;
	}
	/*else if (p == 1) {
		if (t >= knots[interval] && t < knots[interval + 1]) coordinate_Y = (t - knots[interval])/(knots[interval + 1] - knots[interval]);
		else if (t >= knots[interval+1] && t <= knots[interval + 2]) coordinate_Y = (knots[interval+2]-t) / (knots[interval + 2] - knots[interval+1]);
	}*/
	else {
		numerator1 = t - knots[interval]; 
		denominator1 = knots[interval + p] - knots[interval];
		numerator2 = knots[interval + p + 1] - t; 
		denominator2 = knots[interval + p + 1] - knots[interval + 1];

		if (denominator1 == 0) { expr1 = 0.0; }
		else { expr1 = numerator1 / denominator1; }

		if (denominator2 == 0) { expr2 = 0.0; }
		else { expr2 = numerator2 / denominator2; }

		coordinate_Y = expr1 * BSplineBasis( p - 1, interval,t) + expr2 * BSplineBasis(p - 1, interval + 1,t);
		
	}	
	return coordinate_Y;
}


std::vector<float> CurveMath::Basis(double t,int p, int n) {
	/* n = pointsNumber;
	   p = degree
	   m+1 = knots.size();	
	*/
	double numerator1, denominator1, numerator2, denominator2, numerator3, denominator3, expr1 = 0, expr2 = 0;
	std::vector<float> B = std::vector<float>(n, 0);
	if (t == knots[0]) { B[0] = 1.0f; return B; }
	else if (t == knots[n - 1]) { B[n - 1] = 1.0f; return B; }

	int k = getKnotsInterval(t);
	B[k] = 1.0f;
	for (int i = 1; i <= p; i++) {
		numerator2 = knots[k + 1] - t;
		denominator2 = knots[k + 1] - knots[(k-i) + 1];
		B[k - i] = (numerator2 / denominator2)*B[(k-i)+1];
	
		for (int j = k - i + 1; j >= k - 1; j--) {
			numerator1 = t - knots[j];
			denominator1 = knots[j + i] - knots[j];

			numerator3 = knots[j + i + 1] - t;
			denominator3 = knots[j + i + 1] - knots[j + 1];


			B[j] = (numerator1 / denominator1) * B[j] + (numerator3 / denominator3) * B[j + 1];
		}
		B[k] = ((t - knots[k]) / (knots[k + i] - knots[k])) * B[k];
	
	}
	return B;
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

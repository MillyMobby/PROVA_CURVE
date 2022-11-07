#include "CurveMath.h"
int CurveMath::getKnotsInterval(int t) {
	int n = controlPoints.size();
	for (int nodeI = 0; nodeI <= n + degree - 1; nodeI++) {
		if (t >= knots[nodeI] && t < knots[nodeI + 1])
			return nodeI;
	}
	return -1;
}

void CurveMath::generateKnots() {
	knots.clear();

	for (int i = 0; i < degree; i++)
		knots.push_back(0);

	int middle = controlPoints.size() - degree;
	for (int i = 0; i < middle; i++)
		knots.push_back(double(i + 1) / (middle + 1));

	for (int i = 0; i < degree; i++)
		knots.push_back(1);
}

void CurveMath::setDegree(int p) { degree = p; }

Vec3d CurveMath::deCasteljau(std::vector<Vec3d> controlPoints) {
	// p iterazioni
	std::vector<Vec3d> temporaryPoints;
	Vec3d result;
	int segments;
	for (int i = degree + 1; i > 0; i--) {		
		segments = i - 1;
		if (segments == 0) { 
			std::cout << "PUNTO DI DE CASTELJAU: " << controlPoints[0] << std::endl;
			return result; 
		}
		else {
			for (int j = 0; j < segments; j++) {				
				Vec3d lerp = Vec3d::lerp(controlPoints[j], controlPoints[j + 1], t);
				//std::cout << "LERP tra " << controlPoints[j] << " e "<< controlPoints[j + 1] << " ------> " << lerp << std::endl;
				temporaryPoints.push_back(lerp);			
			}
			controlPoints = temporaryPoints;
			 result = temporaryPoints[0];
			temporaryPoints.clear(); 
		}
	}	
	return result;
}

void CurveMath::setControlPoints(std::vector<Vec3d> cp) {
	controlPoints = cp;
}

std::vector<double> CurveMath::fullInsertion(int knotInterval) {
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
}

Vec3d CurveMath::deBoor(std::vector<Vec3d> controlPoints, double t, int knotInterval) {
	//If u lies in[uk, uk + 1) and u != uk, let h = p(i.e., inserting u p times) and s = 0;
	

}
//The general knot insertion algorithm can easily be modified to fulfill our purpose.
// First note that we only need to insert u enough number of times so that u becomes a knot of multiplicity p.
// If u is already a knot of multiplicity s, then inserting it p - s times would be sufficient.
//
	//Input: a value u
	//Output : the point on the curve, C(u)
	//
	//If u lies in[uk, uk + 1) and u != uk, let h = p(i.e., inserting u p times) and s = 0;
	//If u = uk and uk is a knot of multiplicity s, let h = p - s(i.e., inserting u p - s times);
	//Copy the affected control points Pk - s, Pk - s - 1, Pk - s - 2, ..., Pk - p + 1 and Pk - p to a new array and rename them as Pk - s, 0, Pk - s - 1, 0, Pk - s - 2, 0, ..., Pk - p + 1, 0;
	//
	//for r : = 1 to h do
	//for i : = k - p + r to k - s do
	//begin
	//Let ai, r = (u - ui) / (ui + p - r + 1 - ui), 
	//Let Pi, r = (1 - ai, r) Pi - 1, r - 1 + ai, r Pi, r - 1
	//end
	//Pk - s, p - s is the point C(u).
//std::vector<CurvePoint> CurveMath::DeCasteljau(int u, int delta) {
	

//}
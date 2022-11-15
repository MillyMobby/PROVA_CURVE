#include "CurveMath.h"
bool CurveMath::degenere(int pointsNum) {	
	if (pointsNum < degree + 1) return true;
	else return false;
}

int CurveMath::getKnotsInterval(double t) {
	int n = controlPoints.size();
	for (int nodeI = 0; nodeI < knots.size(); nodeI++) {
		if (t >= knots[nodeI] && t <= knots[nodeI + 1]) {
			return nodeI;
		}
	}
	return -1;
}

void CurveMath::generateKnots(int pointsNum) {
	knots.clear();

	for (int i = 0; i <= degree; i++) { 
		knots.push_back(0); std::cout << 0 << " ";}

	int middle = (pointsNum -1)- degree;
	/*if (middle <= 0) { std::cout << " degenere " << std::endl; }
	else 
	{*/
		for (int j = 0; j < middle; j++) {
			double h = double(j + 1) / (middle + 1);
			knots.push_back(double(j + 1) / (middle + 1));
			std::cout << " " << h << " ";
		}
	//}
	for (int k = 0; k <=degree; k++) { 
		knots.push_back(1);	 
		std::cout << " " << 1 << " "; }
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
	return newKnots;
}

void CurveMath::deBoor(std::vector<Vec3d> controlPoints, double t ) {
	int m = getKnotsInterval(t);
	//std::vector<Vec3d> affectedPoints;
	Vec3d evaluatedPoint = Vec3d(0);
	int count = 0;
	
	/*std::cout << "P = 3, NODI TOTALI = " << knots.size() << std::endl;
	std::cout << "INTERVALLO NODO DI INDICE -> " << m << std::endl;
	std::cout << "SUPPORTO NEI NODI [ " << m  << ", "<< m + degree + 1<< ") "<< std::endl;	
	for (int i = m - degree ; i < m +1; i++) {		
		affectedPoints.push_back(controlPoints[i]);
		std::cout << "PUNTO DI CONTROLLO DI INDICE  "<< i << " = " << controlPoints[i] << " nel nuovo vettore è indice ->" << count << "\n";		
	}*/
	
	for (int r = 1; r <= degree; r++) {
		for (int i = m - degree + r; i <= m; i++) {
			std::cout << " r = " << r << " i = " << i << std::endl;			
			double omega = (t - knots[i]) / (knots[i + 1 + degree - r] - knots[i]);
			count++;
			std::cout << "valori omega " << i << "," << r << " ------> " << omega << std::endl;
			evaluatedPoint = Vec3d::lerp(controlPoints[i - 1], controlPoints[i], omega);
		}
	}
	std::cout << "PUNTO DEBOOR_ALG -> "<< evaluatedPoint;
}


#include "CurveMath.h"

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
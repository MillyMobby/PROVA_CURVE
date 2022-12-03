#include "NurbsMath.h"

//Vec3d NurbsMath::deCasteljauNURBS(std::vector<Vec3d> controlPoints, double t, std::vector<float> w) {
//	//// p iterazioni
//	//std::vector<Vec3d> temporaryPoints;
//	//Vec3d result;
//	//int segments;
//	//for (int i = degree + 1; i > 0; i--) {
//	//	segments = i - 1;
//	//	if (segments == 0) {
//	//		//std::cout << "PUNTO DI DE CASTELJAU: " << controlPoints[0] << std::endl;
//	//		return result;
//	//	}
//	//	else {
//	//		for (int j = 0; j < segments; j++) {
//	//			Vec3d lerp = Vec3d::lerp(controlPoints[j] * w[j], controlPoints[j + 1] * w[j + 1], t);
//	//			//std::cout << "LERP tra " << controlPoints[j] << " e "<< controlPoints[j + 1] << " ------> " << lerp << std::endl;
//	//			temporaryPoints.push_back(lerp);
//	//		}
//	//		controlPoints = temporaryPoints;
//	//		result = temporaryPoints[0];
//	//		temporaryPoints.clear();
//	//	}
//	//}
//	//return result;
//}
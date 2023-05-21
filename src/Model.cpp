#include "Model.h"


Model::Model(const char* filename)
{
	std::ifstream readin;
	v.clear();
	vt.clear();
	vn.clear();
	f.clear();
	std::string line;
	readin.open(filename, std::ifstream::in);
	if (readin.fail()) {
		std::cout << "Load OBJ File Failed!!" << std::endl;
		return;
	}
	while (!readin.eof()) {
		std::getline(readin, line);
		std::istringstream iss(line.c_str());
		char useless;
		if (!line.compare(0, 2, "v ")) {
			iss >> useless;
			Vec3f vin;
			for (int i = 0; i < 3; ++i) {
				if (i == 0)iss >> vin.x;
				else if (i == 1) iss >> vin.y;
				else iss >> vin.z;
			}
			v.push_back(vin);
		}
		else if (!line.compare(0, 2, "vt")) {
			iss >> useless;
			iss >> useless;
			Vec3f vin;
			for (int i = 0; i < 3; ++i) {
				if (i == 0)iss >> vin.x;
				else if (i == 1) iss >> vin.y;
				else iss >> vin.z;
			}
			vt.push_back(vin);
		}
		else if (!line.compare(0, 2, "vn")) {
			iss >> useless;
			iss >> useless;
			Vec3f vin;
			for (int i = 0; i < 3; ++i) {
				if (i == 0)iss >> vin.x;
				else if (i == 1) iss >> vin.y;
				else iss >> vin.z;
			}
			vn.push_back(vin);
		}
		else if (!line.compare(0, 2, "f ")) {//只考虑了v,vt,vn都有的情况
			iss >> useless;
			std::vector<int> vin;
			int i, j, k;
			while (iss >> i >> useless >> j >> useless >> k) {
				vin.push_back(i-1);
				vin.push_back(j-1);
				vin.push_back(k-1);
			}
			
			f.push_back(vin);
		}

	}

}

Model::~Model() {
	v.clear();
	vt.clear();
	vn.clear();
	f.clear();
}

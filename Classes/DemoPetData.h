#ifndef __demo_pet_data_h__
#define __demo_pet_data_h__

#include "Pet.h"

struct DemoPet {
	int				iRow;
	int				iCol;
	EnumPetColor	eColor;
	EnumPetSpecial	eType;
	DemoPet(int r, int c, int col, int special);
};

struct DemoMaskPet {
	int iRow;
	int iCol;
	DemoMaskPet(int i, int j) {
		iRow = i;
		iCol = j;
	}
};

struct DemoVec2 {
	int v1;
	int v2;

	DemoVec2(int i, int j) {
		v1 = i;
		v2 = j;
	}
};

struct DemoVec4 {
	int v1;
	int v2;
	int v3;
	int v4;

	DemoVec4(int i, int j, int k, int m) {
		v1 = i;
		v2 = j;
		v3 = k;
		v4 = m;
	}
};

class DemoPetManager {
public:
	static bool getIntroDemo1PetData(std::vector<DemoPet> &demo, /*������*/
										DemoVec2 &touch,/*�����*/
										std::vector<DemoMaskPet> &mask,/*����*/
										std::vector<DemoVec2>&sames,/*ͬɫ*/
										std::vector<DemoVec2>&diffs,/*��ͬɫ*/
										std::vector<DemoVec4>&rows,/*����ʾ*/
										std::vector<DemoVec4>&cols/*����ʾ*/);

	static bool getIntroDemo2PetData(std::vector<DemoPet> &demo, /*������*/
										DemoVec2 &touch,/*�����*/
										std::vector<DemoMaskPet> &mask,/*����*/
										std::vector<DemoVec2>&sames,/*ͬɫ*/
										std::vector<DemoVec2>&diffs,/*��ͬɫ*/
										std::vector<DemoVec4>&rows,/*����ʾ*/
										std::vector<DemoVec4>&cols/*����ʾ*/);

	static bool getLev1DemoPetData(std::vector<DemoPet> &demo);
	static bool getLev2DemoPetData(std::vector<DemoPet> &demo);
	static bool getLev3DemoPetData(std::vector<DemoPet> &demo);

private:
	static bool getDemoPetData(const char*filename, 
								DemoVec2 &touch,/*�����*/
								std::vector<DemoPet> &demo, /*������*/
								std::vector<DemoMaskPet> &mask,/*����*/
								std::vector<DemoVec2>&sames,/*ͬɫ*/
								std::vector<DemoVec2>&diffs,/*��ͬɫ*/
								std::vector<DemoVec4>&rows,/*����ʾ*/
								std::vector<DemoVec4>&cols/*����ʾ*/);
};

#endif
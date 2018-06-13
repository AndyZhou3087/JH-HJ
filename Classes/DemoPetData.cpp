#include "DemoPetData.h"
#include "cocostudio/CocoStudio.h"


DemoPet::DemoPet(int r, int c, int col, int special) {
	if (col >= Max_Pet_Color) {
		col = 0;
	}

	if (special >= Max_Pet_Special) {
		special = 0;
	}

	iRow	= r;
	iCol	= c;
	eColor	= (EnumPetColor)col;
	eType	= (EnumPetSpecial)special;
}

bool DemoPetManager::getIntroDemo1PetData(std::vector<DemoPet> &demo, 
										  DemoVec2 &touch,/*点击处*/
										  std::vector<DemoMaskPet> &mask,/*遮罩*/
										  std::vector<DemoVec2>&sames,/*同色*/
										  std::vector<DemoVec2>&diffs,/*不同色*/
										  std::vector<DemoVec4>&rows,/*行提示*/
										  std::vector<DemoVec4>&cols/*列提示*/) {
	return getDemoPetData("demo_intro1.json", touch, demo, mask, sames, diffs, rows, cols);
}

bool DemoPetManager::getIntroDemo2PetData(std::vector<DemoPet> &demo, 
										  DemoVec2 &touch,/*点击处*/
										  std::vector<DemoMaskPet> &mask,/*遮罩*/
										  std::vector<DemoVec2>&sames,/*同色*/
										  std::vector<DemoVec2>&diffs,/*不同色*/
										  std::vector<DemoVec4>&rows,/*行提示*/
										  std::vector<DemoVec4>&cols/*列提示*/) {
	return getDemoPetData("demo_intro2.json", touch, demo, mask, sames, diffs, rows, cols);
}

bool DemoPetManager::getLev1DemoPetData(std::vector<DemoPet> &demo) {
	DemoVec2 touch(0,0);
	std::vector<DemoMaskPet> mask;
	std::vector<DemoVec2>sames;
	std::vector<DemoVec2>diffs;
	std::vector<DemoVec4>rows;
	std::vector<DemoVec4>cols;
	return getDemoPetData("demo_1.json", touch, demo, mask, sames, diffs, rows, cols);
}

bool DemoPetManager::getLev2DemoPetData(std::vector<DemoPet> &demo) {
	DemoVec2 touch(0,0);
	std::vector<DemoMaskPet> mask;
	std::vector<DemoVec2>sames;
	std::vector<DemoVec2>diffs;
	std::vector<DemoVec4>rows;
	std::vector<DemoVec4>cols;
	return getDemoPetData("demo_2.json", touch, demo, mask, sames, diffs, rows, cols);
}

bool DemoPetManager::getLev3DemoPetData(std::vector<DemoPet> &demo) {
	DemoVec2 touch(0,0);
	std::vector<DemoMaskPet> mask;
	std::vector<DemoVec2>sames;
	std::vector<DemoVec2>diffs;
	std::vector<DemoVec4>rows;
	std::vector<DemoVec4>cols;
	return getDemoPetData("demo_3.json", touch, demo, mask, sames, diffs, rows, cols);
}

bool DemoPetManager::getDemoPetData(const char*filename, 
									DemoVec2 &touch,/*点击处*/
									std::vector<DemoPet> &demo, /*消除物*/
									std::vector<DemoMaskPet> &mask,/*遮罩*/
									std::vector<DemoVec2>&sames,/*同色*/
									std::vector<DemoVec2>&diffs,/*不同色*/
									std::vector<DemoVec4>&rows,/*行提示*/
									std::vector<DemoVec4>&cols/*列提示*/) {
	demo.clear();
	//json 文档
	rapidjson::Document _doc;
	bool bRet = false;
	ssize_t size = 0;
	unsigned char *pBytes = NULL;
	do {
		pBytes = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(filename, "r", &size);
		CC_BREAK_IF(pBytes == NULL || strcmp((char*)pBytes, "") == 0);
		std::string load_str((const char*)pBytes, size);
		CC_SAFE_DELETE_ARRAY(pBytes);
		_doc.Parse<0>(load_str.c_str());
		CC_BREAK_IF(_doc.HasParseError());			
		//生成json文档对像

		if(!_doc.IsObject())
			return false;

		//是否有此成员
		if(!_doc.HasMember("demos"))
			return false;

		//--------------------解析消除物信息-------------------------------
		const rapidjson::Value &pArray = _doc["demos"];
		if(pArray.IsArray()) {
			for (rapidjson::SizeType i = 0; i < pArray.Size(); i++)
			{
				const rapidjson::Value &p = pArray[i];				
				if(p.HasMember("demo"))
				{
					const rapidjson::Value &valueEnt = p["demo"];
					if(valueEnt.HasMember("i") && valueEnt.HasMember("j") && valueEnt.HasMember("c") && valueEnt.HasMember("s"))
					{
						const rapidjson::Value &rowEnt = valueEnt["i"];
						int row = rowEnt.GetInt();

						const rapidjson::Value &colEnt = valueEnt["j"];
						int col = colEnt.GetInt();

						const rapidjson::Value &colorEnt = valueEnt["c"];
						int color = colorEnt.GetInt();

						const rapidjson::Value &specialEnt = valueEnt["s"];
						int special = specialEnt.GetInt();

						DemoPet demoPet(row, col, color, special);
						demo.push_back(demoPet);
					}
				}
			}
		}
		
		//--------------------解析遮罩物信息-------------------------------
		const rapidjson::Value &maskArray = _doc["masks"];
		if(maskArray.IsArray()) {
			for (rapidjson::SizeType i = 0; i < maskArray.Size(); i++)
			{
				const rapidjson::Value &p = maskArray[i];				
				if(p.HasMember("mask"))
				{
					const rapidjson::Value &valueEnt = p["mask"];
					if(valueEnt.HasMember("i") && valueEnt.HasMember("j"))
					{
						const rapidjson::Value &rowEnt = valueEnt["i"];
						int row = rowEnt.GetInt();

						const rapidjson::Value &colEnt = valueEnt["j"];
						int col = colEnt.GetInt();

						DemoMaskPet maskPet(row, col);
						mask.push_back(maskPet);
					}
				}
			}
		}

		//--------------------解析同色消除物信息-------------------------------
		const rapidjson::Value &sameArray = _doc["sames"];
		if(sameArray.IsArray()) {
			for (rapidjson::SizeType i = 0; i < sameArray.Size(); i++)
			{
				const rapidjson::Value &p = sameArray[i];				
				if(p.HasMember("same"))
				{
					const rapidjson::Value &valueEnt = p["same"];
					if(valueEnt.HasMember("i") && valueEnt.HasMember("j"))
					{
						const rapidjson::Value &rowEnt = valueEnt["i"];
						int row = rowEnt.GetInt();

						const rapidjson::Value &colEnt = valueEnt["j"];
						int col = colEnt.GetInt();

						DemoVec2 demoVec2(row, col);
						sames.push_back(demoVec2);
					}
				}
			}
		}	

		//--------------------解析不同色消除物信息-------------------------------
		const rapidjson::Value &diffsArray = _doc["diffs"];
		if(diffsArray.IsArray()) {
			for (rapidjson::SizeType i = 0; i < diffsArray.Size(); i++)
			{
				const rapidjson::Value &p = diffsArray[i];				
				if(p.HasMember("diff"))
				{
					const rapidjson::Value &valueEnt = p["diff"];
					if(valueEnt.HasMember("i") && valueEnt.HasMember("j"))
					{
						const rapidjson::Value &rowEnt = valueEnt["i"];
						int row = rowEnt.GetInt();

						const rapidjson::Value &colEnt = valueEnt["j"];
						int col = colEnt.GetInt();

						DemoVec2 demoVec2(row, col);
						diffs.push_back(demoVec2);
					}
				}
			}
		}

		//--------------------解析行提示消除物信息-------------------------------
		const rapidjson::Value &rowsArray = _doc["rows"];
		if(rowsArray.IsArray()) {
			for (rapidjson::SizeType i = 0; i < rowsArray.Size(); i++)
			{
				const rapidjson::Value &p = rowsArray[i];				
				if(p.HasMember("row"))
				{
					const rapidjson::Value &valueEnt = p["row"];
					if(valueEnt.HasMember("s") && valueEnt.HasMember("d") && valueEnt.HasMember("r") && valueEnt.HasMember("t"))
					{
						const rapidjson::Value &rowEnt = valueEnt["s"];
						int s = rowEnt.GetInt();

						const rapidjson::Value &colEnt = valueEnt["d"];
						int d = colEnt.GetInt();

						const rapidjson::Value &rEnt = valueEnt["r"];
						int r = rEnt.GetInt();

						const rapidjson::Value &tEnt = valueEnt["t"];
						int t = tEnt.GetInt();

						DemoVec4 demoVec4(s, d, r, t);
						rows.push_back(demoVec4);
					}
				}
			}
		}

		//--------------------解析列提示消除物信息-------------------------------
		const rapidjson::Value &colsArray = _doc["cols"];
		if(colsArray.IsArray()) {
			for (rapidjson::SizeType i = 0; i < colsArray.Size(); i++)
			{
				const rapidjson::Value &p = colsArray[i];				
				if(p.HasMember("col"))
				{
					const rapidjson::Value &valueEnt = p["col"];
					if(valueEnt.HasMember("s") && valueEnt.HasMember("d") && valueEnt.HasMember("c") && valueEnt.HasMember("t"))
					{
						const rapidjson::Value &rowEnt = valueEnt["s"];
						int s = rowEnt.GetInt();

						const rapidjson::Value &colEnt = valueEnt["d"];
						int d = colEnt.GetInt();

						const rapidjson::Value &cEnt = valueEnt["c"];
						int c = cEnt.GetInt();

						const rapidjson::Value &tEnt = valueEnt["t"];
						int t = tEnt.GetInt();

						DemoVec4 demoVec4(s, d, c, t);
						cols.push_back(demoVec4);
					}
				}
			}
		}

		//--------------------点击信息-------------------------------
		const rapidjson::Value &touchesArray = _doc["touches"];
		if(touchesArray.IsArray()) {
			for (rapidjson::SizeType i = 0; i < touchesArray.Size(); i++)
			{
				const rapidjson::Value &p = touchesArray[i];				
				if(p.HasMember("touch"))
				{
					const rapidjson::Value &valueEnt = p["touch"];
					if(valueEnt.HasMember("i") && valueEnt.HasMember("j"))
					{
						const rapidjson::Value &rowEnt = valueEnt["i"];
						int i = rowEnt.GetInt();

						const rapidjson::Value &colEnt = valueEnt["j"];
						int j = colEnt.GetInt();

						touch.v1 = i;
						touch.v2 = j;
					}
				}
			}
		}	

		bRet = true;

	} while (0);

	return bRet;
}
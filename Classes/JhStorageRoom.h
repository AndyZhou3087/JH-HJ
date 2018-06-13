/********************************************************************
* 仓库
*********************************************************************/

#ifndef _STORAGE_H_
#define _STORAGE_H_
#include "JhBuilding.h"
#include "cocos2d.h"
#include "JhMyPackage.h"
USING_NS_CC;

class JhStorageRoom :public JhBuilding
{
public:
	JhStorageRoom();
	~JhStorageRoom();

public:

	virtual bool init();

	CREATE_FUNC(JhStorageRoom);

	static void loadStorageData();
	static void save();
	static std::map<int, std::vector<PackageData>> map_storageData;
	static void add(PackageData data);
	static void use(std::string strid, int count = 1);

	static void use(PackageData data);
	static int getCountById(std::string strid);
};
#endif


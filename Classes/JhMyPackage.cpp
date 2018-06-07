#include "JhMyPackage.h"
#include "JhGameDataSave.h"
#include "JhCommonFuncs.h"
#include "JhGlobalData.h"
#include "JhStorageRoom.h"

int JhMyPackage::max = 4;
std::vector<PackageData> JhMyPackage::vec_packages;

JhMyPackage::JhMyPackage()
{

}


JhMyPackage::~JhMyPackage()
{

}

int JhMyPackage::add(PackageData pdata)
{
	if (isFull(pdata))
		return -1;
	else
	{
		int index = 0;
		for (unsigned int i = 0; i < vec_packages.size(); i++)
		{
			if (pdata.strid.compare(vec_packages[i].strid) == 0 && (pdata.type == FOOD || pdata.type == MEDICINAL || pdata.type == RES_1 || pdata.type == RES_2))
			{
				if (vec_packages[i].count < 10)
				{
					vec_packages[i].count++;
					save();
					return 0;
				}
				else
				{
					index++;
				}
			}
			else
			{
				index++;
			}
		}

		if (index == vec_packages.size())
			vec_packages.push_back(pdata);
		save();
	}
	return 0;
}

void JhMyPackage::cutone(std::string strid, int count)
{
	int index = 0;
	int cutcount = count;
	std::vector<PackageData>::reverse_iterator it;
	for (it = vec_packages.rbegin(); it != vec_packages.rend();)
	{
		if (strid.compare(it->strid) == 0)
		{
			it->count -= cutcount;
			if (it->count <= 0)
			{
				cutcount = -it->count;
				it = std::vector<PackageData>::reverse_iterator(vec_packages.erase((++it).base()));
				if (cutcount == 0)
					break;
			}
			else
			{
				break;
			}
		}
		else
			++it;
	}

	save();
}


void JhMyPackage::cutone(PackageData pdata)
{
	int index = 0;
	int cutcount = pdata.count;

	std::vector<PackageData>::reverse_iterator it;
	for (it = vec_packages.rbegin(); it != vec_packages.rend();)
	{
		if (pdata.strid.compare(it->strid) == 0 && pdata.goodvalue == it->goodvalue)
		{
			it->count -= cutcount;
			if (it->count <= 0)
			{
				cutcount = -it->count;
				it = std::vector<PackageData>::reverse_iterator(vec_packages.erase((++it).base()));
				if (cutcount == 0)
					break;
			}
			else
			{
				break;
			}
		}
		else
			++it;
	}

	save();
}

void JhMyPackage::takeoff()
{
	vec_packages.clear();
	save();
}

bool JhMyPackage::isFull(PackageData pdata)
{
	int index = 0;
	bool ret = false;
	if (vec_packages.size() == getMax())
	{
		for (unsigned int i = 0; i < vec_packages.size(); i++)
		{
			if (vec_packages[i].strid.compare(pdata.strid) != 0)
				index++;
			else
			{
				if (vec_packages[i].count >= 10)
					ret = true;
				else
				{
					if (pdata.type == FOOD || pdata.type == MEDICINAL || pdata.type == RES_1 || pdata.type == RES_2)
						ret = false;
					else
						ret = true;
				}
			}

		}
	}

	if (index == getMax())
		ret = true;
	return ret;
}

int JhMyPackage::canTakeCount(PackageData* pdata)
{
	int tcount = (JhMyPackage::getMax() - JhMyPackage::getSize()) * 10;

	for (int i = 0; i < JhMyPackage::getSize(); i++)
	{
		if (vec_packages[i].strid.compare(pdata->strid) == 0)
		{
			int leftcount = 10 - vec_packages[i].count;
			tcount += leftcount;
		}
	}
	if (tcount > pdata->count)
		tcount = pdata->count;
	return tcount;

}

int JhMyPackage::getCountById(std::string strid)
{
	int count = 0;
	for (unsigned int i = 0; i < vec_packages.size(); i++)
	{
		if (vec_packages[i].strid.compare(strid) == 0)
		{
			count++;
		}
	}

	return count;
}

void JhMyPackage::save()
{
	std::string str;
	for (unsigned int i = 0; i < vec_packages.size(); i++)
	{
		//"%s-%d-%d-%d-%d;", sdata.strid.c_str(), sdata.type, sdata.count, sdata.extype, sdata.lv
		std::string onestr = StringUtils::format("%s-%d-%d-%d-%d-%d-%d-%d-%d;", vec_packages[i].strid.c_str(), vec_packages[i].type, vec_packages[i].count, vec_packages[i].extype, vec_packages[i].lv, vec_packages[i].exp, vec_packages[i].goodvalue, vec_packages[i].slv, vec_packages[i].tqu);
		str.append(onestr);
	}
	JhGameDataSave::getInstance()->setPackage(str.substr(0, str.length() - 1));
}

void JhMyPackage::load()
{
	vec_packages.clear();
	std::string datastr = JhGameDataSave::getInstance()->getPackage();
	std::vector<std::string> vec_retstr;
	JhCommonFuncs::split(datastr, vec_retstr, ";");
	for (unsigned int i = 0; i < vec_retstr.size(); i++)
	{
		std::vector<std::string> tmp;
		JhCommonFuncs::split(vec_retstr[i], tmp, "-");
		PackageData data;

		data.strid = tmp[0];
		data.type = atoi(tmp[1].c_str());
		data.count = atoi(tmp[2].c_str());
		data.extype = JhGlobalData::getResExType(data.strid);//atoi(tmp[3].c_str());
		data.lv = atoi(tmp[4].c_str());
		data.exp = atoi(tmp[5].c_str());
		data.goodvalue = atoi(tmp[6].c_str());
		if (tmp.size() >= 9)
		{
			data.slv = atoi(tmp[7].c_str());
			data.tqu = atoi(tmp[8].c_str());
		}

		bool ishassame = false;
		if (data.type == WEAPON || data.type == PROTECT_EQU)
		{
			std::vector<PackageData>::iterator it;
			for (it = JhStorageRoom::map_storageData[data.type].begin(); it != JhStorageRoom::map_storageData[data.type].end();it++)
			{
				if (it->strid.compare(data.strid) == 0)
				{
					it->goodvalue += data.goodvalue;
					if (it->slv < data.slv)
						it->slv = data.slv;
					if (it->lv < data.lv)
						it->lv = data.lv;
					ishassame = true;
				}
			}
		}
		if (ishassame)
			continue;
		vec_packages.push_back(data);
	}
}

int JhMyPackage::getMax()
{
	return max;
}

void JhMyPackage::setMax(int val)
{
	max = val;
}

int JhMyPackage::getSize()
{
	return vec_packages.size();
}
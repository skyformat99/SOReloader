#include "SOManager.h"
#include <iostream>
#include "MacroDefine.h"
#include "../Dependence/TinyXML/tinyxml.h"

using namespace SORELOADER_NAMESPACE;

SOManager::SOManager()
{

}

SOManager::~SOManager()
{

}

bool SOManager::Initialize(std::string configPath)
{
	mConfigFilePath = configPath;
	TiXmlDocument xmlConfig(mConfigFilePath.c_str());
	
	if (false == xmlConfig.LoadFile())
	{
		return false;
	}
	TiXmlElement* root = xmlConfig.FirstChildElement();
	LoadPathConfig(root);
	LoadLibraries(root);
	return true;;
}

bool SOManager::ReloadConfig()
{
	return Initialize(mConfigFilePath);
}

void SOManager::LoadPathConfig(TiXmlElement* root)
{
	TiXmlElement* pathConfig = root->FirstChildElement("Path");
	mLibraryFolderPath = pathConfig->FirstChildElement("LibraryPath")->GetText();
	mLibraryFolderPath4Update = pathConfig->FirstChildElement("LibraryPath4Update")->GetText();
}

void SOManager::LoadLibraries(TiXmlElement* root)
{
	TiXmlElement* librariesConfig = root->FirstChildElement("Library");
	for (TiXmlElement* libraryConfig = librariesConfig->FirstChildElement(); libraryConfig != NULL; libraryConfig = libraryConfig->NextSiblingElement())
	{
		std::string fileName = mLibraryFolderPath + libraryConfig->GetText();
		LoadLibrary(libraryConfig->Value(), fileName);
	}
}

bool SOManager::LoadLibrary(std::string libraryName, std::string fileName)
{
	auto iter = mSharedLibrarys.find(libraryName);
	if (iter == mSharedLibrarys.end())
	{
		iter = mSharedLibrarys.emplace(libraryName, std::list<SharedLibrary>()).first;
	}

	std::list<SharedLibrary> &libraryList = iter->second;
	libraryList.push_front(SharedLibrary());
	SharedLibrary &lib = libraryList.front();
	bool loadSuccess = lib.LoadLibrary(fileName);

	if (!loadSuccess) 
	{ 
		libraryList.pop_front(); 
		return false; 
	}
	if (libraryList.size() > ELibraryListCapacity)
	{
		libraryList.pop_back();
	}
	return true;
}

bool SOManager::UpdateSharedLibrary(std::string library, std::string fileName)
{
	return LoadLibrary(library, mLibraryFolderPath4Update + fileName);
}

bool SOManager::RollbackLibrary(std::string library)
{
	auto iter = mSharedLibrarys.find(library);
	if (iter == mSharedLibrarys.end())
	{
		std::cout << "Can not find Library" << std::endl;
		return false;
	}
	std::list<SharedLibrary> &libraryList = iter->second;
	uint64_t size = libraryList.size();
	if (size <= 1)
	{
		std::cout << "Library List Size Less Than Two!" << std::endl;
		return false;
	}
	libraryList.pop_front();
	return true;
}

SharedLibrary *SOManager::GetLibrary(std::string library)
{
	auto iter = mSharedLibrarys.find(library);
	if (iter != mSharedLibrarys.end())
	{
		std::list<SharedLibrary> &libraryList = iter->second;
		if (libraryList.size() > 0)
		{
			return &libraryList.front();
		}
	}
	return nullptr;
}
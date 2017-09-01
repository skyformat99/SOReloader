#include "SOManager.h"
#include <iostream>
#include "MacroDefine.h"

using namespace SORELOADER_NAMESPACE;

SOManager::SOManager()
{

}

SOManager::~SOManager()
{

}

ESOReloaderError SOManager::Initialize(std::string configPath)
{
	mConfigFilePath = configPath;
	TiXmlDocument xmlConfig(mConfigFilePath.c_str());
	
	if (false == xmlConfig.LoadFile())
	{
		return EConfigError;
	}
	TiXmlElement* root = xmlConfig.FirstChildElement();
	LoadPathConfig(root);
	LoadLibraries(root);
	return EErrorNone;
}

ESOReloaderError SOManager::ReloadConfig()
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

ESOReloaderError SOManager::RollbackLibrary(std::string library)
{
	ESOReloaderError result = EErrorNone;
	auto iter = mSharedLibrarys.find(library);
	if (iter != mSharedLibrarys.end())
	{
		std::list<SharedLibrary> &libraryList = iter->second;
		uint64_t size = libraryList.size();
		if (size > 1)
		{
			libraryList.pop_front();
		}
		else
		{
			result = ELibraryListSizeLessThanTwo;
		}
		
	}
	else
	{
		result = ECanNotFindLibrary;
	}
	return result;
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
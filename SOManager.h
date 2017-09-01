#ifndef __SOMANAGER_H__
#define __SOMANAGER_H__

#include <map>
#include <list>
#include "SharedLibrary.h"
#include "Dependence/TinyXML/tinyxml.h"

SORELOADER_NAMESPACE_BEGIN

class SOManager
{
	enum
	{
		ELibraryListCapacity = 5,
	};

public :
	SOManager();
	~SOManager();
	ESOReloaderError Initialize(std::string configPath);
	ESOReloaderError ReloadConfig();
	bool UpdateSharedLibrary(std::string library, std::string fileName);
	ESOReloaderError RollbackLibrary(std::string library);
	SharedLibrary * GetLibrary(std::string library);

private :
	void LoadPathConfig(TiXmlElement *root);
	void LoadLibraries(TiXmlElement *root);
	bool LoadLibrary(std::string libraryName, std::string fileName);

private :
	std::string mConfigFilePath;
	std::string mLibraryFolderPath;
	std::string mLibraryFolderPath4Update;
	std::map<std::string, std::list<SharedLibrary>> mSharedLibrarys;
};

SORELOADER_NAMESPACE_END

#endif //__SOMANAGER_H__
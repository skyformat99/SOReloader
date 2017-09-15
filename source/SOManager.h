#ifndef __SOMANAGER_H__
#define __SOMANAGER_H__

#include <map>
#include <list>
#include "SharedLibrary.h"

class TiXmlElement;

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
	bool Initialize(std::string configPath);
	bool ReloadConfig();
	bool UpdateSharedLibrary(std::string library, std::string fileName);
	bool RollbackLibrary(std::string library);
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
#ifndef __SHAREDLIBRARY_H__
#define __SHAREDLIBRARY_H__

#include <string>
#include "MacroDefine.h"

SORELOADER_NAMESPACE_BEGIN

class SharedLibrary
{
public:
	SharedLibrary();
	SharedLibrary(std::string libName);
	~SharedLibrary();

	bool LoadLibrary(std::string libName);
	bool IsLoadSuccess();
	void *GetFunction(std::string symbol);

private:
	void *mHandle;
};

SORELOADER_NAMESPACE_END

#endif // __SHAREDLIBRARY_H__

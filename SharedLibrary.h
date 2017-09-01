#ifndef __SHAREDLIBRARY_H__
#define __SHAREDLIBRARY_H__

#include <string>
#include "Error.h"

SORELOADER_NAMESPACE_BEGIN

class SharedLibrary
{
public:
	SharedLibrary();
	SharedLibrary(std::string libName);
	~SharedLibrary();

	ESOReloaderError LoadLibrary(std::string libName);
	bool IsLoadSuccess();
	ESOReloaderError GetErrorCode();
	void *GetFunction(std::string symbol);

private:
	void *mHandle;
	ESOReloaderError mLoadLibraryReturnCode = EErrorNone;
};

SORELOADER_NAMESPACE_END

#endif // __SHAREDLIBRARY_H__

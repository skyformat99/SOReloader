#include "SharedLibrary.h"
#include <dlfcn.h>

using namespace SORELOADER_NAMESPACE;

SharedLibrary::SharedLibrary()
{
	mHandle = NULL;
}

SharedLibrary::SharedLibrary(std::string libName)
{
	LoadLibrary(libName);
}

SharedLibrary::~SharedLibrary()
{
	if (mHandle != NULL)
	{
		dlclose(mHandle);
	}
}

ESOReloaderError SharedLibrary::LoadLibrary(std::string libName)
{
	mHandle = dlopen(libName.c_str(), RTLD_LAZY);
	mLoadLibraryReturnCode = (mHandle != NULL) ? EErrorNone : ELoadLibraryFailed;
	return mLoadLibraryReturnCode;
}

void *SharedLibrary::GetFunction(std::string symbol)
{
	void *func = NULL;
	if (mHandle != NULL)
	{
		 func = dlsym(mHandle, symbol.c_str());
	}
	return func;
}

bool SharedLibrary::IsLoadSuccess()
{
	return mLoadLibraryReturnCode == EErrorNone;
}

ESOReloaderError SharedLibrary::GetErrorCode()
{
	return mLoadLibraryReturnCode;
}
#include "SharedLibrary.h"
#include <dlfcn.h>
#include <iostream>

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
		mHandle = NULL;
	}
}

bool SharedLibrary::LoadLibrary(std::string libName)
{
	mHandle = dlopen(libName.c_str(), RTLD_LAZY);
	const char * error = dlerror();
	if (error != NULL)
	{
		std::cout << error << std::endl;
		return false;
	}
	return true;
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
	return mHandle == NULL;
}
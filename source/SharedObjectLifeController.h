#ifndef __SHAREDOBJECTLIFECONTROLLER_H__
#define __SHAREDOBJECTLIFECONTROLLER_H__

#include "SharedLibrary.h"

SORELOADER_NAMESPACE_BEGIN

template<typename T>
T *CreateSharedObject(SharedLibrary *lib)
{
	std::string functionName("Create");
	functionName += T::ClassName();
	typedef T *Create();
	Create *createFunction = (Create*)lib->GetFunction(functionName);
	return createFunction();
}

template<typename T>
T *DestroySharedObject(SharedLibrary *lib, T *object)
{
	std::string functionName("Destroy");
	functionName += T::ClassName();
	typedef void Destroy(T *);
	Destroy *destroyFunction = (Destroy *)lib->GetFunction(functionName.c_str());
	destroyFunction(object);
}

SORELOADER_NAMESPACE_END

#endif // __SHAREDOBJECTLIFECONTROLLER_H__
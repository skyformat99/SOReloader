#ifndef __SHAREDOBJECTLIFECONTROLLER_H__
#define __SHAREDOBJECTLIFECONTROLLER_H__

#include "SharedLibrary.h"
#include <typeinfo>

SORELOADER_NAMESPACE_BEGIN

template<typename T>
class SharedObjectLifeController
{
public :
	SharedObjectLifeController(SharedLibrary *lib) noexcept;
	~SharedObjectLifeController() noexcept;
	T *operator->();
	T *GetObject();

private :
	T *mObject;
	SharedLibrary *mObjectBelongLib;
};

template<typename T>
SharedObjectLifeController<T>::SharedObjectLifeController(SharedLibrary *lib) noexcept
{
	mObjectBelongLib = lib;
	std::string functionName("Create");
	functionName += T::ClassName();
	typedef T *Create();
	Create *createFunction = (Create*)lib->GetFunction(functionName);
	mObject = createFunction();
}

template<typename T>
SharedObjectLifeController<T>::~SharedObjectLifeController() noexcept
{
	std::string functionName("Destroy");
	functionName += T::ClassName();
	typedef void Destroy(T *);
	Destroy *destroyFunction = (Destroy *)mObjectBelongLib->GetFunction(functionName.c_str());
	destroyFunction(mObject);
}

template<typename T>
T *SharedObjectLifeController<T>::operator->()
{
	return mObject;
}

template<typename T>
T *SharedObjectLifeController<T>::GetObject()
{
	return mObject;
}

SORELOADER_NAMESPACE_END

#endif // __SHAREDOBJECTLIFECONTROLLER_H__
#ifndef __SORELOADER_MACRO_DEFINE__
#define __SORELOADER_MACRO_DEFINE__

#ifndef SORELOADER_NAMESPACE
#define SORELOADER_NAMESPACE soreloader
#endif
#ifndef SORELOADER_NAMESPACE_BEGIN
#define SORELOADER_NAMESPACE_BEGIN namespace SORELOADER_NAMESPACE {
#endif
#ifndef SORELOADER_NAMESPACE_END
#define SORELOADER_NAMESPACE_END }
#endif


#define OFFER_CREATE_FUNCTION(className)\
	extern "C"\
	{\
		className *Create##className()\
		{\
			return new className;\
		}\
	} 

#define OFFER_DESTROY_FUNCTION(className)\
	extern "C"\
	{\
		void Destroy##className(className *object)\
		{\
			if (object != nullptr)\
			{\
				delete object;\
			}\
		}\
	} 

#define OFFER_CREATE_DESTROY_FUNCTION(className)\
	OFFER_CREATE_FUNCTION(className)\
	OFFER_DESTROY_FUNCTION(className)

#define IF_RETURN(condition, returnValue)\
	if (condition)\
	{\
		return returnValue;\
	}

#endif // __SORELOADER_MACRO_DEFINE__
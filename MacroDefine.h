#ifndef __SORELOADER_MACRO_DEFINE__
#define __SORELOADER_MACRO_DEFINE__

#define OFFER_CREATE_DESTROY_FUCNTION(objectName)\
	extern "C"\
	{\
		objectName *Create##objectName()\
		{\
			return new objectName;\
		}\
		void Destroy##objectName(objectName *object)\
		{\
			if (object != nullptr)\
			{\
				delete object;\
			}\
		}\
	} 

#define IF_RETURN(condition, returnValue)\
	if (condition)\
	{\
		return returnValue;\
	}

#endif // __SORELOADER_MACRO_DEFINE__
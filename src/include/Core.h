#pragma once 


#ifdef WIN32 
	#ifdef BNN_EXPORT 
		#define BNN_LIB __declspec(dllexport)	
	#else 
		#define BNN_LIB __declspec(dllimport )
	#endif 
#else 
#define BNN_LIB
#endif 

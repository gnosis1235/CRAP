
#ifndef OS_VERSION_DEFINED_FOR_LMF2ROOT
	#define OS_VERSION_DEFINED_FOR_LMF2ROOT

	//#define LINUX
	//#define dont_use_MFC

	#pragma warning(disable : 4996)
	#pragma warning(disable : 4800)


	#define NUM_CHANNELS (100)
	#define NUM_IONS (100)

	#ifdef LINUX
		#define dont_use_MFC
		#ifndef __int32_IS_DEFINED
			#define __int32_IS_DEFINED
		#define __int32 int
		#define __int16 short
		#define __int64 long long
		#define __int8 char
		#endif
	#endif

#endif
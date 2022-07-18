#pragma once

#if PLATFORM_ANDROID

//#include "Android/AndroidJavaEnv.h"
#endif

#if PLATFORM_ANDROID
class FTypes {
public:
	// FString <=> jstring
	static FString Java2Cpp(JNIEnv* Env, jstring JavaString);
	static jstring Cpp2Java(JNIEnv* Env, const FString&);

	static bool Java2Cpp(JNIEnv* Env, jboolean JavaBool);
	static jboolean Cpp2Java(JNIEnv* Env, bool);
};
#endif
#include "Types.h"
#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"



// FString <=> jstring

FString FTypes::Java2Cpp(JNIEnv* Env, jstring JavaString){
	check(Env != nullptr);

	const char* UTFString = Env->GetStringUTFChars(JavaString, JNI_FALSE);
	FString CPPString = FString(UTF8_TO_TCHAR(UTFString));
	Env->ReleaseStringUTFChars(JavaString, UTFString);

	return MoveTemp(CPPString);
 }

jstring FTypes::Cpp2Java(JNIEnv* Env, const FString& CppString) {
	check(Env != nullptr);

	return Env->NewStringUTF(TCHAR_TO_UTF8(*CppString));
}

bool FTypes::Java2Cpp(JNIEnv* Env, jboolean JavaBool){
	check(Env != nullptr);
	return static_cast<bool>(JavaBool);
}

jboolean FTypes::Cpp2Java(JNIEnv* Env, bool CppBool){
	check(Env != nullptr);
	return static_cast<jboolean>(CppBool);
}
#endif
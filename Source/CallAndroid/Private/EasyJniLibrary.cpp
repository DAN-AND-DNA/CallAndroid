#include "EasyJniLibrary.h"
#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Runtime/Launch/Public/Android/AndroidJNI.h"		// ¹Ù·½µÄÀý×Ó
#include "Android/AndroidJavaEnv.h"
#include "Android/AndroidJava.h"
#include "Runtime/Core/Public/Templates/UnrealTypeTraits.h"
#endif





void UEasyJniLibrary::TestAll() {
}


FString UEasyJniLibrary::GetAndroidID() {
#if PLATFORM_ANDROID
	//ddd1
	if (JNIEnv * Env{ FAndroidApplication::GetJavaEnv() }) {
		const jclass& Class{ FAndroidApplication::FindJavaClass("cn/afternoontea/ue4/DeviceInfo")};
		const jmethodID& Method{ FJavaWrapper::FindStaticMethod(Env, Class, "GetAndroidID", "(Landroid/app/Activity;)Ljava/lang/String;" , false)};
		jstring JavaString = static_cast<jstring>(Env->CallStaticObjectMethod(Class, Method, FJavaWrapper::GameActivityThis));

		const char* Characters{ Env->GetStringUTFChars(JavaString, 0) };

		FString ReturnString(UTF8_TO_TCHAR(Characters));

		Env->ReleaseStringUTFChars(JavaString, Characters);

		return MoveTemp(ReturnString);
	}

	return FString();
#endif

	return FString();
}


FString UEasyJniLibrary::GetHelloWorld() {
#if PLATFORM_ANDROID
	FString ClassName(TEXT("cn/afternoontea/ue4/HelloWorld"));
	FString MethodName(TEXT("Say"));
	bool OpResult = false;

	FString StrResult = UEasyJniLibrary::CallAndroidStaticMethodImpl<FString>(ClassName, MethodName, OpResult);

	if (OpResult) {
		return MoveTemp(StrResult);
	}
	
	return FString();
#endif
	return FString();
}


#if PLATFORM_ANDROID
jclass UEasyJniLibrary::GetClassLoaderClass(JNIEnv* Env) {
	if (Env == nullptr) {
		return nullptr;
	}

	static jclass ClassLoaderClass;

	if (ClassLoaderClass == nullptr) {
		ClassLoaderClass = Env->FindClass("java/lang/ClassLoader");
	}

	return ClassLoaderClass;
}

jmethodID UEasyJniLibrary::GetFindClassMethod(JNIEnv* Env) {
	static jmethodID FindClassMethod;
	if (FindClassMethod == nullptr) {
		if (Env == nullptr) {
			return 0;
		}

		jclass TmpClassLoderClass = UEasyJniLibrary::GetClassLoaderClass(Env);
		if (TmpClassLoderClass == nullptr) {
			return 0;
		}

		FindClassMethod = Env->GetMethodID(TmpClassLoderClass, "findClass", "(Ljava/lang/String;)Ljava/lang/Class;");
	}
	
	return FindClassMethod;
}


bool UEasyJniLibrary::CheckException(JNIEnv* Env) {
	if (Env == nullptr) {
		return false;
	}

	if (Env->ExceptionCheck()) {
#if UE_BUILD_SHIPPING
		Env->ExceptionClear();
#else
		Env->ExceptionDescribe();
		Env->ExceptionClear();
#endif
		return true;
	}

	return false;
}


bool UEasyJniLibrary::FindJavaClass(JNIEnv* Env, const FString& InClassName, jclass& OutJavaClass) {
	if (Env == nullptr) {
		return false;
	}

	jstring ClassName = Env->NewStringUTF(TCHAR_TO_ANSI(*InClassName));
	OutJavaClass = static_cast<jclass>(Env->CallObjectMethod(FAndroidApplication::GetClassLoader(), GetFindClassMethod(Env), ClassName));
	Env->DeleteLocalRef(ClassName);

	if (OutJavaClass == nullptr) {
		return false;
	}

	return true;
}
#endif
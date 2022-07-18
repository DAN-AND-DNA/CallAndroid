// Copyright 2022 Danyang Chen https://github.com/DAN-AND-DNA

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#endif
#include "Signatures.h"
#include "Types.h"
#include <stdarg.h>
#include "EasyJniLibrary.generated.h"






UCLASS()
class CALLANDROID_API UEasyJniLibrary : public UBlueprintFunctionLibrary {
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "CallAndroid")
	static FString GetAndroidID();

	UFUNCTION(BlueprintCallable, Category = "CallAndroid")
	static FString GetHelloWorld();

	static void TestAll();
protected:
	



#if PLATFORM_ANDROID

	static jclass GetClassLoaderClass(JNIEnv* Env);
	static jmethodID GetFindClassMethod(JNIEnv* Env);
	static bool CheckException(JNIEnv* Env);
	static bool FindJavaClass(JNIEnv* Env, const FString& InClassName, jclass& OutJavaClass);


	template<typename FuncReturn>
	static typename TEnableIf<TIsSame<FuncReturn, FString>::Value, FString>::Type CallStaticMethod(JNIEnv* Env, const jclass JavaClass, const jmethodID JavaMethod, ...) {
		check(Env != nullptr);
		check(JavaClass != nullptr);

		va_list Args;
		va_start(Args, JavaMethod);
		jstring JavaStringReturn = static_cast<jstring>(Env->CallStaticObjectMethodV(JavaClass, JavaMethod, Args));
		va_end(Args);

		return FTypes::Java2Cpp(Env, JavaStringReturn);
	
	}

	template<typename FuncReturn, typename... CppArg>
	static typename TEnableIf<TIsSame<FuncReturn, FString>::Value, FString>::Type CallAndroidStaticMethodImpl(const FString& InClassName, const FString& InMethodName, bool& OpResult, CppArg... InArgs) {
		JNIEnv* Env = FAndroidApplication::GetJavaEnv();
		FString MethodSignature = FSignatures::MakeMethodSignature<FString>(InArgs...);

	
		// check
		if (Env == nullptr || InClassName.IsEmpty() || InMethodName.IsEmpty()) {
			OpResult = false;
			return FString();
		}

		// find class
		jclass JavaClass = nullptr;
		if (!FindJavaClass(Env, InClassName, JavaClass)) {
			CheckException(Env);
			OpResult = false;
			return FString();
		}

		// find method
		jmethodID JavaMethod = Env->GetStaticMethodID(JavaClass, TCHAR_TO_ANSI(*InMethodName), TCHAR_TO_ANSI(*MethodSignature));
		if (JavaMethod == nullptr) {
			CheckException(Env);
			OpResult = false;
			return FString();
		}

		// call static method
		FString MethodResult = UEasyJniLibrary::CallStaticMethod<FString>(Env, JavaClass, JavaMethod, FTypes::Cpp2Java(Env, InArgs)...);
		Env->DeleteLocalRef(JavaClass);
		OpResult = true;
		return MoveTemp(MethodResult);
			
	}
#endif

private:
#if PLATFORM_ANDROID
	
#endif

};
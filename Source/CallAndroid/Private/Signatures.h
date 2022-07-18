#pragma once
#include "CoreMinimal.h"


UENUM()
enum class ArgumentType : uint8 {
	ENone = 0,
	EVoid,
	EBoolean,	// jboolean
	EString,	// jstring
	EByte,		// jbyte
	EShort,		// jshort
	EInt,		// jint
	ELong,		// jlong
	EChar,		// jchar
	EFloat,		// jfloat
	EDouble,	// jdouble
};

class FSignatures {

public:

	// return and arg type
	template <typename T>
	static FORCEINLINE ArgumentType GetArgType(const T&) {
		return GetType<T>();
	}

	template <typename T>
	static FORCEINLINE ArgumentType GetReturnType() {
		return GetType<T>();
	}
private:

	// void
	template <typename ArgType>
	static typename TEnableIf<TIsSame<ArgType, void>::Value, ArgumentType>::Type GetType() {
			return ArgumentType::EVoid;
	}

	// jstring
	template <typename ArgType>
	static typename TEnableIf<TIsSame<ArgType, FString>::Value, ArgumentType>::Type GetType() {
		return ArgumentType::EString;
	}
	
	// jboolean
	template <typename ArgType>
	static typename TEnableIf<TIsSame<ArgType, bool>::Value, ArgumentType>::Type GetType() {
		return ArgumentType::EBoolean;
	}


	// jbyte
	template <typename ArgType>
	static typename TEnableIf<TIsSame<ArgType, int8>::Value, ArgumentType>::Type GetType() {
		return ArgumentType::EByte;
	}

	template <typename ArgType>
	static typename TEnableIf<TIsSame<ArgType, uint8>::Value, ArgumentType>::Type GetType() {
		return ArgumentType::EByte;
	}
	
	// jshort
	template <typename ArgType>
	static typename TEnableIf<TIsSame<ArgType, int16>::Value, ArgumentType>::Type GetType() {
		return ArgumentType::EShort;
	}

	// jint
	template <typename ArgType>
	static typename TEnableIf<TIsSame<ArgType, int32>::Value, ArgumentType>::Type GetType() {
		return ArgumentType::EInt;
	}

	// jlong
	template <typename ArgType>
	static typename TEnableIf<TIsSame<ArgType, int64>::Value, ArgumentType>::Type GetType() {
		return ArgumentType::ELong;
	}

	// jchar
	template <typename ArgType>
	static typename TEnableIf<TIsSame<ArgType, uint16>::Value, ArgumentType>::Type GetType() {
		return ArgumentType::EChar;
	}

	// jfloat
	template <typename ArgType>
	static typename TEnableIf<TIsSame<ArgType, float>::Value, ArgumentType>::Type GetType() {
		return ArgumentType::EFloat;
	}

	// jdouble
	template <typename ArgType>
	static typename TEnableIf<TIsSame<ArgType, double>::Value, ArgumentType>::Type GetType() {
		return ArgumentType::EDouble;
	}

	template<typename T>
	static FString GetArgSignature(const T& Arg) {
		switch (GetArgType(Arg)) {
		case ArgumentType::EBoolean: return FString(TEXT("Z"));
		case ArgumentType::EString: return FString(TEXT("Ljava/lang/String;"));
		case ArgumentType::EByte: return FString(TEXT("B"));
		case ArgumentType::EShort: return FString(TEXT("S"));
		case ArgumentType::EInt: return FString(TEXT("I"));
		case ArgumentType::ELong: return FString(TEXT("J"));
		case ArgumentType::EChar: return FString(TEXT("C"));
		case ArgumentType::EFloat: return FString(TEXT("F"));
		case ArgumentType::EDouble: return FString(TEXT("D"));
		default: return (FString());
		}
	}

	// support void
	template<typename T>
	static FString GetReturnSignature() {
		switch (GetReturnType<T>()) {
		case ArgumentType::EVoid: return FString(TEXT("V"));
		case ArgumentType::EBoolean: return FString(TEXT("Z"));
		case ArgumentType::EString: return FString(TEXT("Ljava/lang/String;"));
		case ArgumentType::EByte: return FString(TEXT("B"));
		case ArgumentType::EShort: return FString(TEXT("S"));
		case ArgumentType::EInt: return FString(TEXT("I"));
		case ArgumentType::ELong: return FString(TEXT("J"));
		case ArgumentType::EChar: return FString(TEXT("C"));
		case ArgumentType::EFloat: return FString(TEXT("F"));
		case ArgumentType::EDouble: return FString(TEXT("D"));
		default:return (FString(TEXT("")));
		}
	}

	static void MakeArgsSignatures(FString& CurrentSignatures) {}

	template<typename A, typename... T>
	static void MakeArgsSignatures(FString& CurrentSignatures, const A& Arg, const T& ...Args) {
		CurrentSignatures += GetArgSignature(Arg);
		MakeArgsSignatures(CurrentSignatures, Args...);
	}

public:

	template <typename FuncReturn, typename... FuncArgs>
	static FString MakeMethodSignature(FuncArgs ...InArgs) {
		FString CurrentSignatures(TEXT("("));
		MakeArgsSignatures(CurrentSignatures, InArgs...);
		CurrentSignatures += FString(TEXT(")"));
		CurrentSignatures += GetReturnSignature<FuncReturn>();

		return MoveTemp(CurrentSignatures);
	}

	static void TestAll();
};
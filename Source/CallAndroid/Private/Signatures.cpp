#include "Signatures.h"

void FSignatures::TestAll() {
	check(FSignatures::MakeMethodSignature<void>() == FString(TEXT("()V")));
	check(FSignatures::MakeMethodSignature<void>(true, FString()) == FString(TEXT("(ZLjava/lang/String;)V")));
}

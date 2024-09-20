#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

int main() {
    LLVMContext Context;
    Module *ModuleOb = new Module("hello", Context);
    IRBuilder<> Builder(Context);

    // Declare printf function (extern)
    std::vector<Type *> printfArgs({Builder.getInt8PtrTy()});
    FunctionType *printfType = FunctionType::get(Builder.getInt32Ty(), printfArgs, true);
    FunctionCallee printfFunc = ModuleOb->getOrInsertFunction("printf", printfType);

    // Create main function
    FunctionType *mainType = FunctionType::get(Builder.getInt32Ty(), false);
    Function *mainFunc = Function::Create(mainType, Function::ExternalLinkage, "main", ModuleOb);

    // Create basic block and set the insertion point
    BasicBlock *entry = BasicBlock::Create(Context, "entry", mainFunc);
    Builder.SetInsertPoint(entry);

    // Create global string "Hello, World!\n"
    Value *helloWorldStr = Builder.CreateGlobalStringPtr("Hello, World!\n");

    // Call printf
    Builder.CreateCall(printfFunc, helloWorldStr);

    // Return 0
    Builder.CreateRet(Builder.getInt32(0));

    // Verify and print the generated LLVM IR
    verifyFunction(*mainFunc);
    ModuleOb->print(outs(), nullptr);

    delete ModuleOb;
    return 0;
}

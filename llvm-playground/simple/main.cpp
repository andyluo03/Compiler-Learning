#include <iostream>
#include <llvm-11/llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm-11/llvm/IR/BasicBlock.h>
#include <llvm-11/llvm/IR/DerivedTypes.h>
#include <llvm-11/llvm/Support/TargetSelect.h>
#include <memory>
#include <vector>

#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

// OUTPUT CODE THAT PRODUCES AN EXECUTABLE THAT PRINTS HELLO WORLD

llvm::Function* produceMain (llvm::LLVMContext& context, llvm::Module* module, llvm::IRBuilder<>& builder) {
    llvm::FunctionType* mainType =  llvm::FunctionType::get(builder.getInt32Ty(), {}, false);
    llvm::Function* mainFunc = llvm::Function::Create(mainType, llvm::Function::ExternalLinkage, "main", module);
}

int main () {
    std::cout << "Starting Dummy Program..." << std::endl;

    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmParser();
    llvm::InitializeNativeTargetAsmPrinter();

    llvm::LLVMContext context;
    llvm::Module* module = new llvm::Module("PrintSeven", context);
    llvm::IRBuilder<> builder(context);
    

    llvm::Function* mainFunc = produceMain(context, module, builder);
    llvm::BasicBlock* entry = llvm::BasicBlock::Create(context, "entry", mainFunc);



    std::string errStr = "What";
    llvm::ExecutionEngine* executor = llvm::EngineBuilder(std::unique_ptr<llvm::Module>(module))
        .setErrorStr(&errStr)
        .setEngineKind(llvm::EngineKind::JIT)
        .create();

    std::vector<llvm::GenericValue> noArgs;
    auto result = executor->runFunction(mainFunc, noArgs);

    return 0;
}
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/Debug.h"
#include "llvm/Analysis/BasicAliasAnalysis.h"

using namespace llvm;

namespace {
  struct InstCombineNew : public FunctionPass {
    static char ID; // Pass identification, replacement for typeid
    InstCombineNew() : FunctionPass(ID) {}

    bool runOnFunction(Function &F) override {
      bool changed = false;

      // Iterira kroz sve osnovne blokove funkcije
      for (BasicBlock &BB : F) {
        for (Instruction &I : BB) {
          // Optimizacija 1: Premestanje konstanti sa leve strane u desnu stranu binarnih operatora
          if (auto *BO = dyn_cast<BinaryOperator>(&I)) {
            Value *LHS = BO->getOperand(0);
            Value *RHS = BO->getOperand(1);

            if (auto *C = dyn_cast<ConstantInt>(LHS)) {
              // Premestanje konstante na desnu stranu
              BO->setOperand(0, RHS);
              BO->setOperand(1, LHS);
              changed = true;
              errs() << "Optimizacija 1\n" ;
            }
          }

          // Optimizacija 2: Prevodjenje komparacija sa <, >, ≤, ≥ u = ili ≠
          if (auto *Cmp = dyn_cast<CmpInst>(&I)) {
            if (Cmp->getPredicate() == CmpInst::ICMP_SGT ||
                Cmp->getPredicate() == CmpInst::ICMP_SLT) {
              // Zamenjuje sa ICMP_NE (nije jednako)
              Cmp->setPredicate(CmpInst::ICMP_NE);
              changed = true;
              errs() << "Optimizacija 2\n" ;
            }
          }

          // Optimizacija 3: Sabiranje istih vrednosti sa množenjem ili pomakom
          if (auto *BO = dyn_cast<BinaryOperator>(&I)) {
            if (BO->getOpcode() == Instruction::Add) {
              Value *LHS = BO->getOperand(0);
              Value *RHS = BO->getOperand(1);
              if (LHS == RHS) {
                // Pretvaranje "add X, X" u "mul X, 2" ili "shl X, 1"
                IRBuilder<> Builder(&I);
                Value *NewInst = Builder.CreateShl(LHS, 1);  // ili može biti CreateMul(LHS, ConstantInt::get(LHS->getType(), 2));
                BO->replaceAllUsesWith(NewInst);
                changed = true;
                errs() << "Optimizacija 3\n" ;
              }
            }
          }

          // Optimizacija 4: Množenje sa konstantama koje su stepeni dvojke sa pomakom
          if (auto *BO = dyn_cast<BinaryOperator>(&I)) {
            if (BO->getOpcode() == Instruction::Mul) {
              if (auto *C = dyn_cast<ConstantInt>(BO->getOperand(1))) {
                if (C->getValue().isPowerOf2()) {
                  // Zamena množenja sa pomakom
                  IRBuilder<> Builder(&I);
                  Value *NewInst = Builder.CreateShl(BO->getOperand(0), C->getValue().logBase2());
                  BO->replaceAllUsesWith(NewInst);
                  changed = true;
                  errs() << "Optimizacija 4\n" ;
                }
              }
            }
          }
        }
      }
      return changed;
    }
  };
}

char InstCombineNew::ID = 0;
static RegisterPass<InstCombineNew> X("instcombine-new", "InstCombine Pass New vrsi zamenu odredjenih operacija efikasnijim", false, true);

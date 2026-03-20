//===- MaybeUnused.cpp ----------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// A simple Clang plugin for a lab assignment.
//
// It finds unused function parameters and unused local variables in the main
// source file and marks them with the [[maybe_unused]] attribute in the AST.
//
//===----------------------------------------------------------------------===//

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/Attr.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;

namespace {

static bool isFromMainFile(const Decl *D, const ASTContext &Context) {
  const SourceManager &SM = Context.getSourceManager();
  if (!D->getLocation().isValid())
    return false;
  return SM.isWrittenInMainFile(SM.getSpellingLoc(D->getLocation()));
}

static void addMaybeUnusedIfNeeded(VarDecl *VD, ASTContext &Context) {
  if (!VD || VD->isUsed() || VD->hasAttr<UnusedAttr>())
    return;

  VD->addAttr(UnusedAttr::CreateImplicit(Context));
}

class MaybeUnusedVisitor : public RecursiveASTVisitor<MaybeUnusedVisitor> {
public:
  explicit MaybeUnusedVisitor(ASTContext &Context) : Context(Context) {}

  bool VisitFunctionDecl(FunctionDecl *FD) {
    if (!FD->doesThisDeclarationHaveABody() || !isFromMainFile(FD, Context))
      return true;

    for (ParmVarDecl *Param : FD->parameters()) {
      if (!isFromMainFile(Param, Context))
        continue;
      addMaybeUnusedIfNeeded(Param, Context);
    }

    return true;
  }

  bool VisitVarDecl(VarDecl *VD) {
    if (!VD->isLocalVarDecl() || VD->isImplicit())
      return true;

    if (!isFromMainFile(VD, Context))
      return true;

    addMaybeUnusedIfNeeded(VD, Context);
    return true;
  }

private:
  ASTContext &Context;
};

class MaybeUnusedConsumer : public ASTConsumer {
public:
  explicit MaybeUnusedConsumer(ASTContext &Context) : Visitor(Context) {}

  void HandleTranslationUnit(ASTContext &Context) override {
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
    Context.getTranslationUnitDecl()->dump(llvm::outs(), /*ShowColors=*/false);
  }

private:
  MaybeUnusedVisitor Visitor;
};

class MaybeUnusedAction : public PluginASTAction {
protected:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 llvm::StringRef) override {
    return std::make_unique<MaybeUnusedConsumer>(CI.getASTContext());
  }

  bool ParseArgs(const CompilerInstance &,
                 const std::vector<std::string> &) override {
    return true;
  }
};

} // namespace

static FrontendPluginRegistry::Add<MaybeUnusedAction>
    X("maybe-unused", "mark unused locals and parameters with [[maybe_unused]]");

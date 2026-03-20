// RUN: %clang_cc1 -load %llvmshlibdir/MaybeUnused%pluginext -plugin maybe-unused %s | FileCheck %s

int baz(int p, int q) {
  int used_local = p + q;
  return used_local;
}

// CHECK: FunctionDecl{{.*}} baz 'int (int, int)'
// CHECK: ParmVarDecl{{.*}} p 'int'
// CHECK-NOT: UnusedAttr
// CHECK: ParmVarDecl{{.*}} q 'int'
// CHECK-NOT: UnusedAttr
// CHECK: VarDecl{{.*}} used_local 'int' cinit
// CHECK-NOT: UnusedAttr

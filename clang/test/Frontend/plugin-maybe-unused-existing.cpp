// RUN: %clang_cc1 -load %llvmshlibdir/MaybeUnused%pluginext -plugin maybe-unused %s | FileCheck %s

int bar(int used, [[maybe_unused]] int already_marked_param) {
  [[maybe_unused]] int already_marked_local = 0;
  return used;
}

// CHECK: FunctionDecl{{.*}} bar 'int (int, int)'
// CHECK: ParmVarDecl{{.*}} used 'int'
// CHECK-NOT: UnusedAttr{{.*}}Implicit maybe_unused
// CHECK: ParmVarDecl{{.*}} already_marked_param 'int'
// CHECK: UnusedAttr{{.*}}maybe_unused
// CHECK-NOT: UnusedAttr{{.*}}Implicit maybe_unused
// CHECK: VarDecl{{.*}} already_marked_local 'int' cinit
// CHECK: UnusedAttr{{.*}}maybe_unused
// CHECK-NOT: UnusedAttr{{.*}}Implicit maybe_unused

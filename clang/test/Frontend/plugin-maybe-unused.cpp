// RUN: %clang_cc1 -load %llvmshlibdir/MaybeUnused%pluginext -plugin maybe-unused %s | FileCheck %s

int foo(int a, int b, int c) {
  double value = 0.0;
  int answer = 42;
  return a + b;
}

// CHECK: FunctionDecl{{.*}} foo 'int (int, int, int)'
// CHECK: ParmVarDecl{{.*}} a 'int'
// CHECK-NOT: UnusedAttr
// CHECK: ParmVarDecl{{.*}} b 'int'
// CHECK-NOT: UnusedAttr
// CHECK: ParmVarDecl{{.*}} c 'int'
// CHECK-NEXT: UnusedAttr{{.*}}Implicit maybe_unused
// CHECK: VarDecl{{.*}} value 'double' cinit
// CHECK-NEXT: FloatingLiteral
// CHECK-NEXT: UnusedAttr{{.*}}Implicit maybe_unused
// CHECK: VarDecl{{.*}} answer 'int' cinit
// CHECK-NEXT: IntegerLiteral
// CHECK-NEXT: UnusedAttr{{.*}}Implicit maybe_unused

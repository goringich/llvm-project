This example implements variant 2 of the lab assignment.

Task:
- find all unused local variables and function parameters
- mark them with the [[maybe_unused]] attribute

The plugin works on the Clang AST and adds `UnusedAttr` nodes to declarations in
the main source file.

Plugin name:
- `maybe-unused`

What it does:
- visits function definitions in the main file
- marks unused parameters
- visits local variable declarations
- marks unused local variables
- skips declarations that are already annotated

Desired result in the AST:

  int foo(int a, int b, [[maybe_unused]] int c) {
      [[maybe_unused]] double value = 0.0;
      [[maybe_unused]] int answer = 42;
      return a + b;
  }

Typical usage after building:

Linux:
$ clang -cc1 -load /path/to/libMaybeUnused.so -add-plugin maybe-unused \
        -ast-dump input.cpp

Mac:
$ clang -cc1 -load /path/to/libMaybeUnused.dylib -add-plugin maybe-unused \
        -ast-dump input.cpp

Practical verification:

$ clang++ -Wall -Wextra -Wunused-variable -Wunused-parameter \
          -fsyntax-only sample.cpp

$ clang++ -Xclang -load -Xclang /path/to/libMaybeUnused.so \
          -Xclang -add-plugin -Xclang maybe-unused \
          -Wall -Wextra -Wunused-variable -Wunused-parameter \
          -fsyntax-only sample.cpp

The second command should suppress warnings for declarations that the plugin
marks as `[[maybe_unused]]`.

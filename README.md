# Math

![C/C++ CI](https://github.com/philpoore/Math/workflows/C/C++%20CI/badge.svg)

Just a simple math interpreter.

### Build

```
make
```

### Run

** Command line input:**

```bash
$ math "10+23"
33
```

** File input: **

```bash
& cat test.math
10+23
$ math -f test.math
33
```

** As REPL: **

```bash
$ math
> 10+23
33
> 45 + 45
90
```

# Todo list / Features

- [x] Numbers (double by default)
- [x] Basic Math Operators (+ - \* / % \*\*)
- [x] Basic Math functions (currently only uniary functions; take one arg, return one arg)
  - [x] trigonomic functions (sin, cos, tan, acos, asin, atan)
  - [x] Logorythmic functions (log, log2, log10, exp, exp2)
  - [x] Others
- [x] Global constants (PI)
- [x] Variable Assignment (a=10+10)
- [x] Examples folder
- [ ] Better readme
- [ ] Better getting started guide, requirements etc.
- [x] Allow file input e.g "math file.math"
- [ ] More operators (+= -=, boolean && ||, logic)
- [ ] Comments
- [ ] if / else
- [ ] loops (while, for)
- [ ] Custom Functions
- [ ] AST optimizations
- [ ] Nicer error handling
- [ ] Other Types
  - [ ] strings
  - [ ] arrays
  - [ ] bool
- [ ] Better testing system

# Potential Features

- [ ] Code / Binary generation
- [ ] LLVM backend

# Testing

Tests are arranged in `test/` folder.

```bash
make check
```

# Requirements

- `g++`
- `make`
- `cpp std:lib`
- `clang-format` for formatting
- `node.js` for fuzz test generation

# Development

Pull requests and bug reports welcome.

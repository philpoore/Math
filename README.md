# Math

Just a little math interpreter

### Build

```
make
```

### Run

```
$ math "10+23"
33
```

As REPL:

```
$ math
> 10+23
33
> 45 + 45
90
```

# Features

- Numbers (double by default)
- Basic Math Operators
  - Addition
  - Subtraction
  - Multiplication
  - Divison
  - Modulus
  - Exponent (power of)
- Basic Math functions (currently only uniary functions; take one arg, return one arg)
  - trigonomic functions (sin, cos, tan, acos, asin, atan)
  - Logorythmic functions (log, log2, log10, exp, exp2)
  - Others
- Global constants
- Variable Assignment

# Todo

- Better readme, more examples, better getting started guide, requirements etc.
- Allow file input e.g "math file.math"
- More operators (+= -=, boolean, logic)
- Custom Functions
- AST optimization
- Nicer error handling
- Types
- Better testing system

# Potential Features

- Code generation
- LLVM backend

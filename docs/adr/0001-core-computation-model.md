# 1. Core Computation Model: Combinatory Graph Reduction

## Status
Proposed

## Context
Traditional virtual machines (including typical Lisp or Scheme implementations) rely on von Neumann architecture paradigms: linear byte-code arrays, explicit hardware registers, and a sequential execution stack. 

However, standard formalisms based on bound variables and explicit syntax trees suffer from complex substitution overhead (the variable capture problem), making dynamic logic tracking and mathematical formalization heavy and error-prone. We need a fundamental computation model that unifies code and data into a single, fluid spatial geometry while eliminating naming contexts entirely.

## Decision
We will reject linear instruction-based byte-code and stack-based recursion. Instead, the core execution engine of `CL-Gnosis` will be built as a **Pure Combinatory Graph Reduction Machine** based on Haskell Curry’s combinatory logic ($S, K, I$ basis).

1. **Homogeneous Memory Structure**: Every node in the VM's heap will be represented as an immutable Atom (a primitive combinator) or a mutable Application Node `@(left, right)`.
2. **Variable Elimination**: The compiler will perform meta-abstraction to erase all variable names before execution. 
3. **Execution as Topology**: Computation will progress not by executing instructions, but by topologically rearranging (reducing) the connections within the application graph until a stable normal form is achieved.

## Consequences
* **Positive**: 
  * Total elimination of the variable capture problem and environment lookup overhead during runtime.
  * Operational Monism: Syntax, execution, and proofs become identical representation structures.
  * Natural support for lazy evaluation and sharing (avoiding redundant evaluation of shared graph structures).
* **Negative**:
  * Traditional linear debugging tools and standard call-stack trace analyses become obsolete and unusable.
  * High pressure on memory management and pointers, since computation constantly reshapes the graph in the heap.

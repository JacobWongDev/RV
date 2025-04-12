# Overview

This repository contains three assignments completed for Runtime Verification at Queen's University.

The main purpose of the assignments was to create a montioring framework for properties described as EFSMs,
with a few limitations.

1. EFSMs can have multiple registers, and any transition can be enabled/disabled based on the value of registers.
   From Assignment2 onwards, it is assumed that only one register is permitted per EFSM.
2. The single register must be a natural number, and transitions can only increase its value.
3. The EFSMs in all assignments do not produce any output.

- Assignment1 defines a framework to express EFSMs and has supporting code to test if a string is in the language
  of an EFSM.
- Assignment2 builds on Assignment1 to find empty and non-empty language states (reach set) of an EFSM.
- Assignment3 combines Assignment1 and Assignment2 to produce the framework described above.

For each assignment, the docs can be found in Assignment*/docs/INSTALLING

References:

1. "Automatic Functional Test Generation Using The Extended Finite State Machine Model" by Kwang-Ting Cheng and A.S. Krishnakumar.
2. "Runtime Verification for LTL and TLTL" by Andreas Bauer, Martin Leucker and Christian Schallhart.
# Description

Tools used:
  - C++
  - A High Level Synthesis tool.
  - HLS Libraries

The goal is to create modules, using C++ that can be compiled to RTL with the help of an HLS tool.
Many of the programs, if not all, are evaluated with a testbench.

## Task 1 - 6, weekly tasks of small scaled modules.

### Task 1
  - Exercise 1: 
    - Input 32-bit unsigned, output 16-bit unsigned, the sum of bits that are equal to one.
  - Exercise 2: 
    - Calculation of checksum of a given unsigned integer

### Task 2
  - Exercise 1:
    - Select the minimum bits needed to represent fully the output value.
  - Exercise 2:
    - A function to compute the canonic signed digit (CSD) representation of an signed integer, and a function that receives the CSD representation and an integer, and computes their product with shift and add/subtract operations.
  - Exercise 3:
    - Circuit of Run Length Encoding. From a channel, receive integers and after the accumulation of 10 values, send serially to the output channel, the value and the number of sequential times that the value appeared. For example input 2 2 2 2 3 12 12 1 1 5 will result to output 2 4 3 1 12 2 1 2 5 1.

### Task 3
  - Exercise 1:
    - Remake of Run Length Encoding, only that the process is done as long as there is one new value on the channel. In this way, the circuit has to remember the previous value. 
  - Exercise 2:
    - A module that evaluates the results of a blackjack game.

### Task 4
  - This is a more hardware specific task as it requires specific memory banks, frequency speed, and minimum latency. These are simulated with an HLS tool, by applying the right settings, and the goal is to adjust an initial code (not shown here) in order to allow loop pipeline and initiation interval = 1.

### Task 5
   - This is a module that computes the color number of a graph and the color of each node. The rule is that each node should not have the same color as its neighbors. The color number is the total colors needed to color the graph. This module is designed having throughput performance in mind. 

### Task 6
  - Exercise 1:
    - A theoretical exercise so its not included here.
  - Exercise 2:
    - The module performs a filtering operation with the help of 1D kernel. Identify the reason that restricts the reduction the module's initiation interval (II). Exploit the memory access patrt in order to achieve better II. achieved with the implementation of a circular buffer.   
    

## Task Final, a mid scaled module as the final task of the semester.
  - A custom hardware architecture for a 2D Cellular Automata model. A presentation in English is provided. The model aims at a high throughput architecture by applying an optimization method for the kernel filtering process that can be also used for CA computations.




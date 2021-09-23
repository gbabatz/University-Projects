# Description

This is a team project that was split in 3 parts.
Tools Used:
  - C
  - CodeWarrior for ARM Developer Suite
  - AXD Debugger

## Part 1
  1. Implementation of an algorithm for the Warhol and Solarization effect on an image. The algorithm needs to:
     1. Read an image, in YUV format,
     2. Resize U and V channels (due to YUV format chroma subsampling),
     3. Convert from YUV to RGB channels,
     4. Perform the algorithm of each effect to the RGB formated image, and 
     5. Reverse the process in order to save the final image in YUV format
  2. Application of Loop Optimizations. In our case we applied (combinations, partial, and full):
     - Loop Fusion
     - Loop Unrolling
     - Loop Inversion
  3. Evaluation of executable image statistics:
     - Code size
     - RO Data size
     - RW Data size
     - ZI Data size

     *Each of the above metrics can be crusial for an embedded application, as different data components are stored in different memory sections, and we have limited resources.*

  4. Evaluate the Execution Speed, using metrics:
     - Instruction count
     - S_Cycles
     - N_Cycles
     - Total_Cycles

     *The total performance of an embedded application is evaluated both with the speed of execution and the memory requirements.*

  All the metrics took place with the help of the above mentioned tools.

## Part 2
  From the previous part, we kept as the best performing executable, the one with full loop fusion applied.
  In this part it is required to propose a memory architecture that would improve execution efficiency. It should consider:
  - The different memory sections that data are saved
  - How often each of memory is accessed
  - The speed of each memory

   We proposed a base architecture, in order to compair the results and the final proposal was about a large but slow memory and 3 memory sections that sequentially get smaller but faster.
   The same evaluation as previous is applied, only this time, since there is an actual memory simulated with restrictions, we have the added metric of Wait States for speed.

## Part 3
  This part required to exploit the code structure and reuse data wherever this was possible, which is to say to use buffers. Essentially the goal is to keep a small amount of data that are used often, on a fast memory, and the rest on a slower but also inexpensive one.
  We actually applied buffers on the previous part, so this part is focused mainly on comparisons with the last implementation and an alternative. The final best performance approach is located in Part_3/full_loop_fusion_v1.c

There is also a small presentation (Greek) that sums up the process and the resuls. 

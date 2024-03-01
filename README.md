# um
creates a working implementation of a Turing universal machine emulator
# Architecture
Our modules include "operation", "address_space", and "um".

operations: implements the executions of reading and running the 14 machine 
                instructions
        -holds the 8 registers neccesary for our machine
        -only our run_program function can be 
        accessed outside of operation, all individual instruction
        functions are "private"

address_space: implements a virtual memory space. 
        -Responsible for hiding the data structures used from the client
        -Manages the mapping and unmapping of memory segments.

um: Holds the main function responsible for initializing the address space, 
        reading instructions, and calling the run_program function.
        
# 50 million instructions
How long does it take UM to execute 50 million instructions, and how do 
you know?

It should take about 7 seconds to execute 50 million simple instructions.
We know this because we wrote a test called 500_instruct which runs 500,000 
load value instructions. We timed this test and it took about 1 second to 
complete. So 1 * 5 million / 500,000 = 100. 

For more complex instructions we estimate it would be about 15 seconds based 
on our 500_instruct test and midmark which we know has 80 million (more 
complex) instructions.
        -with our program midmark took around 28 seconds to complete

# Unit Testing
   /*************************************************************************
   ** assume every instruction set after the halt unit-tests ends with a halt
           instruction
   ** all tests are diff tested against demo implementation
   *************************************************************************/

halt.um: a single halt instruction
        tests if the halt instruction stops a program
halt-verbose.um: a halt instruction followed by other
        tests that after a halt occurs no additional instructions are completed
load_val.um: loads values into all 8 registers
        tests if registers are properly set up and accessible
loadval_overwrite.um: loads values into r[1] 8 times
        tests that registers can be overwritten
output_test.um: loads one value (<255) into r1 and outputs it
        tests basic output functionailty
output_complex.um: tests outputing all ascii characters from 0-255 as one byte
        checks output cability for all ascii characters
add.um: basic addition of 6 and 48 then outputted
        check basic addition functionailty
add_complex.um:
        second addition test of 40 and 64
divide.um: basic division of 120 and 2, outputs and halts
    check basic division functionailty
mult.um: 11 * 10 = 110, outputs the result, then halts
        check basic mult functionailty
nand.um: ~(97 & 0x0F) = , outputs the result, then halts
        check basic nand functionailty
print-six.um: prints ascii 6, which is gotton from addition
        testing load value, addition and outputting the result   
cond_mov.um: calls cond mov when register c is 0
        checks functionality when contencts are zero
cond_movNot.um: calls cond mov wehn register != 0
        checks functionality when contents are not zero
input_test: calls input and reads input from input_test.0
        reads in one character and outputs it
input_complex: calls input then output 256 times to read in and out all
                 characters
        checks input cability for all ascii characters
loadProgt: loads a new program 
        ensure program counter points to new segment after new program is 
                loaded run with valgrind to ensure memory is not lost when 
                replacing the segment
        ensure new segement is not deleted, only copied
arbit_skip_test.um: Insert arbitrary data into the instruction stream (e.g.,
        a bunch of halt instructions) and precede that data with an 
        instruction that branches around it.
storeLoad.um: checks that a value can be stored and retreived from 
                address space
        stores a value in r[1][1] and then loads and outputs it
storeLoad_zero.um: checks that a value can be stored and retreived from 
                address space in the instruction memory
        stores a value at r[0][0] and then loads and outputs it
unmap_and_map.um: tests ability of address space to resuse idienftiers and
                to alloctaed memory quickly
        maps and unmaps in a loop of length 500
arith.um: 
        tests add, mult, and divide sequentially
500_000_instruct.um:
        tests loading value instruction 500,000 times
input_end_signal.um:
        it will output 'x' if 32-bit word is not 0, which it should
        not be because end of input has been signaled (which means
        that the word should be filled with all 1's as detailed in
        the spec)
load_map.um: calls load val and then maps segement
        tests that output is 0 
        loads value, then maps, and then loads
        segment which should be 0

# Hours Spent
Analyzing: 10 hours
Preparing: 5 hours
Solving: 37 hours
Total: 52 hours

/**************************************************************
 *
 *                     operation.c
 *
 *      Assignment: um
 *      Authors:  Kaelen Song (ksong05), Mia Taylor (mtaylo01) 
 *      Date:     11/20/2023
 *
 *      This file is responisble for reading and running all 
 *      14 machine instructions that are stored in the 
 *      address space struct.
 *
 **************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "address_space.h"
#include "bitpack.h"
#include <stdbool.h>
#include "uarray.h"
#include "mem.h"

/********** add ********
 * 
 * Adds the values in register b and c and stores the result in register a
 * 
 * Inputs:
 * 		uint32_t *a: a pointer to register a
 *  		uint32_t *b: a pointer to register b
 * 		uint32_t *c: a pointer to register c
 * Returns: -
 ************************/
static inline void add(uint32_t *a, uint32_t *b, uint32_t *c) 
{
        *a = (*b + *c) % (1UL << 32);
}

/********** mult ********
 * 
 * Muliplies the values in register b and c then stores the result in 
 * register a
 * 
 * Inputs:
 * 		uint32_t *a: a pointer to register a
 *  		uint32_t *b: a pointer to register b
 * 		uint32_t *c: a pointer to register c
 * Returns: -
 ************************/
static inline void mult(uint32_t *a, uint32_t *b, uint32_t *c) 
{
        *a = ((*b) * (*c)) % (1UL << 32);
}

/********** divide ********
 * 
 * Divides the values in register b and c then stores the result in register a
 * 
 * Inputs:
 * 		uint32_t *a: a pointer to register a
 *  		uint32_t *b: a pointer to register b
 * 		uint32_t *c: a pointer to register c
 * Returns: -
 ************************/
static inline void divide(uint32_t *a, uint32_t *b, uint32_t *c) 
{
        *a = ((*b) / (*c));
}

/********** bit_NAND ********
 * 
 * Performs bitwise and, and bitwise not on registers b and c, then stores the
 *      result in register a
 * 
 * Inputs:
 * 		uint32_t *a: a pointer to register a
 *  		uint32_t *b: a pointer to register b
 * 		uint32_t *c: a pointer to register c
 * Returns: -
 ************************/
static inline void bit_NAND(uint32_t *a, uint32_t *b, uint32_t *c) 
{
        *a = ~((*b) & (*c));
}

/********** load_val ********
 * 
 * loads the value in val into register a
 * 
 * Inputs:
 * 		uint32_t *a: a pointer to register a
 *  		unsigned val: the value to be loaded
 * Returns: -
 ************************/
static inline void load_val(uint32_t *a, unsigned val) {
        *a = val;
}

/********** output ********
 * 
 * outputs the value in register c as a character.
 * 
 * Inputs:
 * 		uint32_t *c: a pointer to register c
 * Returns: -
 * Notes:  Will fail if value is greater than 255
 ************************/
static inline void output(uint32_t *c) 
{
        if (*c > 255){
                abort();
        }
        putchar(*c);
}

/********** map_segment ********
 * 
 * maps a segement in the address space the size of the value in register c
 *      and stores the identifier in register b
 * 
 * Inputs:
 *              uint32_t *b: a pointer to register b
 * 		uint32_t *c: a pointer to register c
 *		S as: the adress space being used
 * Returns: -
 ************************/
static inline void map_segment(uint32_t *b, uint32_t *c, S as)
{
        assert(as != NULL);
        *b = map(*c, as);
}

/********** map_segment ********
 * 
 * unmaps the memory segement in the address space at the idenfier 
 *      corresponding to the value in register c
 * 
 * Inputs:
 * 		uint32_t *c: a pointer to register c
 *		S as: the adress space being used
 * Returns: -
 ************************/
static inline void unmap_segment(uint32_t *c, S as)
{
        assert(as != NULL);
        unmap(*c, as);
}


static inline void load_program(uint32_t *b, uint32_t *c, S as, 
                                int *prog_count)
{
        assert(as != NULL);
        *prog_count = *c - 1;
        load_prog(*b, as);
}

static inline void conditional_move(uint32_t *a, uint32_t *b, uint32_t *c) 
{
        if(*c != 0) {
                *a = *b;  
        }
}

/********* get_opcode ********
 * 
 * gets the 4 bit op_code from the 32-bit instruction
 * 
 * Inputs:
 * 		uint32_t instruction: the instruction from which the op_code 
 *              will be retrieved 
 *
 * Returns: the op_code as uint8_t
 ************************/
static inline uint8_t get_opcode(uint32_t instruction)
{
        return (uint8_t)Bitpack_getu(instruction, 4, 28);
}

/********** get_ra ********
 * 
 * gets register A from a 32-bit instruction
 * 
 * Inputs:
 * 		uint32_t instruction:  the instruction from which the register 
 *          will be retrieved 
 *
 * Returns: register a as uint8_t
 ************************/
static inline uint8_t get_ra(uint32_t instruction, bool loadval)
{
        if (loadval == true) {
                return (uint8_t)Bitpack_getu(instruction, 3, 25);
        } else {
                return (uint8_t)Bitpack_getu(instruction, 3, 6);
        }
}

/********** get_rb ********
 * 
 * gets register B from a 32-bit instruction
 * 
 * Inputs:
 * 		uint32_t instruction:  the instruction from which the register 
 *          will be retrieved 
 *
 * Returns: register b as uint8_t
 ************************/
static inline uint8_t get_rb(uint32_t instruction)
{
        return (uint8_t)Bitpack_getu(instruction, 3, 3);
}

/********** get_rc ********
 * 
 * gets register C from a 32-bit instruction
 * 
 * Inputs:
 * 		uint32_t instruction:  the instruction from which the register 
 *          will be retrieved 
 *
 * Returns: register c as uint8_t
 ************************/
static  inline uint8_t get_rc(uint32_t instruction)
{
        return (uint8_t)Bitpack_getu(instruction, 3, 0);
}

/********** get_val ********
 * 
 * gets value for the opcode instruction
 * 
 * Inputs: uint32_t instruction:  the instruction from which the value 
 *          will be retrieved 
 * 		
 *
 * Returns: the value as a uint32_t
 ************************/
static inline uint32_t get_val(uint32_t instruction)
{
        return Bitpack_getu(instruction, 25, 0);
}

/********** segmented_load ********
 * 
 * Loads the at address space location [b][c] into register a
 * 
 * Inputs:
 * 		uint32_t *a: a pointer to register a
 *  		uint32_t *b: a pointer to register b
 * 		uint32_t *c: a pointer to register c
 *		S as: the adress space being used
 * Returns: -
 ************************/
static inline void segmented_load(uint32_t *a, uint32_t *b, uint32_t *c, S as)
{
        *a = get(*b, *c, as);
}

/********** segmented_store ********
 * 
 * Stores the value in register c at address space location [a][b]
 * 
 * Inputs: 
 * 		uint32_t *a: a pointer to register a
 *  		uint32_t *b: a pointer to register b
 * 		uint32_t *c: a pointer to register c
 *		S as: the adress space being used
 *
 * Returns: -
************************/
static inline void segmented_store(uint32_t *a, uint32_t *b, uint32_t *c, S as)
{ 
        put(*a, *b, *c, as);
}

/********** input ********
 * 
 * reads a single char from stdin and stores it in register c
 * 
 * Inputs:
 * 		uint32_t *c: a pointer to register c
 *
 * Returns: -
 * Notes: if reaches EOF, c will be set to ~0
************************/
static inline void input(uint32_t *c)
{
        int p;
        if(( p = getchar()) != EOF) {
                *c = p;
        } else {
                *c = ~0;
        }
}

/********** run_program ********
 * 
 * Keeps track of the program counter and calls functions to 
 *     run each instruction
 * 
 * Inputs:
 *         Seq_t address_space: the virtual address space for the 
 *                              program to use
 * Returns: Nothing
 * Will CRE if address space sturct as is NULL
 ************************/
void run_program(AddressSpace_T as, int file_size) 
{
        assert(as != NULL);
        UArray_T my_register = UArray_new(8, sizeof(uint32_t));
        assert(my_register != NULL);
        int prog_count = 0;
        uint32_t instruct = get_instruction(prog_count, as);
        uint8_t opcode = get_opcode(instruct);
        
        while((opcode != 7)) {
                if (opcode > 13) {
                        abort();
                }
                bool is_loadVal = false;
                if (opcode == 13) {
                        is_loadVal = true;
                }
                uint8_t ra = get_ra(instruct, is_loadVal);
                uint8_t rb = get_rb(instruct);
                uint8_t rc = get_rc(instruct);
                switch (opcode) {
                case 0:
                        conditional_move((uint32_t*)UArray_at(my_register, ra),
                                        (uint32_t*)UArray_at(my_register, rb), 
                                        (uint32_t*)UArray_at(my_register, rc));
                        break;
                case 1:
                        segmented_load((uint32_t*)UArray_at(my_register, ra), 
                                        (uint32_t*)UArray_at(my_register, rb), 
                                        (uint32_t*)UArray_at(my_register, rc),
                                        as);
                        break;
                case 2:
                        segmented_store((uint32_t*)UArray_at(my_register, ra), 
                                        (uint32_t*)UArray_at(my_register, rb), 
                                        (uint32_t*)UArray_at(my_register, rc),
                                        as);
                        break;
                case 3:
                        add((uint32_t*)UArray_at(my_register, ra), 
                                        (uint32_t*)UArray_at(my_register, rb), 
                                        (uint32_t*)UArray_at(my_register, rc));
                        break;
                case 4:
                        mult((uint32_t*)UArray_at(my_register, ra), 
                                        (uint32_t*)UArray_at(my_register, rb), 
                                        (uint32_t*)UArray_at(my_register, rc));
                        break;
                case 5:
                        divide((uint32_t*)UArray_at(my_register, ra), 
                                        (uint32_t*)UArray_at(my_register, rb), 
                                        (uint32_t*)UArray_at(my_register, rc));
                        break;
                case 6:
                        bit_NAND((uint32_t*)UArray_at(my_register, ra), 
                                        (uint32_t*)UArray_at(my_register, rb), 
                                        (uint32_t*)UArray_at(my_register, rc));
                        break;
                case 8:
                        map_segment((uint32_t*)UArray_at(my_register, rb), 
                                        (uint32_t*)UArray_at(my_register, rc),
                                        as);
                        break; 
                case 9:
                        unmap_segment((uint32_t*)UArray_at(my_register, rc),
                                        as);
                        break;
                case 10:
                        output((uint32_t*)UArray_at(my_register, rc));
                        break;
                case 11:
                        input((uint32_t*)UArray_at(my_register, rc));
                        break;
                case 12:
                        load_program((uint32_t*)UArray_at(my_register, rb), 
                                        (uint32_t*)UArray_at(my_register, rc),
                                        as, &prog_count);
                        break;
                case 13:
                        load_val((uint32_t*)UArray_at(my_register, ra), 
                                        get_val(instruct));
                        break; 
                }
                prog_count++;
                instruct = get_instruction(prog_count, as);
                opcode = get_opcode(instruct);
        }
        UArray_free(&my_register);
        (void) file_size;
}
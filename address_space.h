/**************************************************************
 *
 *                     address_space.h
 *
 *      Assignment: um
 *      Authors:  Kaelen Song (ksong05), Mia Taylor (mtaylo01) 
 *      Date:     11/20/2023
 *
 *      Defines of functions for use in creating an address 
 *      space struct which stores memory segements of 
 *      instructions.
 *
 **************************************************************/
#ifndef ADDRESS_INCLUDED
#define ADDRESS_INCLUDED
#include <stdio.h>
#include <stdint.h>
#include "seq.h"
#include <stdbool.h>

#define S AddressSpace_T
typedef struct S *S;

S intialize_space(int file_size);
uint32_t get_instruction(uint32_t prog_count, S as);
void put_instruction(uint32_t word, uint32_t prog_count, S as);
uint32_t get(uint32_t segment, int offset, S as);
void put(uint32_t segment, int offset, uint32_t word, S as);
void free_address_space(S *as);
uint32_t map(int size, S as);
void unmap(uint32_t index, S as);
void load_prog(uint32_t index, S as);

#endif
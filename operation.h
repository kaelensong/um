/**************************************************************
 *
 *                     operation.h
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
#ifndef OPERATION_INCLUDED
#define OPERATION_INCLUDED
#include <stdio.h>
#include <stdint.h>
#include "address_space.h"
#include <stdlib.h>
#include <stdio.h>
#include "uarray.h"
#include "assert.h"
#include <stdbool.h>

void run_program(AddressSpace_T as, int file_size);

#endif
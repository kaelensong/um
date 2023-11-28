/**************************************************************
 *
 *                           um.c
 *
 *      Assignment: um
 *      Authors:  Kaelen Song (ksong05), Mia Taylor (mtaylo01) 
 *      Date:     11/20/2023
 *
 *      Holds the main function for our universal machine 
 *      implementation which brings all the modules together 
 *      (address_space, operation) to create a working 
 *      universal machine. 
 *
 **************************************************************/

#include "seq.h"
#include "assert.h"
#include "address_space.h"
#include "bitpack.h"
#include "operation.h"

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>


/********** open_or_fail ********
 * 
 * tests that the given file can be opened, 
 * then opens the file if possible
 * 
 * Inputs: 
 *         char *filename: the filename
 *         char *mode: the mode? ?  TODO
 *		
 * Return: the pointer to the opened file
 * 
 * Notes:
 *    Will CRE file cannot be opened
 ************************/
static FILE *open_or_fail(char *filename, char *mode) {
        FILE *fp = fopen(filename, mode);
        assert(fp != NULL);
        return fp;
}

/********** read_program ********
 * 
 * Uses I/O interface to read 32 bytes at a time, store them in a work and 
 * at the word to m[0][i] of address space
 * 
 * Inputs:
 * 	    seq_t address_space: the address space of the memory
 *
 * Returns: struct AddressSpace_T *
 ************************/
struct AddressSpace_T *read_program(uint32_t file_size, FILE *fp)
{
        AddressSpace_T address_space = intialize_space(file_size/4);
        assert(address_space != NULL);
        map(file_size/4, address_space);

        for (uint32_t k = 0; k < ((file_size)/4); k++){
                uint32_t word = 0;
                uint8_t c = 0;
                for(int i = 0; i < 4; i++) {
                        c = (uint8_t)getc(fp);
                        word = Bitpack_newu(word, 8, 24-(i*8), c);
                }
                put_instruction(word, k, address_space);
        }
        return address_space;
}

/********** main ********
 * 
 * Uses I/O interface to read 32 bytes at a time, store them in a word and 
 * at the word to m[0][i] of address space. Then calls operations.
 * 
 * Inputs:
 * 	    int argc: number of arguments provided on the command line
 *          char *argv[]: char array of arguments provided on command line
 *
 * Returns: 
 *     0 if successful
 * Notes:
 *      Will fail if argc is not 2 or if file cannot be opened
 ************************/
int main(int argc, char *argv[]) {
        if (argc > 2) {
                fprintf(stderr, "Usage: ./um [filename]\n");
                exit(EXIT_FAILURE);
        }
        struct stat st;
        stat(argv[1], &st);
        int file_size = st.st_size;
        FILE *our_file = open_or_fail(argv[1], "r");
        AddressSpace_T as = read_program(file_size, our_file);
        run_program(as, file_size);
        free_address_space(&as);      
        fclose(our_file);
        return 0;
}


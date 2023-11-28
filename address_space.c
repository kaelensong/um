/**************************************************************
 *
 *                     address_space.c
 *
 *      Assignment: um
 *      Authors:  Kaelen Song (ksong05), Mia Taylor (mtaylo01) 
 *      Date:     11/20/2023
 *
 *      Implementation of functions defined in address_space.h.
 *      Creates and implements our address space struct which
 *      holds memory segements of instructions.
 *
 **************************************************************/

#include "address_space.h"
#include <stdlib.h>
#include <stdio.h>
#include "uarray.h"
#include <stdbool.h>
#include "assert.h"
#include "mem.h"

#define S AddressSpace_T

struct S
{
        Seq_T address_seq;
        int curr_elems;
        Seq_T ids;
};

/********** intialize_space ********
 * 
 * Intializes the address space which will be used to store addresses made of 
 * memory segements that hold instructions
 * 
 * Inputs:
 * 	    int file_size: the size of the input file with the instructions
 *
 * Returns: the AddressSpace struct *
 * Expects: the address sequence and UArray of instructions not to be NULL
 ************************/
S intialize_space(int file_size)
{
        struct S *as = malloc(sizeof(struct S));
        assert(as != NULL);
        as->address_seq = Seq_new(1000);
        as->curr_elems = 1;
        as->ids = Seq_new(100);
        UArray_T instructions = UArray_new(file_size, sizeof(uint32_t));
        assert(as->address_seq != NULL);
        assert(instructions != NULL);
        Seq_addhi(as->address_seq, instructions);
        return as;
}

/********** get_instruction ********
 * 
 * gets the instruction at segment 0
 * 
 * Inputs:
 * 	    uint32_t prog_count: the program counter, 
 *              tells us where we are in the program
 *          S as: the AddressSpace struct
 *
 * Returns: the instruction at the given index
 * Will CRE if address space sturct as is NULL
 ************************/
uint32_t get_instruction(uint32_t prog_count, S as)
{
        assert(as != NULL);
        return get(0, prog_count, as);
}

/********** put_instruction ********
 * 
 * puts an instruction at given index in segment 0
 * 
 * Inputs:
 * 	    uint32_t word: the instruction to be put in the segement
 *          uint32_t index: the index where the instruction is to be put
 *          S as: the AddressSpace struct
 *
 * Returns: - 
 * Will CRE if address space sturct as is NULL
 ************************/
void put_instruction(uint32_t word, uint32_t index, S as)
{
        assert(as != NULL);
        put(0, index, word, as);
}

/********** get ********
 * 
 * gets the given word at a given offset, in  a given segment
 * 
 * Inputs:
 * 	    uint32_t segement: the segment from which the word is 
 *                              to be retrieved from
 *          uint32_t offset: the index where the word exists
 *          S as: the AddressSpace struct
 *
 * Returns: uint32_t of the word being retrieved
 * Notes:
 * Will CRE if address space sturct as is NULL
 * If the memory segment specifed is out of bounds or unmapped, 
 * the program will failthe desired instruction
 ************************/
uint32_t get(uint32_t segment, int offset, S as)
{
        assert(as != NULL);
        UArray_T curr = (UArray_T)Seq_get(as->address_seq, segment); 
        assert(curr != NULL);  
        uint32_t *word = (uint32_t*)UArray_at(curr, offset); 
        return *word;
}

/********** put ********
 * 
 * Updates the element at the specified offset in the specified segment of the 
 * given AddressSpace struct with the provided word.
 * 
 * Inputs:
 * 	   uint32_t segment: The index of the segment in the AddressSpace 
 *                               struct
 *         int offset: The offset within the segment where the word 
 *                               will be stored.
 *         uint32_t word: The word to be stored in the specified location.
 *         S as: The AddressSpace struct containing the segment to be updated.
 * Returns: Nothing
 * Notes:
 * Will CRE if address space sturct as is NULL
 * If the memory segment specified is out of bounds or unmapped, 
 * the program will fail
 ************************/
void put(uint32_t segment, int offset, uint32_t word, S as)
{
        assert(as != NULL);
        UArray_T curr = (UArray_T)Seq_get(as->address_seq, segment);
        uint32_t *elem = (uint32_t*)UArray_at(curr, offset);
        *elem = word;
}

/********** free_address_space ********
 * 
 * Frees the memory allocated for the AddressSpace structure and all its 
 *      mapped elements.
 * 
 * Inputs:
 * 	      S *as: A pointer to the AddressSpace struct to be freed.
 *
 * Returns: Nothing
 * Notes:      
 *     - Frees the memory of with each mapped element in the 
 *              AddressSpace struct
 *     - Frees the memory of with the sequence of mapped elements 
 *              and available IDs.
 *     - Frees the memory of with the AddressSpace struct
  * Will CRE if address space sturct as is NULL
 ************************/
void free_address_space(S *as)
{
        assert(*as != NULL);
        assert(as != NULL);
        for(int i = 0; i < Seq_length((*as)->address_seq); i++){
                if (Seq_get((*as)->address_seq, i) != NULL) {
                        UArray_T tofree = (UArray_T)Seq_put((*as)->address_seq,
                                                i, NULL);
                        UArray_free(&tofree);
                }
        }
        Seq_free(&(*as)->address_seq);
        Seq_free(&(*as)->ids);
        free(*as);
}

/********** map ********
 * 
 * Maps a new memory segment in the address space, returning the index at 
 * which the element is mapped. If available, reuses an ID from the sequence
 * of available IDs; otherwise, assigns a new ID.
 * 
 * Inputs:
 *         int size: The size of the new element to be mapped.
 *         S as: The AddressSpace struct in which the new element will 
 *                     be mapped.
 *
 * Returns:  uint32_t containing the index/indentfier at which the element
 *       is mapped.
 * Will CRE if address space sturct as is NULL
 ************************/
uint32_t map(int size, S as)
{
        assert(as != NULL);
        uint32_t id = 0;
        UArray_T seg = UArray_new(size, sizeof(uint32_t));
        assert(seg != NULL);
        for (int i = 0; i < UArray_length(seg); i++) {
                uint32_t *elem = (uint32_t*)UArray_at(seg, i);
                *elem = (uint32_t)0;
        }
        if (Seq_length(as->ids) > 0) {
                id = (uint32_t)(uintptr_t)Seq_remhi(as->ids);
                Seq_put(as->address_seq, id, seg);
                
        } else {
                Seq_addhi(as->address_seq, seg);
                id = as->curr_elems;
        }
        as->curr_elems = as->curr_elems + 1;
        return id; 
}

/********** unmap ********
 * 
 * Unmaps an memory segment at the specified index in the address space,
 * freeing the memory associated with the element.
 * 
 * Inputs:
 *         uint32_t index: The index of the element to be unmapped.
 *         S as: The AddressSpace struct containing the elements to 
 *              be unmapped.
 *
 * Returns: Nothing * Notes: if the memory segment being free'd is m[0] or is 
        already unmapped, 
 * the program will fail
 * - Will CRE if address space sturct as is NULL
 ************************/
void unmap(uint32_t index, S as)
{
        assert(as != NULL);
        if (index == 0) {
                abort();
        }
        UArray_T tofree = Seq_put(as->address_seq, index, NULL);
        UArray_free(&tofree); 
        as->curr_elems = as->curr_elems - 1;
        Seq_addhi(as->ids, (void *)(uintptr_t)index);
}

/********** load_prog ********
 * 
 * Loads memory segemnt at index into segment 0
 * 
 * Inputs:
 *         uint32_t index: The index of the new program
 *         S as: The AddressSpace struct where the new program 
 *              will be taken from
 *
 * Returns: Nothing 
 * Note: the memory segment being loaded cannot be of size 0
 *       Will CRE if address space sturct as is NULL
 ************************/
void load_prog(uint32_t index, S as) 
{
        assert(as != NULL);
        UArray_T tofree = (UArray_T)Seq_get(as->address_seq, 0);
        UArray_T toCopy = (UArray_T)Seq_get(as->address_seq, index);
        int length = UArray_length(toCopy); 
        UArray_T new_prog =  UArray_copy(toCopy, length);
        assert(new_prog != NULL);
        Seq_put(as->address_seq, 0, new_prog);
        UArray_free(&tofree);
}
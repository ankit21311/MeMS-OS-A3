/*

All the main functions with respect to the MeMS are inplemented here

read the function discription for more details



NOTE: DO NOT CHANGE THE NAME OR SIGNATURE OF FUNCTIONS ALREADY PROVIDED

you are only allowed to implement the functions

you can also make additional helper functions a you wish



REFER DOCUMENTATION FOR MORE DETAILS ON FUNCTIONS AND THEIR FUNCTIONALITY

*/



// add other headers as required

#include <stdio.h>

#include <stdlib.h>

#include <sys/mman.h>



/*

Use this macro where ever you need PAGE_SIZE.

As PAGESIZE can differ system to system we should have flexibility to modify this

macro to make the output of all system same and conduct a fair evaluation.

*/

#define PAGE_SIZE 4096



// Define a flag for anonymous mapping

#define MAP_ANONYMOUS 0x20



// global variables

struct chainNode *memoryChainHead;

size_t virtualAddressStart;



/* ------------------- mmap and munmap ------------------- */



void *allocate_memory_mmap(size_t size)

{ // Checking if size is valid

      if (size == 0) {

        printf("Invalid memory allocation size: 0\n");

        return NULL;

    }



    // Calculate aligned memory size to match page boundaries

    size_t alignedSize = (size + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);



    // Allocate aligned memory using mmap

    void *allocatedMemory = mmap(NULL, alignedSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (allocatedMemory == MAP_FAILED) {

        printf("Error allocating memory using mmap\n");

        return NULL;

    }



    // Return allocated memory pointer

    return allocatedMemory;

}




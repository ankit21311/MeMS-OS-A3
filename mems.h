/*
<<<<<<< HEAD



=======

>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389
All the main functions with respect to the MeMS are inplemented here
<<<<<<< HEAD



=======

>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389
read the function discription for more details

<<<<<<< HEAD






=======


>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389
NOTE: DO NOT CHANGE THE NAME OR SIGNATURE OF FUNCTIONS ALREADY PROVIDED
<<<<<<< HEAD



you are only allowed to implement the functions



=======

you are only allowed to implement the functions

>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389
you can also make additional helper functions a you wish

<<<<<<< HEAD






REFER DOCUMENTATION FOR MORE DETAILS ON FUNCTIONS AND THEIR FUNCTIONALITY



=======


REFER DOCUMENTATION FOR MORE DETAILS ON FUNCTIONS AND THEIR FUNCTIONALITY

>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389
*/
<<<<<<< HEAD







=======



>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389
// add other headers as required

#include <stdio.h>

<<<<<<< HEAD

#include <stdio.h>



#include <stdlib.h>



#include <sys/mman.h>







=======
#include <stdlib.h>

#include <sys/mman.h>



>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389
/*
<<<<<<< HEAD



=======

>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389
Use this macro where ever you need PAGE_SIZE.
<<<<<<< HEAD



As PAGESIZE can differ system to system we should have flexibility to modify this



macro to make the output of all system same and conduct a fair evaluation.



*/



#define PAGE_SIZE 4096
=======
>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389

As PAGESIZE can differ system to system we should have flexibility to modify this

<<<<<<< HEAD





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







void deallocate_memory_munmap(void *allocatedMemory, size_t size) {

    // Check if the allocated memory is valid

    if (!allocatedMemory) {

        printf("Invalid memory pointer provided for deallocation\n");

        return;

    }



    // Attempt to deallocate the memory using munmap
    int munmapResult ;
    munmapResult = munmap(allocatedMemory, size);



    // Handle potential errors during deallocation

    if (munmapResult == -1) {

        printf("Error while de-allocating memory using munmap\n");

        // Implement appropriate error handling mechanism (e.g., retry logic or notification)

    }



    // Inform the user about successful deallocation

    printf("Memory successfully deallocated using munmap !\n");

}









/* ------------------- Sub Chain Node ------------------- */







// subChain node structure of free list



typedef struct subChainNode



{



    struct subChainNode *next;



    struct subChainNode *prev;



    int type; // 0 for PROCESS and 1 for HOLE







    size_t v_ptr_start_index;



    size_t chunk_size;



} subChainNode;







// constructor



subChainNode* initializeSubChainNode(int type, size_t size, size_t v_ptr_start_index) {

    // Allocate memory for the subChainNode structure
        if (newNode == NULL) {
        // Handle memory allocation failure, e.g., return NULL or take other actions.
        
        return NULL;
    }

    subChainNode* newNode = (subChainNode*) allocate_memory_mmap(sizeof(subChainNode));



    // Set initial values for next and prev pointers to indicate no linked nodes

    newNode->next = NULL;

    newNode->prev = NULL;



    // Assign the provided type, chunk size, and virtual pointer start index

    newNode->type = type;

    newNode->chunk_size = size;

    newNode->v_ptr_start_index = v_ptr_start_index;



    // Return the newly created subChainNode

    return newNode;

}


/* ------------------- Main Chain Node ------------------- */







// mainChain node structure of free list



typedef struct chainNode



{



    struct chainNode *next;



    struct chainNode *prev;



    struct subChainNode *subChainHead;







    size_t segment_size;



    size_t v_ptr_start;



    void *p_ptr;



    void *v_ptr;







    void *store_v_to_free;



} chainNode;







chainNode *createChainNode(size_t segment_size)



{



    chainNode *newNode = (chainNode *)allocate_memory_mmap(sizeof(chainNode));



    newNode->next = NULL;



    newNode->prev = NULL;



    newNode->subChainHead = NULL;







    newNode->segment_size = segment_size;



    newNode->v_ptr_start = virtualAddressStart;



    virtualAddressStart += segment_size;







    // physical memory allocation



    newNode->p_ptr = allocate_memory_mmap(segment_size);







    // virtual memory allocation



    newNode->store_v_to_free = allocate_memory_mmap(sizeof(size_t) * segment_size);



    size_t *ptr = (size_t *)newNode->store_v_to_free;



    size_t i = 0;

    while (i < segment_size) {

    ptr[i] = (size_t)newNode->v_ptr_start + i;

    i++;

}



    newNode->v_ptr = (void *)(*ptr);



    return newNode;



}







/*



Initializes all the required parameters for the MeMS system. The main parameters to be initialized are:



1. the head of the free list i.e. the pointer that points to the head of the free list



2. the starting MeMS virtual address from which the heap in our MeMS virtual address space will start.



3. any other global variable that you want for the MeMS implementation can be initialized here.



Input Parameter: Nothing



Returns: Nothing
=======
macro to make the output of all system same and conduct a fair evaluation.

*/
>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389

<<<<<<< HEAD
=======
#define PAGE_SIZE 4096
>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389


<<<<<<< HEAD
*/
=======
>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389

<<<<<<< HEAD
=======
// Define a flag for anonymous mapping
>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389

<<<<<<< HEAD
=======
#define MAP_ANONYMOUS 0x20
>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389

<<<<<<< HEAD
void mems_init()
=======
>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389


<<<<<<< HEAD
=======
// global variables
>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389

<<<<<<< HEAD
{
=======
struct chainNode *memoryChainHead;
>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389

<<<<<<< HEAD
=======
size_t virtualAddressStart;
>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389


<<<<<<< HEAD
    memoryChainHead = NULL;
=======
>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389

<<<<<<< HEAD
=======
/* ------------------- mmap and munmap ------------------- */
>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389


<<<<<<< HEAD
    virtualAddressStart = 1000;
=======
>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389

<<<<<<< HEAD


}
=======
void *allocate_memory_mmap(size_t size)
>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389

{ // Checking if size is valid

      if (size == 0) {

<<<<<<< HEAD




/*



This function will be called at the end of the MeMS system and its main job is to unmap the



allocated memory using the munmap system call.



Input Parameter: Nothing



Returns: Nothing



*/



void mems_deallocate_chain() {

    chainNode *temp = memoryChainHead;



    while (temp != NULL) {

        subChainNode *subTemp = temp->subChainHead;



        while (subTemp != NULL) {

            subChainNode *toDelete = subTemp;

            subTemp = subTemp->next;



            toDelete->next = NULL;

            toDelete->prev = NULL;

            deallocate_memory_munmap(toDelete, sizeof(subChainNode));

        }



        chainNode *toDeleteMain = temp;

        temp = temp->next;



        deallocate_memory_munmap(toDeleteMain->p_ptr, toDeleteMain->segment_size);

        deallocate_memory_munmap(toDeleteMain->store_v_to_free, toDeleteMain->segment_size);

        deallocate_memory_munmap(toDeleteMain, sizeof(chainNode));

    }

}



void mems_finish() {
=======
        printf("Invalid memory allocation size: 0\n");
>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389

<<<<<<< HEAD
    mems_deallocate_chain();
=======
        return NULL;
>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389

<<<<<<< HEAD
    mems_init();
=======
    }
>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389

<<<<<<< HEAD
}
=======
>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389


<<<<<<< HEAD
=======
    // Calculate aligned memory size to match page boundaries
>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389

<<<<<<< HEAD
=======
    size_t alignedSize = (size + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389

<<<<<<< HEAD



/* ------------------- Helper function ------------------- */



void adjustSubChainNode(subChainNode *node, size_t size, size_t index) {

  if (node->chunk_size <= size) {

    return; // No need to split if the existing size is smaller or equal to the requested size

  }



  // Create a new sub-chain node to hold the remaining chunk

  subChainNode *newNode = initializeSubChainNode(1, node->chunk_size - size, index + 1);



  // Update pointers for insertion into the sub-chain

  newNode->next = node->next;

  if (node->next != NULL) {

    node->next->prev = newNode;

  }



  // Insert the new sub-chain node after the current node

  node->next = newNode;

  newNode->prev = node;



  // Update the chunk size of the original node

  node->chunk_size = size;

}
=======
>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389


<<<<<<< HEAD

/* --------------------------------------------------------- */







/*
=======
    // Allocate aligned memory using mmap
>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389

<<<<<<< HEAD


Allocates memory of the specified size by reusing a segment from the free list if



a sufficiently large segment is available.







Else, uses the mmap system call to allocate more memory on the heap and updates



the free list accordingly.
=======
    void *allocatedMemory = mmap(NULL, alignedSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389

<<<<<<< HEAD






Note that while mapping using mmap do not forget to reuse the unused space from mapping



by adding it to the free list.



Parameter: The size of the memory the user program wants



Returns: MeMS Virtual address (that is created by MeMS)
=======
    if (allocatedMemory == MAP_FAILED) {
>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389

<<<<<<< HEAD


*/



void *mems_malloc(size_t size)



{

 

    size_t newNodesize ;

    newNodesize = ((size + PAGE_SIZE - 1) / PAGE_SIZE) * PAGE_SIZE;



    // base case



    if (!memoryChainHead)



    {



        memoryChainHead = createChainNode(newNodesize);



        memoryChainHead->subChainHead = initializeSubChainNode(0, newNodesize, 0);



        adjustSubChainNode(memoryChainHead->subChainHead, size, size);



        return memoryChainHead->v_ptr;



    }







    // using free heap memory from MeMS



    chainNode *temp = memoryChainHead;



    chainNode *last = NULL;



    while (temp != NULL)



    {

	size_t index ;

        index = 0;



        subChainNode *subTemp = temp->subChainHead;



        while (subTemp != NULL)



        {



            // a sufficient hole has been found



            if (subTemp->type == 1 && subTemp->chunk_size >= size)



            {



                index +=  size;



                adjustSubChainNode(subTemp, size, index);







                subTemp->type = 0;



                return temp->v_ptr + subTemp->v_ptr_start_index;



            }



            index +=  subTemp->chunk_size;



            subTemp = subTemp->next;



        }



        last = temp;



        temp = temp->next;



    }







    // add a new Main Chain node at the end of free list



    chainNode *newNode = createChainNode(newNodesize);



    newNode->subChainHead = initializeSubChainNode(0, newNodesize, 0);



    adjustSubChainNode(newNode->subChainHead, size, size);







    last->next = newNode;



    newNode->prev = last;



    return newNode->v_ptr;



}
=======
        printf("Error allocating memory using mmap\n");
>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389

        return NULL;

<<<<<<< HEAD





/*



this function print the stats of the MeMS system like



1. How many pages are utilised by using the mems_malloc



2. how much memory is unused i.e. the memory that is in freelist and is not used.



3. It also prints details about each node in the main chain and each segment (PROCESS or HOLE) in the sub-chain.



Parameter: Nothing



Returns: Nothing but should print the necessary information on STDOUT



*/
=======
    }
>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389

<<<<<<< HEAD


void mems_print_stats()



{



    printf("----- MeMS SYSTEM STATS -----\n");

    

    size_t main_chain_length ;



    main_chain_length = 0;



    for (chainNode *temp = memoryChainHead; temp != NULL; temp = temp->next)



    {



        main_chain_length = main_chain_length + 1 ;



    }



    size_t pages_used ;



    size_t space_unused ;

    

    pages_used = 0;



    space_unused = 0;



    size_t *subchain_length_array = (size_t *)allocate_memory_mmap(sizeof(size_t) * main_chain_length);

    

    size_t subchain_num ;

    subchain_num = 0;



    for (chainNode *temp = memoryChainHead; temp != NULL; temp = temp->next)



    {



        printf("MAIN[%lu:%lu]-> ", temp->v_ptr_start, temp->v_ptr_start + temp->segment_size - 1);



        size_t subchain_length = 0;







        int d = temp->v_ptr_start;



        for (subChainNode *subTemp = temp->subChainHead; subTemp != NULL; subTemp = subTemp->next)



        {



            if (subTemp->type == 0)



            {



                printf("P[%lu:%lu] <-> ", d + subTemp->v_ptr_start_index, d + subTemp->v_ptr_start_index + subTemp->chunk_size - 1);



            }



            else



            {



                printf("H[%lu:%lu] <-> ", d + subTemp->v_ptr_start_index, d + subTemp->v_ptr_start_index + subTemp->chunk_size - 1);



                space_unused += subTemp->chunk_size;



            }



            subchain_length++;



        }



        printf(" NULL\n");







        pages_used = pages_used + temp->segment_size / PAGE_SIZE;



        subchain_length_array[subchain_num] = subchain_length;



        subchain_num = subchain_num + 1 ;



    }







    printf("Pages used: %lu\n", pages_used);



    printf("Space unused: %lu\n", space_unused);



    printf("Main Chain Length: %lu\n", main_chain_length);



    printf("Sub-chain Length array: [");



    int i = 0;

    while (i < main_chain_length) {

    printf("%lu, ", subchain_length_array[i]);

    i++;

	}



    printf("]\n");



    printf("-----------------------------\n");







    deallocate_memory_munmap(subchain_length_array, sizeof(size_t) * main_chain_length);



}
=======
>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389


<<<<<<< HEAD





/*



Returns the MeMS physical address mapped to ptr ( ptr is MeMS virtual address).



Parameter: MeMS Virtual address (that is created by MeMS)



Returns: MeMS physical address mapped to the passed ptr (MeMS virtual address).



*/



void *mems_get(void *v_ptr)



{



    for (chainNode *temp = memoryChainHead; temp != NULL; temp = temp->next)



    {



        if (temp->v_ptr <= v_ptr && v_ptr < temp->v_ptr + temp->segment_size)



        {



            return temp->p_ptr + (size_t)v_ptr - (size_t)temp->v_ptr;



        }



    }



}
=======
    // Return allocated memory pointer
>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389

    return allocatedMemory;

<<<<<<< HEAD





void fragment_memory(subChainNode *memoryChainHead)



{



    subChainNode *temp = memoryChainHead;



    while (temp != NULL)



    {



        if (temp->type == 1)



        {



            if (temp->prev != NULL && temp->prev->type == 1)



            {



                temp->prev->chunk_size = temp->prev->chunk_size + temp->chunk_size;



                temp->prev->next = temp->next;



                if (temp->next != NULL)



                {



                    temp->next->prev = temp->prev;



                }



                subChainNode *to_delete = temp;



                temp = temp->prev;







                to_delete->next = NULL;



                to_delete->prev = NULL;



                deallocate_memory_munmap(to_delete, sizeof(subChainNode));



            }



        }



        temp = temp->next;



    }



}







/*



this function free up the memory pointed by our virtual_address and add it to the free list



Parameter: MeMS Virtual address (that is created by MeMS)



Returns: nothing



*/
=======
}
>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389

<<<<<<< HEAD


void mems_free(void *v_ptr)



{



    for (chainNode *temp = memoryChainHead; temp != NULL; temp = temp->next)



    {



        if (temp->v_ptr <= v_ptr && v_ptr < temp->v_ptr + temp->segment_size)



        {

        

            size_t index ;



            index = (size_t)v_ptr - (size_t)temp->v_ptr;



            subChainNode *subTemp = temp->subChainHead;



            while (subTemp != NULL)



            {



                if (subTemp->v_ptr_start_index == index)



                {



                    subTemp->type = 1;



                    fragment_memory(temp->subChainHead);



                    break;



                }



                subTemp = subTemp->next;



            }



            break;



        }



    }



}
=======



>>>>>>> bb26461765c892fafc74e71cb3f65b83562fc389
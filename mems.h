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

}









/* ------------------- Sub Chain Node ------------------- */







// subChain node structure of free list



typedef struct subChainNode {

    struct subChainNode *next;

    struct subChainNode *prev;

    

    size_t chunk_size;

    int type; // 0 for PROCESS and 1 for HOLE

    size_t v_ptr_start_index;

    

} subChainNode;



// Constructor

subChainNode* initializeSubChainNode(int type, size_t size, size_t v_ptr_start_index) {

    // Allocate memory for the subChainNode structure

    subChainNode* newNode = (subChainNode*)allocate_memory_mmap(sizeof(subChainNode));



    // Initialize next and prev pointers to indicate no linked nodes

    newNode->next = NULL;

    newNode->prev = NULL;



    // Assign the provided type, chunk size, and virtual pointer start index

    newNode->chunk_size = size;

    newNode->type = type;

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

    

    void *v_ptr;



    void *p_ptr;



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



*/



void mems_init()



{



    memoryChainHead = NULL;



    virtualAddressStart = 1000;



}







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

    mems_deallocate_chain();

    mems_init();

}







/* ------------------- Helper function ------------------- */



void adjustSubChainNode(subChainNode *node, size_t size, size_t index) {

  if (node->chunk_size == size || node->chunk_size < size) {

    return; // No need to split if the existing size is smaller or equal to the requested size

  }



  // Create a new sub-chain node to hold the remaining chunk

  subChainNode *newNode = initializeSubChainNode(1, node->chunk_size - size, index + 1);



  // Update pointers for insertion into the sub-chain

  newNode->next = node->next;

  if (node != NULL && node->next != NULL) {

    node->next->prev = newNode;

}





  // Insert the new sub-chain node after the current node

  node->next = newNode;

  newNode->prev = node;



  // Update the chunk size of the original node

  node->chunk_size = size;

}



/* --------------------------------------------------------- */







/*



Allocates memory of the specified size by reusing a segment from the free list if



a sufficiently large segment is available.







Else, uses the mmap system call to allocate more memory on the heap and updates



the free list accordingly.







Note that while mapping using mmap do not forget to reuse the unused space from mapping



by adding it to the free list.



Parameter: The size of the memory the user program wants



Returns: MeMS Virtual address (that is created by MeMS)



*/



void *mems_malloc(size_t size)



{

 

    size_t newNodesize ;

    newNodesize = (size + PAGE_SIZE - 1) / PAGE_SIZE * PAGE_SIZE;



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



                // Calculate and return the final pointer value by adding the offsets

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







/*



this function print the stats of the MeMS system like



1. How many pages are utilised by using the mems_malloc



2. how much memory is unused i.e. the memory that is in freelist and is not used.



3. It also prints details about each node in the main chain and each segment (PROCESS or HOLE) in the sub-chain.



Parameter: Nothing



Returns: Nothing but should print the necessary information on STDOUT



*/



void mems_print_stats()



{



    printf("----- MeMS SYSTEM STATS -----\n");

    

    size_t main_chain_length ;



    main_chain_length = 0;



	chainNode *temp = memoryChainHead;

	while (temp != NULL) {

	    main_chain_length = main_chain_length + 1;

	    temp = temp->next;

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

        

        size_t subchain_length ;

        subchain_length = 0;





	int d ;

        d = temp->v_ptr_start;



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



            subchain_length = subchain_length + 1 ;



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







/*



Returns the MeMS physical address mapped to ptr ( ptr is MeMS virtual address).



Parameter: MeMS Virtual address (that is created by MeMS)



Returns: MeMS physical address mapped to the passed ptr (MeMS virtual address).



*/



void *mems_get(void *v_ptr)



{



	chainNode *temp = memoryChainHead;



	while (temp != NULL) {

	    if (temp->v_ptr <= v_ptr && v_ptr < temp->v_ptr + temp->segment_size) {

	        return temp->p_ptr + (size_t)v_ptr - (size_t)temp->v_ptr;

	    }

	    temp = temp->next;

	}

	}







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



void mems_free(void *v_ptr)



{



for (chainNode *currentNode = memoryChainHead; currentNode != NULL; currentNode = currentNode->next) {

    // Check if v_ptr is within the range of the current node

    if (v_ptr >= currentNode->v_ptr && v_ptr < currentNode->v_ptr + currentNode->segment_size) {

        // Calculate the index within the current node

        size_t index ;

        index = v_ptr - currentNode->v_ptr;



        // Traverse the sub-chain

for (subChainNode *subNode = currentNode->subChainHead; subNode != NULL; subNode = subNode->next) {

    if (subNode->v_ptr_start_index == index) {

        subNode->type = 1;

        fragment_memory(currentNode->subChainHead);

        return;

    }

    }



	}

}





}
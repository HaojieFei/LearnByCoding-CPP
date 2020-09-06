//
//  LeakDetector.cpp
//
//
#include <iostream>
#include <cstring>

// do not overload new in this file
#define __NEW_OVERLOAD_IMPLEMENTATION__
#include "LeakDetector.hpp"

typedef struct MemoryList {
    struct  MemoryList *next, *prev;
    size_t 	size;      
    bool    isArray;   
    char    *fileName;      
    unsigned int lineNum; 
} MemoryList;
static unsigned long memoryAllocated = 0;     // record used memory
static MemoryList root = {
    &root, &root, // the first element's prev pointer and next pointer point to itself
    0, false,             
    NULL, 0       
};

static LeakDetector exit_counter;
unsigned int LeakDetector::callCount = 0;

void* AllocateMemory(size_t size, bool isArray, char *fileName, unsigned lineNum) {

    size_t newSize = sizeof(MemoryList) + size;
    MemoryList *newElem = (MemoryList*)malloc(newSize);
    
    newElem->next = root.next;
    newElem->prev = &root;
    newElem->size = size;
    newElem->isArray = isArray;
    newElem->fileName = NULL;
    
    if (fileName) {
        newElem->fileName = (char *)malloc(strlen(fileName)+1);
        strcpy(newElem->fileName, fileName);
    }
    
    newElem->lineNum = lineNum;
    
    root.next->prev = newElem;
    root.next = newElem;
    
    
    memoryAllocated += size;
    //convert to char * so that pointer only moves 1 byte when pointer plus 1
    return (char*)newElem + sizeof(MemoryList);
}


void  DeleteMemory(void* ptr, bool isArray) {
    
    MemoryList *currentElem = (MemoryList *)((char *)ptr - sizeof(MemoryList));

    if (currentElem->isArray != isArray) return;

    currentElem->prev->next = currentElem->next;
    currentElem->next->prev = currentElem->prev;
    memoryAllocated -= currentElem->size;
    
    if (currentElem->fileName) free(currentElem->fileName);
    free(currentElem);
}

// overload new operator
void* operator new(size_t size) {
    return AllocateMemory(size, false, NULL, 0);
}
void* operator new[](size_t size) {
    return AllocateMemory(size, true, NULL, 0);
}
void* operator new(size_t size, char *fileName, unsigned int lineNum) {
    return AllocateMemory(size, false, fileName, lineNum);
}
void* operator new[](size_t size, char *fileName, unsigned int lineNum) {
    return AllocateMemory(size, true, fileName, lineNum);
}
// overload delete operator
void operator delete(void *ptr) noexcept {
    DeleteMemory(ptr, false);
}
void operator delete[](void *ptr) noexcept {
    DeleteMemory(ptr, true);
}

unsigned int LeakDetector::Detect(void) noexcept {
    unsigned int count = 0;
    // iterate the chain and check whether root's next pointer points to itself
    MemoryList *ptr = root.next;
    while (ptr && ptr != &root)
    {
        
        if(ptr->isArray)
            std::cout << "Leak[] ";
        else
            std::cout << "Leak   ";
        std::cout << ptr << " Size " << ptr->size;
        if (ptr->fileName)
            std::cout << " (locate in " << ptr->fileName << " line " << ptr->lineNum << ")";
        else
            std::cout << " (can't find position)";
        std::cout << std::endl;
    
        ++count;
        ptr = ptr->next;
    }

    if (count)
        std::cout << "Total " << count << " leaks, size "<< memoryAllocated << " bytes." << std::endl;
    return count;
}



#include "memory_management.h"

head_t *firstBlock, *lastBlock;

//-----------------------------------------------------------------------
head_t* getNextBlock(int size)
{
	head_t *currentBlock = firstBlock;
	while(currentBlock) {
		if (currentBlock->std.free == 1 && currentBlock->std.size > size)
			return currentBlock;
		currentBlock = currentBlock->std.nextBlock;
	}
	return NULL;
}
//-----------------------------------------------------------------------
void split_header(head_t* header,int size){

  
  head_t* smallHead  = ((void*)(header + 1) + size); 

  
  smallHead->std.free = 1;
  smallHead->std.nextBlock = header->std.nextBlock;
  smallHead->std.data = (void*)(smallHead + 1) ;
  smallHead->std.size = header->std.size - size - sizeof(smallHead);

  header->std.nextBlock = smallHead;
  header->std.size = size;
  
}

//-----------------------------------------------------------------------
void* createNewBlock(head_t* header,int size){

  int total_size;
	void *block;

	total_size = sizeof(head_t) + size;
	block = sbrk(total_size);

	header = block;
	header->std.size = size;
	header->std.free = 0;
	header->std.nextBlock = NULL;
  

	if (!firstBlock){
		firstBlock = header;
  }
	if (lastBlock){
		lastBlock->std.nextBlock = header;
  }
	lastBlock = header;
	header->std.data = (void*)(header+1);
	return ((header->std.data));
}

//-----------------------------------------------------------------------
void* _malloc(int size)
{

	head_t *header;
	if (!size)
		return NULL;
	
	header = getNextBlock(size);

	if (header) {
		header->std.free = 0;
    split_header(header,size);
		return (void*)(header + 1);
	}

  return createNewBlock(header,size);

}
//-----------------------------------------------------------------------
void merge(){
  
  head_t *curr = firstBlock;
  head_t *nextCurr;
  while (curr)
  {

    if (curr->std.free == 1){
      
      nextCurr = curr->std.nextBlock;
      if(nextCurr->std.free == 1){
        
       curr->std.nextBlock = nextCurr->std.nextBlock;
       curr->std.size = nextCurr->std.size + curr->std.size;        
      }
      
    } 
    curr = curr->std.nextBlock;
    
  }
}

//-----------------------------------------------------------------------
void _free(void *block)
{
	head_t *curr = firstBlock;
  while (curr)
  {

    if (curr->std.data == block){
      curr->std.free = 1;
      
      merge();
      return;
    } else{
      curr = curr->std.nextBlock;
    }
  }
}

//------------------------------------------------------------------------------

void dumpBlock(void *block){


  head_t* currentHead = firstBlock;

  //search the list for a free bloc
  while (currentHead) {
    int size = currentHead->std.size;
    int isfree = currentHead->std.free;
    head_t* nextnode = currentHead->std.nextBlock;
    void* vnode = currentHead->std.data;
    
    printf("%p\n",currentHead);
    printf("%d\n",size);
    printf("%d\n",isfree);
    printf("%p\n",nextnode);
    printf("%p\n",vnode);
    printf("block %p\n\n\n",block);

    currentHead = currentHead->std.nextBlock;

  }
  //if first block has been used then create new block

}
//-----------------------------------------------------------------------
int tests()
{
  int* ptr;
  ptr = _malloc(15);
  
  int* ptr2;
  ptr2 = _malloc(15);
  
  int* ptr3;
  ptr3 = _malloc(50);
  
  int* ptr4;
  ptr4 = _malloc(15);
  
  dumpBock(0);
  
  printf("lets free\n");
  _free(ptr3);
  _free(ptr2);
  _free(ptr4);

  dumpBock(ptr3);

  printf("add more\n\n");
  int* ptr5;
  int* ptr6;
  int* ptr7;
  ptr5 = _malloc(7);
  ptr6 = _malloc(20);
  ptr7 = _malloc(13);
  dumpBlock(0);
  
  exit(0);
}
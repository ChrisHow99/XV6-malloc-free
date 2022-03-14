
#include "kernel/types.h"
#include "user/user.h"
#define NULL 0

typedef char ALIGN[16];

union header {
	struct {
		int size;
		int free;
		union header* nextBlock;
        void* data;
	} std;
	ALIGN stub;
};

typedef union header head_t;

void* _malloc(int size);

void _free(void *ptr);
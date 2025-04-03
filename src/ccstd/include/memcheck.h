/*
// let's keep this as a prototype
// the idea is that you use these in place of all frees and mallocs to 
// help you get a better idea of what you
size_t malloccount = 0;
size_t freecount = 0;

size_t getfreecount()
{
    return freecount;
}

size_t getmalloccount()
{
    return malloccount;
}

// todo: add callee information
void free(void* addr)
{
    printf("freeing 0x%p\n", addr);
    free(addr);
    freecount++;
}

// todo: add callee information
void* malloc(size_t size)
{
    void* ret;

    malloccount++;
    ret = malloc(size);

    printf("allocating 0x%p\n", ret);

    return ret;
}
// size_t getfreecount();
// size_t getmalloccount();
*/
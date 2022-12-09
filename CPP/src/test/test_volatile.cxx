#define MEMORY_ADDRESS 0x100000U

int main(void) {
    int value = 0;
    volatile int *ptr = (int *) MEMORY_ADDRESS;
    while (1) {
        value = *ptr;
        if (value) break;
    }

    return 0;
}
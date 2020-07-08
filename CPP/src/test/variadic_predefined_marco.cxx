#include <stdio.h>
#ifndef log_err
#define log_err(M, ...) fprintf(stdout, "[ERROR] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define log_warn(M, ...) fprintf(stdout, "[Warning] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define log_info(M, ...) fprintf(stdout, "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif


int main() {
	log_err("HELLO-1");
	log_err("HELLO-%s", "two");
	return 0;
}

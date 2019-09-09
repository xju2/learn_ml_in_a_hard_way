#include <stdio.h>
#include <string>

int main ()
{
	char buffer [50];
	int n, a=5, b=3;
	n=sprintf (buffer, "%d plus %d is %d", a, b, a+b);
	printf ("[%s] is a string %d chars long\n",buffer,n);
	printf("total character %d\n", n);
	for(int i=0; i < n; ++i){
		printf("%c", buffer[i]);
	}
	printf("\n");
	std::string p1(buffer);

	a=7, b=17;
	n=sprintf (buffer, "%d plus %d is %d", a, b, a+b);
	const char* const_buff = (const char*) buffer;
	for(int i=0; i < n; ++i){
		printf("%c", const_buff[i]);
	}
	printf("\n");
	std::string p2(buffer);
	printf("%s\n", p1.c_str());
	printf("%s\n", p2.c_str());
	return 0;
}

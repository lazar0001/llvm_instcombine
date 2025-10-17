#include <stdio.h>
#include <stdbool.h>

int main(){
	int a = 5;
	int b = 10;
	int c = 5;
	
	int d = 20;
	
	//optimizacija 1
	int sum1 = 5 + b; 
	d = 10 + d;
	
	//optimizacija 2
	bool cmp1 = (a > b);
	bool cmp11 = (a < b);
	
	//optimizacija 3
	int mul1 = a + a;
	
	//optimizacija 4
	int mul2 = a * 4096;
	
}

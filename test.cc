
int hello(){

	long long intb;
	int * inta;
	int * intc;
	int (*fun)(int a,int b);
	
	((int (*)(int,int))intb)(3,4);
	return fun(1,2);

}

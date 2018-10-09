/* ======================================================================
	FileName: pointerExample.c
   Purpose: To understand how a pointer works.
	----------------------------------------------------------------------
	How to Run: 
	A. At a terminal, type
		$ gcc pointerExample.c -o pointerExample 
		$ ./pointerExample
   ----------------------------------------------------------------------
	Notes:
	A. It is better for us trying to understand each case in ascending 
		order.
	----------------------------------------------------------------------
   Sangeeta Biswas, Ph.D.
	Assistant Professor,
	Dept. of CSE, University of Rajshahi,
	Rajshahi-6205, Bangladesh.
	sangeeta.cse.ru@gmail.com / sangeeta.cse@ru.ac.bd
	----------------------------------------------------------------------
	15/11/2017
	======================================================================
*/

#include<stdio.h>	// For printf()

typedef struct MixData{
	float x;
	int y;
	char c;
} MD;

void case1();
void case2();
void specificTypePointer(float *b);
void case3();
void case4();
void case5();
void voidPointer(void *vp);
void case6();
float *sumVoidFloat(void *vp);
void case7();
void *sumVoidVoid(void *vp);
void case8();

int main(){
	printf("Case-1\n***************\n");
	case1();

	printf("\nCase-2\n***************\n");
	case2();

	printf("\nCase-3\n***************\n");
	case3();

	printf("\nCase-4\n***************\n");
	case4();

	printf("\nCase-5\n***************\n");
	case5();

	printf("\nCase-6\n***************\n");
	case6();

	printf("\nCase-7\n***************\n");
	case7();

	printf("\nCase-8\n***************\n");
	case8();

	return 0;
}

void case8(){
	float x = 10.89;
	/*
		NULL pointer is a pointer which points nothing. 
		NULL is a special reserved value of a pointer. 
		When a pointer has the null value then it points
		nowhere.
	*/
	float *p;

	p = &x;
	printf("Before using NULL pointer (p)\n");
	printf("******************************\n");
	printf("Address\t\tValue\t\tPoint-To:\n");
	printf("-------------------------------------------\n");
	printf("%p\t %p\t %f\n\n", &p, p, *p);

	p = NULL;
	printf("After using NULL pointer (p)\n");
	printf("******************************\n");
	printf("Address\t\tValue\t\tPoint-To:\n");
	printf("-------------------------------------------\n");
	printf("%p\t %p\n\n", &p, p);

	/* 
		We will get segmentation fault, if we try to
		print *p. 
	*/
	// printf("%f",*p);
}

void case7(){
	float x[3];
	float *result;
	
	x[0] = 10.89;
	x[1] = 102.389;
	x[2] = 53.53;

	/* 
		Like variable pointer, we need to cast the return 
		type here. 
	*/
   result = (float *) sumVoidVoid(x);
	printf("Sum of elements of x: %f\n\n", *result);
}

void *sumVoidVoid(void *vp){
	/*
		A static local variable is a variable that has 
		scope (i.e., extent) only inside the function
		but has lifetime during the entire execution
		period of the program. 
		
		Like a global variable, a static local variable 
		is also stored in the data section of a process. 
		On the other hand, a (normal) local variable is 
		stored in the stack section of a process. Therefore, 
		its storage space is deallocated when the function 
		is not executing. As a result we cannot return the 
		pointer of any (normal) local variable. But we can 
		return the pointer of a static local variable. 

		So, whenever we need to send any variable from a 
		function by reference we need to declare it as a 
		static variable.			
	*/
	static float sum;
	float *p;

	p = (float *)vp;	
	sum = *p + *(p+1) + *(p+2);

	return &sum;
}


void case6(){
	float x[3];
	float *result;
	
	x[0] = 10.89;
	x[1] = 102.389;
	x[2] = 53.53;
   result = sumVoidFloat(x);
	printf("Sum of elements of x: %f\n\n", *result);
}

float *sumVoidFloat(void *vp){
	static float sum;
	float *p;

	p = (float *)vp;	
	sum = *p + *(p+1) + *(p+2);

	return &sum;
}

void case5(){
	float x[3];
	
	x[0] = 10.89;
	x[1] = 102.389;
	x[2] = 53.53;
   voidPointer(x);
}

void voidPointer(void *vp){
	float *p;

	/* p will hold the base address of a floating array which 
		is pointed by the temporary &void pointer, vp.
	*/
	p = (float *)vp; 

	printf("Value of x:\n------------\n");
	printf("x[0]: %f, x[1]: %f, x[2]: %f\n", *p, *(p+1), *(p+2));
}

void case4(){
	float x;
	int y;
 	MD mD;

	/* 
		Interesting  but dangerous VOID pointer. VOID pointer
		does not point to any specific type of data, instead
		it can point to any type of data by the help of casting.

		At the beginning VOID pointer points to nothing. After 
		that it acts slightly similar to function overloading/
		operator overloading in C++. We will have to cast it to 
		the appropriate type by prefixing it with a type name in 
		parentheses. 

		VOID pointer is also known as generic pointer.
	*/
	void *p;	
	
	x = 10.89;
	p = &x;	// Now p points to a float.
	printf("x: %f\n\n",*(float *)p);

	y = 10;
	p = &y;	// Now p points to an int.
	printf("y: %d\n\n", *(int *)p);

	mD.x = 20.33;
	mD.y = 45;
	mD.c = 'y';
	p = &mD;	// Now p points to a structure.
	printf("md.x: %f, md.y: %d, md.c: %c\n\n", (*(MD *)p).x, (*(MD *)p).y, (*(MD *)p).c);
}

void case3(){
	float x[3], y[3];
	float *p;
	
	x[0] = 10.89;
	x[1] = 102.389;
	x[2] = 53.53;

   printf("Value of x:\n------------\n");
	printf("x[0]: %f, x[1]: %f, x[2]: %f\n", x[0], x[1], x[2]);

	/*	
		The array varible's name holds the address to the first 
		element in the array, which can be assigned to another
		pointer variable. Pointer variable can be accessesed like
		an array using index number.
	*/
	p = x;
	printf("*x: %f, *(x+1): %f, *(x+2): %f\n", *x, *(x+1), *(x+2));
	printf("*p: %f, *(p+1): %f, *(p+2): %f\n", *p, *(p+1), *(p+2));
	printf("p[0]: %f, p[1]: %f, p[2]: %f\n", p[0], p[1], p[2]);

	/*	
		Even though the array varible's name holds the address to 
		the first element in the array, it acts like as a constant 
		pointer. Therefore, it cannot be changed or assigned to point
		to another array variable. As a result we will get error 
		message, if we try to do the following things.
	*/
	//y = x;
	//y = p; 
	//printf("Value of y:\n------------\n");
	//printf("y[0]: %f, y[1]: %f, y[2]: %f\n\n", y[0], y[1], y[2]);
}

void case2(){
	float x;
	
	x = 10.89;
   specificTypePointer(&x);
}

void specificTypePointer(float *b){
	float y;
	float *p;

	/*	
		Here b is a temporary pointer variable which points
		to variable x of function case2(). Therefore, we
		need to use b instead of &x used in case1(), during 
		assigning its value to another pointer variable.
	*/
	p = b;

	/* 
		Like case1(), we need to use * before the pointer
		variable b before using the value of the variable
		it points to (i.e., x).
	*/
	y = *b;

	printf("Value of:\n------------\n");
	printf("x: %f, y: %f, p: %p\n\n", *b, y, p);
	printf("Address of:\n------------\n");
	printf("x: %p, y: %p, p: %p\n\n", b, &y, &p);
	printf("'p' points to the variable whose value: %f\n", *p);
}

void case1(){
	float x, y; // Two float type variables.
	float *p; 	// A pointer variable which points to a float.

	
	x = 10.89; // Assigning value to x.

	/* 
		p holds the address of x, i.e., it points 
		to x. 
	*/
	p = &x;

	/* 
		*p is equivalent to x. Therefore, we can 
		access the value of x by *p. 
	*/
	y = *p; // It's equivalent to y = x.

	printf("Value of:\n------------\n");
	printf("x: %f, y: %f, p: %p\n\n", x, y, p);
	printf("Address of:\n------------\n");
	printf("x: %p, y: %p, p: %p\n\n", &x, &y, &p);
	printf("'p' points to the variable whose value: %f\n", *p);
}

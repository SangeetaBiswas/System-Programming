#include <sched.h>
#include <stdio.h>

int main() {
	int a;

	a = sched_getscheduler(0);
	printf("%d\n", a);

	return 0;
}

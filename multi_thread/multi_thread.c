#include <pthread.h>
#include <stdio.h>
#include <string.h>

int count;

pthread_mutex_t count_lock;

void * foo (void * arg) {
	pthread_mutex_lock (&count_lock);
	for (int i = 0; i < 100000; i++) {
		count++;
	}
	pthread_mutex_unlock (&count_lock);
}

int main () {
	pthread_t thread1;
	pthread_t thread2;
	char a[100];
	memset (a, 0, sizeof (a));
	pthread_create (&thread1, NULL, foo, "thread11");
	pthread_create (&thread2, NULL, foo, "thread22");
	pthread_join (thread1, NULL);
	pthread_join (thread2, NULL);
	pthread_mutex_destroy (&count_lock);
	printf ("%d\n", count);
	return 0;
}

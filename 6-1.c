#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4

typedef struct {
    const int* array;
    int start;
    int end;
    int target;
    pthread_mutex_t* mutex;
    int* found_index;
} ThreadData;

void* search_thread(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    for (int i = data->start; i < data->end; ++i) {
        if (data->array[i] == data->target) {
            pthread_mutex_lock(data->mutex);
            if (*data->found_index == -1 || i < *data->found_index) {
                *data->found_index = i;
            }
            pthread_mutex_unlock(data->mutex);
            break;
        }
    }
    return NULL;
}

int main() {
    int array[] = {2, 3, 5, 7, 3, 13, 17, 19, 23, 3};
    int size = sizeof(array)/sizeof(array[0]);
    int target;
    int found_index = -1;
    pthread_mutex_t mutex;
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    printf("Введи число: ");
    scanf("%d", &target);

    pthread_mutex_init(&mutex, NULL);
    
    int part_size = size / NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_data[i].array = array;
        thread_data[i].start = i * part_size;
        thread_data[i].end = (i == NUM_THREADS-1) ? size : (i+1)*part_size;
        thread_data[i].target = target;
        thread_data[i].mutex = &mutex;
        thread_data[i].found_index = &found_index;
        pthread_create(&threads[i], NULL, search_thread, &thread_data[i]);
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    if (found_index != -1) {
        printf("Первое число находится на позиции: %d\n", found_index);
    } else {
        printf("Такого числа в списке нет, ты ж сам писал их в программе\n");
    }

    return 0;
}

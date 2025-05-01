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
    int** indices;
    int* count;
    int* capacity;
} ThreadData;

void add_index(ThreadData* data, int index) {
    pthread_mutex_lock(data->mutex);
    
    if (*data->count >= *data->capacity) {
        *data->capacity *= 2;
        *data->indices = realloc(*data->indices, *data->capacity * sizeof(int));
    }
    
    (*data->indices)[(*data->count)++] = index;
    pthread_mutex_unlock(data->mutex);
}

void* search_all_thread(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    for (int i = data->start; i < data->end; ++i) {
        if (data->array[i] == data->target) {
            add_index(data, i);
        }
    }
    return NULL;
}

int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

int main() {
    int array[] = {3, 1, 4, 3, 5, 3, 9, 3, 7, 3};
    int size = sizeof(array)/sizeof(array[0]);
    int target;
    pthread_mutex_t mutex;
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    
    int* indices = NULL;
    int count = 0;
    int capacity = 10;

    printf("Введи число: ");
    scanf("%d", &target);

    indices = malloc(capacity * sizeof(int));
    pthread_mutex_init(&mutex, NULL);

    int part_size = size / NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_data[i].array = array;
        thread_data[i].start = i * part_size;
        thread_data[i].end = (i == NUM_THREADS-1) ? size : (i+1)*part_size;
        thread_data[i].target = target;
        thread_data[i].mutex = &mutex;
        thread_data[i].indices = &indices;
        thread_data[i].count = &count;
        thread_data[i].capacity = &capacity;
        pthread_create(&threads[i], NULL, search_all_thread, &thread_data[i]);
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    qsort(indices, count, sizeof(int), compare);

    if (count > 0) {
        printf("Найденные позиции числа: ");
        for (int i = 0; i < count; ++i) {
            printf("%d ", indices[i]);
        }
        printf("\n");
    } else {
        printf("Такого числа в списке нет, ты ж  сам их писал\n");
    }

    free(indices);
    return 0;
}

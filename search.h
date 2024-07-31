#ifndef SEARCH_H
#define SEARCH_H

struct hashmap* training (int bucket_INPUT);
void stop_word (struct hashmap *hm);
double rank (struct hashmap* hm, char* word);
void print_map( struct hashmap * hm);
void swap(double* x, double* y);
void BubbleSort(double documents[]);
void append (double documents[],int arr[]);
void swapint(int* x, int* y);
void read_query(struct hashmap* hm);


#endif

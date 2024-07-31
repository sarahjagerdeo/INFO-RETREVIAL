#ifndef HASHMAP_H
#define HASHMAP_H

// struct llnode {
//         char* word;
//         char* document_id;
//         int num_occurrences;
//         struct llnode* next;
// };

struct hashmap {
        struct wordNode** map;
        int num_buckets;
        int num_elements;
        
};


struct wordNode {
    char * word; //Word that we get from file 
    int DocFreq; //Doc freq Number of documents
    struct wordNode* next;
    struct docNode* head;
};

struct docNode{
    char * doc; //Document ID
    int TermFreq; //total frequency of word in file
    struct docNode * next;
};

struct hashmap* hm_create(int num_buckets);
void hash_table_insert(struct hashmap* hm, char* word, char* doc, int TermFreq);
void hm_remove(struct hashmap* hm, char* word);
void hm_destroy(struct hashmap* hm);
int hash(struct hashmap* hm, char* word);
double hm_get_tf(struct hashmap *hm, char* word, char * doc);
double hm_get_df (struct hashmap* hm, char* word);
int hash_code(struct hashmap* hm, char* word);

#endif
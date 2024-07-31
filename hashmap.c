#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"


struct hashmap* hm_create(int num_buckets){

    //create the hashmap
    //Allocate memory for the hashmap
   struct hashmap* hm = calloc(sizeof(struct hashmap), 1);
   //create memory for hashmap
   hm->num_buckets = num_buckets; 
   //Allocate memory for the nodes
   hm->map = calloc(sizeof(struct wordNode*),hm->num_buckets);
   for(int i = 0; i<hm->num_buckets; i++) {
       hm->map[i] = NULL;
   }
   hm->num_elements = 0;
   return hm;

}

double hm_get_df (struct hashmap* hm, char* word){
    //Similar to check function
    int hash1 = hash_code(hm,word);
    //We need a node to iterate with 
    struct wordNode * cur;
    cur = hm->map[hash1];
    while(cur){
        //We check to see if the word were pointing at is the right word
         if(strcmp(cur->word, word) == 0){
             //if so then we get the doc freq which is associated with the word node 
             //then we cast it to a double to make it easier to calculate the idf
            return (double)cur->DocFreq; //return value associated with key which is the num or occurences
        }
        //else if we continue and keep going
        cur = cur->next;
    }
    //return 0 if not there
    return 0;

}

double hm_get_tf(struct hashmap *hm, char* word, char * doc){
    //Similar to check function
    int hash1 = hash_code(hm,word);
    //We need a node to iterate with 
    struct wordNode * cur;
    cur = hm->map[hash1];
    
    while(cur != NULL){
         //We check to see if the word were pointing at is the right word
        if(strcmp(cur->word,word) == 0){
            //Create a node to go through thr doc node 
            struct docNode * temp4 = cur->head;
            //Go through the the head of the linked list 
            //The doc node
            while(temp4){
                //Check to see if the head node 
                //For the doc linked list is the doc we are pointing at
                if(strcmp(temp4->doc, doc) == 0){
                    //If so then we return the temp frequency 
                    return (double)temp4->TermFreq;
                }
                //Move on to the next
                temp4= temp4->next;
            }  
        }
        //Move on to the next
      cur = cur->next;
    }

    return 0;
}

void hm_remove(struct hashmap* hm, char* word){
    //Compute Hash code and then create a null pointer to go through 
    int hash1 = hash_code(hm,word);
    struct wordNode * cur = hm->map[hash1];
    if (cur == NULL){
        //obviously if there is no word node theres nothing to remove so u return out
        return;
    } 
    //Otherwise theres a case where you have a word node that points to a word
    //If so we want to create null pointers to go through the list
        if(strcmp(cur->word,word) == 0){
            hm->map[hash1] = cur->next;
            //HAVE A DOC NODE
            struct docNode * current_doc1 = cur->head;
            //Have a temp doc node that we can put the current doc node in 
            //So we cant lose it
            struct docNode * temp;  
            //Go through the doc nodes in the doc list and free everything starting up    
            while(current_doc1){
                temp = current_doc1;
                //Have to free the nodes
                current_doc1 = temp->next;
                free(temp->doc);
                free(temp);
            }
        
          //Have to free the word nodes
          free(cur->word);
          free(cur);

          return;

        }

       while(cur){
           //We check to see if the next node were looking at is the word were loooking for
            if(strcmp(cur->next->word,word) == 0){
            struct docNode * current_doc3 = cur->next->head;
            struct docNode * temp;  
            struct wordNode * temp_word = cur->next;    
            //We have a bunch of nodes to iterate with 
            //Because we have to free these nodes and remove the stuff from the hasmap  
            while(current_doc3){
                temp = current_doc3;
                //We free the doc nodes
                current_doc3 = temp->next;
                free(temp->doc);
                free(temp);
            }
        //Then we free the next word nodes
            cur->next = temp_word->next;
            free(temp_word->word);
            free(temp_word);
            //hm->num_elements--;

            return;
            }
            cur = cur->next;
        }

}
    
void hash_table_insert(struct hashmap* hm, char* word, char* doc, int TermFreq){
    //Calculate Hash code for the map

    int hash1 = hash_code(hm,word); 
    
    //If there is literally nothing we need:
    //a word node
    //doc node
    //We need to create a wordnode that points to a word and the doc frequency 
    //We need a doc node that points to a document and the term frequency 
    if(hm->map[hash1] == NULL){
        //Create space for the word node 
        hm->map[hash1] = calloc(sizeof(struct wordNode),1);
        hm->map[hash1]->word = strdup(word);
        hm->map[hash1]->DocFreq = 1;
        //Create space for the docnode
        hm->map[hash1]->head = calloc(sizeof(struct docNode),1);
        hm->map[hash1]->next = NULL;
        hm->map[hash1]->head->doc = strdup(doc);
        hm->map[hash1]->head->TermFreq = TermFreq;
        hm->map[hash1]->head->next = NULL;    
        hm->num_elements++; 

        //Return out
        return;
    }

    //MOVING ONTO THE SECOND CASE
    //The case where we have a word node and we traverse through it

    //Create a pointer to traverse through with 
    struct wordNode * cur;
    cur = hm->map[hash1];

    while(cur) {
    //Case where there is a word node and we see if the word node
    //Is pointing to the word that we want to point to 
        if(strcmp(cur->word, word) == 0) {
        //scond case if the above is true we have a doc node to traverse through with
        //to see if there is doc node stuff there
            struct docNode * doc_Node = cur->head;
            while(doc_Node) {
            //while going through the doc list we see if there is a doc node pointing to a doc
            //if there is we just increase the term frequency and move on
                if(strcmp(doc_Node->doc, doc) == 0) {
                    doc_Node->TermFreq += TermFreq;
                    return;
                // else we move on to the next and create everything that we need for the doc list
                }else if(doc_Node->next == NULL){
                    cur->DocFreq++; //increase doc freq
                    //allocate space for the docnode that we are creating now 
                    doc_Node->next = calloc(sizeof(struct docNode),1);
                    doc_Node->next->doc = strdup(doc);
                    doc_Node->next->TermFreq = TermFreq;
                    //End of the doc node list 
                    doc_Node->next->next = NULL;
                    return;
                }else {
                    //increment to the next so we can continue
                    doc_Node = doc_Node->next;
                }
            }
        //Last case wher the cur->next is null where we will need a word node and doc node to continue with 
        //Else if for (strcmp(cur->word,word) == 0)
        }else if(cur->next == NULL) {
            //allocate space for the wordnode that we are creating now 
            cur->next = calloc(sizeof(struct wordNode),1);
            cur->next->word = strdup(word);
            cur->next->DocFreq = 1;
            //Create space for the docnode
            cur->next->head = calloc(sizeof(struct docNode),1);
            cur->next->next = NULL;
            cur->next->head->doc = strdup(doc);
            cur->next->head->TermFreq = TermFreq;
            cur->next->head->next = NULL;
        
            hm->num_elements++;
            //Return out 
            return;
        //ELSE EVERYTHING ELSE ISNT HAPPENING THEN WE MOVE ON
        }else {
            cur = cur->next;
        }
    }
}

void hm_destroy(struct hashmap* hm){

 //Go through all the buckets
    for(int i =0; i < hm->num_buckets; i++){
        struct wordNode * temporary = hm->map[i];
        //Have a word node u can iterate through with 
        while(temporary != NULL){
            //Create a word node that we can store temporary in so we can free it
            struct wordNode * temp_delete = temporary;
            temporary = temporary->next;
            //Call remove so we can free evrything and start destroying the hashmap
            hm_remove(hm,temp_delete->word);
        }
       
    }
    //Fully free all nodes that make the hashmap
    //Destroy the whole hashmap
  free(hm->map);
  free(hm);

}

int hash_code(struct hashmap* hm, char* word){
    
    //Have all variables for the function
    int ASCII_word = 0;
    int sum =0 ;
    int answer =0 ;
    int length1 = strlen(word);
    //sum up the values 
    for(int i =0; i < length1; i ++){
        ASCII_word = word[i];
        sum += ASCII_word;
    }
    //Mod funtion to calcualte hash 
      answer = sum % hm->num_buckets; 


    return answer;

}


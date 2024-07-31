#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"
#include "search.h"



int main(void){

  //Get the bucket input
  int bucket_INPUT;
  printf("How many buckets?:\n");
  scanf("%d", &bucket_INPUT);
  //Create the hashmap with the bucket input from the user
  struct hashmap* hm = training(bucket_INPUT);
  //Call read query
  read_query(hm);
  
 } 






 struct hashmap* training (int bucket_INPUT){

    char car[1021];

    //we have an array to put what the file reads into the hashmap
    //Create a hashmap with what the user puts in

    struct hashmap * hm = hm_create(bucket_INPUT);

    //Read in all the three  files that are in the directory
    //After you read it in call put and put the stuff into the hashmap 

    FILE * file_ptr;

     if((file_ptr = fopen("p5docs/D1.txt","r")) == NULL){
        fprintf(stderr,"ERROR: Cannot open file: \n");
        return NULL;
    }
    while (fscanf(file_ptr, "%s", car) == 1){
        hash_table_insert(hm,car,"D1.txt",1);
    }

    fclose(file_ptr);


    FILE * file_ptr2;
    
    if((file_ptr2= fopen("p5docs/D2.txt","r")) == NULL){
        fprintf(stderr,"ERROR: Cannot open file: \n");
        return NULL;
    }


    while (fscanf(file_ptr2, "%s", car) == 1){
        hash_table_insert(hm,car,"D2.txt",1);
    }

    fclose(file_ptr2);

    
    FILE * file_ptr3;
 
    if((file_ptr3= fopen("p5docs/D3.txt","r")) == NULL){
        fprintf(stderr,"ERROR: Cannot open file: \n");
        return NULL;
    }

    while (fscanf(file_ptr3, "%s", car) == 1){
        hash_table_insert(hm,car,"D3.txt",1);
        

    }  

    fclose(file_ptr3);

    //After all that call stop words to stop the words and return a hashmap

   stop_word(hm);

    return hm;

}


 void read_query(struct hashmap* hm){

  //Below the variables are char arrays for what the user inputs 
  char str [1021]; //to hold what the user puts in
  double scores[3]; //to hold the ranking scores
  int arr1[3]; //int array to sort the document ids with the scores
  char arr_docID [3][50]= {"D1.txt",
                              "D2.txt",
                              "D3.txt"};

    //A char array is above to have all my document ids so i can pass them into functions


  while(1){
  //Have a while loop going so we can keep asking the user for stuff
  //Get string from user
  //Prompt the user
  printf("Enter search string or X to exit: \n");
  printf("NOTE: The system can only take 1021 characters! \n");
  scanf(" %[^\n]%*c", str);

  //Error checking to see if the user puts in X
  //This is case sensitive and will only work if the person 
  //Puts in an uppercase letter
      if(*str == 'X'){
       exit(1);   
    }
 
  
  char* token; //A char* to hold the token that we are reading that the user inputs 
  //so we can split
  double idfcase; //idf score varible
  double tf_score; //tf score variable
  char* copy2; //a copy so we dont lose the string
  char* copy;  //another copy to not lose the string and since we use strdup we can free it 

  for(int i=0; i<3; i++){
      //We need s for loop so we can hash these scores for each document 
      scores[i] = 0;
      //Create space for the copy
      copy = malloc(strlen(str) +1); 
       strcpy(copy,str);
       //Make a copy to keep in the token
       token = strtok(copy," ");

       //While that token node is not null we can get the ranking scores
       //We can use token to split the strings by the spaces when the user inputs them 
       while(token != NULL){
       copy2 = strdup(token);
       idfcase = rank(hm,token);
       tf_score = hm_get_tf(hm,token,arr_docID[i]); 
       //The scores are now put into an array
       scores[i] += (tf_score*idfcase);
       token = strtok(NULL, " ");
       //We have to free the memory allocated or else we have leaks
       free(copy2);
       } 
    strcpy(str,copy);
    free(copy);
    } 

    //Call append and destory the map
    append(scores,arr1); 
    hm_destroy(hm);
   }

 }
 
 void stop_word (struct hashmap *hm){

    //The stop word function will only stop a word when the idf is equal to zero
    double idf_calc;
    for(int i =0; i < hm->num_buckets; i++){
        struct wordNode* cur;
        cur = hm->map[i];
        while(cur != NULL){
            //We calculate the score so that we can check and see if the idf is 0 
            //If it is we just remove the word with the remove function
            idf_calc = rank(hm, cur->word);
            struct wordNode* temp = cur;
            cur = cur->next;
            if(idf_calc ==0){
                hm_remove(hm,temp->word);
            }
        }
    }
}


double rank (struct hashmap* hm, char* word){
    //idf = log(n/df)
    //if df is 0
    //then log(n/(df+1))
    //double log(double x)
    double idf;
    double df = hm_get_df(hm,word);

    if(df == 0){
        df = df+1;
    }
    idf = log(3/df);
    //Return calculated idf
    return idf;

}

void swap(double* x, double* y){
    //swap for bubble sort
    double temporary_forSort = *x;
    *x = *y;
    *y = temporary_forSort;

}

void swapint (int* x, int* y){
    //Swap for the int array that sorts documents
    int temporary_forSort1 = *x;
    *x = *y;
    *y = temporary_forSort1;

}

void append (double documents[], int arr[]){

  //ALL of bubble sort to sort the scores in the document from highest to lowest 
    for(int i=0; i<2; i++){
        for(int j =0; j < 3-i-1; j++){
            if(documents[j] < documents[j+1]){
                swap(&documents[j], &documents[j+1]);
                swapint(&arr[j], &arr[j+1]);
            }
        }
    }


    //APPEND INTO FILE

    for(int i=0; i < 3; i++){
        arr[i] = i;
    }
    //Open the scores file
    FILE * scoresfile;
    scoresfile = fopen("search_scores.txt","a+");

    for(int i=0; i<3; i++){

      //this is to rank the documents with their scores
      //The index of the array holds a document with their scores
      //When "computer architecture gw" is the string thats in then we have
      //d1,d2,d3
        if(arr[i] == 1){
            fprintf(scoresfile,"%s : %f\n","D2.txt",documents[i]);
            if(documents[i] > 0){
              printf("%s \n","D2.txt");
            }
        } else if(arr[i] == 2){
            fprintf(scoresfile,"%s : %f\n","D3.txt",documents[i]);
            if(documents[i] > 0){
              printf("%s \n","D3.txt");
            }
        } else {
            fprintf(scoresfile,"%s : %f\n","D1.txt",documents[i]);
            if(documents[i] > 0){
              printf("%s \n","D1.txt");
            }
        }

    }
    //print the new lines
    fprintf(scoresfile,"\n");

    fclose(scoresfile);


 }

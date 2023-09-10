#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

#define Hash_size 1000000        // defining a compfortable hash size
#define SIGN_DEL (c*)(0xFFFFFFFFFFFFFFFUL)     // a deleted node marker which helps to indicate that a node was deleted and no need to keep looking in the hash function
                                                           // it's not a valid adderss

///////////////////////////////////////////////////////////////////////////

typedef struct Course    // struct defention for the main course node
{
    char co[50];
    int ho;
    char code[50];
    char dep[50];
    char topics[300];
}c;

///////////////////////////////////////////////////////////////////////////   declaring all global variables needed such that tha hash tables and the collision counts

int curr_size1 = 23, curr_size2 = 23;
c* hashtable[Hash_size];   c* doublehashtable[Hash_size];   int collision_count1 = 0;   int collision_count2 = 0;  int cnt = 0;

///////////////////////////////////////////////////////////////////////////

int hash_func(char *key, int sz){    // since we are dealing with a string key, we need to come up with a function to produce different indcies for each key
   unsigned long hashkey = 5381;
    int c;
    while (c = *key++){
        hashkey = ((hashkey << 5) + hashkey) + c;
         for (int i = 0; i < (signed)strlen(key); i++) {
               hashkey+=key[i];
         }
}
        hashkey%=sz;

return hashkey;
}
///////////////////////////////////////////////////////////////////////////

double loadfactor1(){
  double sum = (double)cnt/curr_size1;
  return sum;
}

///////////////////////////////////////////////////////////////////////////

double loadfactor2(){
  return ((double)cnt)/curr_size2;
}

///////////////////////////////////////////////////////////////////////////
    void  hash_table_set(){    // a function to set the hash table pointers to null
         for (int i = 0; i < curr_size1; i++) {
               hashtable[i] = NULL;
        }

  }
  ///////////////////////////////////////////////////////////////////////////
  void  hash_table_set2(){    // a function to set all the pointers in the double hash table into null
       for (int i = 0; i < curr_size1; i++) {
             doublehashtable[i] = NULL;
      }

}

///////////////////////////////////////////////////////////////////////////

void table_print() {    // a function to print both tables
  printf("=========================================================\n");

     printf("\t(OPEN ADDRESSING METHOD HASH TABLE)\n\n");
    for (int i = 0; i < curr_size1; i++) {
        if (hashtable[i] == NULL) {     // if the location is empty
            printf("\t%d -->\t------\n", i);
        }
        else if(hashtable[i] == SIGN_DEL){   // if the location is for a deleted node
             printf("\t%d -->\t------(DELETED)\n", i);
           }
           else {
             printf("\t%d -->\t%s\n", i, hashtable[i]->co);
           }
  }
printf("-----------------------------------------------\n");
printf("\t(DOUBLE HASHING METHOD HASH TABLE)\n\n");    // printing the other table in the same function
  for (int j = 0; j < curr_size1; j++) {
    if (doublehashtable[j] == NULL) {
        printf("\t%d -->\t------\n", j);
    }
       else if(doublehashtable[j] == SIGN_DEL){
         printf("\t%d -->\t------(DELETED)\n", j);
       }
       else {
         printf("\t%d -->\t%s\n", j, doublehashtable[j]->co);
       }
  }

  printf("=========================================================\n");

}

///////////////////////////////////////////////////////////////////////////

int prime(int n){
    for(int i = 2; i * i <= n; i++){
        if(n%i == 0){
            return 0;
        }
    }
    return 1;
}
void rehash1(){
    int new_size = curr_size1*2;
    while(!prime(new_size)){
        new_size++;
    }
    c* new_table[new_size];
    for(int i = 0; i < new_size; i++){
        new_table[i] = NULL;
    }
    for(int i = 0; i < curr_size1; i++){
        if(hashtable[i] == NULL || hashtable[i] == SIGN_DEL){
            continue;
        }
        int l, k, flag = 0;
        char *token = malloc(sizeof(hashtable[i]->co)*10);
        strcpy(token, hashtable[i]->co);
        l = hash_func(token, new_size);   // getting the returning index of the first hash function(linear)
        for (int j = 0; j < new_size; j++, flag++) {
           k = (j+l)%new_size;   // this is the formula for linear probing to solve collisions
           // if (j >= 1) {
           //     flag = 1;
           // }
              if (new_table[k] == NULL || new_table[k] == SIGN_DEL) {
                new_table[k] = malloc(sizeof(c));
                strcpy(new_table[k]->co,hashtable[i]->co);    // assigning all read values from the file into a course node as described below
                new_table[k]->ho = hashtable[i]->ho;
                strcpy(new_table[k]->code,hashtable[i]->code);
                strcpy(new_table[k]->dep,hashtable[i]->dep);
                strcpy(new_table[k]->topics,hashtable[i]->topics);
                break;
              }
              if (flag >= 1) {  // flag for collision count purpose
                     collision_count1++;
                   }
        }
    }
    curr_size1 = new_size;
    for(int i = 0; i < curr_size1; i++){
        if(new_table[i] == NULL){
            hashtable[i] = NULL;
            continue;
        }
        hashtable[i] = malloc(sizeof(c));
        strcpy(hashtable[i]->co,new_table[i]->co);    // assigning all read values from the file into a course node as described below
        hashtable[i]->ho = new_table[i]->ho;
        strcpy(hashtable[i]->code,new_table[i]->code);
        strcpy(hashtable[i]->dep,new_table[i]->dep);
        strcpy(hashtable[i]->topics,new_table[i]->topics);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

c* search_course(char *course){   // a afunction to search and lookup for a node in the open addressing hash table
    int indx = hash_func(course, curr_size1), k;

    indx = hash_func(course, curr_size1);
    for (int j = 0; j < curr_size1; j++) {
     k = (j+indx);
     if (hashtable[k] == NULL) {
          return NULL;
     }
     if (hashtable[k] == SIGN_DEL) {
          continue;
     }
     if (strcmp(hashtable[k]->co, course) == 0 && hashtable[k] != NULL ) {
        printf("Course is found at index %d\n", k);
         return hashtable[k];
     }
    }

    return NULL;

}
///////////////////////////////////////////////////////////////////////////

int insert_course(c* course){           // inserting and dealing with collision using linar probing
   int i,k, flag = 0;
           if (course == NULL) {
                  return 0;
               }
               i = hash_func(course->co, curr_size1);   // getting the returning index of the first hash function(linear)
        for (int j = 0; j < curr_size1; j++, flag++) {
               k = (j+i)%curr_size1;   // this is the formula for linear probing to solve collisions

                  if (hashtable[k] == NULL || hashtable[k] == SIGN_DEL) {
                    hashtable[k] = course;
                //    printf("%f\n", loadfactor1());
                    if (loadfactor1() > 0.55) {
                        rehash1();
                    }
                    return 1;
                  }
                  if (flag >= 1) {  // flag for collision count purpose
                         collision_count1++;
                       }
               }

  //printf("%f\n", loadfactor1());

          return 0;
        }


///////////////////////////////////////////////////////////////////////////

c* delete_course(char *course){   // a function to delete a course from the open addressing hash table
  c* temp;  int k;
  int indx = hash_func(course, curr_size1);       //  calling the hash function to get the random index
  for (int j = 0; j < curr_size1; j++) {
     k = (j+indx)%curr_size1;   // the known formula
  if (hashtable[k] == NULL) {
          return NULL;
     }
     if (hashtable[k] == SIGN_DEL) {
          continue;
     }
  if (strcmp(hashtable[k]->co, course) == 0 && hashtable[k] != NULL ) {
    // temp = hashtable[indx];
        hashtable[k] = SIGN_DEL;
      //   return temp;
  }
}
   return NULL;
}
///////////////////////////////////////////////////////////////////////////  signitures for double hashing functions
void  doublehash_table_set();
int partofdoublehash(char* key, int sz);                                         //    to make things clear i splitted functions for both methods
int double_hash(char *key);                                              //    the above the main are for open addressing(linear probling) and the below are for double hashing
int insert_course2(c* course);
c* search_course2(char *course);
c* delete_course2(char *course);
void saveout(FILE *output);
void rehash2();
int hash2(char* key, int sz);
int hash1(char* x);
///////////////////////////////////////////////////////////////////////////



int main(){


  char topics[300]; char code[30];  char dep[20];  char co[75], q, n[10], m[10];      // declaring all needed variables
  int ho,opt;

// hash_table_set();
// hash_table_set2();


//========================================================  // input file reading
FILE* courses = fopen("offered_courses.txt", "r");

c *temp[13];

for (int o = 0; o < 13; o++) {
   temp[o] = (c*)malloc(sizeof(c));
}
int i = 0;
do{
      fscanf(courses,"%[^:]:%d#%[^#]#%[^/]/%[^\n]\n",co,&ho,code,dep,topics);
  //    printf("%s\n", co);
        strcpy(temp[i]->co,co);    // assigning all read values from the file into a course node as described below
        temp[i]->ho = ho;
        strcpy(temp[i]->code,code);
        strcpy(temp[i]->dep,dep);
        strcpy(temp[i]->topics,topics);
        insert_course2(temp[i]);
        insert_course(temp[i]);
       cnt++;  // counter for nodes in the table
       // if (loadfactor1() > 0.5) {
       //     rehash1();
       // }
     i++;
} while (!feof(courses));   // keep reading untill the end of file is reached

     printf("\nData have been read and the hash tables have been created\n");
     fclose(courses);
//========================================================
// printf("\n\n\n");
// rehash1();
// rehash2();

do{
  printf("=========================================================\n");       // main menu
printf("\nPlease Choose an Option:\n\n");
printf("\n1. Print hashed tables (including empty spots).\n"
             "2. Print out table size and the load factor.\n"
                "3. Print out the used hash functions.\n"
           "4. Insert a new record to hash table (insertion will be done on both hash tables).\n"
                 "5. Search for a specific word (specify which table to search in).\n"
         "6. Delete a specific record (from both tables).\n"
                 "7. Compare between the two methods in terms of number of collisions occurred.\n"
                     "8. Save hash table back to a file named saved_courses.txt (of the double hashing)\n9. Exit\n");
                     printf("=========================================================\n");


                     scanf("%d", &opt);

                     switch (opt) {

                     case 1:{   // case one for option 1 which is printing the table

                             table_print();
                            // printf("%s\n", hashtable[1]->co);
                            // printf("%s\n", hashtable[0]->co);
                            // for (int g = 0; g < curr_size1; g++) {
                            //       printf("%s\n", doublehashtable[g]->co);
                            // }
                       break;
                     }
                     case 2:{ // case two for option 2 which is printing the table size and load factor
                           double uu = ((double)cnt/(double)curr_size1);
                                 printf("\nTable Size for both hash tables: %d\nLoad Factor for both hash tables: %f\n", curr_size1, uu);
                                 // if (uu > 0.6) {
                                 //   rehash1();
                                 // }

                              //   printf("\n%f  %f", loadfactor1(), loadfactor2());
                       break;
                     }

                     case 3:{ // case three for option 3 which is printing the used hash functions

                                     printf("The used hash functions are: -hash 1 which is for linear probing in the open addressing method\nHere's it's equation: ");
                                     printf("\n"

                                   "    hashkey = ((hashkey << 5) + hashkey) + c;\n"
                                    "for (int i = 0; i < (signed)strlen(key); i++) {\n"
                                      "    hashkey+=key[i];\n"
                                "    } "
                                "   }  "
                              "     \nhashkey%=curr_size;\n");
                              printf("-----------------------------------------------\n");

                                 printf("-hash 2 which is a part of the double hash function and it's a simple hash function\nHere it's equation: \n"
                                 "int l = strlen(key);\n"
                                   "l*=4324;\n"
                                 "l%=curr_size; return l;\n");
printf("-----------------------------------------------\n");
      printf("and finally, the double hash function which it's equation is:\n"

                           "k = hash1(course->co) + j*hash2(course->co)modHash_size;\n");



                       break;
                     }

                     case 4:{        // case four for option 4 which is inserting a course into the tables
                           c* temp2  = NULL;  int t, pmt, p;
                                        temp2 = (c*)malloc(sizeof(c));
                                      printf("Enter information:\n");
                                      printf("Course Name: ");   scanf("\n%[^\n]s", temp2->co);
                                      printf("\n");

                                      printf("Credit Hourse: ");
                                      t = scanf("%d", &temp2->ho);
                                     while(t!=1){
                                        while((pmt=getchar()) != EOF && pmt != '\n');
                                        printf("Invalid input... please try again: ");
                                        t = scanf("%d", &p);
                                      }     printf("\n");

                                      printf("Course Code: "); scanf("\n%[^\n]s", temp2->code); printf("\n");
                                      printf("Departement Name: "); scanf("\n%[^\n]s", temp2->dep); printf("\n");
                                      printf("Course Topics: "); scanf("\n%[^\n]s", temp2->topics); printf("\n");
                                      cnt++;

                                      insert_course(temp2);
                                      insert_course2(temp2);

                                      printf("Insertion is done on both tables\n");



                       break;
                     }

                     case 5:{        // case five for option 5 which is searching for a course in the needed table
                        int t, num, pmt;  char v[30];

                                      printf("=Enter 1 to search in the open addressing hash table\n=Enter 2 to search in the double hash table: \n");
                                      t = scanf("%d", &num);
                                      while(t!=1 || num<1 || num>2){
                                        while((pmt=getchar()) != EOF && pmt != '\n');
                                        printf("Invalid input... please try again: ");
                                        t = scanf("%d", &num);
                                      }     printf("\n");
                                      printf("-----------------------------------------------\n");

                                      if (num == 1) {
                                           printf("Enter Course Name: ");
                                           scanf("\n%[^\n]s", v);
                                        c* temp5 = search_course(v);
                                        if (temp5 == NULL) {
                                          printf("Course Not Found!\n");
                                        }
                                           else{ //printf("Course is Found\nHere is the info: ");
                                             printf("%s   %d   %s   %s   %s \n", temp5->co, temp5->ho, temp5->code, temp5->dep, temp5->topics);

                                         }
                                      } if (num == 2) {
                                        printf("Enter Course Name: ");
                                        scanf("\n%[^\n]s", v);
                                     c* temp55 = search_course2(v);
                                     if (temp55 == NULL) {
                                       printf("Course Not Found!\n");
                                     }
                                        else{
                                        //   printf("Course is Found\nHere is the info: ");
                                          printf("%s   %d   %s   %s   %s \n", temp55->co, temp55->ho, temp55->code, temp55->dep, temp55->topics);
                                      }
                                      }

                       break;
                     }

                     case 6:{     // case six for option 6 which is deleting course in both tables
                       char x[40];
                       printf("Enter a course name to be delete: ");
                         scanf("\n%[^\n]s", x);
                       delete_course(x);
                       delete_course2(x);
                       printf("\nCourse Delete Successfuly\n");

                       break;
                     }

                     case 7:{   // case seven for option 7 which is printing number of collisions occured in each table
                                     printf("Collisions for Open Addressing hash table: %d\nCollisions for double hashing hash table: %d\n", collision_count1, collision_count2);
                       break;
                     }

                     case 8:{   // case eight for option 8 which is printing the table in a file

                      FILE* output = fopen("saved_courses.txt", "w");
                             saveout(output);
                            fclose(output);


                       break;
                     }


                     }
}while(opt!=9);






  return 0;
}

//////////////////-------------------------------///////////////////////////----------------------///////////////////////



int partofdoublehash(char* key, int sz){   // a made up hash function for the double
   int sum = 1;

   for (int i = 0; i < curr_size2; i++) {
       sum+=key[i];
   }
     sum%=7;
  return (sum%sz);
}

////////////////////////////////////////////////////////////////////////

int hash2(char* key, int sz){  // i used the above hash and the hash used in linear probling to produce the double hash function
       int l = strlen(key);
       l*=4324;
       l%=sz;
         return l;
//return (key%curr_size2);

}

int hash1(char* x){    // a function to hash a course into the table using the formula below
  int prime=7;
  return (prime - (strlen(x) %prime));
}

////////////////////////////////////////////////////////////////////////


int insert_course2(c* course){           // inserting and dealing with collision using double hashing
   int i,k, flag = 0;
           if (course == NULL) {
                  return 0;
               }
          //      i = partofdoublehash(course->co, curr_size2);
        for (int j = 0; j < curr_size2; j++, flag++) {
               k = (hash2(course->co,curr_size2) + j*hash1(course->co))%curr_size2;   // the signature equation for double hashing (using two hashing functions)
               // if (j >= 1) {
               //     flag = 1;     // flag for collision purpose
               // }

                  if (doublehashtable[k] == NULL || doublehashtable[k] == SIGN_DEL) {
                      doublehashtable[k] = course;
                      if (loadfactor2() > 0.55) {
                          rehash2();
                      }
                        return 1;
                  }
                  if (flag >= 1) {
                    collision_count2++;
                  }
               }

// if (flag >= 1) {
//   collision_count2++;
// }

          return 0;
        }
////////////////////////////////////////////////////////////////

  c* search_course2(char *course){   // a function to search and look up for a course in double hash method hash table
                                      // all those functions are similiar among both tables but the difference is the hash equation
    int indx, k;

    //   indx = partofdoublehash(course, curr_size2);
    for (int j = 0; j < curr_size2; j++) {
      k = (hash2(course, curr_size2) + j*hash1(course))%curr_size2;    // same as in insert function
     if (doublehashtable[k] == NULL) {
          return NULL;
     }
     if (doublehashtable[k] == SIGN_DEL) {
          continue;
     }
     if (strcmp(doublehashtable[k]->co, course) == 0 && doublehashtable[k] != NULL ) {
         printf("Course is found at index %d\n", k);
         return doublehashtable[k];
     }
    }

    return NULL;

  }
//////////////////////////////////////////////////////////////////////////////
  c* delete_course2(char *course){   // to delete a course from double hash method hash table

    c* temp = (c*)malloc(sizeof(c));  int k;
  //  int indx = partofdoublehash(course, curr_size2);
    for (int j = 0; j < curr_size2; j++) {
        k = (hash2(course, curr_size2) + j*hash1(course))%curr_size2;
    if (doublehashtable[k] == NULL) {
            return NULL;
       }
       if (doublehashtable[k] == SIGN_DEL) {
            continue;
       }
       printf("%s\n", doublehashtable[k]->co);
    if (strcmp(doublehashtable[k]->co, course) == 0 ) {
       temp = doublehashtable[k];
      doublehashtable[k] = SIGN_DEL;
      printf("HERE\n");
      //     free(temp);
      return temp;
    }
    }
       return NULL;


  }

void rehash2(){
    int new_size = curr_size2*2;
    while(!prime(new_size)){
        new_size++;
    }
    c* new_table[new_size];
    for(int i = 0; i < new_size; i++){
        new_table[i] = NULL;
    }
    for(int i = 0; i < curr_size2; i++){
        if(doublehashtable[i] == NULL || doublehashtable[i] == SIGN_DEL){
            continue;
        }
        int l, k, flag = 0;
        char *token = (char*)malloc(sizeof(doublehashtable[i]->co)*10);
        strcpy(token, doublehashtable[i]->co);
      //  l = partofdoublehash(token, new_size);
        for (int j = 0; j < new_size; j++, flag++) {
           k = (hash2(token, new_size) + j*hash1(token))%new_size;     // this is the formula for linear probing to solve collisions
           // if (j >= 1) {
           //     flag = 1;
           // }
              if (new_table[k] == NULL || new_table[k] == SIGN_DEL) {
                new_table[k] = (c*)malloc(sizeof(c));
                strcpy(new_table[k]->co,doublehashtable[i]->co);    // assigning all read values from the file into a course node as described below
                new_table[k]->ho = doublehashtable[i]->ho;
                strcpy(new_table[k]->code,doublehashtable[i]->code);
                strcpy(new_table[k]->dep,doublehashtable[i]->dep);
                strcpy(new_table[k]->topics,doublehashtable[i]->topics);
                break;
              }
              if (flag >= 1) {  // flag for collision count purpose
                     collision_count2++;
                   }
        }
    }
    curr_size2 = new_size;
    for(int i = 0; i < curr_size2; i++){
        if(new_table[i] == NULL){
            doublehashtable[i] = NULL;
            continue;
        }
        doublehashtable[i] = (c*)malloc(sizeof(c));
        strcpy(doublehashtable[i]->co,new_table[i]->co);    // assigning all read values from the file into a course node as described below
        doublehashtable[i]->ho = new_table[i]->ho;
        strcpy(doublehashtable[i]->code,new_table[i]->code);
        strcpy(doublehashtable[i]->dep,new_table[i]->dep);
        strcpy(doublehashtable[i]->topics,new_table[i]->topics);
    }
}
///////////////////////////////////////////////////////////////////////

  void saveout(FILE* output){   // a function to save the double hash table to a file

    for (int j = 0; j < curr_size2; j++) {
      if (doublehashtable[j] == NULL) {
          fprintf(output,"\t%d -->\t------\n", j);
      }
         else if(doublehashtable[j] == SIGN_DEL){
           fprintf(output,"\t%d -->\t------(DELETED)\n", j);
         }
         else {
           fprintf(output,"\t%d -->\t%s\n", j, doublehashtable[j]->co);
         }
    }
  }

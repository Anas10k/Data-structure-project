#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define WORD_MAX 50
#define WORD_MEANING_MAX 200
#define MAX_WORDS_IN_FILE 400

#define ZERO 48
#define NINE 57

struct node;
typedef struct node*BST;

struct node {
    int number;
    char word[WORD_MAX];
    char meaning[WORD_MEANING_MAX];
    BST right;
    BST left;

};

//Prototypes of the functions
void displayChoices();
void displayMenu(BST T,char *word, int *found,int *counterNumberInFile,char *new_word_to_insert, char *new_meaning_to_insert, char ch ,char *word_to_delete,char *word_to_search);
BST read_file_dictionary (BST T, int *counterNumberInFile);
BST insert (BST T, char *word, char *meaning, int number);
BST Search (BST T, char *word,int *found);
int update (BST T,char *word,int *found);
BST insertNewWord (BST T, char *,char *, int** );
void inOrderTraversal(BST T);
void print_words_with_same_char(BST T, char key);
BST findMin(BST T);
BST delete_node(BST T , char *word);
BST delete_specific_node(BST T, char key, int *flag);
void print_in_file(BST T,FILE *save_data);

int main (){
    //create BST
    BST T = NULL;
    int *found =0 ;
    char *new_meaning;
    char *word;
    char *word_to_search= (char* )malloc (sizeof(char)*WORD_MAX);
    char *word_to_delete = (char* )malloc (sizeof(char)*WORD_MAX);
    char *meaning;
    char *new_word_to_insert =(char* )malloc (sizeof(char)*WORD_MAX);
    char *new_meaning_to_insert = (char* )malloc (sizeof(char)*WORD_MEANING_MAX);
    int number;
    int *counterNumberInFile;
    int *flag=0;
    char ch;

    displayMenu( T,word, &found,&counterNumberInFile,new_word_to_insert, new_meaning_to_insert, ch ,word_to_delete,word_to_search);


    return 0 ;
}
BST read_file_dictionary (BST T,int *counterNumberInFile){
    char dot = '.';
    char colon = ':';
    char c;
    char *word = (char*)malloc(sizeof(char)*WORD_MAX);
    char *meaning = (char*)malloc(sizeof(char)*WORD_MEANING_MAX);

    int number = 0;
    FILE *dictionary_file;
    dictionary_file = fopen("dectionary.txt","r");//open the file and read from it

    //to check if the file is empty
    if (dictionary_file == NULL){
        printf("File does not exist");
        return (-1);
    }
    *counterNumberInFile = 0;

    //define 3 flags for the process of reading from file and save the results into the tree
    // is key for the word and is value for the meaning of the word and is space is for when reaching the end of the meaning and there is more than one space
    int isKey = 0, isValue = 0, wasSpace = 0, tempNumber = 0, tempNumberFlag = 0, tempNumberCounter = 0;
    //tempNumberFlag is for if we read a number in the meaning itself and we want to continue reading the value without any problem
    //tempCounterNumber is for the increment of the (counter of array meaning) so as whenever we read a number in the definition we add one to the counter of string meaning.
    //define counter for tracking inserting to array for key or value
    int counter = 0;
    int flagFirstWord = 1;
    while((c = fgetc(dictionary_file))!= EOF){
        if (c == '\n') continue;
        if(c >= ZERO && c <= NINE){ // Mean if current character is a number
            if(isValue){// Mean if we was reading a dictionary value
                tempNumberFlag = 1;
                tempNumberCounter++;
                tempNumber = tempNumber * 10 + ((int)c - ZERO);
                continue;
            }
            else{
                number = number * 10 + ((int)c - ZERO);
                continue;
            }
        }

        if(c  == dot){
            isKey = 1;
            if(flagFirstWord){
                flagFirstWord = 0;
                continue;
            }
            isValue = 0;
            meaning[counter] = '\0';
            counter = 0;
            wasSpace = 0;
            number = tempNumber;
            tempNumber = 0;
            tempNumberFlag = 0;
            tempNumberCounter = 0;

            if (T == NULL)
                T = insert(T,word,meaning,number-1);//number-1 is because i start saving numbers in ( int number) from number 2 because the usage of tempNumber
            else
                insert(T,word,meaning,number-1);
            *counterNumberInFile = *counterNumberInFile + 1;
            memset(word, 0, WORD_MAX);//this to make the array of characters empty
            memset(meaning, 0, WORD_MEANING_MAX);
            continue;
        }

        if(c == colon){
            isKey = 0;
            isValue = 1;
            counter = 0;
            continue;
        }

        if(isKey){
            if(c == ' ') continue;
            word[counter++] = c;
            continue;
        }

        if(isValue){
            if(wasSpace && (c == ' ' || c == '\t') )continue;
            else if(c == ' ' || c == '\t') wasSpace = 1;
            else wasSpace = 0;
            if((c <= ZERO || c >= NINE) && tempNumberFlag){ // inject number to dictionary value
                meaning[counter] = '\0';
                char *numberToString = (char*)malloc(sizeof(char)*tempNumberCounter + 1);
                sprintf(numberToString, "%d", tempNumber);
                strcat(meaning, numberToString);
                counter += tempNumberCounter;
                tempNumber = 0;
                tempNumberFlag = 0;
                tempNumberCounter = 0;
            }
            meaning[counter++] = c;
            continue;
        }

    }
    //This is for the last word in the file (because of exiting the loop due to EOF
     insert(T,word,meaning,number);
     *counterNumberInFile = *counterNumberInFile + 1;

    fclose(dictionary_file);
    return T;
}
//To insert the dictionary into the BST
BST insert (BST T, char *word, char *meaning, int number){
    if (T == NULL){
            BST newChild = (BST)malloc (sizeof(struct node));
            T = newChild;
            newChild->left = newChild->right = NULL;
            strcpy(newChild->word, word);
            strcpy(newChild->meaning, meaning);
            newChild->number = number;
        }
    else{
        if(strcmp(word, T->word) == -1){
            T->left = insert( T->left,word,meaning,number);
        }
        else if (strcmp(word, T->word) == 1){
            T->right = insert(T->right,word,meaning,number);
        }
    }
    return T;
}
//To search for a word and display its meaning
BST Search (BST T, char *word_to_search,int *found){
    BST temp = T;
    *found = 0;
    while (temp!= NULL && *found == 0){
        if (strcmp(temp->word, word_to_search) == 0){
            printf("The word is found!\n");
            printf("The meaning of this word is : %s\n", temp->meaning);
            *found = 1;
            return temp;
        }
        else if (strcmp(temp->word, word_to_search) == 1)
            temp = temp->left;
        else
            temp = temp->right;
    }
    if (*found == 0){
        printf("The word that you are searching for does not exist in this dictionary!\n");
        return temp;
    }
}
//This function is to update the meaning of a specific word in the dictionary.
int update (BST T,char *word_to_search,int *found){
    char *new_meaning =(char* )malloc (sizeof(char)*WORD_MEANING_MAX);
    printf("Please enter the word that you want to update its meaning : ");
    scanf("%s", word_to_search);
    BST temp = T;
    BST target_word = Search (temp,word_to_search,&found);
    if (found == 0)
        return 0;
    else{
        printf("This is the current meaning!\n");
        printf("Enter the new meaning for (%s) : ",target_word->word );
         scanf("%s", new_meaning);
        strcpy(target_word->meaning,new_meaning);
        printf("So after updating : %s means : %s",target_word->word, target_word->meaning);
    }
}
//This function is to insert a new word from the user with its meaning in the dictionary
BST insertNewWord (BST T,char *word, char *new_meaning,int **new_number){
    if (T == NULL){
        BST newChild = (BST)malloc(sizeof(struct node));
        T = newChild;
        newChild->left = newChild->right = NULL;
        strcpy(newChild->word, word);
        strcpy(newChild->meaning, new_meaning);
        **new_number = **new_number + 1;
        newChild->number = **new_number;
    }
    else{
        if(strcmp(word, T->word) == -1){
            T->left = insertNewWord(T->left,word,new_meaning, new_number);
        }
        else if (strcmp(word, T->word) == 1){
            T->right = insertNewWord(T->right,word,new_meaning,new_number);
        }
    }
    return T;
}
//This function is to print all nodes in the tree in alphabetic order
void inOrderTraversal(BST T){
    if (T != NULL){
        inOrderTraversal(T->left);
        printf("%d.%s:%s\n\n",T->number,T->word,T->meaning);
        inOrderTraversal(T->right);
    }

}
void print_words_with_same_char(BST T, char key){
    if(T->word[0] >= key){
        if(T->left)
            print_words_with_same_char(T->left, key);
    }
    if(T->word[0] == key)
        printf("%s: %s\n",T->word,T->meaning);
    if(T->word[0] <= key){
        if(T->right)
            print_words_with_same_char(T->right, key);
    }
}

BST findMin(BST T){
    if (T == NULL)
        return T;
    else if (T->left == NULL)
        return T;
    else
        return(findMin(T->left));
}

BST delete_node(BST T , char *word){
    BST temp, c;
    if (T == NULL){
        printf("The word (%s) is not found!\n ", word);
        return T;
        }
    else if (strcmp(word,T->word) == -1)
        T->left =delete_node(T->left,word);
    else if (strcmp(word,T->word) == 1)
        T->right =delete_node(T->right,word);
    else if (T->left && T->right){
            temp = findMin(T->right);
            strcpy(T->word,temp->word);
            strcpy(T->meaning,temp->meaning);
            T->number = temp->number;
            T->right = delete_node(T->right,T->word);
        }
        else{
            temp = T;
            if (T->left == NULL)
                c = T->right;
            if (T->right == NULL)
                c= T->left;
            free(temp);
            return c;
        }
        return T;
    }

BST delete_specific_node(BST T, char key, int *flag){
    BST temp, c;

    if (T == NULL){
        printf("The char (%c) is not found!\n", key);
        *flag = 1;
        return T;
    }
    else if (key < T->word[0])
        T->left =delete_specific_node(T->left,key,flag);
    else if (key > T->word[0])
        T->right =delete_specific_node(T->right,key,flag);
    else if (T->left && T->right){
           // BST temp;
            temp = findMin(T->right);
            T->number = temp->number;
            strcpy(T->word,temp->word);
            strcpy(T->meaning,temp->meaning);
            T->right = delete_specific_node(T->right,key,flag);
        }
        else{
            temp = T;
            if (T->left == NULL)
                c = T->right;
            if (T->right == NULL)
                c= T->left;
            free(temp);
            return c;
        }
        return T;}
void print_in_file(BST T,FILE *save_data){
   /* FILE *save_data;
    save_data = fopen("test2.txt","w");*/
    if (T != NULL){
        print_in_file(T->left,save_data);
        fprintf(save_data,"%d.%s:%s\n\n",T->number,T->word,T->meaning);
        print_in_file(T->right,save_data);
    }
}
//It is a menu of operations that are required
void displayChoices(){
    printf("Choose from the menu below:\n");
    printf("1) Read dictionary file .\n");
    printf("2) Search for a word and display its meaning.\n");
    printf("3) Update the meaning of an existing word.\n");
    printf("4) Insert a new word from the user with its meaning. \n");
    printf("5) Print all words in the dictionary in an alphabetic order with their associated meanings. \n");
    printf("6) Print all words that start with a specific character in an alphabetic order.\n");
    printf("7) Delete a word from the dictionary. \n");
    printf("8) Delete all words that start with a specific letter.\n");
    printf("9) Save all words back in file dictionary.txt.\n");
    printf("10) Exit.\n");
}

void displayMenu(BST T,char *word, int *found,int *counterNumberInFile,char *new_word_to_insert, char *new_meaning_to_insert, char ch ,char *word_to_delete,char *word_to_search){
    int flag_read = 0;
    int isNull = 0;
    char *read_dictionary = "Please read the file and create the dictionary first !\n";
    int choice;
    displayChoices();

    while(1)
    {
        scanf("%d", &choice);

        if(choice == 10)
        {
            printf("Goodbye");
            break;
        }

        switch(choice)
        {
            case 1:
                flag_read = 1;
                T  = read_file_dictionary(T,&counterNumberInFile);
                break;

            case 2:
                if (!flag_read)
                    printf(read_dictionary);
                else{
                    printf("Please enter the word that you want to search for : ");
                    scanf("%s",word_to_search);
                    Search (T,word_to_search,&found);
                }
                break;

            case 3:
                if (!flag_read)
                    printf(read_dictionary);
                else
                    update(T,word_to_search,&found);
                break;

            case 4:
                if (!flag_read)
                    printf(read_dictionary);
                else{
                    int **new_number;
                    new_number = &counterNumberInFile;
                    printf("Enter the new word that you want to insert into the dictionary : ");
                    scanf("%s", new_word_to_insert);
                    printf("Enter the meaning :");
                    scanf("%s", new_meaning_to_insert);
                    insertNewWord (T, new_word_to_insert,new_meaning_to_insert,&new_number);
                }

                break;

            case 5:
                if (!flag_read)
                    printf(read_dictionary);
                else
                    inOrderTraversal(T);
                break;

            case 6:
                if (!flag_read)
                    printf(read_dictionary);
                else{
                    printf("Please enter the character that you want to print its words : ");
                    scanf(" %c", &ch);
                    print_words_with_same_char(T, ch);
                }

                break;

            case 7:
                if (!flag_read)
                    printf(read_dictionary);
                else{
                    printf("Please enter the word that you want to delete : ");
                    scanf("%s", word_to_delete);
                    T = delete_node(T , word_to_delete);
                    counterNumberInFile--;
                }

                break;

            case 8:
                if (!flag_read)
                    printf(read_dictionary);
                else{
                    printf("Please enter the character that you want to print its words : ");
                    scanf(" %c", &ch);
                    while(!isNull){
                        T = delete_specific_node(T, ch, &isNull);
                        if(isNull){
                            inOrderTraversal(T);
                        }
                        else{
                            BST temp = T;
                        }
                    }
                }

                break;

            case 9:
                if (!flag_read)
                    printf(read_dictionary);
                else{
                    FILE *save_data;
                    save_data = fopen("dictionary.txt","w");
                    print_in_file(T,save_data);
                    fclose(save_data);
                    }

                break;

            default:
                printf("Please number from 1 to 10\n");
        }
            printf("\n\n\n");
            displayChoices();
    }

}

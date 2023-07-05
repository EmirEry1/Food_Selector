#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>

struct Rating_individual{
    char * foods[10];
};

char * fullList[10] = {"Spagetti", "Rice", "Chicken", "Meatballs", "Salad", "Fish", "Cheese", "Bread", "Pizza", "Soup"};

struct Rating_pair{
    char *food;
    int rating;
};

struct Tresh_List{
    char * * foods;
    int size;
};

int find_food(struct Rating_pair* ratingPairs, const char* food, int size){
    for(int i = 0; i < size; i++){
        if(ratingPairs[i].food == food){
            return i;
        }
    }
    return -1;
}

int check_food(char * * foods, const char* food, int size){
    for (int i = 0; i < size; i++) {
        if(foods[i]==food){
            return 1;
        }
    }
    return 0;
}

struct Tresh_List calculateTreshold(int tresh, struct Rating_pair* list,int size){
    int index = 0;
    char * * foods = malloc(sizeof(char)*30*size);
    struct Tresh_List tresh_list;
    tresh_list.foods = foods;
    for(int i = 0; i< size; i++){
        if(list[i].rating >= tresh){
            tresh_list.foods[index] = list[i].food;
            index ++;
        }
    }
    tresh_list.size = index;
    if(tresh_list.size == 0) {printf("%s", "You are eating at home today.");}
    return tresh_list;
}

char * max_food(struct Rating_pair* ratings, int list_size){
    int max = 0;
    char* max_food = ratings[0].food;
    for(int i = 0; i < list_size; i++){
        if(ratings[i].rating > max){
            max = ratings[i].rating;
            max_food = ratings[i].food;
        }
    }
    return max_food;
}

struct Tresh_List createList(int n_people, struct Rating_individual* ratings,int num_food){
    struct Rating_pair* final_ratings;
    final_ratings = (struct Rating_pair*) malloc(n_people*num_food*sizeof(struct Rating_pair));
    int last_index = 0;
    for (int i = 0; i < n_people; i++) {
        for (int j = 0; j < num_food; j++) {
            int index = find_food(final_ratings,ratings[i].foods[j],num_food*n_people);
            if(index!=-1){
                final_ratings[index].rating += num_food-j;
            }
            else {
                final_ratings[last_index].food = ratings[i].foods[j];
                final_ratings[last_index].rating = num_food-j;
                last_index++;
            }
        }
    }

    struct Tresh_List treshList;
    treshList = calculateTreshold(10,final_ratings,last_index);

    if(treshList.size > 0){
        printf("List of the foods that have past the threshold of 10: ");
        printf("\n");
        printf("\n");
        for(int i = 0; i < treshList.size; i++){
            printf("%s", treshList.foods[i]);
            printf("\n");
        }
    }

    free(final_ratings);
    return treshList;
}

char* chooseFood(int n_people, struct Rating_individual* ratings, int list_size){
    struct Rating_pair* final_ratings;
    final_ratings = (struct Rating_pair*) malloc(sizeof(struct Rating_pair)*list_size);
    for (int i = 0; i < list_size; ++i) {
        final_ratings[i].food = ratings[0].foods[i];
        final_ratings[i].rating = 0;
    }
    for (int i = 0; i < n_people; i++) {
        for (int j = 0; j < list_size; j++) {
            int index = find_food(final_ratings,ratings[i].foods[j],list_size*n_people);
            final_ratings[index].rating += list_size-j;
        }
    }

    for(int i = 0; i < list_size; i++){
        printf("%s", final_ratings[i].food);
        printf(": ");
        printf("%d", final_ratings[i].rating);
        printf("\n");
    }
    char* m_food = max_food(final_ratings, list_size);
    free(final_ratings);
    return m_food;
}

int main() {
    srand(time(NULL));
    int n_people = 0;
    printf("Please enter the number of people:");
    scanf("%d", &n_people);

    struct Rating_individual* ratings = (struct Rating_individual*) malloc(sizeof(struct Rating_individual)*n_people*5);
    for (int i = 0; i < n_people; i++) {
        printf("Person ");
        printf("%d",i+1);
        printf("\n");
        printf("\n");
        for(int j = 0; j < 5; j++) {
            int random_number = rand()%10;
            while (check_food(ratings[i].foods,fullList[random_number], 5)){
                random_number = rand()%10;
            }
            ratings[i].foods[j] = fullList[random_number];
            printf("%d" ,j+1);
            printf("%s", ") ");
            printf("%s",ratings[i].foods[j]);
            printf("\n");
        }
        printf("\n");
    }

    struct Tresh_List tresh_list = createList(n_people, ratings,5);


    if(tresh_list.size > 0){

        printf("***********");
        printf("\n");

        for (int i = 0; i < n_people; i++) {
            for (int j = 0; j < 5; j++) {
                ratings[i].foods[j] = "";
            }
        }
        for (int i = 0; i < n_people; i++) {
            printf("Person ");
            printf("%d",i+1);
            printf("\n");
            printf("\n");
            for(int j = 0; j < tresh_list.size; j++) {
                int random_number = rand()%tresh_list.size;
                while (check_food(ratings[i].foods,tresh_list.foods[random_number], tresh_list.size)){
                    random_number = rand()%tresh_list.size;
                }
                ratings[i].foods[j] = tresh_list.foods[random_number];
                printf("%d" ,j+1);
                printf("%s", ") ");
                printf("%s",ratings[i].foods[j]);
                printf("\n");
            }
            printf("\n");
        }

        printf("\n");
        printf("\n");

        char* food = chooseFood(n_people,ratings,tresh_list.size);
        printf("\n");
        printf("You will eat: ");
        printf("%s", food);
    }

    free(tresh_list.foods);
    free(ratings);
    return 0;
}

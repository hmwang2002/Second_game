//
// Created by Lenovo on 2022/1/15.
//
#include <stdlib.h>
#define Enemy1_width 50
#define Enemy2_height 50
typedef struct createEnemy{
    int x;
    int y;
    int hp;
    int status;
} CreateEnemy;
CreateEnemy enemy1[50];
CreateEnemy *create_level1(){
    CreateEnemy *enemy = malloc(sizeof(CreateEnemy));
}

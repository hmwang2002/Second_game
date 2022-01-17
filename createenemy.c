//
// Created by Lenovo on 2022/1/15.
//
#include <stdlib.h>
#include <time.h>
typedef struct createEnemy{
    int x;
    int y;
    int hp;
    int status;
} CreateEnemy;

CreateEnemy *create_level1(){
    srand((unsigned )time(NULL));
    CreateEnemy *enemy = malloc(sizeof(CreateEnemy));
    enemy->x = rand() * rand() % 800;
    enemy->y = 0;
    enemy->hp = 10;
    enemy->status = 1;
    return enemy;
}
CreateEnemy *create_level2(){
    srand((unsigned) time(NULL));
    CreateEnemy *enemy = malloc(sizeof(CreateEnemy));
    enemy->x = rand() * rand() * rand() % 800;
    enemy->y = 0;
    enemy->hp = 50;
    enemy->status = 1;
    return enemy;
}

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

typedef struct createEnemy2{
    int x;
    int y;
    int hp;
    int reload;
    int status;
} CreateEnemy2;

typedef struct createEnemy3{
    int x;
    int y;
    int hp;
    int reload;
    int status;
}CreateEnemy3;

CreateEnemy *create_level1(){
    srand((unsigned )time(NULL));
    CreateEnemy *enemy = malloc(sizeof(CreateEnemy));
    enemy->x = rand() * rand() % 800;
    enemy->y = 0;
    enemy->hp = 10;
    enemy->status = 1;
    return enemy;
}
CreateEnemy2 *create_level2(){
    srand((unsigned) time(NULL));
    CreateEnemy2 *enemy = malloc(sizeof(CreateEnemy2));
    enemy->x = rand() * rand() * rand() % 800;
    enemy->y = 0;
    enemy->hp = 50;
    enemy->reload = 0;
    enemy->status = 1;
    return enemy;
}

CreateEnemy3 *create_level3(){
    srand((unsigned ) time(NULL));
    CreateEnemy3 *enemy = malloc(sizeof(CreateEnemy3));
    enemy->x = rand() * rand() % 800;
    enemy->y = 0;
    enemy->hp = 100;
    enemy->reload = 0;
    enemy->status = 1;
    return enemy;
}
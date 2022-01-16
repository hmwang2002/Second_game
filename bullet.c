//
// Created by Lenovo on 2022/1/16.
//
#include <stdlib.h>
typedef struct createBullet{
    int x;
    int y;
    int level;
} CreateBullet;

int bullet_speed = 8;
CreateBullet *CreateBullet_1(){
    CreateBullet *bullet = malloc(sizeof(CreateBullet));
    bullet->level = 1;
    return bullet;
}
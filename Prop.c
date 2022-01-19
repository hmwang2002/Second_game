//
// Created by Lenovo on 2022/1/19.
//
#include <stdlib.h>
typedef struct createProp{
    int level;
    int x;
    int y;
} CreateProp;

CreateProp *prop(){
    CreateProp *p = malloc(sizeof(CreateProp));
    return p;
}
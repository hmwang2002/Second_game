#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "createenemy.c"
#include "bullet.c"
#define Width 800
#define Height 800
#define Plane_File "myplane.png"
#define FrameRate 35
#define Enemy1_File "enemy1.png"
#define Enemy1_Destroy_File "enemy1_down2.png"
#define Bullet1_File "bullet1.png"

int background_speed = 2;
int plane_speed = 8;
int enemy1_speed = 4;
int whether_create_enemy1 = 0;
typedef struct player{
    int x;
    int y;
    int level;
    int HP;
} Player;

SDL_Window *window = NULL;
SDL_Renderer  *Renderer = NULL;

SDL_Surface *Background = NULL;
SDL_Texture *BackgroundTexture = NULL;
SDL_Rect BackgroundRect;

TTF_Font *ScoreFont = NULL;
SDL_Surface *ScorePointSurface = NULL;
SDL_Surface *ScoreSurface = NULL;
SDL_Surface *HP = NULL;
SDL_Surface *HP_Score = NULL;
SDL_Texture *HP_ScoreTexture = NULL;
SDL_Texture *HPTexture = NULL;
SDL_Texture *ScoreTexture = NULL;
SDL_Texture *ScorePointTexture = NULL;
SDL_Rect ScoreRect;
SDL_Rect ScorePointRect;
SDL_Rect HPRect;
SDL_Rect HP_Score_Rect;
SDL_Color FontColor = {255,10,10,255};//红色

SDL_Texture *PlayerTexture = NULL;
SDL_Rect PlayerRect;
SDL_Texture *Enemy1Texture = NULL;
SDL_Rect Enemy1Rect = {0,0,50,50};
CreateEnemy *enemy1[50];

SDL_Texture *Bullet_1_Texture = NULL;
SDL_Rect Bullet_1_Rect;
CreateBullet *bullet1[100];
int reload_bullet1;

void Quit();
void LOAD();

void BattlefieldLoad();
void Initialize_my_plane(Player *Player_main);
void Enemy1_level1(Player *Player_main, int *score);
void BattlefieldQuit(SDL_Surface *Battlefield,SDL_Texture *BattlefieldTexture, SDL_Texture *BattlefieldTexture_1 ,
                     Player *p);
bool Is_Bombed(CreateBullet *bullet, CreateEnemy *enemy);

int main(int argc, char *argv[])
{
    TTF_Init();
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(
            "Thunder Storm",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            Width,
            Height,
            0
    );
    Renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);

    LOAD();
    while(1){
        int flag = 0;
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch (event.type) {
                case SDL_QUIT:
                    flag = 1;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    printf("%d , %d\n",event.button.x,event.button.y);
                    if(event.button.x >= 270 && event.button.x <= 510 &&
                    event.button.y >= 294 && event.button.y <= 392){
                        BattlefieldLoad();
                        LOAD();
                    }else if(event.button.x >= 270 && event.button.x <= 510 &&
                    event.button.y >= 402 && event.button.y <= 500){
                        flag = 1;
                    }
                    break;
                default:
                    break;
            }
        }
        if(flag)break;

    }
    Quit();
    SDL_Quit();
    return 0;
}

void Quit(){
    SDL_FreeSurface(ScoreSurface);
    SDL_FreeSurface(ScorePointSurface);
    SDL_FreeSurface(Background);
    SDL_FreeSurface(HP);
    SDL_DestroyTexture(HPTexture);
    SDL_DestroyTexture(BackgroundTexture);
    SDL_DestroyTexture(ScoreTexture);
    SDL_DestroyTexture(ScorePointTexture);
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(ScoreFont);
}

void LOAD(){
    Background = IMG_Load("Background.png");
    BackgroundTexture = SDL_CreateTextureFromSurface(Renderer,Background);
    BackgroundRect.x = 0;
    BackgroundRect.y = 0;
    BackgroundRect.w = Width;
    BackgroundRect.h = Height;
    SDL_RenderCopy(Renderer,BackgroundTexture,NULL,&BackgroundRect);
    SDL_RenderPresent(Renderer);
}

void BattlefieldLoad(){
    Player *Player_main = malloc(sizeof(Player));
    Initialize_my_plane(Player_main);
    /**
     * 玩家飞机的初始化
     */
    Bullet_1_Rect.w = 5;
    Bullet_1_Rect.h = 11;
    Bullet_1_Texture = IMG_LoadTexture(Renderer,Bullet1_File);

    Enemy1Texture = IMG_LoadTexture(Renderer,Enemy1_File);
    int score = 0;
    char score_[100] = {0};
    int Hp = 100;
    char Hp_[3] = {0};
    SDL_Surface *Battlefield = NULL;
    SDL_Texture *BattlefieldTexture = NULL;
    SDL_Rect BattlefieldRect;
    Battlefield = IMG_Load("battlefield.png");
    BattlefieldTexture = SDL_CreateTextureFromSurface(Renderer,Battlefield);
    BattlefieldRect.x = 0;
    BattlefieldRect.y = 0;
    BattlefieldRect.w = Width;
    BattlefieldRect.h = Height;
    SDL_Texture *BattlefieldTexture_1 = NULL;
    SDL_Rect BattlefieldRect_1;
    BattlefieldTexture_1 = SDL_CreateTextureFromSurface(Renderer,Battlefield);
    BattlefieldRect_1.x = 0;
    BattlefieldRect_1.y = -Height;
    BattlefieldRect_1.w = Width;
    BattlefieldRect_1.h = Height;
    SDL_RenderCopy(Renderer,BattlefieldTexture_1,NULL,&BattlefieldRect_1);
    SDL_RenderPresent(Renderer);
    while(1){
        long begin = SDL_GetTicks();
        srand((unsigned)time(NULL));
        SDL_RenderClear(Renderer);
        ScoreFont = TTF_OpenFont("BERNHC.TTF",50);
        ScoreSurface = TTF_RenderUTF8_Blended(ScoreFont,"Score:",FontColor);
        ScoreTexture = SDL_CreateTextureFromSurface(Renderer,ScoreSurface);
        ScoreRect.x = 600;
        ScoreRect.y = 0;
        ScoreRect.w = 60;
        ScoreRect.h = 60;
        itoa(score,score_,10);
        ScorePointSurface = TTF_RenderUTF8_Blended(ScoreFont,score_,FontColor);
        ScorePointTexture = SDL_CreateTextureFromSurface(Renderer,ScorePointSurface);
        ScorePointRect.x = 700;
        ScorePointRect.y = 0;
        ScorePointRect.w = 40;
        ScorePointRect.h = 60;

        HP = TTF_RenderUTF8_Blended(ScoreFont,"HP: ",FontColor);
        HPTexture = SDL_CreateTextureFromSurface(Renderer,HP);
        HPRect.x = 630;
        HPRect.y = 65;
        HPRect.w = 40;
        HPRect.h = 60;
        itoa(Hp,Hp_,10);
        HP_Score = TTF_RenderUTF8_Blended(ScoreFont,Hp_,FontColor);
        HP_ScoreTexture = SDL_CreateTextureFromSurface(Renderer,HP_Score);
        HP_Score_Rect.x = 700;
        HP_Score_Rect.y = 65;
        HP_Score_Rect.w = 40;
        HP_Score_Rect.h = 60;
        /**
         * 解决字体和血量
         */
        if(BattlefieldRect.y >= Height)BattlefieldRect.y = -Height;
        if(BattlefieldRect_1.y >= Height)BattlefieldRect_1.y = -Height;
        BattlefieldRect.y += background_speed;
        BattlefieldRect_1.y += background_speed;
        SDL_RenderCopy(Renderer,BattlefieldTexture,NULL,&BattlefieldRect);
        SDL_RenderCopy(Renderer,BattlefieldTexture_1,NULL,&BattlefieldRect_1);
        /**
         * copy的顺序决定了覆盖的优先级
         * 解决了地图滚动的问题
         */

        SDL_RenderCopy(Renderer,ScoreTexture,NULL,&ScoreRect);
        SDL_RenderCopy(Renderer,ScorePointTexture,NULL,&ScorePointRect);
        SDL_RenderCopy(Renderer,HPTexture,NULL,&HPRect);
        SDL_RenderCopy(Renderer,HP_ScoreTexture,NULL,&HP_Score_Rect);

        Enemy1_level1(Player_main,&score);


        PlayerRect.x = Player_main->x;
        PlayerRect.y = Player_main->y;
        SDL_RenderCopy(Renderer,PlayerTexture,NULL,&PlayerRect);
        SDL_RenderPresent(Renderer);
        SDL_Event game_event;
        while(SDL_PollEvent(&game_event)){
            switch (game_event.type) {
                case SDL_QUIT:
                    BattlefieldQuit(Battlefield,BattlefieldTexture,BattlefieldTexture_1,Player_main);
                    return;
                case SDL_MOUSEBUTTONDOWN:
                    printf("%d , %d\n",game_event.button.x,game_event.button.y);
                    break;
                case SDL_KEYDOWN:
                    switch (game_event.key.keysym.sym) {
                        /**
                         * SDL无法解决同时按键
                         * 被迫增加了新的按键
                         */
                        case SDLK_q:
                            if(Player_main->y >= 0){
                                Player_main->y -= plane_speed;
                            }
                            if(Player_main->x >= 0){
                                Player_main->x -= plane_speed;
                            }
                            break;
                        case SDLK_e:
                            if(Player_main->y >= 0){
                                Player_main->y -= plane_speed;
                            }
                            if(Player_main->x <= 740){
                                Player_main->x += plane_speed;
                            }
                            break;
                        case SDLK_z:
                            if(Player_main->y <= 740){
                                Player_main->y += plane_speed;
                            }
                            if(Player_main->x >= 0){
                                Player_main->x -= plane_speed;
                            }
                            break;
                        case SDLK_c:
                            if(Player_main->y <= 740){
                                Player_main->y += plane_speed;
                            }
                            if(Player_main->x <= 740){
                                Player_main->x += plane_speed;
                            }
                            break;
                        case SDLK_w:
                            if(Player_main->y >= 0){
                                Player_main->y -= plane_speed;
                            }
                            break;
                        case SDLK_s:
                            if(Player_main->y <= 740){
                                Player_main->y += plane_speed;
                            }
                            break;
                        case SDLK_a:
                            if(Player_main->x >= 0){
                                Player_main->x -= plane_speed;
                            }
                            break;
                        case SDLK_d:
                            if(Player_main->x <= 740){
                                Player_main->x += plane_speed;
                            }
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }

        long current = SDL_GetTicks();
        long cost = current - begin;
        long frame = 1000 / FrameRate;
        long delay = frame - cost;
        if(delay > 0){
            SDL_Delay(delay);
        }
    }


}
void Initialize_my_plane(Player *Player_main){
    Player_main->HP = 100;
    Player_main->x = 370;
    Player_main->y = 740;
    Player_main->level = 1;
    PlayerTexture = IMG_LoadTexture(Renderer,Plane_File);
    PlayerRect.x = Player_main->x;
    PlayerRect.y = Player_main->y;
    PlayerRect.w = 60;
    PlayerRect.h = 60;
}
void Enemy1_level1(Player *Player_main, int *score){
    if(whether_create_enemy1 == 0){
        for (int i = 0; i < 50; i++) {
            if(enemy1[i] == NULL){
                enemy1[i] = create_level1();
                break;
            }
        }
        whether_create_enemy1 = 100;
    }else{
        whether_create_enemy1--;
    }
    for (int i = 0; i < 50; i++) {
        if(enemy1[i] != NULL && enemy1[i]->y <= 800 && enemy1[i]->status == 1){
            if(enemy1[i]->x <= 750){
                Enemy1Rect.x = enemy1[i]->x;
            }else{
                enemy1[i]->x -= 100;
                Enemy1Rect.x = enemy1[i]->x;
            }
            Enemy1Rect.y = enemy1[i]->y;
            enemy1[i]->y += enemy1_speed;
            SDL_RenderCopy(Renderer,Enemy1Texture,NULL,&Enemy1Rect);
        }else if(enemy1[i] != NULL && enemy1[i]->y > 800){
            CreateEnemy *p = enemy1[i];
            enemy1[i] = NULL;
            free(p);
        }else if(enemy1[i] != NULL && enemy1[i]->status == 0){
            Enemy1Texture = IMG_LoadTexture(Renderer,Enemy1_Destroy_File);
            Enemy1Rect.x = enemy1[i]->x;
            Enemy1Rect.y = enemy1[i]->y;
            SDL_RenderCopy(Renderer,Enemy1Texture,NULL,&Enemy1Rect);
            Enemy1Texture = IMG_LoadTexture(Renderer,Enemy1_File);
            enemy1[i]->status = -1;
        }else if(enemy1[i] != NULL && enemy1[i]->status == -1){
            CreateEnemy *clear_enemy = enemy1[i];
            enemy1[i] = NULL;
            free(clear_enemy);
        }
    }
    if(reload_bullet1 == 0){
        for (int i = 0; i < 100; i++) {
            if(bullet1[i] == NULL){
                bullet1[i] = CreateBullet_1();
                bullet1[i]->x = Player_main->x + PlayerRect.w / 2;
                bullet1[i]->y = Player_main->y;
                break;
            }
        }
        reload_bullet1 = 10;
    }else{
        reload_bullet1--;
    }
    for (int i = 0; i < 100; i++) {
        if(bullet1[i] != NULL){
            if(bullet1[i]->y >= 0){
                bullet1[i]->y -= bullet_speed;
                Bullet_1_Rect.x = bullet1[i]->x;
                Bullet_1_Rect.y = bullet1[i]->y;
                SDL_RenderCopy(Renderer,Bullet_1_Texture,NULL,&Bullet_1_Rect);
            }else{
                CreateBullet *p_bullet = bullet1[i];
                bullet1[i] = NULL;
                free(p_bullet);
            }
        }
    }

    for (int i = 0; i < 50; i++) {
        if(enemy1[i] != NULL){
            for (int j = 0; j < 100; j++) {
                if(bullet1[j] != NULL && Is_Bombed(bullet1[j],enemy1[i]) && enemy1[i] ->status == 1){
                    enemy1[i]->hp -= 10;
                    enemy1[i]->status = 0;
                    CreateBullet *clear_bullet = bullet1[j];
                    bullet1[j] = NULL;
                    free(clear_bullet);
                    *score += 10;
                    break;
                }
            }
        }

    }
}

bool Is_Bombed(CreateBullet *bullet, CreateEnemy *enemy){
    if(bullet->x >= enemy->x && bullet->x <= enemy->x + Enemy1Rect.w
    && bullet->y >= enemy->y && bullet->y <= enemy->y + Enemy1Rect.h){
        return 1;
    }else{
        return 0;
    }
}

void BattlefieldQuit(SDL_Surface *Battlefield,SDL_Texture *BattlefieldTexture, SDL_Texture *BattlefieldTexture_1
                     ,Player *p){
    SDL_FreeSurface(Battlefield);
    SDL_DestroyTexture(BattlefieldTexture);
    SDL_DestroyTexture(BattlefieldTexture_1);
    SDL_RenderClear(Renderer);
    for (int i = 0; i < 50; i++) {
        CreateEnemy *p1 = enemy1[i];
        enemy1[i] = NULL;
        free(p1);
    }
    for (int i = 0; i < 100; i++) {
        CreateBullet *p2 = bullet1[i];
        bullet1[i] = NULL;
        free(p2);
    }
    free(p);
}
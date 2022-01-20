#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "createenemy.c"
#include "bullet.c"
#include "Prop.c"
#define Width 800
#define Height 800
#define Plane_File "myplane.png"
#define FrameRate 35
#define Enemy1_File "enemy1.png"
#define Enemy1_Destroy_File "enemy1_down2.png"
#define Enemy2_File "dj1.png"
#define Enemy2_Destroy_File "dj1(1).png"
#define Bullet1_File "bullet1.png"
#define Enemy_Bullet1_File "bullet2.png"
#define Enemy3_File "dj2.png"
#define Enemy3_Destroy_File "dj2(2).png"
#define EnemyBullet2_File "enemy_bullet2.png"
#define Blood_File "bloodadd.png"
#define Sapphire_File "dlw.png"
#define Gold_File "jinbi.png"
#define Plane2_File "fj.png"
#define Bullet2_File "mybullet2.png"
#define Boss_File "dj3.png"
#define Boss_Destroy_File "dj3(2).png"
#define BossBullet_File "mybullet3.png"
#define Player_Destroy_File "fj2.png"
#define Player_Destroy_File1 "me_destroy_3.png"
int background_speed = 2;
int plane_speed = 8;
int enemy1_speed = 4;
int enemy2_speed = 4;
int enemy3_speed = 3;
int whether_create_enemy1 = 0;
int whether_create_enemy2 = 0;
int whether_create_enemy3 = 0;
int whether_update_plane = 0;
int whether_create_blood = 0;
int count_of_Sapphire = 0;
int count_of_gold = 0;

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

TTF_Font *FinalFont = NULL;
SDL_Surface *FinalSurface = NULL;
SDL_Texture *FinalTexture = NULL;
SDL_Rect FinalRect;

SDL_Texture *PlayerTexture = NULL;
SDL_Rect PlayerRect;
SDL_Texture *Enemy1Texture = NULL;
SDL_Rect Enemy1Rect = {0,0,50,50};
CreateEnemy *enemy1[50];

SDL_Texture *Enemy2Texture = NULL;
SDL_Rect Enemy2Rect = {0,0,100,100};
CreateEnemy2 *enemy2[50];

SDL_Texture *Bullet_1_Texture = NULL;
SDL_Rect Bullet_1_Rect;
CreateBullet *bullet1[100];

SDL_Texture *EnemyBullet1_Texture = NULL;
SDL_Rect EnemyBullet1_Rect;
CreateBullet *EnemyBullet1[100];

SDL_Texture *Enemy3Texture = NULL;
SDL_Rect Enemy3Rect = {0,0,200,200};
CreateEnemy3 *enemy3[50];

SDL_Texture *EnemyBullet2_Texture = NULL;
SDL_Rect EnemyBullet2_Rect = {0,0,20,44};
CreateBullet *EnemyBullet2[100];

SDL_Texture *Blood_Texture = NULL;
SDL_Rect Blood_Rect = {0,0,20,20};
SDL_Texture *Sapphire_Texture = NULL;
SDL_Rect Sapphire_Rect = {0,0,25,25};
SDL_Texture *Gold_Texture = NULL;
SDL_Rect Gold_Rect = {0,0,25,25};
CreateProp *props[50];

SDL_Texture *BossTexture = NULL;
SDL_Rect BossRect = {0,0,300,300};
SDL_Texture *BossBullet1Texture = NULL;
SDL_Rect BossBullet1Rect = {0,0,20,44};
SDL_Texture *BossBullet2Texture = NULL;
SDL_Rect BossBullet2Rect = {0,0,40,88};
CreateBullet *boss_bullet_1[100];
CreateBullet *boss_bullet_2[200];
Boss_ *boss;

int reload_bullet1;

void Quit();
void LOAD();

void BattlefieldLoad();
void Initialize_my_plane(Player *Player_main);
void shoot(Player *Player_main);
void shoot1(Player *Player_main);
void Enemy_shoot1(CreateEnemy2 *enemy);
void Enemy_shoot2(CreateEnemy3 *enemy);
void Enemy1_level1(int *score);
void Enemy2_level2(int *score);
void Enemy3_level3(int *score);
void Enemy_Bullet1_Fly(Player *Player_main);
void Enemy_Bullet2_Fly(Player *Player_main);
void BattlefieldQuit(SDL_Surface *Battlefield,SDL_Texture *BattlefieldTexture, SDL_Texture *BattlefieldTexture_1 ,
                     Player *p);
void PropsFly(Player *Player_main);
void Boss(Player *Player_main);
bool Is_Bombed(CreateBullet *bullet, CreateEnemy *enemy);
bool Is_Bombed1(CreateBullet *bullet,CreateEnemy2 *enemy);
bool Is_Attacked1(Player *Player_main, CreateBullet *Enemy_bullet1);
bool Is_Bombed2(CreateBullet *bullet,CreateEnemy3 *enemy);
bool GetProps(CreateProp *prop, Player *Player_main);
bool Is_Bombed_boss(CreateBullet *bullet);
bool Is_Attacked_boss(Player *Player_main,CreateBullet *bullet);
bool Is_Attacked_boss1(Player *Player_main,CreateBullet *bullet);

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
    boss = create_boss();
    Bullet_1_Rect.w = 5;
    Bullet_1_Rect.h = 11;
    Bullet_1_Texture = IMG_LoadTexture(Renderer,Bullet1_File);

    EnemyBullet1_Texture = IMG_LoadTexture(Renderer,Enemy_Bullet1_File);
    EnemyBullet1_Rect.w = 5;
    EnemyBullet1_Rect.h = 11;

    EnemyBullet2_Texture = IMG_LoadTexture(Renderer,EnemyBullet2_File);

    Enemy1Texture = IMG_LoadTexture(Renderer,Enemy1_File);
    Enemy2Texture = IMG_LoadTexture(Renderer,Enemy2_File);
    Enemy3Texture = IMG_LoadTexture(Renderer,Enemy3_File);
    Gold_Texture = IMG_LoadTexture(Renderer,Gold_File);
    Sapphire_Texture = IMG_LoadTexture(Renderer,Sapphire_File);
    Blood_Texture = IMG_LoadTexture(Renderer,Blood_File);
    BossTexture = IMG_LoadTexture(Renderer,Boss_File);
    BossBullet1Texture = IMG_LoadTexture(Renderer,EnemyBullet2_File);
    BossBullet2Texture = IMG_LoadTexture(Renderer,BossBullet_File);

    int score = 0;
    char score_[100] = {0};
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
        itoa(Player_main->HP,Hp_,10);
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
        if(count_of_gold >= 4){
            Bullet_1_Texture = IMG_LoadTexture(Renderer,Bullet2_File);
            Bullet_1_Rect.w = 20;
            Bullet_1_Rect.h = 44;
            for (int i = 0; i < 100; i++) {
                if(bullet1[i] != NULL){
                    bullet1[i]->level = 2;
                }
            }
        }
        if(count_of_Sapphire == 4)whether_update_plane = 1;
        if(whether_update_plane == 1){
            PlayerTexture = IMG_LoadTexture(Renderer,Plane2_File);
            PlayerRect.w = 80;
            PlayerRect.h = 80;
            plane_speed = 12;
        }
        if(score % 50 == 0){
            whether_create_blood = 1;
        }

        SDL_RenderCopy(Renderer,ScoreTexture,NULL,&ScoreRect);
        SDL_RenderCopy(Renderer,ScorePointTexture,NULL,&ScorePointRect);
        SDL_RenderCopy(Renderer,HPTexture,NULL,&HPRect);
        SDL_RenderCopy(Renderer,HP_ScoreTexture,NULL,&HP_Score_Rect);

        PlayerRect.x = Player_main->x;
        PlayerRect.y = Player_main->y;
        SDL_RenderCopy(Renderer,PlayerTexture,NULL,&PlayerRect);

        PropsFly(Player_main);
        if(Player_main->level < 3){
            shoot(Player_main);
        }else{
            shoot1(Player_main);
        }
        if(Player_main->level >= 2 && Player_main->level < 4){
            Enemy_Bullet1_Fly(Player_main);
        }
        if(Player_main->level == 3){
            Enemy_Bullet2_Fly(Player_main);
        }

        if(score >= 200 && score <= 400)Player_main->level = 2;
        if(score > 400 && score < 1500)Player_main->level = 3;
        if(score >= 1500)Player_main->level = 4;
        /**
         * 升级
         */
         if(Player_main->level < 3){
             Enemy1_level1(&score);
         }
        if(Player_main->level >= 2 && Player_main->level < 4){
            Enemy2_level2(&score);
        }
        if(Player_main->level == 3){
            Enemy3_level3(&score);
        }
        if(Player_main->level == 4){
            BossRect.x = boss->x;
            BossRect.y = boss->y;
            SDL_RenderCopy(Renderer,BossTexture,NULL,&BossRect);
            Boss(Player_main);
            if(boss->status == 0){
                BossTexture = IMG_LoadTexture(Renderer,Boss_Destroy_File);
                SDL_RenderCopy(Renderer,BossTexture,NULL,&BossRect);
                boss->status = -1;
            }else if(boss->status == -1){
                FinalFont = TTF_OpenFont("BERNHC.TTF",100);
                FinalSurface = TTF_RenderUTF8_Blended(FinalFont,"You win!",FontColor);
                FinalTexture = SDL_CreateTextureFromSurface(Renderer,FinalSurface);
                FinalRect.x = 300;
                FinalRect.y = 250;
                FinalRect.w = 200;
                FinalRect.h = 200;
                SDL_RenderCopy(Renderer,FinalTexture,NULL,&FinalRect);
                SDL_RenderPresent(Renderer);
                SDL_Delay(3000);
                BattlefieldQuit(Battlefield,BattlefieldTexture,BattlefieldTexture_1,Player_main);
                return;
            }
        }

        if(Player_main->HP < 0)Player_main->HP = 0;
        if(Player_main->HP == 0){
            if(plane_speed == 12){
                PlayerTexture = IMG_LoadTexture(Renderer,Player_Destroy_File);
            }else{
                PlayerTexture = IMG_LoadTexture(Renderer,Player_Destroy_File1);
            }
            SDL_RenderCopy(Renderer,PlayerTexture,NULL,&PlayerRect);
            FinalFont = TTF_OpenFont("BERNHC.TTF",100);
            FinalSurface = TTF_RenderUTF8_Blended(FinalFont,"You lose!",FontColor);
            FinalTexture = SDL_CreateTextureFromSurface(Renderer,FinalSurface);
            FinalRect.x = 300;
            FinalRect.y = 250;
            FinalRect.w = 200;
            FinalRect.h = 200;
            SDL_RenderCopy(Renderer,FinalTexture,NULL,&FinalRect);
            SDL_RenderPresent(Renderer);
            SDL_Delay(3000);
            BattlefieldQuit(Battlefield,BattlefieldTexture,BattlefieldTexture_1,Player_main);
            return;
        }

        SDL_RenderPresent(Renderer);
        SDL_Event game_event;
        while(SDL_PollEvent(&game_event)){
            switch (game_event.type) {
                case SDL_QUIT:
                    BattlefieldQuit(Battlefield,BattlefieldTexture,BattlefieldTexture_1,Player_main);
                    return;
                case SDL_MOUSEBUTTONDOWN:
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
void shoot(Player *Player_main){
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

}

void shoot1(Player *Player_main){
    if(reload_bullet1 == 0){
        int cnt = 3;
        for (int i = 0; i < 100; i++) {
            if(cnt == 0){
                reload_bullet1 = 10;
                break;
            }else if(bullet1[i] == NULL && cnt == 3){
                bullet1[i] = CreateBullet_1();
                bullet1[i]->x = Player_main->x;
                bullet1[i]->y = Player_main->y;
                cnt--;
            }else if(bullet1[i] == NULL && cnt == 2){
                bullet1[i] = CreateBullet_1();
                bullet1[i]->x = Player_main->x + PlayerRect.w / 2;
                bullet1[i]->y = Player_main->y;
                cnt--;
            }else if(bullet1[i] == NULL && cnt == 1){
                bullet1[i] = CreateBullet_1();
                bullet1[i]->x = Player_main->x + PlayerRect.w;
                bullet1[i]->y = Player_main->y;
                cnt--;
            }
        }
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
                free(bullet1[i]);
                bullet1[i] = NULL;
            }
        }
    }
}

void Enemy1_level1( int *score){
    if(whether_create_enemy1 == 0){
        for (int i = 0; i < 50; i++) {
            if(enemy1[i] == NULL){
                enemy1[i] = create_level1();
                break;
            }
        }
        whether_create_enemy1 = 50;
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
            if(whether_create_blood == 1){
                int num = rand() % 30;
                if(num <= 4){
                    for (int j = 0; j < 50; j++) {
                        if(props[j] == NULL){
                            props[j] = prop();
                            props[j]->x = enemy1[i]->x;
                            props[j]->y = enemy1[i]->y;
                            props[j]->level = 1;
                            break;
                        }
                    }
                    whether_create_blood = 0;
                }
            }
            CreateEnemy *clear_enemy = enemy1[i];
            enemy1[i] = NULL;
            free(clear_enemy);
        }
    }

    for (int i = 0; i < 50; i++) {
        if(enemy1[i] != NULL){
            for (int j = 0; j < 100; j++) {
                if(bullet1[j] != NULL && Is_Bombed(bullet1[j],enemy1[i]) && enemy1[i] ->status == 1 ){
                    if(bullet1[j]->level == 1){
                        enemy1[i]->hp -= 10;
                    }
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
void Enemy2_level2( int *score){
    if(whether_create_enemy2 == 0){
        for (int i = 0; i < 50; i++) {
            if(enemy2[i] == NULL){
                enemy2[i] = create_level2();
                if(enemy2[i]->x <= 120 ){
                    enemy2[i]->x += 100;
                }
                break;
            }
        }
        whether_create_enemy2 = 50;
    }else{
        whether_create_enemy2--;
    }
    for (int i = 0; i < 50; i++) {
        if(enemy2[i] != NULL && enemy2[i]->y <= 800 && enemy2[i]->status == 1){
            if(enemy2[i]->x <= 700){
                Enemy2Rect.x = enemy2[i]->x;
            }else{
                enemy2[i]->x -= 150;
                Enemy2Rect.x = enemy2[i]->x;
            }
            Enemy2Rect.y = enemy2[i]->y;
            enemy2[i]->y += enemy2_speed;
            SDL_RenderCopy(Renderer,Enemy2Texture,NULL,&Enemy2Rect);
            Enemy_shoot1(enemy2[i]);
        }else if(enemy2[i] != NULL && enemy2[i]->y > 800){
            CreateEnemy2 *p = enemy2[i];
            enemy2[i] = NULL;
            free(p);
        }else if(enemy2[i] != NULL && enemy2[i]->status == 0){
            Enemy2Texture = IMG_LoadTexture(Renderer,Enemy2_Destroy_File);
            Enemy2Rect.x = enemy2[i]->x;
            Enemy2Rect.y = enemy2[i]->y;
            SDL_RenderCopy(Renderer,Enemy2Texture,NULL,&Enemy2Rect);
            Enemy2Texture = IMG_LoadTexture(Renderer,Enemy2_File);
            enemy2[i]->status = -1;
        }else if(enemy2[i] != NULL && enemy2[i]->status == -1){
            int ans = 0;
            if(whether_create_blood == 1){
                int num = rand() % 10;
                if(num <= 4){
                    ans = 1;
                    for (int j = 0; j < 50; j++) {
                        if(props[j] == NULL){
                            props[j] = prop();
                            props[j]->x = enemy2[i]->x;
                            props[j]->y = enemy2[i]->y;
                            props[j]->level = 1;
                            Blood_Rect.x = props[j]->x;
                            Blood_Rect.y = props[j]->y;
                            SDL_RenderCopy(Renderer,Blood_Texture,NULL,&Blood_Rect);
                            break;
                        }
                    }
                    whether_create_blood = 0;
                }
            }
            if( ans == 0){
                int num = rand() % 30;
                if(num >= 5 && num <= 10){
                    ans = 1;
                    for (int j = 0; j < 50; j++) {
                        if(props[j] == NULL){
                            props[j] = prop();
                            props[j]->x = enemy2[i]->x;
                            props[j]->y = enemy2[i]->y;
                            props[j]->level = 2;
                            Gold_Rect.x = props[j]->x;
                            Gold_Rect.y = props[j]->y;
                            SDL_RenderCopy(Renderer,Gold_Texture,NULL,&Gold_Rect);
                            break;
                        }
                    }
                }
            }
            if( ans == 0){
                int num = rand() % 30;
                if(num >= 24){
                    for (int j = 0; j < 50; j++) {
                        if(props[j] == NULL){
                            props[j] = prop();
                            props[j]->x = enemy2[i]->x;
                            props[j]->y = enemy2[i]->y;
                            props[j]->level = 3;
                            Sapphire_Rect.x = props[j]->x;
                            Sapphire_Rect.y = props[j]->y;
                            SDL_RenderCopy(Renderer,Sapphire_Texture,NULL,&Sapphire_Rect);
                            break;
                        }
                    }
                }
            }
            CreateEnemy2 *clear_enemy = enemy2[i];
            enemy2[i] = NULL;
            free(clear_enemy);
            *score += 20;
        }
    }

    for (int i = 0; i < 50; i++) {
        if(enemy2[i] != NULL){
            for (int j = 0; j < 100; j++) {
                if(bullet1[j] != NULL && Is_Bombed1(bullet1[j],enemy2[i]) && enemy2[i] ->status == 1){
                    if(bullet1[j]->level == 1){
                        enemy2[i]->hp -= 20;
                    }else if(bullet1[j]->level == 2){
                        enemy2[i]->hp -= 50;
                    }
                    if(enemy2[i]->hp <= 0){
                        enemy2[i]->status = 0;
                    }
                    CreateBullet *clear_bullet = bullet1[j];
                    bullet1[j] = NULL;
                    free(clear_bullet);
                }
            }
        }

    }
}

void Enemy3_level3(int *score){
    if(whether_create_enemy3 == 0){
        for (int i = 0; i < 50; i++) {
            if(enemy3[i] == NULL){
                enemy3[i] = create_level3();
                if(enemy3[i]->x >= 600){
                    enemy3[i]->x -= 250;
                }
                break;
            }
        }
        whether_create_enemy3 = 200;
    }else{
        whether_create_enemy3--;
    }
    for (int i = 0; i < 50; i++) {
        if(enemy3[i] != NULL && enemy3[i]->y <= 800 && enemy3[i]->status == 1){
            Enemy3Rect.x = enemy3[i]->x;
            Enemy3Rect.y = enemy3[i]->y;
            enemy3[i]->y += enemy3_speed;
            SDL_RenderCopy(Renderer,Enemy3Texture,NULL,&Enemy3Rect);
            Enemy_shoot2(enemy3[i]);
        }else if(enemy3[i] != NULL && enemy3[i]->y > 800){
            free(enemy3[i]);
            enemy3[i] = NULL;
        }else if(enemy3[i] != NULL && enemy3[i]->status == 0){
            Enemy3Texture = IMG_LoadTexture(Renderer,Enemy3_Destroy_File);
            Enemy3Rect.x = enemy3[i]->x;
            Enemy3Rect.y = enemy3[i]->y;
            SDL_RenderCopy(Renderer,Enemy3Texture,NULL,&Enemy3Rect);
            Enemy3Texture = IMG_LoadTexture(Renderer,Enemy3_File);
            enemy3[i]->status = -1;
        }else if(enemy3[i] != NULL && enemy3[i]->status == -1){
            int ans = 0;
            if(whether_create_blood == 1){
                int num = rand() % 20;
                if(num <= 4){
                    ans = 1;
                    for (int j = 0; j < 50; j++) {
                        if(props[j] == NULL){
                            props[j] = prop();
                            props[j]->x = enemy3[i]->x;
                            props[j]->y = enemy3[i]->y;
                            props[j]->level = 1;
                            Blood_Rect.x = props[j]->x;
                            Blood_Rect.y = props[j]->y;
                            SDL_RenderCopy(Renderer,Blood_Texture,NULL,&Blood_Rect);
                            break;
                        }
                    }
                    whether_create_blood = 0;
                }
            }
            if(ans == 0){
                int num = rand() % 20;
                if(num >= 5 && num <= 10){
                    ans = 1;
                    for (int j = 0; j < 50; j++) {
                        if(props[j] == NULL){
                            props[j] = prop();
                            props[j]->x = enemy3[i]->x;
                            props[j]->y = enemy3[i]->y;
                            props[j]->level = 2;
                            Gold_Rect.x = props[j]->x;
                            Gold_Rect.y = props[j]->y;
                            SDL_RenderCopy(Renderer,Gold_Texture,NULL,&Gold_Rect);
                            break;
                        }
                    }
                }
            }
            if(ans == 0){
                int num = rand() % 20;
                if(num >= 11){
                    for (int j = 0; j < 50; j++) {
                        if(props[j] == NULL){
                            props[j] = prop();
                            props[j]->x = enemy3[i]->x;
                            props[j]->y = enemy3[i]->y;
                            props[j]->level = 3;
                            Sapphire_Rect.x = props[j]->x;
                            Sapphire_Rect.y = props[j]->y;
                            SDL_RenderCopy(Renderer,Sapphire_Texture,NULL,&Sapphire_Rect);
                            break;
                        }
                    }
                }
            }
            free(enemy3[i]);
            enemy3[i] = NULL;
            *score += 50;
        }
    }

    for (int i = 0; i < 50; i++) {
        if(enemy3[i] != NULL){
            for (int j = 0; j < 100; j++) {
                if(bullet1[j] != NULL && Is_Bombed2(bullet1[j],enemy3[i]) && enemy3[i]->status == 1){
                    if(bullet1[j]->level == 1){
                        enemy3[i]->hp -= 20;
                    }else if(bullet1[j]->level == 2){
                        enemy3[i]->hp -= 50;
                    }
                    if(enemy3[i]->hp <= 0){
                        enemy3[i]->status = 0;
                    }
                    CreateBullet *clear_bullet = bullet1[j];
                    bullet1[j] = NULL;
                    free(clear_bullet);
                }
            }
        }

    }
}
void Boss(Player *Player_main){
    if(boss->reload1 == 0){
        int cnt = 4;
        for (int i = 0; i < 100; i++) {
            if(cnt == 0){
                boss->reload1 = 30;
                break;
            }
            if(boss_bullet_1[i] == NULL){
                boss_bullet_1[i] = CreateBullet_2();
                boss_bullet_1[i]->x = cnt * BossRect.w / 5  + BossRect.x - 10;
                boss_bullet_1[i]->y = BossRect.y + 100;
                BossBullet1Rect.x = boss_bullet_1[i]->x;
                BossBullet1Rect.y = boss_bullet_1[i]->y;
                SDL_RenderCopy(Renderer,BossBullet1Texture,NULL,&BossBullet1Rect);
                cnt--;
            }
        }
    }else{
        boss->reload1--;
    }
    if(boss->reload2 == 0){
        int cnt = 7;
        for (int i = 0; i < 200; i++) {
            if(cnt == 0){
                boss->reload2 = 180;
                break;
            }else if(boss_bullet_2[i] == NULL){
                boss_bullet_2[i] = CreateBullet_3();
                boss_bullet_2[i]->x = (cnt - 1) * 130;
                boss_bullet_2[i]->y = BossRect.y + 30;
                BossBullet2Rect.x = boss_bullet_2[i]->x;
                BossBullet2Rect.y = boss_bullet_2[i]->y;
                SDL_RenderCopy(Renderer,BossBullet2Texture,NULL,&BossBullet2Rect);
                cnt--;
            }
        }
    }else{
        boss->reload2--;
    }
    for (int i = 0; i < 100; i++) {
        if(boss_bullet_1[i] != NULL && boss_bullet_1[i]->y <= 800 && !Is_Attacked_boss(Player_main,boss_bullet_1[i])){
            boss_bullet_1[i]->y += 6;
            BossBullet1Rect.y = boss_bullet_1[i]->y;
            BossBullet1Rect.x = boss_bullet_1[i]->x;
            SDL_RenderCopy(Renderer,BossBullet1Texture,NULL,&BossBullet1Rect);
        }else if(boss_bullet_1[i] != NULL && boss_bullet_1[i]->y >= 800 ){
            free(boss_bullet_1[i]);
            boss_bullet_1[i] = NULL;
        }else if (boss_bullet_1[i] != NULL && boss_bullet_1[i]->y <= 800 && Is_Attacked_boss(Player_main,boss_bullet_1[i])){
            if(boss_bullet_1[i]->level == 2){
                Player_main->HP -= 10;
            }else if(boss_bullet_1[i]->level == 3){
                Player_main->HP -= 20;
            }
            free(boss_bullet_1[i]);
            boss_bullet_1[i] = NULL;
        }
    }
    for (int i = 0; i < 200; i++) {
        if(boss_bullet_2[i] != NULL && boss_bullet_2[i]->y <= 800 && !Is_Attacked_boss1(Player_main,boss_bullet_2[i])){
            boss_bullet_2[i]->y += 6;
            BossBullet2Rect.y = boss_bullet_2[i]->y;
            BossBullet2Rect.x = boss_bullet_2[i]->x;
            SDL_RenderCopy(Renderer,BossBullet2Texture,NULL,&BossBullet2Rect);
        }else if(boss_bullet_2[i] != NULL && boss_bullet_2[i]->y >= 800 ){
            free(boss_bullet_2[i]);
            boss_bullet_2[i] = NULL;
        }else if (boss_bullet_2[i] != NULL && boss_bullet_2[i]->y <= 800 && Is_Attacked_boss1(Player_main,boss_bullet_2[i])){
            if(boss_bullet_2[i]->level == 2){
                Player_main->HP -= 10;
            }else if(boss_bullet_2[i]->level == 3){
                Player_main->HP -= 20;
            }
            free(boss_bullet_2[i]);
            boss_bullet_2[i] = NULL;
        }
    }
    for (int i = 0; i < 100; i++) {
        if(bullet1[i] != NULL && Is_Bombed_boss(bullet1[i]) && boss->status == 1){
            if(bullet1[i]->level == 1){
                boss->hp -= 20;
            }else if(bullet1[i]->level == 2){
                boss->hp -= 50;
            }
            if(boss->hp <= 0)boss->status = 0;
            free(bullet1[i]);
            bullet1[i] = NULL;
        }
    }
}
void Enemy_shoot1(CreateEnemy2 *enemy){
    if(enemy->reload == 0){
        for (int i = 0; i < 100; i++) {
            if(EnemyBullet1[i] == NULL){
                EnemyBullet1[i] = CreateBullet_1();
                EnemyBullet1[i]->x = enemy->x + Enemy2Rect.w / 2 ;
                EnemyBullet1[i]->y = enemy->y + Enemy2Rect.h;
                EnemyBullet1_Rect.x = EnemyBullet1[i]->x;
                EnemyBullet1_Rect.y = EnemyBullet1[i]->y;
                SDL_RenderCopy(Renderer,EnemyBullet1_Texture,NULL,&EnemyBullet1_Rect);
                break;
            }
        }
        enemy->reload = 40;
    }else{
        enemy->reload --;
    }

}

void Enemy_shoot2(CreateEnemy3 *enemy){
    if(enemy->reload == 0){
        int cnt = 2;
        for (int i = 0; i < 100; i++) {
            if(cnt == 0){
                enemy->reload = 40;
                break;
            }
            if(EnemyBullet2[i] == NULL && cnt == 2){
                EnemyBullet2[i] = CreateBullet_2();
                EnemyBullet2[i]->x = enemy->x + Enemy3Rect.w / 4 - 10;
                EnemyBullet2[i]->y = enemy->y + Enemy3Rect.h;
                EnemyBullet2_Rect.x = EnemyBullet2[i]->x;
                EnemyBullet2_Rect.y = EnemyBullet2[i]->y;
                SDL_RenderCopy(Renderer,EnemyBullet2_Texture,NULL,&EnemyBullet2_Rect);
                cnt--;
            }else if(EnemyBullet2[i] == NULL && cnt == 1){
                EnemyBullet2[i] = CreateBullet_2();
                EnemyBullet2[i]->x = enemy->x + Enemy3Rect.w * 3 / 4 ;
                EnemyBullet2[i]->y = enemy->y + Enemy3Rect.h;
                EnemyBullet2_Rect.x = EnemyBullet2[i]->x;
                EnemyBullet2_Rect.y = EnemyBullet2[i]->y;
                SDL_RenderCopy(Renderer,EnemyBullet2_Texture,NULL,&EnemyBullet2_Rect);
                cnt--;
            }
        }
    }else{
        enemy->reload--;
    }

}

void Enemy_Bullet1_Fly(Player *Player_main){
    for (int i = 0; i < 100; i++) {
        if(EnemyBullet1[i] != NULL && EnemyBullet1[i]->y <= 800 && !Is_Attacked1(Player_main,EnemyBullet1[i])){
            EnemyBullet1[i]->y += Enemy_Bullet_Speed1 ;
            EnemyBullet1_Rect.x = EnemyBullet1[i]->x;
            EnemyBullet1_Rect.y = EnemyBullet1[i]->y;
            SDL_RenderCopy(Renderer,EnemyBullet1_Texture,NULL,&EnemyBullet1_Rect);
        }else if(EnemyBullet1[i] != NULL && EnemyBullet1[i]->y > 800){
            free(EnemyBullet1[i]);
            EnemyBullet1[i] = NULL;
        }else if(EnemyBullet1[i] != NULL && Is_Attacked1(Player_main,EnemyBullet1[i])){
            Player_main->HP -= 5;
            free(EnemyBullet1[i]);
            EnemyBullet1[i] = NULL;
        }
    }
}

void Enemy_Bullet2_Fly(Player *Player_main){
    for (int i = 0; i < 100; i++) {
        if(EnemyBullet2[i] != NULL && EnemyBullet2[i]->y <= 800 && !Is_Attacked1(Player_main,EnemyBullet2[i])){
            EnemyBullet2[i]->y += Enemy_Bullet_Speed1;
            EnemyBullet2_Rect.x = EnemyBullet2[i]->x;
            EnemyBullet2_Rect.y = EnemyBullet2[i]->y;
            SDL_RenderCopy(Renderer,EnemyBullet2_Texture,NULL,&EnemyBullet2_Rect);
        }else if(EnemyBullet2[i] != NULL && EnemyBullet2[i]->y > 800){
            free(EnemyBullet2[i]);
            EnemyBullet2[i] = NULL;
        }else if(EnemyBullet2[i] != NULL && Is_Attacked1(Player_main,EnemyBullet2[i])){
            Player_main->HP -= 10;
            free(EnemyBullet2[i]);
            EnemyBullet2[i] = NULL;
        }
    }
}

void PropsFly(Player *Player_main){
    for (int i = 0; i < 50; i++) {
        if(props[i] != NULL && props[i]->y <= 800 && !GetProps(props[i],Player_main)){
            props[i]->y += 4;
            if(props[i]->level == 1){
                Blood_Rect.x = props[i]->x;
                Blood_Rect.y = props[i]->y;
                SDL_RenderCopy(Renderer,Blood_Texture,NULL,&Blood_Rect);
            }else if(props[i]->level == 2){
                Gold_Rect.x = props[i]->x;
                Gold_Rect.y = props[i]->y;
                SDL_RenderCopy(Renderer,Gold_Texture,NULL,&Gold_Rect);
            }else{
                Sapphire_Rect.x = props[i]->x;
                Sapphire_Rect.y = props[i]->y;
                SDL_RenderCopy(Renderer,Sapphire_Texture,NULL,&Sapphire_Rect);
            }
        }else if(props[i] != NULL && props[i]->y > 800){
            free(props[i]);
            props[i] = NULL;
        } else if(props[i] != NULL && GetProps(props[i],Player_main)){
            if(props[i]->level == 1){
                Player_main->HP += 20;
                if(Player_main->HP > 100)Player_main->HP = 100;
            }else if(props[i]->level == 2){
                count_of_gold++;
            }else{
                count_of_Sapphire++;
            }
            free(props[i]);
            props[i] = NULL;
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
bool Is_Bombed1(CreateBullet *bullet,CreateEnemy2 *enemy){
    if(bullet->x >= enemy->x && bullet->x <= enemy->x + Enemy2Rect.w
       && bullet->y >= enemy->y && bullet->y <= enemy->y + Enemy2Rect.h){
        return 1;
    }else{
        return 0;
    }
}
bool Is_Bombed2(CreateBullet *bullet,CreateEnemy3 *enemy){
    if(bullet->x >= enemy->x && bullet->x <= enemy->x + Enemy3Rect.w
       && bullet->y >= enemy->y && bullet->y <= enemy->y + Enemy3Rect.h){
        return 1;
    }else{
        return 0;
    }
}

bool Is_Attacked1(Player *Player_main, CreateBullet *Enemy_bullet1){
    if(Enemy_bullet1->x >= Player_main->x && Enemy_bullet1->x <= Player_main->x + PlayerRect.w
       && Enemy_bullet1->y >= Player_main->y && Enemy_bullet1->y <= Player_main->y + PlayerRect.h){
        return 1;
    }else{
        return 0;
    }
}
bool GetProps(CreateProp *prop, Player *Player_main){
    if(prop->x >= Player_main->x && prop->x <= Player_main->x + PlayerRect.w &&
    prop->y >= Player_main->y && prop->y <= Player_main->y + PlayerRect.h){
        return 1;
    } else{
        return 0;
    }
}

bool Is_Bombed_boss(CreateBullet *bullet){
    if(bullet->x >= boss->x && bullet->x <= boss->x + BossRect.w
    && bullet->y >= boss->y && bullet->y <= boss->y + BossRect.h){
        return 1;
    }else{
        return 0;
    }
}

bool Is_Attacked_boss(Player *Player_main,CreateBullet *bullet){
    if(bullet->x >= Player_main->x && bullet->x <= Player_main->x + PlayerRect.w
       && bullet->y >= Player_main->y && bullet->y <= Player_main->y + PlayerRect.h){
        return 1;
    }else{
        return 0;
    }
}
bool Is_Attacked_boss1(Player *Player_main,CreateBullet *bullet){
    if(((bullet->x >= Player_main->x && bullet->x <= Player_main->x + PlayerRect.w - 5) || (bullet->x + BossBullet2Rect.w - 5 >= Player_main->x
    && bullet->x + BossBullet2Rect.w <= Player_main->x + PlayerRect.w))
    && bullet->y >= Player_main->y && bullet->y <= Player_main->y + PlayerRect.h){
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
        free(enemy1[i]);
        enemy1[i] = NULL;
    }
    for (int i = 0; i < 100; i++) {
        free(bullet1[i]);
        bullet1[i] = NULL;
    }
    for (int i = 0; i < 50; i++) {
        free(enemy2[i]);
        enemy2[i] = NULL;
    }
    for (int i = 0; i < 50; i++) {
        free(enemy3[i]);
        enemy3[i] = NULL;
    }
    for (int i = 0; i < 50; i++) {
        free(props[i]);
        props[i] = NULL;
    }
    for (int i = 0; i < 100; i++) {
        free(boss_bullet_1[i]);
    }
    for (int i = 0; i < 200; i++) {
        free(boss_bullet_2[i]);
    }
    plane_speed = 8;
    whether_create_enemy1 = 0;
    whether_create_enemy2 = 0;
    whether_create_enemy3 = 0;
    whether_update_plane = 0;
    whether_create_blood = 0;
    count_of_Sapphire = 0;
    count_of_gold = 0;
    free(p);
}
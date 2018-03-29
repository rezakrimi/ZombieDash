#include "Genio.h"

struct object
{
    G_Rect pos;
    G_Texture* texture;
};
enum groundType{ MID, LEFT, RIGHT, TILT, VALLEY};
struct Ground
{
    G_Rect pos;
    G_Texture* texture;
    groundType check;
};
struct bat
{
    G_Rect pos;
    G_Texture* texture;
    bool alive;
}bat;
//bat rect
G_Rect batCrop={0,0,48,58};

Ground ground[7];

enum GameState{ MENU_START, MENU_PAUSE, MENU_END, MENU_CHARACTER, PLAY, EXIT};
GameState gamestate = MENU_START;

//windows position
G_Rect winPosition = { SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 900, 505};
G_Rect backgroundPosition ={ 0, 0, 900, 505};

//hearts position
G_Rect heart2={800,50,30,30};
G_Rect heart3={750,50,30,30};

//variables
int frame=0;
long int meter=0, coinCounter=0;
char score[20];
char coinshow[20];
bool jump=false, goingDown=false;
int saveLevelTilt, level;
int jumpScale=-1;
int Glevel;
int life=3;
bool injury = false;
bool bonus = false;
bool weapon = false;
int bonusTimeGun, bonusTimeShield;
bool shieldOn = false;
int jumpHeight;
bool gunShoot = false;
bool playMusic=true;

//functions
void clear();
void initFirst();
void load();
void play();
void menuStart();
void menuPause();
void menuCharacter();
void menuEnd();
bool collision(object obj1,object obj2);
void generateGround(Ground * ground,Ground * prev);
void generateBlade(Ground * ground);
void generateBat(Ground* ground);
void generateBox(Ground* ground);
void generateCoin(Ground* ground);
void boxGet();
void jumpFun();
bool deathValley();
void bonusFinishGun();
void bonusFinishShield();
void draw();


//objects
object backGroundOne, backGroundTwo;
object menu_start_newgame, menu_start_runner, menu_start_logo;
object mouse, pause_button, SCORE, COINSHOW;
object characterBoy, characterGirl;
object menu_pause_resume, menu_pause_retry, menu_pause_quit;
object menu_end_quit, menu_end_retry, menu_end_background1, menu_end_background2, menu_end_game_over;
object heart;
object blade, blade_injury;
object box;
object bonusBackround, bonusGun;
object gun, shield;
object coin;
object fire;
object alert;
object mute;

G_Rect alertCrop={0,0,64,56};
G_Rect fireCrop = { 0, 0, 83, 75};

//runner
object runner;
G_Rect runnerCrop = { 0, 0, 40, 60};

//textures
G_Texture * menu_start_newgame1;
G_Texture * menu_start_newgame2;
G_Texture * menu_start_background;
G_Texture * road_left;
G_Texture * road_right;
G_Texture * road_mid;
G_Texture * road_tilt;
G_Texture * menu_char_chBoyB;
G_Texture * menu_char_chBoyC;
G_Texture * menu_char_chGirlB;
G_Texture * menu_char_chGirlC;
G_Texture * body_boy;
G_Texture * body_girl;

G_Font * font;
G_Font * fontCoin;

//sounds
G_Music* backgroundMusic;
G_Sound* gunfire;
G_Sound* heartLess;
G_Sound* getCoin;

int main()
{
    
    G_InitSDL();
    
    //Start up SDL and create window & renderer & first initialize & load
    G_CreateWindow("zombie dash",winPosition,0,0,0);
    load();
    initFirst();
    srand(time(NULL));
    
    //main loop
    while( gamestate != EXIT)
    {
        G_Update();
        
        switch(gamestate)
        {
            case MENU_START:menuStart();break;
            case MENU_PAUSE:menuPause();break;
            case MENU_CHARACTER:menuCharacter();break;
            case PLAY:play();break;
            case MENU_END:menuEnd();break;
        }
        
        frame++;
    }
    
    
    //clear memory
    clear();
    
    return 0;
}

bool collision(object obj1,object obj2)
{
    if(((obj1.pos.x >= obj2.pos.x) && (obj1.pos.x-obj2.pos.x)<obj2.pos.w)||
       ((obj2.pos.x >= obj1.pos.x) && ((obj2.pos.x-obj1.pos.x)<obj1.pos.w)))
    {
        if(((obj1.pos.y >= obj2.pos.y) && (obj1.pos.y-obj2.pos.y)<obj2.pos.h)||
           ((obj2.pos.y >= obj1.pos.y) && ((obj2.pos.y-obj1.pos.y)<obj1.pos.h)))
            return true;
    }
    return false;
}
bool collisionBat(G_Rect obj1,G_Rect obj2){
    if(((obj1.x >= obj2.x) && (obj1.x-obj2.x)<obj2.w)||
       ((obj2.x >= obj1.x) && ((obj2.x-obj1.x)<obj1.w)))
    {
        if(((obj1.y >= obj2.y) && (obj1.y-obj2.y)<obj2.h)||
           ((obj2.y >= obj1.y) && ((obj2.y-obj1.y)<obj1.h)))
            return true;
    }
    return false;
}
void generateBat(Ground* ground)
{
    bat.pos.y=ground->pos.y-90;
    bat.pos.x=ground->pos.x + 500;
    bat.alive=true;
    alert.pos.y=bat.pos.y;
}
void generateGround(Ground * ground,Ground * prev)
{
    ground->pos.y = prev->pos.y;
    int x=rand()%5;//x tedade halate zamin
    switch(prev->check)
    {
        case MID:
            switch(x)
        {
            case 0:ground->texture=road_mid;ground->check=MID;break;
            case 1:ground->texture=road_mid;ground->check=MID;break;
            case 2: ground->texture=road_right;ground->check=RIGHT;break;
            case 3: if ( prev->pos.y < 500-150)
            {ground->texture=road_mid;ground->check=MID;}
            else
            {ground->texture=road_right;ground->check=RIGHT;}break;
            case 4: if ( prev->pos.y < 500-150)
            {ground->texture=road_tilt;ground->check=TILT;}
            else
            {ground->texture=road_right;ground->check=RIGHT;}break;
        }break;
        case LEFT:
            switch(x)
        {
            case 0: ground->texture=road_mid;ground->check=MID;break;
            case 1: ground->texture=road_mid;ground->check=MID;break;
            case 2: ground->texture=road_right;ground->check=RIGHT;break;
            case 3: if ( prev->pos.y < 500-150)
            {ground->texture=road_mid;ground->check=MID;}
            else
            {ground->texture=road_right;ground->check=RIGHT;}break;
            case 4:if ( prev->pos.x < 500-150)
            {ground->texture=road_tilt;ground->check=TILT;}
            else
            {ground->texture=road_mid;ground->check=MID;}break;
                
        }break;
        case RIGHT:
            switch(x)
        {
            default: ground->texture=NULL;ground->check=VALLEY;ground->pos.y = prev->pos.y;
        }break;
        case TILT:
            switch(x)
        {
            case 0: ground->texture=road_mid;ground->pos.y +=90;ground->check=MID;break;
            case 1: ground->texture=road_mid;ground->pos.y +=90;ground->check=MID;break;
            case 2: ground->texture=road_mid;ground->pos.y +=90;ground->check=MID;break;
            case 3: ground->texture=road_right;ground->pos.y +=90;ground->check=RIGHT;break;
            case 4: ground->texture=road_right;ground->pos.y +=90;ground->check=RIGHT;break;
        }break;
        case VALLEY:
            switch(x)
        {
            default: ground->texture=road_left;ground->check=LEFT;ground->pos.y=rand()%180+250;break;
        }break;
    }
}
void generateBlade(Ground * ground)
{
    if( ground->check == MID )
        blade.pos = { ground->pos.x, ground->pos.y, 46, 52};
}
void generateBox(Ground* ground)
{
    if( ground->check != TILT)
        box.pos = { ground->pos.x, ground->pos.y-140, 54, 47};
}
void generateCoin(Ground* ground)
{
    if( coin.pos.x < 0 || coin.pos.x > 900)
        coin.pos = { ground->pos.x, ground->pos.y-100, 54, 47};
}
void boxGet()
{
    int temp = rand()%3;
    if( temp == 0 )
    {
        bonusTimeShield = G_GetTicks();
        G_Rect Animate = { 354, -152, 192, 152};
        bonusBackround.pos = { 900, 30, 900, 77};
        while(bonusBackround.pos.x != 0)
        {
            G_Update();
            draw();
            bonusBackround.pos.x --;
            if(Animate.y != 50)
                Animate.y ++;
            G_Draw(bonusBackround.texture,&bonusBackround.pos,false);
            G_Draw(shield.texture,&Animate,false);
            
            shieldOn = true;
        }
        
    }
    else if( life<3 && temp == 1 )
    {
        life++;
        G_Rect lifeAnimate = { 377, -120, 146, 120};
        bonusBackround.pos = { 900, 30, 900, 77};
        while(bonusBackround.pos.x != 0)
        {
            G_Update();
            draw();
            bonusBackround.pos.x --;
            if(lifeAnimate.y != 50)
                lifeAnimate.y ++;
            G_Draw(bonusBackround.texture,&bonusBackround.pos,false);
            G_Draw(heart.texture,&lifeAnimate,false);
        }
    }
    else
    {
        bonusTimeGun = G_GetTicks();
        weapon = true;
        G_Rect Animate = { 354, -152, 192, 152};
        bonusBackround.pos = { 900, 30, 900, 77};
        while(bonusBackround.pos.x != 0)
        {
            G_Update();
            draw();
            bonusBackround.pos.x --;
            if(Animate.y != 50)
                Animate.y ++;
            G_Draw(bonusBackround.texture,&bonusBackround.pos,false);
            G_Draw(bonusGun.texture,&Animate,false);
        }
    }
    G_Delay(1000);
}
void jumpFun()
{
    if(frame%2==0)
        runner.pos.y += jumpScale*3;
    if(runner.pos.y < jumpHeight)
        jumpScale = 1;
    for(int i=0; i<7 ; i++)
    {
        if(ground[i].pos.x < 90 && ground[i].pos.x > -60 && ground[i].texture != NULL)
            Glevel = ground[i].pos.y - 25;
        if(ground[i].pos.x < 90 && ground[i].pos.x > -60 && ground[i].texture == NULL)
            Glevel = 480;
    }
    if(runner.pos.y > Glevel)
    {
        jump=false;
        jumpScale = -1;
    }
}
bool deathValley()
{
    bool result = false;
    if( jump && jumpScale == -1)
        return result;
    for(int i=0 ; i<7 ; i++)
    {
        if(i==6 && ground[i].pos.x < 50 && ground[i].pos.x > -60  && ground[i].texture == NULL && (runner.pos.y >= ground[0].pos.y-25 || runner.pos.y >= ground[i].pos.y-25))
        {
            if(!jump )
                result = true;
            if( jump && 2*(ground[0].pos.y-runner.pos.y-25) < 3*(ground[0].pos.x-runner.pos.x-40))
                result =true;
        }
        else if(i!=6 && ground[i].pos.x < 50 && ground[i].pos.x > -60  && ground[i].texture == NULL && (runner.pos.y >= ground[i+1].pos.y-25 || runner.pos.y >= ground[i].pos.y-25))
        {
            if(!jump)
                result = true;
            if( jump && 2*(ground[i+1].pos.y-runner.pos.y-25) < 3*(ground[i+1].pos.x-runner.pos.x-40))
                result = true;
        }
    }
    return result;
}
void bonusFinishGun()
{
    G_Rect Animate = { 354, 50, 192, 152};
    weapon = false;
    for(int i=0 ; i < 6 ; i++)
    {
        G_Update();
        draw();
        if(i%2==0)
            G_Draw(bonusGun.texture,&Animate,false);
        G_Delay(200);
    }
}
void bonusFinishShield()
{
    G_Rect Animate = { 354, 50, 192, 152};
    shieldOn = false;
    for(int i=0 ; i < 6 ; i++)
    {
        G_Update();
        draw();
        if(i%2==0)
            G_Draw(shield.texture,&Animate,false);
        G_Delay(200);
    }
}
void load()
{
    //sounds
    backgroundMusic=G_LoadMusic("./Music/background.mp3");
    gunfire=G_LoadSound("./Music/gun_1_fire.wav");
    heartLess=G_LoadSound("./Music/injury.wav");
    getCoin=G_LoadSound("./Music/coin.wav");
    //load blade injury
    blade_injury.texture = G_LoadImage("./pic/blade_injury.png");
    
    //load background
    backGroundOne.texture=G_LoadImage("./pic/changjing3.jpg");
    backGroundTwo.texture=G_LoadImage("./pic/changjing3.jpg");
    
    //load start menu
    menu_start_newgame1 = G_LoadImage("./pic/menu_start_newgame1.png");
    menu_start_newgame2 = G_LoadImage("./pic/menu_start_newgame2.png");
    menu_start_runner.texture = G_LoadImage("./pic/menu_start_runner.png");
    menu_start_background = G_LoadImage("./pic/menu_start_background.png");
    menu_start_logo.texture = G_LoadImage("./pic/menu_start_logo.png");
    
    //load pause menu
    pause_button.texture = G_LoadImage("./pic/pause_bottom.png");
    
    //load grounds
    road_left = G_LoadImage("./pic/road_left.png");
    road_right = G_LoadImage("./pic/road_right.png");
    road_mid = G_LoadImage("./pic/road_mid.png");
    road_tilt = G_LoadImage("./pic/road_tilt.png");
    
    //load pause menu
    pause_button.texture = G_LoadImage("./pic/pause_bottom.png");
    menu_pause_resume.texture = G_LoadImage("./pic/menu_pause_resume.png");
    menu_pause_quit.texture = G_LoadImage("./pic/menu_pause_quit.png");
    menu_pause_retry.texture = G_LoadImage("./pic/menu_pause_retry.png");
    mute.texture=G_LoadImage("./pic/mute.png");
    
    //load character menu
    menu_char_chBoyB = G_LoadImage("./pic/menu_char_chBoyB.png");
    menu_char_chBoyC = G_LoadImage("./pic/menu_char_chBoyC.png");
    menu_char_chGirlB = G_LoadImage("./pic/menu_char_chGirlB.png");
    menu_char_chGirlC = G_LoadImage("./pic/menu_char_chGirlC.png");
    
    //load body
    body_boy = G_LoadImage("./pic/body_boy.png");
    body_girl = G_LoadImage("./pic/body_girl.png");
    
    //open font
    font = G_OpenFont("./arial.ttf",40);
    fontCoin = G_OpenFont("./arial.ttf",40);
    //load
    menu_end_background1.texture = G_LoadImage("./pic/menu_end_background1.png");
    menu_end_background2.texture = G_LoadImage("./pic/menu_end_background2.png");
    menu_end_game_over.texture = G_LoadImage("./pic/menu_end_game_over.png");
    
    //heart life
    heart.texture=G_LoadImage("./pic/ui_game_btn.png");
    
    //bonus
    bonusBackround.texture=G_LoadImage("./pic/bonus.png");
    bonusGun.texture = G_LoadImage("./pic/bonus_gun.png");
    //load blade
    blade.texture = G_LoadImage("./pic/blade.png");
    //bat
    bat.texture = G_LoadImage("./pic/bat.png");
    //box
    box.texture = G_LoadImage("./pic/box.png");
    //load gun
    gun.texture = G_LoadImage("./pic/gun.png");
    fire.texture = G_LoadImage("./pic/gunFire.png");
    //load shield
    shield.texture = G_LoadImage("./pic/shield.png");
    //load alert
    alert.texture=G_LoadImage("./pic/bat_alert.png");
    //load coin
    coin.texture = G_LoadImage("./pic/coin.png");
}
void initFirst()
{
    injury = false;
    weapon = false;
    shieldOn = false;
    jump = false;
    life = 3;
    meter = 0;
    coinCounter = 0;
    blade.pos = { 900,0,0,0};
    //initialize back ground
    backGroundOne.pos = { 0, 0, 900, 505};
    backGroundTwo.pos = { 900, 0, 900, 505};
    
    //initialize start menu
    menu_start_runner.pos = { 900, 75, 182, 251};
    menu_start_newgame.pos = { 303, 300, 294, 101};
    menu_start_logo.pos = { 195, -100, 511, 228};
    
    //pause menu
    pause_button.pos = { 2, 2, 47, 33};
    menu_pause_resume.pos = { 341, 30 , 218, 115};
    menu_pause_retry.pos = { 348, 50+115, 204, 113};
    menu_pause_quit.pos = { 380, 90+115+113, 140, 110};
    mute.pos={0,505-70,65,63};
    
    //ground initialize
    for(int i=0 ; i<7 ; i++)
    {
        ground[i].texture = road_mid;
        ground[i].check = MID;
    }
    
    ground[0].pos =  { 0, 505-227, 150, 250};
    ground[1].pos = { 150, 505-227, 150, 250};
    ground[2].pos = { 300, 505-227, 150, 250};
    ground[3].pos = { 450, 505-227, 150, 250};
    ground[4].pos = { 600, 505-227, 150, 250};
    ground[5].pos = { 750, 505-227, 150, 250};
    ground[6].pos = { 900, 505-227, 150, 250};
    
    //character menu
    characterBoy.texture = menu_char_chBoyB;
    characterBoy.pos = { 168, 150, 198, 218};
    characterGirl.texture = menu_char_chGirlB;
    characterGirl.pos = { 198+168+168, 150, 198, 218};
    
    //end menu
    menu_end_retry.pos = { 900-204-140, 505-113, 204, 113};
    menu_end_quit.pos = { 900-140, 505-110, 140, 110};
    menu_end_background1.pos = { 900, 20, 900, 80};
    menu_end_background2.pos = { -291, 0, 291, 66};
    menu_end_game_over.pos = { 187, -218, 525, 218};
    
    //ruuner
    runner.pos = { 50, 505-260, 50, 67};
    
    //score pos
    SCORE.pos = {800,2,100,30};
    //coin count pos
    COINSHOW.pos = {400,2,100,30};
    
    //life hearts
    heart.pos={850,50,30,30};
    
    //blade injury
    blade_injury.pos = { 0, 0, 900, 505};
    //bat
    bat.pos={-10,0,48,58};
    box.pos={0,0,54,47};
    //alert
    alert.pos={900-70,0,64,56};
    //coin
    coin.pos={0,0,57,54};
    
    
}
void draw()
{
    //drawing background
    G_Draw(backGroundOne.texture,&backGroundOne.pos);
    G_Draw(backGroundTwo.texture,&backGroundTwo.pos);
    //drawing pause button
    G_Draw(pause_button.texture,&pause_button.pos,false);
    //drawing road
    for(int i=0 ; i<7 ; i++)
        G_Draw(ground[i].texture,&ground[i].pos,false);
    //draw ruuner
    G_Draw(runner.texture,&runnerCrop,&runner.pos,false);
    //drawing heart life
    switch(life){
        case 3: {G_Draw(heart.texture,&heart.pos,false);G_Draw(heart.texture,&heart2,false);G_Draw(heart.texture,&heart3,false);};break;
        case 2: {G_Draw(heart.texture,&heart.pos,false);G_Draw(heart.texture,&heart2,false);};break;
        case 1:{G_Draw(heart.texture,&heart.pos,false);}break;
    }
    //drawing blade and box & coin
    G_Draw(blade.texture,&blade.pos,false);
    G_Draw(box.texture,&box.pos,false);
    G_Draw(coin.texture,&coin.pos,false);
    //drawing font
    G_Draw(SCORE.texture,&SCORE.pos,false);
    if(frame%200==0)
        meter++;
    G_DestroyTexture(SCORE.texture);
    G_Draw(COINSHOW.texture,&COINSHOW.pos,false);
    G_DestroyTexture(COINSHOW.texture);
    
    //
    if(injury)
        G_Draw(blade_injury.texture,&blade_injury.pos,false);
    //drawing bat
    if(bat.alive){
        G_Draw(bat.texture,&batCrop,&bat.pos,false);
    }
    if(shieldOn)
    {
        shield.pos = { runner.pos.x-30, runner.pos.y-30, 110, 140};
        G_Draw(shield.texture,&shield.pos,false);
    }
    //draw gun
    if(weapon)
    {
        gun.pos = { runner.pos.x+10, runner.pos.y+38, 42, 18};
        G_Draw(gun.texture,&gun.pos,false);
    }
    //drawing gun fire
    if(gunShoot && weapon)
    {
        G_Draw(fire.texture,&fireCrop,&fire.pos, false);
        if(frame%80==0)
            fireCrop.x+=83;
        if(fireCrop.x>=4*83){
            fireCrop.x=0;
            gunShoot=false;}
    }
}
void play()
{
    //score
    sprintf(score,"%d M",meter);
    SCORE.texture = G_LoadFont(font,score,0,0,0);
    //coin
    sprintf(coinshow,"coin : %d",coinCounter);
    COINSHOW.texture = G_LoadFont(fontCoin,coinshow,255,255,0);
    
    //moving background
    if(frame%10==0)
    {
        backGroundOne.pos.x--;
        backGroundTwo.pos.x--;
    }
    if(backGroundOne.pos.x==-900)
        backGroundOne.pos.x=900;
    if(backGroundTwo.pos.x==-900)
        backGroundTwo.pos.x=900;
    
    //moving ground
    if(frame%1==0)
    {
        for(int i=0 ; i<7 ; i++)
            ground[i].pos.x --;
        blade.pos.x --;
        box.pos.x --;
        coin.pos.x--;
    }
    for(int i=0 ; i<7 ; i++)
        if(ground[i].pos.x == -150)
        {
            ground[i].pos.x=900;
            if(i==0)
            {
                Ground * pointer2ground = &ground[0];
                Ground * pointer2groundPrev = &ground[6];
                generateGround(pointer2ground,pointer2groundPrev);
                generateBlade(pointer2ground);
                srand(time(NULL));
                int temp = rand()%3;
                if( temp == 0 )
                    generateBat(pointer2ground);
                if( temp == 1 )
                    generateBox(pointer2ground);
                if( temp == 2 )
                    generateCoin(pointer2ground);
            }
            else
            {
                Ground * pointer2ground = &ground[i];
                Ground * pointer2groundPrev = &ground[i-1];
                generateGround(pointer2ground,pointer2groundPrev);
            }
        }
    
    //ruuner
    if(frame%20==0)
    {
        runnerCrop.x += 40;
        if( runnerCrop.x == 320-40 )
            runnerCrop.x = 0;
    }
    
    //runner jump
    if(!jump)
    {
        for(int i=0;i<7;i++)
        {
            if(ground[i].pos.x==75 && ground[i].texture!=NULL && ground[i].texture!=road_tilt && !goingDown)
                runner.pos.y=ground[i].pos.y-25;
            if (ground[i].pos.x==75 && ground[i].texture==road_tilt)
            {goingDown=true;saveLevelTilt=ground[i].pos.y;}
        }
        if (goingDown)
        {
            if(frame%2==0)
                runner.pos.y+=2;
            if(runner.pos.y>saveLevelTilt+62)
            {
                goingDown=false;
                saveLevelTilt=runner.pos.y;
            }
        }
    }
    for (int i=0;i<7;i++)
    {
        if(ground[i].pos.x==70 && ground[i].texture!=NULL)
            jumpHeight=ground[i].pos.y-280;
    }
    if(jump)
        jumpFun();
    
    //blade collision
    if(collision(blade,runner) && !shieldOn)
        injury = true;
    else if(!collision(blade,runner) && !collisionBat(bat.pos,runner.pos))
    {
        if(injury == true){
            life --;
            G_PlaySound(heartLess, 0);
        }
        injury = false;
        
    }
    //bat collision
    if(collisionBat(bat.pos,runner.pos) && !shieldOn && bat.alive)
    {
        injury = true;
    }
    if(life == 0)
        gamestate = MENU_END;
    
    //box collision
    if(collision(box,runner))
    {
        bonus = true;
        box.pos.x = -100;
    }
    if(!collision(box,runner) && bonus)
    {
        boxGet();
        bonus = false;
    }
    //coin collision
    if(collision(coin,runner))
    {
        coinCounter ++;
        coin.pos.x = 900;
        G_PlaySound(getCoin, 0);
    }
    //fire
    fire.pos={ runner.pos.x+70, runner.pos.y+10, 83, 75};
    //killing bat
    if(collisionBat(fire.pos, bat.pos) && gunShoot && weapon)
    {
        bat.pos.x = -100;
        coinCounter ++;
        bat.alive=false;
    }
    //bonus time
    if( G_GetTicks() - bonusTimeGun > 15000 && weapon )
        bonusFinishGun();
    if( G_GetTicks() - bonusTimeShield > 15000 && shieldOn )
        bonusFinishShield();
    
    //valley death
    if(deathValley())
        while(runner.pos.y != 505)
        {
            G_Update();
            runner.pos.y ++;G_Delay(3);
            //drawing background
            G_Draw(backGroundOne.texture,&backGroundOne.pos);
            G_Draw(backGroundTwo.texture,&backGroundTwo.pos);
            //drawing pause button
            G_Draw(pause_button.texture,&pause_button.pos,false);
            //drawing road
            for(int i=0 ; i<7 ; i++)
                G_Draw(ground[i].texture,&ground[i].pos,false);
            //draw ruuner
            G_Draw(runner.texture,&runnerCrop,&runner.pos,false);
            
            gamestate = MENU_END;
        }
    
    //using mouse and key
    mouse.pos = { G_motion.x, G_motion.y, 5, 5};
    switch(G_Event())
    {
        case G_KEYDOWN:
        {
            switch(G_Keyboard)
            {
                case GK_LSHIFT : jump=true;break;
                case GK_ESCAPE : gamestate = MENU_PAUSE;break;
                case GK_RETURN : if(weapon)gunShoot=true;if(weapon)G_PlaySound(gunfire, 0);break;
            }
        }break;
        case G_QUIT:
            gamestate = EXIT;
            break;
        case G_MOUSEBUTTONDOWN:
            switch(G_Mouse)
        {
            case G_BUTTON_LEFT :
                if(collision(mouse,pause_button))
                    gamestate = MENU_PAUSE;break;
        }break;
    }
    
    //drawing
    draw();
    
    if(bat.alive)
    {
        if(bat.pos.x>900)
        {
            G_Draw(alert.texture,&alertCrop,&alert.pos,false);
            if( frame%100==0 && alertCrop.x==48)
                alertCrop.x=0;
            if( frame%100==50 && alertCrop.x==0)
                alertCrop.x=48;
        }
        if(frame%1==0)
            bat.pos.x-=2;
        if( frame%100==0 && batCrop.x==48)
            batCrop.x=0;
        if( frame%100==50 && batCrop.x==0)
            batCrop.x=48;
        if(bat.pos.x==-48)
            bat.alive=false;
    }
}
void menuStart()
{
    //new game bottom
    if(frame%500==0)
        menu_start_newgame.texture = menu_start_newgame1;
    else if(frame%500==250)
        menu_start_newgame.texture = menu_start_newgame2;
    
    //runner
    if( menu_start_runner.pos.x != 600 )
        if(frame%2==0)
            menu_start_runner.pos.x--;
    
    //logo
    if( menu_start_logo.pos.y != 80 )
        if(frame%2==0)
            menu_start_logo.pos.y++;
    
    //using mouse
    mouse.pos = { G_motion.x, G_motion.y, 5, 5};
    
    switch(G_Event())
    {
        case G_QUIT:
            gamestate = EXIT;
            break;
        case G_MOUSEBUTTONDOWN:
            switch(G_Mouse)
        {
            case G_BUTTON_LEFT :
                if(collision(mouse,menu_start_newgame))
                    gamestate = MENU_CHARACTER;
                break;
        }
    }
    
    //draw
    G_Draw(menu_start_background,&backgroundPosition,false);
    G_Draw(menu_start_logo.texture,&menu_start_logo.pos,false);
    G_Draw(menu_start_newgame.texture,&menu_start_newgame.pos,false);
    G_Draw(menu_start_runner.texture,&menu_start_runner.pos,false);
    
}
void menuPause()
{
    //mouse
    mouse.pos = { G_motion.x, G_motion.y, 5, 5};
    
    switch(G_Event())
    {
        case G_QUIT:
            gamestate = EXIT;
            break;
        case G_MOUSEBUTTONDOWN:
            switch(G_Mouse)
        {
            case G_BUTTON_LEFT :
            {
                if(collision(mouse,menu_pause_resume))
                    gamestate = PLAY;
                if(collision(mouse,menu_pause_quit))
                    gamestate = EXIT;
                if(collision(mouse,menu_pause_retry))
                {initFirst();gamestate = PLAY;}
                
            }break;
        }
    }
    
    G_Draw(menu_pause_resume.texture,&menu_pause_resume.pos,false);
    G_Draw(menu_pause_retry.texture,&menu_pause_retry.pos,false);
    G_Draw(menu_pause_quit.texture,&menu_pause_quit.pos,false);
    
}
void menuCharacter()
{
    //mouse
    mouse.pos = { G_motion.x, G_motion.y, 5, 5};
    
    if(collision(mouse,characterBoy))
        characterBoy.texture = menu_char_chBoyC;
    if(collision(mouse,characterGirl))
        characterGirl.texture = menu_char_chGirlC;
    if(!collision(mouse,characterBoy))
        characterBoy.texture = menu_char_chBoyB;
    if(!collision(mouse,characterGirl))
        characterGirl.texture = menu_char_chGirlB;
    
    switch(G_Event())
    {
        case G_QUIT:
            gamestate = EXIT;
            break;
        case G_MOUSEBUTTONDOWN:
            switch(G_Mouse)
        {
            case G_BUTTON_LEFT :
            {
                if(collision(mute, mouse))
                    playMusic=false;
                if(collision(mouse,characterBoy))
                {gamestate = PLAY;runner.texture=body_boy;if(playMusic)G_PlayMusic(backgroundMusic,-1);}
                if(collision(mouse,characterGirl))
                {gamestate = PLAY;runner.texture=body_girl;if(playMusic)G_PlayMusic(backgroundMusic,-1);}
            }break;
        }
    }
    
    //drawing
    G_Draw(characterBoy.texture,&characterBoy.pos,false);
    G_Draw(characterGirl.texture,&characterGirl.pos,false);
    G_Draw(mute.texture, &mute.pos,false);
}
void menuEnd()
{
    while(menu_end_background1.pos.x!=0)
    {
        G_Update();
        menu_end_background1.pos.x -= 2;
        if(menu_end_background2.pos.x!=0 )
            menu_end_background2.pos.x ++;
        if(menu_end_game_over.pos.y != 0)
            menu_end_game_over.pos.y ++;
        G_Draw(menu_end_background1.texture,&menu_end_background1.pos,false);
        G_Draw(menu_end_background2.texture,&menu_end_background2.pos,false);
        G_Draw(menu_end_game_over.texture,&menu_end_game_over.pos,false);G_Delay(1);
        
    }
    
    //mouse
    mouse.pos = { G_motion.x, G_motion.y, 5, 5};
    
    switch(G_Event())
    {
        case G_QUIT:
            gamestate = EXIT;
            break;
        case G_MOUSEBUTTONDOWN:
            switch(G_Mouse)
        {
            case G_BUTTON_LEFT :
            {
                if(collision(mouse,menu_end_retry))
                {initFirst();gamestate = PLAY;}
                if(collision(mouse,menu_end_quit))
                    gamestate = EXIT;
            }break;
        }
    }
    
    //score
    sprintf(score,"SCORE : %d M",meter);
    SCORE.texture = G_LoadFont(font,score,255,255,255);
    G_Rect hiScore = { 300, 205, 300, 50};
    //drawing font
    G_Draw(SCORE.texture,&hiScore,false);
    G_DestroyTexture(SCORE.texture);
    //coins
    sprintf(coinshow,"coins : %d",coinCounter);
    COINSHOW.texture = G_LoadFont(fontCoin,coinshow,255,255,0);
    G_Rect hiCoin = { 300, 260, 300, 50};
    //drawing font
    G_Draw(COINSHOW.texture,&hiCoin,false);
    G_DestroyTexture(COINSHOW.texture);
    
    
    G_Draw(menu_pause_retry.texture,&menu_end_retry.pos,false);
    G_Draw(menu_pause_quit.texture,&menu_end_quit.pos,false);
    G_Draw(menu_end_background1.texture,&menu_end_background1.pos,false);
    G_Draw(menu_end_background2.texture,&menu_end_background2.pos,false);
    G_Draw(menu_end_game_over.texture,&menu_end_game_over.pos,false);
    
}
void clear()
{
    //clear start menu pics
    G_DestroyTexture(menu_start_newgame2);
    G_DestroyTexture(menu_start_newgame1);
    G_DestroyTexture(menu_start_runner.texture);
    G_DestroyTexture(menu_start_background);
    G_DestroyTexture(menu_start_logo.texture);
    
    //clear play background
    G_DestroyTexture(backGroundOne.texture);
    
    G_DestroyTexture(backGroundTwo.texture);
    
    //clear road
    G_DestroyTexture(road_left);
    G_DestroyTexture(road_mid);
    G_DestroyTexture(road_tilt);
    G_DestroyTexture(road_right);
    
    //clear pause menu
    G_DestroyTexture(pause_button.texture);
    G_DestroyTexture(menu_pause_quit.texture);
    G_DestroyTexture(menu_pause_resume.texture);
    G_DestroyTexture(menu_pause_retry.texture);
    G_DestroyTexture(mute.texture);
    
    //clear chracter menu
    G_DestroyTexture(menu_char_chBoyB);
    G_DestroyTexture(menu_char_chBoyC);
    G_DestroyTexture(menu_char_chGirlB);
    G_DestroyTexture(menu_char_chGirlC);
    
    //clear menu end
    G_DestroyTexture(menu_end_background1.texture);
    G_DestroyTexture(menu_end_background2.texture);
    G_DestroyTexture(menu_end_game_over.texture);
    
    //clear runner
    G_DestroyTexture(body_boy);
    G_DestroyTexture(body_girl);
    
    //clear font
    G_DestroyTexture(SCORE.texture);
    
    //clear heart
    G_DestroyTexture(heart.texture);
    
    //clear blade
    G_DestroyTexture(blade.texture);
    G_DestroyTexture(blade_injury.texture);
    
    //clear bat
    G_DestroyTexture(bat.texture);
    
    //clear box
    G_DestroyTexture(box.texture);
    
    //clear bonus
    G_DestroyTexture(bonusBackround.texture);
    G_DestroyTexture(bonusGun.texture);
    //clear gun
    G_DestroyTexture(gun.texture);
    //clear shield
    G_DestroyTexture(shield.texture);
    //clear coin
    G_DestroyTexture(coin.texture);
    G_FreeMusic(backgroundMusic);
}

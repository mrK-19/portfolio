#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define ACTION_NUM 3
#define STATE_NUM 100
#define HEAL_MP 8
#define LIGHT_MP 5

typedef struct{
    int hp;
    int maxhp;
    int mp;
    int maxmp;
    int atk;
    int def;
    int agt;
    double light_res;  // resistance against light magic
}character;

double epsilon;
double alpha;
double _gamma;
int episode;

// physical attack from player1 to player2
void fist(character *p1, character *p2){
    int rd, bd = p1->atk / 2 - p2->def / 4;
    if(bd < 2) rd = rand()%2;
    else if(2 <= bd && bd < 9) rd = bd - rand()%3;
    else rd = floor(bd * 7 / 8.0 + (bd / 4.0 + 1) * (rand()%256) / 256.0);
    p2->hp -= rd;
    if(p2->hp < 0) p2->hp = 0;
}
// heal magic
void heal(character *p1){
    p1->mp -= HEAL_MP;
    p1->hp += 85 + rand()%16;
    if(p1->hp > p1->maxhp) p1->hp = p1->maxhp;
}

// light magic
void light(character *p1, character *p2){
    p1->mp -= LIGHT_MP;
    p2->hp -= floor((50 + rand()%16) * p2->light_res);
    if(p2->hp < 0) p2->hp = 0;
}

// quantify HP rate
int quantify(character *p){
    for(int i=0; i<10; i++){
        if(floor(p->maxhp * i/10.0) <= p->hp && p->hp < floor(p->maxhp * (i+1)/10.0)){
            return i;
        }
    }
    return 9;
}

// determine state
int state(character *p1, character *p2){
    return quantify(p1) * 10 + quantify(p2);
}

// remove action which player doesn't have MP to do
int unable(character *p){
    int tmp = 0;
    if(p->mp < HEAL_MP) tmp++;
    if(p->mp < LIGHT_MP) tmp++;
    return tmp;
}


// select action whose Q-value is largest
int argmax(character *p, int state, double qvalue[STATE_NUM][ACTION_NUM]){
    int tmp = 0;
    for(int i=1; i<ACTION_NUM - unable(p); i++){
        if(qvalue[state][i-1] < qvalue[state][i]) tmp = i;
        else if(qvalue[state][i-1] == qvalue[state][i]){
            if(rand()%2 == 0) tmp = i;
        }
    }
    return tmp;
}

// determine policy
int policy(character *p1, character *p2, double qvalue[STATE_NUM][ACTION_NUM]){
    if((double)rand()/RAND_MAX < epsilon) return rand()%(ACTION_NUM - unable(p1));
    else return argmax(p1, state(p1, p2), qvalue);
}

// update Q-value
void updateq(character *p1, character *p2, int prev_s, int curr_s, int a, double qvalue[STATE_NUM][ACTION_NUM]){
    int reward;
    if(p1->hp == 0) reward = -1;
    else if(p2->hp == 0) reward = 1;
    else reward = 0;
    double qmax = qvalue[curr_s][argmax(p1, curr_s, qvalue)];
    qvalue[prev_s][a] = (1-alpha) * qvalue[prev_s][a] + alpha * (reward + _gamma * qmax);
}

// player action
void playerturn(character *p1, character *p2, int a){
    switch(a){
        case 0:
          fist(p1, p2);
          break;
        case 1:
          heal(p1);
          break;
        case 2:
          light(p1, p2);
          break;
    }
}

// enemy action
void enemyturn(character *p1, character *p2){
    if(p2->hp < floor(p2->maxhp / 4.0) && p2->mp >= HEAL_MP){
        if(rand()%3 == 0){
            heal(p2);
            return;
        }
    }
    if(p2->mp >= LIGHT_MP){
        if(rand()%4 != 0){
            light(p2, p1);
            return;
        }
    }
    fist(p2, p1);
    return;
}

// reset the game (state)
void reset(character *p1, character *p2){
    p1->hp = p1->maxhp;
    p1->mp = p1->maxmp;
    p2->hp = p2->maxhp;
    p2->mp = p2->maxmp;
}

// make Q-table as csv file
void printq(double qvalue[STATE_NUM][ACTION_NUM], FILE *fp){
    char filename[8];
    for(int i=0; i<ACTION_NUM; i++){
        sprintf(filename, "q%d.csv", i);
        fp = fopen(filename, "w");
        for(int k=0; k<STATE_NUM; k++){
            fprintf(fp, "%lf,", qvalue[k][i]);
            if(k%10==9) fprintf(fp, "\n");
        }
        fclose(fp);
    }
}
// Q-learning
void learn(character *player, character *enemy, double qvalue[STATE_NUM][ACTION_NUM]){
    int prev_s, curr_s, a, win = 0, turn = 0, winturn = 0;
    for(int j=0; j<episode; j++){
        while(player->hp != 0 && enemy->hp != 0){
            turn++;
            prev_s = state(player, enemy);
            a = policy(player, enemy, qvalue);
            // player turn => enemy turn
            if((double)rand()/RAND_MAX < player->agt * 4 / (player->agt * 4 + enemy->agt)){
                playerturn(player, enemy, a);
                if(enemy->hp == 0) break;
                else enemyturn(player, enemy);
            }
            // enemy turn => player turn
            else{
                enemyturn(player, enemy);
                if(player->hp == 0) break;
                else playerturn(player, enemy, a);
            }
            curr_s = state(player, enemy);
            updateq(player, enemy, prev_s, curr_s, a, qvalue);
        }
        curr_s = state(player, enemy);
        updateq(player, enemy, prev_s, curr_s, a, qvalue);
        // initialize
        if(enemy->hp == 0) win++;
        winturn += turn;
        turn = 0;
        reset(player, enemy);
    }
}
// demonstration
void demo(character *player, character *enemy, double qvalue[STATE_NUM][ACTION_NUM], double *rate, double *averageturn){
    int prev_s, curr_s, a, win = 0, turn = 0, winturn = 0;
    for(int j=0; j<episode; j++){
        while(player->hp != 0 && enemy->hp != 0){
            turn++;
            prev_s = state(player, enemy);
            a = policy(player, enemy, qvalue);
            // player turn => enemy turn
            if((double)rand()/RAND_MAX < player->agt * 4 / (player->agt * 4 + enemy->agt)){
                playerturn(player, enemy, a);
                if(enemy->hp == 0) break;
                else enemyturn(player, enemy);
            }
            // enemy turn => player turn
            else{
                enemyturn(player, enemy);
                if(player->hp == 0) break;
                else playerturn(player, enemy, a);
            }
        }
        // initialize
        if(enemy->hp == 0) win++;
        winturn += turn;
        turn = 0;
        reset(player, enemy);
    }
    *rate = (double)win / episode * 100;
    *averageturn = win != 0 ? (double)winturn / win : 0;
}

int main(){ 
    double qvalue[STATE_NUM][ACTION_NUM] = {0}, rate, averageturn;
    character player = {138, 138, 123, 123, 125, 69, 82, 0.75};
    character enemy = {240, 240, 40, 40, 107, 110, 55, 1};
    FILE *fp1, *fp2;
    srand((unsigned)time(NULL));

    // learning rate（experiment 3）
    //alpha = 0.001;
    alpha = 0.01;
    //alpha = 0.1;
    //alpha = 0.5;

    // discount rate（experiment 4）
    //_gamma = 0.1;
    //_gamma = 0.5;
    _gamma = 0.9;
    //_gamma = 0.99;

    fp1 = fopen("rate.csv", "w");
    fp2 = fopen("averageturn.csv", "w");

    //random（experiment 1）
    epsilon = 1;
    episode = 1000;
    demo(&player, &enemy, qvalue, &rate, &averageturn);
    fprintf(fp1, "%lf,\n", rate);
    fprintf(fp2, "%lf,\n", averageturn);

    for(int i=1; i<=10000; i++){
        //Q-learning
        epsilon = 0.3;
        episode = i;
        learn(&player, &enemy, qvalue);
        //greedy
        epsilon = 0;
        episode = 1000;
        demo(&player, &enemy, qvalue, &rate, &averageturn);
        fprintf(fp1, "%lf,\n", rate);
        fprintf(fp2, "%lf,\n", averageturn);
    }
    fclose(fp1);
    fclose(fp2);
    return 0;
}

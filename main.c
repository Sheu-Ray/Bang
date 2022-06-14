#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<time.h>
#include"Bang_structure.h"

int draw_card[80] = {0}; //抽卡堆
int fold_card[80] = {0}; //棄卡堆
int fold_card_flag = 0;  //棄卡數量

struct CARD card[80];     //卡牌定義
struct CAREER career[16]; //角色卡定義

struct Player player[4];
int current_player_id_turn = 0; //紀錄當前回合玩家id //1~4

void initialization(); //init.c card and career

/*
char * getPosition(int position);
char * getCareer(int career);
void print_position(struct Player player1, struct Player player2, struct Player player3, struct Player player4);
void print_player(struct Player player);
*/
void print_allPlayers(struct Player player1, struct Player player2, struct Player player3, struct Player player4);
void print_hand(struct Player player);


int check_end_game(){
  //
  return 0;
}


int main(void){
  initialization();

  printf("請輸入遊玩人數(1~4) : ");
  int player_num = 0;
  char temp[10];
  //scanf("%d",&player_num);
  fgets(temp,10,stdin);
  player_num = atoi(temp);
  while(player_num>4 || player_num<1){
    printf("請輸入遊玩人數(1~4) : ");
    //scanf("%d",&player_num);
    fgets(temp,10,stdin);
    player_num = atoi(temp);
  };

  //根據遊玩人數決定是否為AI
  for( int i = 0 ;i < 4 ; i++){
    player[i].AI = ( i >= player_num );
  }

  //隨機身份 (position)
  // 1 == sheriff  警長
  // 2 == deputy sheriff  副警長
  // 3 == outlaw    歹徒
  // 4 == renegade  叛徒
  // in 4 player [1 sheriff] [2 outlaw] [1 renegade]

  srand( time(NULL) );
  int rand_num = rand() % 4;
  player[rand_num].position = 1;
  current_player_id_turn = rand_num + 1;
  player[(rand_num + ((rand() % 3)+1) ) %4 ].position = 4;

  for( int i = 0 ;i < 4 ; i++){
    if(player[i].position == 0)
      player[i].position = 3;
  }

  //隨機角色 (career)
  int for_rand_career[16] = {0};

  for( int i = 0 ;i < 4 ; i++){
    while(1){
      int career_num = rand() % 16;
      if( for_rand_career[career_num] == 0 ){
        for_rand_career[career_num] = 1;
        player[i].career = career_num;
        break;
      }
    }
  }

  //根據career決定血量health
  for( int i = 0 ;i < 4 ; i++){
    player[i].health = career[ player[i].career ].health;
    if( player[i].position == 1 ) player[i].health++;
  }

  //隨機抽卡
  for( int i = 0 ;i < 4 ; i++){
    for( int j = 0 ;j < player[i].health ; j++){ //張數
      while(1){
        int draw_card_num = rand() % 80;
        if( draw_card[draw_card_num] == 0 ){
          draw_card[draw_card_num] = 1;
          player[i].hand[draw_card_num] = 1;
          player[i].card_amount++;
          break;
        }
      }
    }
  }

  int end_game = 0;
  while( !(end_game) ){
    system("clear");
    print_allPlayers(player[0],player[1],player[2],player[3]);
    print_hand(player[current_player_id_turn-1]);

    // only for test
    // player[current_player_id_turn-1].career = 4;
    // only for test

    int normal = 1;
    if( player[current_player_id_turn-1].career == 4 ){
      while(1){
        printf("是否要用角色能力 : 從其他玩家的手牌中抽取第一張卡 ( y | n ) : ");
        char ans[3];
        fgets(ans,3,stdin);
        if(ans[0] == 'y'){
          //fix bug 如果所有其他玩家都沒有手牌
          normal = 0;
          while(1){
            printf("選擇一名其他玩家(1~4) : ");
            fgets(ans,3,stdin);
            int target_player_id = atoi(ans);
            if(target_player_id ==current_player_id_turn){
              printf("您不能選擇自己 請重新選取\n");
              sleep(2);
              continue;
            }
            if(player[target_player_id-1].card_amount != 0 ){ //持有手牌可供抽取
              while(1){
                int card_num = rand() % 80;
                if( player[target_player_id-1].hand[card_num] == 1 ){
                  player[target_player_id-1].hand[card_num] = 0;
                  player[target_player_id-1].card_amount--;

                  player[current_player_id_turn-1].hand[card_num] = 1;
                  player[current_player_id_turn-1].card_amount++;
                  break;
                }
              }
              break;
            }
            else{
              printf("此玩家沒有手牌可供抽取 請重新選擇\n");
            }
          }

          //正常抽取第二張卡
          for( int j = 0 ;j < 1 ; j++){ //張數
            while(1){
              int draw_card_num = rand() % 80;
              if( draw_card[draw_card_num] == 0 ){
                draw_card[draw_card_num] = 1;
                player[current_player_id_turn-1].hand[draw_card_num] = 1;
                break;
              }
            }
          }
          
          system("clear");
          print_allPlayers(player[0],player[1],player[2],player[3]);
          print_hand(player[current_player_id_turn-1]);
          printf("已完成使用能力抽卡 目前持有手牌如上\n");
          sleep(5);

          break;
        }
        else if(ans[0] == 'n'){
          break;
        }
      }

    }
    else if( player[current_player_id_turn-1].career == -2 ){

    }
    if( normal ){ //normal
      printf("將開始進行本回合的抽卡\n");
      sleep(2);
      for( int j = 0 ;j < 2 ; j++){ //張數
        while(1){
          int draw_card_num = rand() % 80;
          if( draw_card[draw_card_num] == 0 ){
            draw_card[draw_card_num] = 1;
            player[current_player_id_turn-1].hand[draw_card_num] = 1;
            break;
          }
        }
      }

      system("clear");
      print_allPlayers(player[0],player[1],player[2],player[3]);
      printf("本回合的抽卡已完成 持有手牌如下\n");
      print_hand(player[current_player_id_turn-1]);
      sleep(2);
    }

    //出牌階段
    while(1){
      printf("請選擇要出的卡片號碼 (0~79) [-1為結束出牌階段] : ");
      char ans_num[5];
      fgets(ans_num,4,stdin);
      int target_card_id = atoi(ans_num);
      if( target_card_id == -1 ){
        printf("\n結束出牌階段 將輪到下一位玩家的回合\n");
        sleep(3);
        break;
      }

    }


    

    current_player_id_turn = (current_player_id_turn) % 4 + 1;  //1~4
    end_game = check_end_game();
  }
  
  return 0;
}

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<time.h>
#include<poll.h>
#include"Bang_structure.h"

int draw_card[80] = {0}; //抽卡堆
int draw_card_top = -1; 
//int draw_card_top_exist = 0;
int fold_card[80] = {0}; //棄卡堆
int fold_card_flag = 0;  //棄卡數量

struct CARD card[80];     //卡牌定義
struct CAREER career[16]; //角色卡定義

struct Player player[4];
int current_player_id_turn = 0; //紀錄當前回合玩家id //1~4
char ch; //for clear stdin

void initialization(); //init.c card and career
void clear_stdin();
int get_winner(struct Player player[4]);
void normal_draw_card(int current_player_id_turn);

void Black_Jack(int current_player_id_turn);
void Pedro_Ramirez(int current_player_id_turn);
void Kit_Carlson(int current_player_id_turn);

void print_allPlayers(struct Player player[4], int viewer);
void print_hand(struct Player player);
void print_winner(struct Player player[4], int winner);
void print_allPosition(struct Player player[4]); //end of game


int check_end_game(){
  if (get_winner(player) > 0){
  	return 1;
  }
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
    print_allPlayers(player,current_player_id_turn);
    print_hand(player[current_player_id_turn-1]);

    // only for test
    //player[current_player_id_turn-1].career = 6;
    //player[0].career = 4;
    // only for test

    int normal = 1;
    
    //Black_Jack
    if(player[current_player_id_turn-1].career == 1){
    	normal = 0;
    	printf("將開始進行本回合的抽卡\n");
    	sleep(2);
    	Black_Jack(current_player_id_turn);
    }
    
    //Kit_Carlson
    if(player[current_player_id_turn-1].career == 6){
    	normal = 0;
    	printf("將開始進行本回合的抽卡\n");
    	sleep(2);
    	Kit_Carlson(current_player_id_turn);
    }
	
   //Jesse Jones	
    if( player[current_player_id_turn-1].career == 4 ){
      while(1){

        clear_stdin();
	
        printf("是否要用角色能力 : 從其他玩家的手牌中抽取第一張卡 ( y | n ) : ");
        char ans[3];
        fgets(ans,3,stdin);
        if(ans[0] == 'y'){
          //fix bug 如果所有其他玩家都沒有手牌
          normal = 0;
          while(1){
            clear_stdin();
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
	  normal_draw_card(current_player_id_turn);
          
          system("clear");
          print_allPlayers(player,current_player_id_turn);
          print_hand(player[current_player_id_turn-1]);
          printf("已完成使用能力抽卡 目前持有手牌如上\n");
          sleep(4);

          break;
        }
        else if(ans[0] == 'n'){
          break;
        }
      }

    }
    //Pedro Ramirez
    else if( player[current_player_id_turn-1].career == 9 ){ 
      if (fold_card_flag > 0){
        while (1){
          clear_stdin();
          printf("是否要使用角色能力 : 從棄牌堆中抽取第一張卡 ( y | n ) : ");
          char ans[3];
          fgets(ans, 3, stdin);
          if (ans[0] == 'y'){
    	      normal = 0;
    	      Pedro_Ramirez(current_player_id_turn);
    	      break;
          }
          else if (ans[0] == 'n'){
            break;
          }
        }
      }
    }
    else if( player[current_player_id_turn-1].career == -2 ){

    }


    if( normal ){ //normal
      printf("將開始進行本回合的抽卡\n");
      sleep(2);
      normal_draw_card(current_player_id_turn);


      system("clear");
      print_allPlayers(player,current_player_id_turn);
      printf("本回合的抽卡已完成 持有手牌如下\n");
      print_hand(player[current_player_id_turn-1]);
      sleep(3);
    }

    //出牌階段
    while(1){
      //while( (ch = getchar()) !='\n' && ch != EOF );
      //setbuf(stdin,NULL);
      //fseek(stdin,0,SEEK_END);
      //ungetc('<',stdin);
      //while( (ch = getchar()) != '<' ){
      //  printf("ch = '%c'\n",ch);
      //};
      //scanf("%*[^<]%*c");
      // clear stdin
      clear_stdin();

      printf("請選擇要出的卡片號碼 (0~79) [-1為結束出牌階段] : ");
      char ans_num[5];
      fgets(ans_num,4,stdin);
      int target_card_id = atoi(ans_num);
      if( target_card_id == -1 ){
        printf("\n結束出牌階段 將輪到下一位玩家的回合\n");
        sleep(3);
        break;
      }else{
        printf("所選擇的號碼是 %d \n",target_card_id);
        sleep(1);
      }
      //only for test: 把出牌動作當棄牌用
      /*
      if ( player[current_player_id_turn-1].hand[target_card_id] ){
        player[current_player_id_turn-1].hand[target_card_id] = 0;
        player[current_player_id_turn-1].card_amount --;
      	fold_card[fold_card_flag] = target_card_id;
      	fold_card_flag ++;
      }
      */

    }

    current_player_id_turn = (current_player_id_turn) % 4 + 1;  //1~4
    
    //only for test - kill player of specific postion
    /*
    for (int i=0; i<4; i++){
      if (player[i].position == 3 ||player[i].position == 4 ){
      player[i].health = 0;
      }
    }
    */
    
    end_game = check_end_game();
  }
  system("clear");
  print_winner(player, get_winner(player));
  print_allPosition(player);
  
  return 0;
}

// return winner's position ID(1=sheriff 3=outlaws 4=renegade)
int get_winner(struct Player player[4]){
  int sheriff = -1, outlaw1 = -1, outlaw2 = -1, renegade = -1; //player index: 0-3
  //get all position's player index
  for (int i=0; i<4; i++){
    if (player[i].position == 1){
      sheriff = i;
    }
    if (player[i].position == 3){
      if (outlaw1 < 0){
        outlaw1 = i;
      }
      else{
        outlaw2 = i;
      }
    }
    if (player[i].position == 4){
      renegade = i;
    }
  }
  //find winner
  if (player[outlaw1].health <= 0 && player[outlaw2].health <= 0 && player[renegade].health <= 0){
    return 1; //sheriff win
  }
  if (player[outlaw1].health <= 0 && player[outlaw2].health <= 0 && player[sheriff].health <= 0){
    return 4; //renegade win
  }
  if (player[sheriff].health <= 0){
    return 3; //outlaws win
  }
  return 0; //no one win
}

void Black_Jack(int current_player_id_turn){
    int ability = 0;
    for( int j = 0 ;j < 2 ; j++){ //張數
      while(1){
        int draw_card_num = rand() % 80;
        if( draw_card[draw_card_num] == 0 ){
          draw_card[draw_card_num] = 1;
          player[current_player_id_turn-1].hand[draw_card_num] = 1;
          if(card[draw_card_num].suit == 1 || card[draw_card_num].suit == 2) ability = draw_card_num;
          break;
        }
      }
    }
    system("clear");
    print_allPlayers(player,current_player_id_turn);
    printf("本回合的抽卡已完成 持有手牌如下\n");
    print_hand(player[current_player_id_turn-1]);
    if(ability){
    	while(1){
        clear_stdin();
    		printf("是否要用角色能力 :在抽牌階段，可以選擇亮出抽出的第二張牌(%3d. %s)，若該牌是紅心或方塊，可以再多抽一張牌 ( y | n ) :", ability, card[ability].name);
    		char ans[3];
    		fgets(ans,3,stdin);
    		if(ans[0] == 'y'){
    			system("clear");
    			char tmp[4];
    			printf("%3d. %s\n", ability, card[ability].name);
          		clear_stdin();
     			printf("以上是第二張手牌, 待所有玩家看到後, 按任意鍵結束顯示：");
     			fgets(tmp,3,stdin);
     			while(1){
        			int draw_card_num = rand() % 80;
        			if( draw_card[draw_card_num] == 0 ){
          			draw_card[draw_card_num] = 1;
          			player[current_player_id_turn-1].hand[draw_card_num] = 1;
          			break;
        			}
      			}
      			system("clear");
          		print_allPlayers(player,current_player_id_turn);
          		print_hand(player[current_player_id_turn-1]);
         		printf("已完成使用能力抽卡 目前持有手牌如上\n");
         		sleep(3);
         		break;
     		}
     		else if(ans[0] == 'n'){
     			break;
     		}
     	}
    }
}

void Pedro_Ramirez(int current_player_id_turn){
  fold_card_flag --;
  player[current_player_id_turn-1].hand[fold_card[fold_card_flag]] = 1;
  player[current_player_id_turn-1].card_amount ++ ;
  
    normal_draw_card(current_player_id_turn);//正常抽取第二張卡
    
    system("clear");
    print_allPlayers(player,current_player_id_turn);
    print_hand(player[current_player_id_turn-1]);
    printf("已完成使用能力抽卡 目前持有手牌如上\n");
    sleep(3);
    return;
}

void Kit_Carlson(int current_player_id_turn){
    printf("發動角色能力 :抽牌階段時，可以從遊戲牌堆上方拿三張起來看，選擇兩張，放回一張到遊戲牌堆頂端\n");
    int cardChoice[3] = {-1};
    char ans[4];
    int target_card;
    for( int j = 0 ;j < 3 ; j++){ //張數
      if(draw_card_top != -1){
      	  printf("%3d. %s \n",draw_card_top, card[draw_card_top].name);
	  cardChoice[j] = draw_card_top;
	  draw_card_top = -1;
	  continue;
      }
      while(1){
        int draw_card_num = rand() % 80;
        if( draw_card[draw_card_num] == 0 && draw_card_num != cardChoice[0] && draw_card_num != cardChoice[1] && draw_card_num != cardChoice[2]){
          printf("%3d. %s \n",draw_card_num, card[draw_card_num].name);
	  cardChoice[j] = draw_card_num;
          break;
        }
      }
    }
    printf("以上是牌堆的前三張牌, 請輸入要放回牌堆的牌的編號：");
    while(1){
    	clear_stdin();
    	fgets(ans,4,stdin);
    	target_card = atoi(ans);
    	if(target_card != cardChoice[0] && target_card != cardChoice[1] && target_card != cardChoice[2] ){
    		printf("您輸入的編號並非抽出的牌之一 請重新輸入要放回牌堆的牌的編號：");
    		continue;
    	}
    	break;
    }
    draw_card_top = target_card;
    
    for( int j = 0 ;j < 3 ; j++){ //張數
    	if(cardChoice[j] != target_card){
           player[current_player_id_turn-1].hand[cardChoice[j]] = 1;
           player[current_player_id_turn-1].card_amount ++ ;
       }
    }
    system("clear");
    print_allPlayers(player,current_player_id_turn);
    print_hand(player[current_player_id_turn-1]);
    printf("已完成使用能力抽卡 目前持有手牌如上\n");
    sleep(3);
}

void normal_draw_card(int current_player_id_turn){
      if(draw_card_top != -1){
      	 draw_card[draw_card_top] = 1;
         player[current_player_id_turn-1].hand[draw_card_top] = 1;
         draw_card_top = -1;
      }
      else{
        while(1){
          int draw_card_num = rand() % 80;
          if( draw_card[draw_card_num] == 0 ){
            draw_card[draw_card_num] = 1;
            player[current_player_id_turn-1].hand[draw_card_num] = 1;
            break;
          }
        }
       }
}

void clear_stdin(){
  //clear stdin
  while(1){
    struct pollfd pfd ={ .fd = fileno(stdin) , .events = POLLIN };
    if( poll(&pfd,1,0) > 0){
      if( pfd.revents && POLLIN ){
        // printf("stdin\n");
        ungetc(' ',stdin);
        scanf("%*[^\n]%*c");
      }
      else break;
    }
    else break;
  }
  //clear stdin
}


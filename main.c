#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<time.h>
#include<poll.h>
#include"Bang_structure.h"

int draw_card[80] = {0}; //抽卡堆
int draw_card_top = -1; 
int fold_card[80] = {0}; //棄卡堆
int fold_card_flag = 0;  //棄卡數量
void drop_card(int current_player_id_turn,int target_card_id);
void normal_draw_card(int current_player_id_turn);
void check_draw_card();

struct CARD card[80];     //卡牌定義
struct CAREER career[16]; //角色卡定義

struct Player player[4];
int current_player_id_turn = 0; //紀錄當前回合玩家id //1~4
char ch; //for clear stdin

void initialization(); //init.c card and career
void clear_stdin();
void clean_fgets_buffer(char temp[]);
int get_winner(struct Player player[4]);
void Black_Jack(int current_player_id_turn);
void Pedro_Ramirez(int current_player_id_turn);
void Kit_Carlson(int current_player_id_turn);
void Vulture_Sam();


void print_allPlayers(struct Player player[4], int viewer);
void print_hand(struct Player player);
void print_winner(struct Player player[4], int winner);
void print_allPosition(struct Player player[4]); //end of game
int left(int self); //ID: 1-4
int distance(struct Player player[4], int self, int target); // self & target: 1-4

int bang(int current_player_id_turn, int target_card_id);
int beer(int current_player_id_turn, int target_card_id);
void auto_beer();
int saloow(int current_player_id_turn, int target_card_id);
int stagecoach(int current_player_id_turn, int target_card_id);
int general_store(int current_player_id_turn, int target_card_id);
int panic(int current_player_id_turn, int target_card_id);
int cat_balou(int current_player_id_turn, int target_card_id);
int indians(int current_player_id_turn, int target_card_id);
int duel(int current_player_id_turn, int target_card_id);
int gatling(int current_player_id_turn, int target_card_id);
void gatling_each(int target_player_id);

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
  char temp[20];
  //scanf("%d",&player_num);
  fgets(temp,20,stdin);
  clean_fgets_buffer(temp);
  player_num = atoi(temp);
  while(player_num>4 || player_num<1){
    printf("請輸入遊玩人數(1~4) : ");
    clear_stdin();
    fgets(temp,20,stdin);
    clean_fgets_buffer(temp);
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
    // player[current_player_id_turn-1].career = 4;
    // only for test

    int normal = 1;
    
    //Black_Jack
    if(player[current_player_id_turn-1].career == 1){
    	normal = 0;
    	printf("將開始進行本回合的抽卡\n");
    	sleep(2);
    	Black_Jack(current_player_id_turn);
    }
    //Jesse Jones	
    else if( player[current_player_id_turn-1].career == 4 ){
      while(1){

        clear_stdin();

        printf("是否要用角色能力 : 從其他玩家的手牌中抽取第一張卡 ( y | n ) : ");
        char ans[20];
        fgets(ans,20,stdin);
        if(ans[0] == 'y'){
          //fix bug 如果所有其他玩家都沒有手牌
          normal = 0;
          while(1){
            clear_stdin();
            printf("選擇一名其他玩家(1~4) : ");
            fgets(ans,20,stdin);
            int target_player_id = atoi(ans);
            if(target_player_id ==current_player_id_turn){
              printf("您不能選擇自己 請重新選取\n");
              sleep(1);
              continue;
            }
            if(!(target_player_id>=1 && target_player_id<=4)){
              printf("請重新輸入有效值\n");
              sleep(1);
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
          sleep(3);

          break;
        }
        else if(ans[0] == 'n'){
          break;
        }
      }

    }
    //Kit_Carlson
    else if(player[current_player_id_turn-1].career == 6){
    	normal = 0;
    	printf("將開始進行本回合的抽卡\n");
    	sleep(2);
    	Kit_Carlson(current_player_id_turn);
    }
    //Pedro Ramirez
    else if( player[current_player_id_turn-1].career == 9 ){ 
      if (fold_card_flag > 0){
        while (1){
          clear_stdin();
          char ans[20];
          if (player[current_player_id_turn-1].AI == 1){
            ans[0] == 'y';
          }
          else{
            printf("是否要使用角色能力 : 從棄牌堆中抽取第一張卡 ( y | n ) : ");
            fgets(ans, 20, stdin);
          }
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


    if( normal ){ //normal
      printf("將開始進行本回合的抽卡\n");
      sleep(2);
      normal_draw_card(current_player_id_turn);
      normal_draw_card(current_player_id_turn);

      system("clear");
      print_allPlayers(player,current_player_id_turn);
      printf("\n本回合的抽卡已完成 持有手牌如下");
      print_hand(player[current_player_id_turn-1]);
      sleep(2);
    }

    //出牌階段
    int check_bang = 0; //檢查是否有用過bang
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
      system("clear");
      print_allPlayers(player,current_player_id_turn);
      printf("\n持有手牌如下");
      print_hand(player[current_player_id_turn-1]);

      printf("請選擇要出的卡片號碼 (0~79) [-1為結束出牌階段] : ");
      char ans_num[20];
      fgets(ans_num,20,stdin);
      clean_fgets_buffer(ans_num);
      int target_card_id = atoi(ans_num);
      if( target_card_id == 0 && (ans_num[0] != '0') ){
        printf("%s不是合法的輸入\n",ans_num);
        sleep(1);
        continue;
      }
      if( target_card_id > 79 || target_card_id < -1 ){
        printf("%s不是合法的輸入\n",ans_num);
        sleep(1);
        continue;
      }
      else if( target_card_id == -1 ){
        system("clear");
        print_allPlayers(player,current_player_id_turn);
        printf("\n結束出牌階段\n");
        sleep(1);
        break;
      }else{
        printf("所選擇的號碼是 %d \n",target_card_id);
        sleep(1);
      }

      // if (0){ // only for test: uncomment this line & comment next line
      if (!( player[current_player_id_turn-1].hand[target_card_id] )){
        printf("未持有這張牌 請重新輸入卡片號碼\n");
        sleep(1);
        continue;
      }

      int check_for_done_use_card = 0; //確認是否有完成使用卡片 有則要丟棄

      if( target_card_id >= 0 && target_card_id <= 24 ){ //0-24 bang
        if(check_bang == 1 \
        && player[current_player_id_turn-1].career != 15 \
        && player[current_player_id_turn-1].weapon != 72 \
        && player[current_player_id_turn-1].weapon != 73 ){
          printf("本回合已使用過bang!\n");
          sleep(1);
          continue;
        }
        check_for_done_use_card = bang(current_player_id_turn,target_card_id);
        if( check_for_done_use_card == 1 ){
          check_bang = 1;
          drop_card(current_player_id_turn,target_card_id);
        }
      }
      else if( target_card_id >= 53 && target_card_id <= 58 ){ //53-58 beer
        int alive_count = 0;
        for (int p=0; p<4; p++){
          if (player[p].health > 0){
            alive_count ++;
          }
        }
        if (alive_count <= 2){
          printf("剩下兩名玩家時無法使用此卡恢復血量(子彈)\n");
          sleep(1);
          continue;
        }

        int health_limit = career[player[current_player_id_turn-1].career].health;
        if (player[current_player_id_turn-1].position == 1){
          health_limit ++;
        }
        if (player[current_player_id_turn-1].health >= health_limit){
          printf("已達血量(子彈)上限，無法再恢復血量(子彈)\n");
          sleep(2);
          continue;
        }

        check_for_done_use_card = beer(current_player_id_turn, target_card_id);
        if (check_for_done_use_card == 1){
          drop_card(current_player_id_turn, target_card_id);
        }
      }
      else if ( target_card_id == 59 ){ //59 saloow
        check_for_done_use_card = saloow(current_player_id_turn, target_card_id);
        if (check_for_done_use_card == 1){
          drop_card(current_player_id_turn, target_card_id);
        }
      } 
      else if ( target_card_id >= 48 && target_card_id <= 50 ){ //48-49 stagecoach AND 50 wells_fargo
        check_for_done_use_card = stagecoach(current_player_id_turn, target_card_id);
        if (check_for_done_use_card == 1){
          drop_card(current_player_id_turn, target_card_id);
        }
      }
      else if ( target_card_id >= 51 && target_card_id <= 52 ){ //51-52 general store
        check_for_done_use_card = general_store(current_player_id_turn, target_card_id);
        if (check_for_done_use_card == 1){
          drop_card(current_player_id_turn, target_card_id);
        }
      }
      else if ( target_card_id >= 40 && target_card_id <= 43 ){ //40-43 panic
        int target_count = 0;
        for (int p=1; p<=4; p++){
          if (distance(player, current_player_id_turn, p) == 1){
            target_count ++;
          }
        }
        if (target_count <= 0){
          printf("no player is 1 away from you\n");
          sleep(1);
          continue;
        }
        
        check_for_done_use_card = panic(current_player_id_turn, target_card_id);
        if (check_for_done_use_card == 1){
          drop_card(current_player_id_turn, target_card_id);
        }
      }
      else if ( target_card_id >= 44 && target_card_id <= 47 ){ //44-47 cat balon
        check_for_done_use_card = cat_balou(current_player_id_turn, target_card_id);
        if (check_for_done_use_card == 1){
          drop_card(current_player_id_turn, target_card_id);
        }
      }
      else if ( target_card_id >= 38 && target_card_id <= 39 ){ //38-39 indians
        check_for_done_use_card = indians(current_player_id_turn, target_card_id);
        if (check_for_done_use_card == 1){
          drop_card(current_player_id_turn, target_card_id);
        }
      }
      else if ( target_card_id >= 60 && target_card_id <= 62 ){ //60-62 duel
        check_for_done_use_card = duel(current_player_id_turn, target_card_id);
        if (check_for_done_use_card == 1){
          drop_card(current_player_id_turn, target_card_id);
        }
      }
      else if ( target_card_id == 37 ){ //60-62 gatling
        check_for_done_use_card = gatling(current_player_id_turn, target_card_id);
        if (check_for_done_use_card == 1){
          drop_card(current_player_id_turn, target_card_id);
        }
      }
      
      // 檢查死亡 beer 自動回血;
      auto_beer();
      Vulture_Sam(); //接收卡牌
      end_game = check_end_game(); //每完成一次出牌要檢查
      if(end_game == 1) break;
    }
    if(end_game == 1) break;
    //多餘牌要棄掉
    while(player[current_player_id_turn-1].card_amount > player[current_player_id_turn-1].health){
      system("clear");
      print_allPlayers(player,current_player_id_turn);
      printf("\n持有手牌如下");
      print_hand(player[current_player_id_turn-1]);
      printf("你的手牌數量大於生命 進入棄牌階段\n");
      printf("你還需要棄掉 %d 張卡片\n",player[current_player_id_turn-1].card_amount - player[current_player_id_turn-1].health);
      sleep(1);
      printf("請選擇要棄掉的卡牌號碼 : ");
      char ans_num[20];
      clear_stdin();
      fgets(ans_num,20,stdin);
      clean_fgets_buffer(ans_num);
      int target_card_id = atoi(ans_num);
      if( target_card_id == 0 && (ans_num[0] != '0') ){
        printf("%s不是合法的輸入\n",ans_num);
        sleep(1);
        continue;
      }
      if( target_card_id > 79 || target_card_id < -1 ){
        printf("%s不是合法的輸入\n",ans_num);
        sleep(1);
        continue;
      }
      if(player[current_player_id_turn-1].hand[target_card_id] == 0){
        printf("未持有這張牌 請重新選擇\n");
        sleep(1);
        continue;
      }
      else{
        drop_card(current_player_id_turn,target_card_id);
      }
    }

    system("clear");
    print_allPlayers(player,current_player_id_turn);
    printf("你的回合已經結束 3秒後將進入下一位玩家的回合\n");
    sleep(3);

    current_player_id_turn = (current_player_id_turn) % 4 + 1;  //1~4
    while(player[current_player_id_turn-1].health <1 ){
      current_player_id_turn = (current_player_id_turn) % 4 + 1;  //1~4
    }
    
    //only for test - kill player of specific postion
    /*
    for (int i=0; i<4; i++){
      if (player[i].position == 3 ||player[i].position == 4 ){
      player[i].health = 0;
      }
    }
    end_game = check_end_game();
    if(end_game == 1) break;
    */
    //only for test - kill player of specific postion

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
        player[current_player_id_turn-1].card_amount ++ ;
        if(j == 1 && (card[draw_card_num].suit == 1 || card[draw_card_num].suit == 2)) ability = draw_card_num;
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
      char ans[20];
      fgets(ans,20,stdin);
      if(ans[0] == 'y'){
        system("clear");
        char tmp[20];
        printf("%3d. %s\n", ability, card[ability].name);
        clear_stdin();
        printf("以上是第二張手牌, 待所有玩家看到後, 按任意鍵結束顯示：");
        fgets(tmp,20,stdin);
        while(1){
          int draw_card_num = rand() % 80;
          if( draw_card[draw_card_num] == 0 ){
            draw_card[draw_card_num] = 1;
            player[current_player_id_turn-1].hand[draw_card_num] = 1;
            player[current_player_id_turn-1].card_amount ++ ;
            break;
          }
        }
        system("clear");
        print_allPlayers(player,current_player_id_turn);
        print_hand(player[current_player_id_turn-1]);
        printf("已完成使用能力抽卡 目前持有手牌如上\n");
        sleep(2);
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
  char ans[20];
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
    fgets(ans,20,stdin);
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
  return;
}

void normal_draw_card(int current_player_id_turn){
  check_draw_card();
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
  player[current_player_id_turn-1].card_amount ++ ;
}

void clean_fgets_buffer(char temp[]){
  //printf("'%s'\n",temp);
  if(temp[strlen(temp)-1] != '\n') {
    char ch;
    while(ch = getchar() != '\n');
  }  
}

void clear_stdin(){
  //clear stdin
  while(1){
    struct pollfd pfd ={ .fd = fileno(stdin) , .events = POLLIN };
    if( poll(&pfd,1,0) > 0){
      if( pfd.revents && POLLIN ){
        //printf("clear_stdin\n");
        ungetc(' ',stdin);
        scanf("%*[^\n]%*c");
      }
      else break;
    }
    else break;
  }
  //clear stdin
}
void drop_card(int current_player_id_turn,int target_card_id){
  player[current_player_id_turn-1].hand[target_card_id] = 0;
  player[current_player_id_turn-1].card_amount --;
  fold_card[fold_card_flag] = target_card_id;
  fold_card_flag ++;
}
int bang(int current_player_id_turn, int target_card_id){
  int target_player_id;
  int dis;

  while(1){
    printf("此卡片的能力為 %s 是否要使用( y | n ) : ",card[target_card_id].description);
    clear_stdin();
    char ans[20];
    fgets(ans,20,stdin);
    clean_fgets_buffer(ans);
    if(ans[0] == 'n'){
      return 0;
    }
    else if( ans[0] == 'y' ) {
      //choose player
      while(1){
        clear_stdin();
        printf("選擇一名其他玩家(1~4) : ");
        fgets(ans,20,stdin);
        clean_fgets_buffer(ans);
        target_player_id = atoi(ans);
        if(target_player_id ==current_player_id_turn){
          printf("您不能選擇自己 請重新選取\n");
          sleep(1);
          continue;
        }
        if(!(target_player_id>=1 && target_player_id<=4)){
          printf("請重新輸入有效值\n");
          sleep(1);
          continue;
        }
        if( player[target_player_id-1].health < 1 ){
          printf("目標玩家已死亡 請重新選取\n");
          sleep(1);
          continue;
        }
        break;
      }
      printf("player%d 向 player%d 使用了bang!\n",current_player_id_turn,target_player_id);
      sleep(1);
      //dis
      dis = distance(player, current_player_id_turn, target_player_id); // 1 ~ 4

      //射程 range
      int range = 1;
      if(  player[current_player_id_turn-1].weapon == 74 \
        || player[current_player_id_turn-1].weapon == 75 \
        || player[current_player_id_turn-1].weapon == 76 ){
          range = 2;
      }
      else if( player[current_player_id_turn-1].weapon == 77){
          range = 3;
      }
      else if( player[current_player_id_turn-1].weapon == 78){
          range = 4;
      }
      else if( player[current_player_id_turn-1].weapon == 79){
          range = 5;
      }

      if(dis > range){ // bug?
        printf("此名玩家在射程範圍之外 將返回出牌階段\n");
        sleep(1);
        return 0;
      }

      // missed
      int for_miss_card_num = -1;
      int missed = 1; // basic 1
      if( player[current_player_id_turn-1].career == 12){
        missed = 2;
      }
      if( player[target_player_id-1].career == 5 ){ //角色能力 自帶啤酒桶
        printf("對方為Jourdonnais 發動了角色能力！將開始進行抽牌檢定\n");
        sleep(1);
        //抽牌檢定
        check_draw_card();
        if(draw_card_top != -1){
          draw_card[draw_card_top] = 1;
          for_miss_card_num = draw_card_top;
          draw_card_top = -1;
        }
        else{
          while(1){
            int draw_card_num = rand() % 80;
            if( draw_card[draw_card_num] == 0 ){
              draw_card[draw_card_num] = 1;
              for_miss_card_num = draw_card_num;
              break;
            }
          }
        }
        if(card[for_miss_card_num].suit == 2){ //成功判定
          printf("對方使用了角色能力 成功躲過\n");
          sleep(1);
          missed--;
        }
        else{
          printf("對方使用了角色能力 判定失敗\n");
          sleep(1);
        }
        fold_card[fold_card_flag] = for_miss_card_num;
        fold_card_flag ++;
      }

      if(missed < 1){
        printf("對方幸運的躲過了你的子彈\n");
        sleep(1);
        return 1;
      }

      // 裝備啤酒桶 
      if( player[target_player_id-1].barrel != -1){
        printf("對方為有裝備啤酒桶！將開始進行抽牌檢定\n");
        sleep(1);
        //抽牌檢定
        check_draw_card();
        if(draw_card_top != -1){
          draw_card[draw_card_top] = 1;
          for_miss_card_num = draw_card_top;
          draw_card_top = -1;
        }
        else{
          while(1){
            int draw_card_num = rand() % 80;
            if( draw_card[draw_card_num] == 0 ){
              draw_card[draw_card_num] = 1;
              for_miss_card_num = draw_card_num;
              break;
            }
          }
        }
        if(card[for_miss_card_num].suit == 2){ //成功判定
          printf("對方裝備的啤酒桶 成功躲過\n");
          sleep(1);
          missed--;
        }
        else{
          if( player[target_player_id-1].career == 7){  //lucky duke
            int for_miss_card_num2 = 0;
            printf("對方為Lucky_Duke 將抽兩張卡 並選其一作為判定卡\n");
            sleep(1);
            //抽牌檢定
            check_draw_card();
            if(draw_card_top != -1){
              draw_card[draw_card_top] = 1;
              for_miss_card_num2 = draw_card_top;
              draw_card_top = -1;
            }
            else{
              while(1){
                int draw_card_num = rand() % 80;
                if( draw_card[draw_card_num] == 0 ){
                  draw_card[draw_card_num] = 1;
                  for_miss_card_num2 = draw_card_num;
                  break;
                }
              }
            }
            if(card[for_miss_card_num2].suit == 2){ //成功判定
              printf("對方裝備的啤酒桶 成功躲過\n");
              sleep(1);
              missed--;
            }
            else{
              printf("對方裝備的啤酒桶 判定失敗\n");
              sleep(1);
            }
            fold_card[fold_card_flag] = for_miss_card_num2;
            fold_card_flag ++;
          }
          else{
            printf("對方裝備的啤酒桶 判定失敗\n");
            sleep(1);
          }
        }
        fold_card[fold_card_flag] = for_miss_card_num;
        fold_card_flag ++;
      }

      if(missed < 1){
        printf("對方幸運的躲過了你的子彈\n");
        sleep(1);
        return 1;
      }
      
      //失手
      while(1){
        int have_miss = 0;
        for(int i = 25;i<37;i++){
          if( player[target_player_id-1].hand[i] ) //25-36
            have_miss++;
        }

        if(have_miss == 0){
          printf("對方未持有missed\n");
          sleep(1);
          break;
        }
        else{
          for(int i = 25;i<37;i++){
            if( player[target_player_id-1].hand[i] ) {
              printf("對方使用了失手！將開始進行抽牌檢定\n");
              sleep(1);
              //抽牌檢定
              check_draw_card();
              if(draw_card_top != -1){
                draw_card[draw_card_top] = 1;
                for_miss_card_num = draw_card_top;
                draw_card_top = -1;
              }
              else{
                while(1){
                  int draw_card_num = rand() % 80;
                  if( draw_card[draw_card_num] == 0 ){
                    draw_card[draw_card_num] = 1;
                    for_miss_card_num = draw_card_num;
                    break;
                  }
                }
              }
              if(card[for_miss_card_num].suit == 2){ //成功判定
                printf("對方使用了失手 成功躲過\n");
                sleep(1);
                missed--;
                if(missed < 1){
                  printf("對方幸運的躲過了你的子彈\n");
                  sleep(1);
                  return 1;
                }
              }

              else{
                if( player[target_player_id-1].career == 7){  //lucky duke
                  int for_miss_card_num2 = 0;
                  printf("對方為Lucky_Duke 將抽兩張卡 並選其一作為判定卡\n");
                  sleep(1);
                  //抽牌檢定
                  check_draw_card();
                  if(draw_card_top != -1){
                    draw_card[draw_card_top] = 1;
                    for_miss_card_num2 = draw_card_top;
                    draw_card_top = -1;
                  }
                  else{
                    while(1){
                      int draw_card_num = rand() % 80;
                      if( draw_card[draw_card_num] == 0 ){
                        draw_card[draw_card_num] = 1;
                        for_miss_card_num2 = draw_card_num;
                        break;
                      }
                    }
                  }
                  if(card[for_miss_card_num2].suit == 2){ //成功判定
                    printf("對方使用了失手 成功躲過\n");
                    sleep(1);
                    missed--;
                  }
                  else{
                    printf("對方使用了失手 判定失敗\n");
                    sleep(1);
                  }
                  fold_card[fold_card_flag] = for_miss_card_num2;
                  fold_card_flag ++;
                }
                else{
                  printf("對方使用了失手 判定失敗\n");
                  sleep(1);
                }
              }

              fold_card[fold_card_flag] = i;
              fold_card_flag ++;
              fold_card[fold_card_flag] = for_miss_card_num;
              fold_card_flag ++;
              player[target_player_id-1].hand[i] = 0;
              player[target_player_id-1].card_amount--;
            }

          }
        }
      }

      if( player[target_player_id-1].career == 2 ){  // bang = missed
        while(1){
          int have_miss = 0;
          for(int i = 0;i<25;i++){
            if( player[target_player_id-1].hand[i] ) //0-24
              have_miss++;
          }

          if(have_miss == 0){
            //printf("對方未持有missed\n");
            //sleep(1);
            break;
          }
          else{
            for(int i = 0;i<25;i++){
              if( player[target_player_id-1].hand[i] ) {
                printf("對方使用了bang做為失手！將開始進行抽牌檢定\n");
                sleep(1);
                //抽牌檢定
                int for_miss_card_num;
                check_draw_card();
                if(draw_card_top != -1){
                  draw_card[draw_card_top] = 1;
                  for_miss_card_num = draw_card_top;
                  draw_card_top = -1;
                }
                else{
                  while(1){
                    int draw_card_num = rand() % 80;
                    if( draw_card[draw_card_num] == 0 ){
                      draw_card[draw_card_num] = 1;
                      for_miss_card_num = draw_card_num;
                      break;
                    }
                  }
                }
                if(card[for_miss_card_num].suit == 2){ //成功判定
                  printf("對方使用了bang做為失手 成功躲過\n");
                  sleep(1);
                  missed--;
                  if(missed < 1){
                    printf("對方幸運的躲過了你的子彈\n");
                    sleep(1);
                    return 1;
                  }
                }
                else{
                  printf("對方使用了bang做為失手 判定失敗\n");
                  sleep(1);
                }
                fold_card[fold_card_flag] = i;
                fold_card_flag ++;
                fold_card[fold_card_flag] = for_miss_card_num;
                fold_card_flag ++;
                player[target_player_id-1].hand[i] = 0;
                player[target_player_id-1].card_amount--;
              }

            }
          }
        }
      }

      if(missed < 1){  // 保險
        printf("對方幸運的躲過了你的子彈\n");
        sleep(1);
        return 1;
      }
      else{
        printf("你的子彈 成功擊中對方！\n");
        sleep(2);
        player[target_player_id-1].health--;

        if(player[target_player_id-1].career == 0){  //Bart_Cassidy
          printf("對方為Bart_Cassidy 發動了角色能力[被傷害時將抽一張卡]\n");
          sleep(1);
          normal_draw_card(target_player_id);
        }
        else if(player[target_player_id-1].career == 3){  //El_Gringo
          printf("對方為El_Gringo 發動了角色能力[被傷害時可隨機從你手上抽一張卡]\n");
          sleep(1);
          if(player[current_player_id_turn-1].card_amount < 1){
            printf("你手上無卡牌可供對方抽取\n");
            return 1;
          }

          while(1){
            int draw_card_num = rand() % 80;
            if( player[current_player_id_turn-1].hand[draw_card_num] == 1 ){
              player[current_player_id_turn-1].hand[draw_card_num] = 0;
              player[target_player_id-1].hand[draw_card_num] = 1;
              break;
            }
          }
          player[current_player_id_turn-1].card_amount -- ;
          player[target_player_id-1].card_amount ++ ;
          
        }

        return 1;
      }
    }
  }
}

int beer(int current_player_id_turn, int target_card_id){
  while(1){
    printf("此卡片的能力為 %s 是否要使用( y | n ) : ",card[target_card_id].description);
    clear_stdin();
      char ans[20];
      fgets(ans,20,stdin);
      clean_fgets_buffer(ans);
      if(ans[0] == 'n'){
        return 0;
      }
      else if (ans[0] == 'y'){
        player[current_player_id_turn-1].health ++;
        printf("成功使用\n");
        sleep(1);
        return 1;
    }
  }
  return 0;
}

int saloow(int current_player_id_turn, int target_card_id){
  int health_limit = 0;
  while(1){
    printf("此卡片的能力為 %s 是否要使用( y | n ) : ",card[target_card_id].description);
    clear_stdin();
    char ans[20];
    fgets(ans,20,stdin);
    clean_fgets_buffer(ans);
      if(ans[0] == 'n'){
        return 0;
      }
      else if (ans[0] == 'y'){
        for (int i=0; i<4; i++){
          health_limit = career[player[i].career].health;
          if (player[i].position == 1){
            health_limit++;
          }
          if ( player[i].health < health_limit ){
            player[i].health++;
          }
        }
        printf("成功使用\n");
        sleep(1);
        return 1;
    }
  }
  return 0;
}

int stagecoach(int current_player_id_turn, int target_card_id){
  while(1){
    printf("此卡片的能力為 %s 是否要使用( y | n ) : ",card[target_card_id].description);
    clear_stdin();
    char ans[20];
    fgets(ans,20,stdin);
    clean_fgets_buffer(ans);
    if(ans[0] == 'n'){
      return 0;
    }else if (ans[0]=='y'){
      normal_draw_card(current_player_id_turn);
      normal_draw_card(current_player_id_turn);
      if ( target_card_id == 50 ){ //well_fargo
        normal_draw_card(current_player_id_turn);
      }
      printf("成功使用\n");
      sleep(1);
      return 1;
    }
  }
  return 0;
}

int general_store(int current_player_id_turn, int target_card_id){
  int alive_count = 0;
  int cardChoice[4];
  int choosed_card_id, choosed_card_index = -1;
  int current_chooser_id = current_player_id_turn;
  while(1){
    printf("此卡片的能力為 %s 是否要使用( y | n ) : ",card[target_card_id].description);
    clear_stdin();
    char ans[20];
    char choose[20];
    fgets(ans,20,stdin);
    clean_fgets_buffer(ans);
    if(ans[0] == 'n'){
      return 0;
    }else if (ans[0] == 'y'){
      system("clear");
      print_allPlayers(player, 0);
      // get alive player mount & initialize cardChoice
      for (int i=0; i<4; i++){
        cardChoice[i] = -17492058;
        if (player[i].health > 0){
          alive_count ++;
        }
      }
      // get cardChoice
      for (int i=0; i<alive_count; i++){
        if(draw_card_top != -1){
          draw_card[draw_card_top] = 1;
          cardChoice[i] = draw_card_top;
          draw_card_top = -1;
        }
        else{
          while(1){
            int draw_card_num = rand() % 80;
            if( draw_card[draw_card_num] == 0 ){
              draw_card[draw_card_num] = 1;
              cardChoice[i] = draw_card_num;
              break;
            }
          }
        }
      }
      // everyone choose 1 card into hand card
      for (int i=0; i<alive_count; i++){ //everyone
        printf("\n");
        for (int j=0; j<4; j++){ //cards that can be choosed
          if (cardChoice[j] >= 0){
            printf("%3d. %s\n", cardChoice[j], card[cardChoice[j]].name);
          }
        }
        while(1){ //get chosed card id
          printf("player%d, 請從以上卡牌中輸入要加入手牌的牌的編號 : ", current_chooser_id);
          clear_stdin();
          fgets(choose,20,stdin);
          clean_fgets_buffer(choose);
          choosed_card_id = atoi(choose);
          choosed_card_index = -1;
          for (int j=0; j<4; j++){
            if (choosed_card_id == cardChoice[j]){
              choosed_card_index = j;
              break;
            }
          }
          if (choosed_card_index < 0){
            printf("您輸入的編號並非抽出的牌之一\n");
            continue;
          }
          break;
        }
        // put the card into current_chooser_id's hand
        draw_card[cardChoice[choosed_card_index]] = 1;
        player[current_chooser_id-1].hand[cardChoice[choosed_card_index]] = 1;
        player[current_chooser_id-1].card_amount ++;
        cardChoice[choosed_card_index] = -17492058;
        //find next chooser
        current_chooser_id = left(current_chooser_id); 
        while(player[current_chooser_id-1].health <= 0){ //skip dead player
          current_chooser_id = left(current_chooser_id); 
        }
      }
      printf("\n成功使用\n");
      printf("全部玩家都從雜貨店中選好牌了，現在請Player%d繼續出牌\n", current_player_id_turn);
      sleep(3);
      return 1;
    }
  }
  return 0;
}

int panic(int current_player_id_turn, int target_card_id){
  int target_player_id;
  while(1){
    printf("此卡片的能力為 %s 是否要使用( y | n ) : ",card[target_card_id].description);
    clear_stdin();
    char ans[20];
    fgets(ans,20,stdin);
    clean_fgets_buffer(ans);
    if(ans[0] == 'n'){
      return 0;
    }else if (ans[0]=='y'){
      while(1){
        printf("請選擇一名與您距離為1的玩家(1~4) : ");
        clear_stdin();
        fgets(ans,20,stdin);
        clean_fgets_buffer(ans);
        target_player_id = atoi(ans);
        //check input target_player_id
        if(target_player_id ==current_player_id_turn){
          printf("您不能選擇自己 請重新選取\n");
          sleep(1);
          continue;
        }
        if(!(target_player_id>=1 && target_player_id<=4)){
          printf("請重新輸入有效值\n");
          sleep(1);
          continue;
        }
        if (player[target_player_id-1].health <= 0){
          printf("目標玩家已死亡  請選擇其他玩家\n");
          sleep(1);
          continue;
        }
        if(distance(player, current_player_id_turn, target_player_id) != 1){
          printf("您只能選擇與您距離為1的玩家 請重新選取\n");
          sleep(1);
          continue;
        }
        
        //only for test
        /*
        player[target_player_id-1].weapon = 79;
        player[target_player_id-1].scope = 65;
        player[target_player_id-1].dynamic = 71;
        */
        //only for test
        
        system("clear");
        print_allPlayers(player, current_player_id_turn);
        printf("\n以下是 Player%d 擁有的牌 : \n", target_player_id);
        
        //get target_player_id's cards
        int showed_card[81] = {0}; //80 showed cards + nshowed hand card
        if (player[target_player_id-1].weapon > -1){
          showed_card[player[target_player_id-1].weapon] = 'w';
          printf("%3d. %s\n", player[target_player_id-1].weapon, card[player[target_player_id-1].weapon].name);
        }
        if (player[target_player_id-1].hourse > -1){
          showed_card[player[target_player_id-1].hourse] = 'h';
          printf("%3d. %s\n", player[target_player_id-1].hourse, card[player[target_player_id-1].hourse].name);
        }
        if (player[target_player_id-1].dynamic > -1){
          showed_card[player[target_player_id-1].dynamic] = 'd';
          printf("%3d. %s\n", player[target_player_id-1].dynamic, card[player[target_player_id-1].dynamic].name);
        }
        if (player[target_player_id-1].scope > -1){
          showed_card[player[target_player_id-1].scope] = 's';
          printf("%3d. %s\n", player[target_player_id-1].scope, card[player[target_player_id-1].scope].name);
        }
        if (player[target_player_id-1].barrel > -1){
          showed_card[player[target_player_id-1].barrel] = 'b';
          printf("%3d. %s\n", player[target_player_id-1].barrel, card[player[target_player_id-1].barrel].name);
        }
        if (player[target_player_id-1].card_amount > 0){
          showed_card[80] = 'c'; //cards
          printf(" 80. Randomly choose from the hand cards.\n");
        }
        //get user's choice
        int choosed_card_id;
        while(1){
          printf("請輸入想要的牌的號碼 : ");
          clear_stdin();
          fgets(ans,20,stdin);
          choosed_card_id = atoi(ans);
          if (choosed_card_id < 0 || choosed_card_id > 80){
            printf("請重新輸入有效值\n");
            sleep(1);
            continue;
          }
          if (showed_card[choosed_card_id] <= 0){
            printf("您輸入的編號並非可選的牌之一\n");
            sleep(1);
            continue;
          }
          // current player get the card from target player
          if(choosed_card_id < 80){ // hourse. weapon...
            if (showed_card[choosed_card_id] == 'w'){
              player[target_player_id-1].weapon = -1;
            }
            if (showed_card[choosed_card_id] == 'h'){
              player[target_player_id-1].hourse = -1;
            }
            if (showed_card[choosed_card_id] == 'd'){
              player[target_player_id-1].dynamic = -1;
            }
            if (showed_card[choosed_card_id] == 's'){
              player[target_player_id-1].scope = -1;
            }
            if (showed_card[choosed_card_id] == 'b'){
              player[target_player_id-1].barrel = -1;
            }
            player[current_player_id_turn-1].hand[choosed_card_id] = 1;
            player[current_player_id_turn-1].card_amount ++;
          }
          else{ //hand cards...
            while(1){
              int draw_card_num = rand() % 80;
              if( player[target_player_id-1].hand[draw_card_num] == 1 ){
                player[target_player_id-1].hand[draw_card_num] = 0;
                player[target_player_id-1].card_amount --;
                player[current_player_id_turn-1].hand[draw_card_num] = 1;
                player[current_player_id_turn-1].card_amount ++;
                break;
              }
            }
          }
          break;
        }
        printf("成功使用\n");
        sleep(1);
      }
      return 1;
    }
  }
  return 0;
}

int cat_balou(int current_player_id_turn, int target_card_id){
  int target_player_id;
  int target_player_card[80] = {0};

  while(1){
    printf("此卡片的能力為 %s 是否要使用( y | n ) : ",card[target_card_id].description);
    clear_stdin();
    char ans[20];
    fgets(ans,20,stdin);
    clean_fgets_buffer(ans);
    if(ans[0] == 'n'){
      return 0;
    }else if (ans[0]=='y'){
      while(1){
        //get target player ID
        printf("請選擇一名玩家(1~4) : ");
        clear_stdin();
        fgets(ans,20,stdin);
        clean_fgets_buffer(ans);
        target_player_id = atoi(ans);
        if ( target_player_id < 1 || target_player_id > 4 ){
          printf("請重新輸入有效值\n");
          sleep(1);
          continue;
        }
        if ( target_player_id == current_player_id_turn ){
          printf("您不能選擇自己 請重新選取\n");
          sleep(1);
          continue;
        }
        if ( player[target_player_id-1].health <= 0 ){
          printf("目標玩家已死亡 請選擇其他玩家\n");
          sleep(1);
          continue;
        }
        printf("\nPlayer%d 必須選出一張牌放棄...\n", target_player_id);
        break;
      }
      sleep(3);
      
      //only for test
      /*
      player[target_player_id-1].weapon = 79;
      player[target_player_id-1].scope = 65;
      player[target_player_id-1].dynamic = 71;
      */
      //only for test
      
      //target_player's turn
      system("clear");
      print_allPlayers(player, target_player_id);
      //print target_player's card
      printf("\n您是Player%d，您擁有的牌有：\n", target_player_id);
      for (int i=0; i<80; i++){
        target_player_card[i] = player[target_player_id-1].hand[i];
        if (target_player_card[i] ){
          printf("%3d. %s\n", i, card[1].name);
        }
      }
      printf("\n");
      if (player[target_player_id-1].weapon > 0){
        target_player_card[player[target_player_id-1].weapon] = 'w';
        printf("%3d. %s\n", player[target_player_id-1].weapon, card[player[target_player_id-1].weapon].name);
      }
      if (player[target_player_id-1].hourse > 0){
        target_player_card[player[target_player_id-1].hourse] = 'h';
        printf("%3d. %s\n", player[target_player_id-1].hourse, card[player[target_player_id-1].hourse].name);
      }
      if (player[target_player_id-1].dynamic > 0){
        target_player_card[player[target_player_id-1].dynamic] = 'd';
        printf("%3d. %s\n", player[target_player_id-1].dynamic, card[player[target_player_id-1].dynamic].name);
      }
      if (player[target_player_id-1].scope > 0){
        target_player_card[player[target_player_id-1].scope] = 's';
        printf("%3d. %s\n", player[target_player_id-1].scope, card[player[target_player_id-1].scope].name);
      }
      if (player[target_player_id-1].barrel > 0){
        target_player_card[player[target_player_id-1].barrel] = 'b';
        printf("%3d. %s\n", player[target_player_id-1].barrel, card[player[target_player_id-1].barrel].name);
      }      
      //target_player choose the card
      printf("Player%d，請從以上選擇一張牌放棄", target_player_id);
      int choosed_card_id;
      while(1){
        clear_stdin();
        char ans[20];
        fgets(ans,20,stdin);
        clean_fgets_buffer(ans);
        choosed_card_id = atoi(ans);
        if ( choosed_card_id < 0 || choosed_card_id > 79 ){
          printf("請重新輸入有效值\n");
          sleep(1);
          continue;
        }
        if ( target_player_card[choosed_card_id] == 0 ){
          printf("您並未擁有這張牌 請重新選擇\n");
          sleep(1);
          continue;
        }
        // pick target_card_id from target_player_id
        if ( target_player_card[choosed_card_id] == 1 ){//hand card
          drop_card(target_player_id, choosed_card_id);
        }
        else{
          if ( target_player_card[choosed_card_id] == 'w' ){
            player[target_player_id-1].weapon = -1;
          }
          if ( target_player_card[choosed_card_id] == 'h' ){
            player[target_player_id-1].hourse = -1;
          }
          if ( target_player_card[choosed_card_id] == 'd' ){
            player[target_player_id-1].dynamic = -1;
          }
          if ( target_player_card[choosed_card_id] == 's' ){
            player[target_player_id-1].scope = -1;
          }
          if ( target_player_card[choosed_card_id] == 'b' ){
            player[target_player_id-1].barrel = -1;
          }
          fold_card[fold_card_flag] = choosed_card_id;
          fold_card_flag ++;
        }
        break;
      }
      printf("\n成功使用\n");
      printf("現在請Player%d繼續出牌\n", current_player_id_turn);
      sleep(3);
      break;
    }
    return 1;
  } 
  return 0;
}

int indians(int current_player_id_turn, int target_card_id){
  while(1){
    printf("此卡片的能力為 %s 是否要使用( y | n ) : ",card[target_card_id].description);
    clear_stdin();
    char ans[20];
    fgets(ans,20,stdin);
    clean_fgets_buffer(ans);
    if (ans[0] == 'n'){
      return 0;
    }else if(ans[0] == 'y'){
      printf("\n");
      for (int i=0; i<4; i++){ //each player
        if (player[i].health > 0 && i != current_player_id_turn-1){
          int bang_to_drop = -1;
          for (int j=0; j<=24; j++){
            if (player[i].hand[j] == 1){
              bang_to_drop = j;
              break;
            }
          }
          if (bang_to_drop >= 0){ //drop that bang
            drop_card(i+1, bang_to_drop);
            printf("Player%d 丟出一張Bang\n", i+1);
            sleep(1);
          }
          else{ //-1 health
            player[i].health --;
            printf("Player%d 並未持有Bang，因此失去1血量(子彈)\n", i+1);
            sleep(1);
          }
        }
      }
      printf("\n成功使用\n");
      sleep(3);
      break;
    }
    return 1;
  }
  return 0;
}

int duel(int current_player_id_turn, int target_card_id){
  int target_player_id;
  while(1){
    printf("此卡片的能力為 %s 是否要使用( y | n ) : ",card[target_card_id].description);
    clear_stdin();
    char ans[20];
    fgets(ans,20,stdin);
    clean_fgets_buffer(ans);
    if (ans[0] == 'n'){
      return 0;
    }else if(ans[0] == 'y'){
      while(1){
        //get target player ID
        printf("請選擇一名玩家(1~4) :  ");
        clear_stdin();
        fgets(ans,20,stdin);
        clean_fgets_buffer(ans);
        target_player_id = atoi(ans);
        if ( target_player_id < 1 || target_player_id > 4 ){
          printf("請重新輸入有效值\n");
          sleep(1);
          continue;
        }
        if ( target_player_id == current_player_id_turn ){
          printf("您不能選擇自己 請重新選取\n");
          sleep(1);
          continue;
        }
        if ( player[target_player_id-1].health <= 0 ){
          printf("目標玩家已死亡  請選擇其他玩家\n");
          sleep(1);
          continue;
        }
        
        // get 2 player's bang IDs
        int current_bang[25], target_bang[25];
        int current_bang_index = 0, target_bang_index = 0;
        for (int i=0; i<25; i++){
          current_bang[i] = -1;
          target_bang[i] = -1;
        }
        for (int i=0; i<25; i++){
          if (player[current_player_id_turn-1].hand[i] == 1){
            current_bang[current_bang_index] = i;
            current_bang_index ++;
          }
          if (player[target_player_id-1].hand[i] == 1){
            target_bang[target_bang_index] = i;
            target_bang_index ++;
          }
        }
        // duel 
        printf("\n");
        for (int i=0; i<25; i++){
          //target_player's turn
          if (target_bang[i] >= 0){
            printf("Player%d 丟出一張Bang\n", target_player_id);
            drop_card(target_player_id, target_bang[i]);
            sleep(1);
          }
          else{
            printf("Player%d 沒有Bang了，因此失去1血量(子彈)，決鬥結束\n", target_player_id);
            player[target_player_id-1].health --;
            sleep(1);
            break;
          }
          //current_player's turn
          if (current_bang[i] >= 0){
            printf("Player%d 丟出一張Bang\n", current_player_id_turn);
            drop_card(current_player_id_turn, current_bang[i]);
            sleep(1);
          }
          else{
            printf("Player%d 沒有Bang了，因此失去1血量(子彈)，決鬥結束\n", current_player_id_turn);
            player[current_player_id_turn-1].health --;
            sleep(1);
            break; 
          }
        }
        break;
      }
      printf("\n成功使用\n");
      sleep(3);
      break;
    }
    return 1;
  }
  return 0;
}

int gatling(int current_player_id_turn, int target_card_id){
  int target_player_id;
  while(1){
    printf("此卡片的能力為 %s 是否要使用( y | n ) : ",card[target_card_id].description);
    clear_stdin();
    char ans[20];
    fgets(ans,20,stdin);
    clean_fgets_buffer(ans);
    if (ans[0] == 'n'){
      return 0;
    }
    else if (ans[0] == 'y'){
      for (int i=1; i<=4; i++ ){ //attack every other players
        if ( i != current_player_id_turn && player[i-1].health > 0 ){
          target_player_id = i;
          gatling_each(target_player_id);
        }
      }
      
      printf("\n成功使用\n");
      sleep(3);
      break;
    }
  }
	return 0;
}

void check_draw_card(){
  int card_amount = 0;
  for(int i=0;i<80;i++)
    if(draw_card[i] == 0 )
      card_amount++;

  if( card_amount < 3){
    for(int i=0;i<fold_card_flag;i++){
      draw_card[fold_card[i]] = 0;
      fold_card[i] = 0;
    }
    fold_card_flag = 0;
  }
}

void Vulture_Sam(){
  int player_Vulture_Sam = -1;
  for(int i=0;i<4;i++){
    if(player[i].career == 14){
      player_Vulture_Sam = i;
      break;
    }
  }
  if(player_Vulture_Sam == -1) return;

  if(player[player_Vulture_Sam].health < 1) return;

  int check_die = -1;
  for(int i=0;i<4;i++){
    if(player[i].health < 1 && player[i].Vulture_Sam == 0){
      player[i].Vulture_Sam = 1;
      printf("player%d 發動了角色能力 [ 當一位玩家死亡時，接收該死亡玩家的手牌和場上的裝備牌到自己的手牌中。 ]\n",player_Vulture_Sam+1);
      printf("拿走了死者所有持有的卡片\n");

      if( player[i].weapon != -1 ){
        player[player_Vulture_Sam].hand[ player[i].weapon ] = 1;
        player[player_Vulture_Sam].card_amount++;
      }
      if( player[i].hourse != -1 ){
        player[player_Vulture_Sam].hand[ player[i].hourse ] = 1;
        player[player_Vulture_Sam].card_amount++;
      }
      if( player[i].jail != -1 ){
        player[player_Vulture_Sam].hand[ player[i].jail ] = 1;
        player[player_Vulture_Sam].card_amount++;
      }
      if( player[i].dynamic != -1 ){
        player[player_Vulture_Sam].hand[ player[i].dynamic ] = 1;
        player[player_Vulture_Sam].card_amount++;
      }
      if( player[i].scope != -1 ){
        player[player_Vulture_Sam].hand[ player[i].scope ] = 1;
        player[player_Vulture_Sam].card_amount++;
      }
      if( player[i].barrel != -1 ){
        player[player_Vulture_Sam].hand[ player[i].barrel ] = 1;
        player[player_Vulture_Sam].card_amount++;
      }

      if(player[i].card_amount > 0){
        for(int j = 0;j<80;j++){
          if(player[i].hand[j] == 1){
            player[i].hand[j] = 0;
            player[i].card_amount--;
            player[player_Vulture_Sam].hand[j] = 1;
            player[player_Vulture_Sam].card_amount++;

          }
        }
      }
      sleep(2);
    }
  }
}

void auto_beer(){
  for(int i=0;i<4;i++){
    if(player[i].health < 1){
      for(int j=53;j<58;j++){
        if(player[i].hand[j] != 0){
          player[i].health++;
          drop_card(i+1,j);
          printf("player%d 遭受致命傷害 使用了啤酒回到1點血量\n",i+1);
          sleep(2);
          break;
        }
      }
    }
  }
}

void gatling_each(int target_player_id){
  // missed
  int for_miss_card_num = -1;
  int missed = 1; // basic 1
  
  // 裝備啤酒桶 
  if( player[target_player_id-1].barrel != -1){
    printf("Player%d 有裝備啤酒桶！將開始進行抽牌檢定\n", target_player_id);
    sleep(1);
    //抽牌檢定
    check_draw_card();
    if(draw_card_top != -1){
      draw_card[draw_card_top] = 1;
      for_miss_card_num = draw_card_top;
      draw_card_top = -1;
    }
    else{
      while(1){
        int draw_card_num = rand() % 80;
        if( draw_card[draw_card_num] == 0 ){
          draw_card[draw_card_num] = 1;
          for_miss_card_num = draw_card_num;
          break;
        }
      }
    }
    if(card[for_miss_card_num].suit == 2){ //成功判定
      printf("Player%d 裝備的啤酒桶 成功躲過\n", target_player_id);
      sleep(1);
      missed--;
    }
    else{
      if( player[target_player_id-1].career == 7){  //lucky duke
        int for_miss_card_num2 = 0;
        printf("Player%d 為Lucky_Duke 將抽兩張卡 並選其一作為判定卡\n", target_player_id);
        sleep(1);
        //抽牌檢定
        check_draw_card();
        if(draw_card_top != -1){
          draw_card[draw_card_top] = 1;
          for_miss_card_num2 = draw_card_top;
          draw_card_top = -1;
        }
        else{
          while(1){
            int draw_card_num = rand() % 80;
            if( draw_card[draw_card_num] == 0 ){
              draw_card[draw_card_num] = 1;
              for_miss_card_num2 = draw_card_num;
              break;
            }
          }
        }
        if(card[for_miss_card_num2].suit == 2){ //成功判定
          printf("Player%d 裝備的啤酒桶 成功躲過\n", target_player_id);
          sleep(1);
          missed--;
        }
        else{
          printf("Player%d 裝備的啤酒桶 判定失敗\n", target_player_id);
          sleep(1);
        }
        fold_card[fold_card_flag] = for_miss_card_num2;
        fold_card_flag ++;
      }
      else{
        printf("Player%d 裝備的啤酒桶 判定失敗\n", target_player_id);
        sleep(1);
      }
    }
    fold_card[fold_card_flag] = for_miss_card_num;
    fold_card_flag ++;
  }

  if(missed < 1){
    printf("Player%d 幸運的躲過了你的子彈\n", target_player_id);
    sleep(1);
    return;
  }
  
  //失手
  while(1){
    int have_miss = 0;
    for(int i = 25;i<37;i++){
      if( player[target_player_id-1].hand[i] ) //25-36
        have_miss++;
    }

    if(have_miss == 0){
      printf("Player%d 未持有missed\n", target_player_id);
      sleep(1);
      break;
    }
    else{
      for(int i = 25;i<37;i++){
        if( player[target_player_id-1].hand[i] ) {
          printf("Player%d 使用了失手！將開始進行抽牌檢定\n", target_player_id);
          sleep(1);
          //抽牌檢定
          check_draw_card();
          if(draw_card_top != -1){
            draw_card[draw_card_top] = 1;
            for_miss_card_num = draw_card_top;
            draw_card_top = -1;
          }
          else{
            while(1){
              int draw_card_num = rand() % 80;
              if( draw_card[draw_card_num] == 0 ){
                draw_card[draw_card_num] = 1;
                for_miss_card_num = draw_card_num;
                break;
              }
            }
          }
          if(card[for_miss_card_num].suit == 2){ //成功判定
            printf("Player%d 使用了失手 成功躲過\n", target_player_id);
            sleep(1);
            missed--;
            if(missed < 1){
              printf("Player%d 幸運的躲過了你的子彈\n", target_player_id);
              sleep(1);
              return;
            }
          }

          else{
            if( player[target_player_id-1].career == 7){  //lucky duke
              int for_miss_card_num2 = 0;
              printf("Player%d 為Lucky_Duke 將抽兩張卡 並選其一作為判定卡\n", target_player_id);
              sleep(1);
              //抽牌檢定
              check_draw_card();
              if(draw_card_top != -1){
                draw_card[draw_card_top] = 1;
                for_miss_card_num2 = draw_card_top;
                draw_card_top = -1;
              }
              else{
                while(1){
                  int draw_card_num = rand() % 80;
                  if( draw_card[draw_card_num] == 0 ){
                    draw_card[draw_card_num] = 1;
                    for_miss_card_num2 = draw_card_num;
                    break;
                  }
                }
              }
              if(card[for_miss_card_num2].suit == 2){ //成功判定
                printf("Player%d 使用了失手 成功躲過\n", target_player_id);
                sleep(1);
                missed--;
              }
              else{
                printf("Player%d 使用了失手 判定失敗\n", target_player_id);
                sleep(1);
              }
              fold_card[fold_card_flag] = for_miss_card_num2;
              fold_card_flag ++;
            }
            else{
              printf("Player%d 使用了失手 判定失敗\n", target_player_id);
              sleep(1);
            }
          }

          fold_card[fold_card_flag] = i;
          fold_card_flag ++;
          fold_card[fold_card_flag] = for_miss_card_num;
          fold_card_flag ++;
          player[target_player_id-1].hand[i] = 0;
          player[target_player_id-1].card_amount--;
        }

      }
    }
  }

  if( player[target_player_id-1].career == 2 ){  // bang = missed
    while(1){
      int have_miss = 0;
      for(int i = 0;i<25;i++){
        if( player[target_player_id-1].hand[i] ) //0-24
          have_miss++;
      }

      if(have_miss == 0){
        //printf("對方未持有missed\n");
        //sleep(1);
        break;
      }
      else{
        for(int i = 0;i<25;i++){
          if( player[target_player_id-1].hand[i] ) {
            printf("Player%d 使用了bang做為失手！將開始進行抽牌檢定\n",target_player_id);
            sleep(1);
            //抽牌檢定
            int for_miss_card_num;
            check_draw_card();
            if(draw_card_top != -1){
              draw_card[draw_card_top] = 1;
              for_miss_card_num = draw_card_top;
              draw_card_top = -1;
            }
            else{
              while(1){
                int draw_card_num = rand() % 80;
                if( draw_card[draw_card_num] == 0 ){
                  draw_card[draw_card_num] = 1;
                  for_miss_card_num = draw_card_num;
                  break;
                }
              }
            }
            if(card[for_miss_card_num].suit == 2){ //成功判定
              printf("Player%d 使用了bang做為失手 成功躲過\n", target_player_id);
              sleep(1);
              missed--;
              if(missed < 1){
                printf("Player%d 幸運的躲過了你的子彈\n", target_player_id);
                sleep(1);
                return;
              }
            }
            else{
              printf("Player%d 使用了bang做為失手 判定失敗\n", target_player_id);
              sleep(1);
            }
            fold_card[fold_card_flag] = i;
            fold_card_flag ++;
            fold_card[fold_card_flag] = for_miss_card_num;
            fold_card_flag ++;
            player[target_player_id-1].hand[i] = 0;
            player[target_player_id-1].card_amount--;
          }

        }
      }
    }
  }

  if(missed < 1){  // 保險
    printf("Player%d 幸運的躲過了你的子彈\n", target_player_id);
    sleep(1);
    return;
  }
  else{
    printf("你的子彈 成功擊中Player%d！\n", target_player_id);
    sleep(2);
    player[target_player_id-1].health--;

    if(player[target_player_id-1].career == 0){  //Bart_Cassidy
      printf("Player%d 為Bart_Cassidy 發動了角色能力[被傷害時將抽一張卡]\n", target_player_id);
      sleep(1);
      normal_draw_card(target_player_id);
    }
    else if(player[target_player_id-1].career == 3){  //El_Gringo
      printf("Player%d 為El_Gringo 發動了角色能力[被傷害時可隨機從你手上抽一張卡]\n", target_player_id);
      sleep(1);
      if(player[current_player_id_turn-1].card_amount < 1){
        printf("你手上無卡牌可供對方抽取\n");
        return;
      }

      while(1){
        int draw_card_num = rand() % 80;
        if( player[current_player_id_turn-1].hand[draw_card_num] == 1 ){
          player[current_player_id_turn-1].hand[draw_card_num] = 0;
          player[target_player_id-1].hand[draw_card_num] = 1;
          break;
        }
      }
      player[current_player_id_turn-1].card_amount -- ;
      player[target_player_id-1].card_amount ++ ;
      
    }
    return;
  }
}

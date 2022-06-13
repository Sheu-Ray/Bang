//********************************************************/
// 目前 印出裝備部分 要等card[]資料全部輸入完成才能生效
// (現階段是在main塞一兩個card.name測試用) 
//
// getPosition和 getCareer 和main()是現階段測試成果用
// 等身分角色結構確定&資料輸入完之後，再改用struct存(同card呼叫法) 
// 
// 不會變動的function有:(93-191行)
// print_allPlayers、print_player、print_hand、print_position 
//********************************************************/
#include <stdio.h>
#include <string.h>

struct CARD{
	int ID;
	char name[16];
	int suit;
	int rank;
};

struct Player{
	int player_ID;
	int health;
	int AI;
	int position; //身分
	int career; //職業 
	int weapon; //武器 
	int hourse; //馬 
	int jail; //監獄  
	int dynamic; //炸彈 
	int scope; //瞄準鏡   
	int barrel; //酒桶 
	int hand[80]; //手牌 
};

struct CARD card[80];

char * getPosition(int position){
	switch(position){
		case 1:
			return "Sceriffo";
		case 2:
			return "Fuorilegge";
		case 3:
			return "Rinnegato";
		default:
			return NULL;
	}
	return NULL;
}

char * getCareer(int career){
	switch(career){
		case 0:
			return "Bart Cassidy";
		case 1:
			return "Black Jack";
		case 2:
			return "Calamity Janet";
		case 3:
			return "El Gringo";
		case 4:
			return "Jesse Jones";
		case 5:
			return "Jourdonnais";
		case 6: 
			return "Kit Carlson";
		case 7:
			return "Lucky Duke";
		case 8:
			return "Paul Regret";
		case 9:
			return "Pedro Ramirez";
		case 10:
			return "Rose Doolan";
		case 11:
			return "Sid Ketchum";
		case 12:
			return "Slab the Killer";
		case 13:
			return "Suzy Lafayette";
		case 14:
			return "Vulture Sam";
		case 15:
			return "Willy the Kid";
		default:
			return NULL;
	}
		
	return NULL;
}

void print_position(struct Player player1, struct Player player2, struct Player player3, struct Player player4){
	if (player1.position == 1 || player1.health <= 0){
		printf("< Player1 is %s >\n", getPosition(player1.position));
	}
	if (player2.position == 1 || player2.health <= 0){
		printf("< Player2 is %s >\n", getPosition(player2.position));
	}
	if (player3.position == 1 || player3.health <= 0){
		printf("< Player3 is %s >\n", getPosition(player3.position));
	}
	if (player4.position == 1 || player4.health <= 0){
		printf("< Player4 is %s >\n", getPosition(player4.position));
	}
	return;
}

void print_player(struct Player player){
	//player_ID
	printf("Player%d ", player.player_ID);
	//health
	printf("( ");
	if (player.health <= 0){
		printf("DEAD ");
	}
	else{
		for (int i=0; i<player.health; i++){
			printf("1");
		}
		for (int i=0; i<5-player.health; i++){
			printf(" ");
		}
	}
	printf(") ");
	//career
	printf(" %s", getCareer(player.career));
	if (strlen(getCareer(player.career))<=13){
		printf("\t");
	}
	
	//dynamic
	if (player.dynamic == 1){
		printf("[D]");
	}
	else{
		printf("   ");
	}
	//jail
	if (player.jail == 1){
		printf("[J]");
	}
	else{
		printf("   ");
	}
	
	printf("|");
	//weapon
	if (player.weapon > 0){ //改: weaponID範圍 
		printf("[%s]", card[player.weapon].name);
	}
	//scope
	if (player.scope > 0){ //改: scopeID範圍 
		printf("[Scope]");
	}
	//hourse
	if (player.hourse > 0){ //改: hourseID範圍 
		printf("[%s]", card[player.hourse].name);
	}
	//barrel
	if (player.barrel > 0){ //改: barrelID範圍 
		printf("[Barrel]");
	}
	
	printf("\n");
	return;
}

void print_allPlayers(struct Player player1, struct Player player2, struct Player player3, struct Player player4){
	//印出公開身分: 警長&死掉的人的身分(歹徒.叛徒...) 
	print_position(player1, player2, player3, player4);
	//印每個人攤在桌上的牌(生命、職業、裝備、炸彈監獄酒桶...) 
	print_player(player1);
	print_player(player2);
	print_player(player3);
	print_player(player4);
	return;
}

void print_hand(struct Player player){
	int handcount = 0;
	printf("\n\nYou are Player%d(%s), your hand cards:\n", player.player_ID, getPosition(player.position));
	for (int i=0; i<80; i++){
		if (player.hand[i] == 1){
			handcount ++;
			printf("%3d. %s\n", i, card[i].name);
		}
	}
	printf("(Choose at least %d more cards.)\n", handcount-player.health);
	return;
}

int main(){
	/************測試用 **********/ 
	card[5].name[0] = 'G';
	card[5].name[1] = 'u';
	card[5].name[2] = 'n';
	card[5].name[3] = '0';
	card[5].name[4] = '1';
	card[5].name[5] = 0;
	card[3].name[0] = 'H';
	card[3].name[1] = 'o';
	card[3].name[2] = 'u';
	card[3].name[3] = 'r';
	card[3].name[4] = 's';
	card[3].name[5] = 'e';
	card[3].name[6] = 0;
	card[1].name[0] = 'B';
	card[1].name[1] = 'a';
	card[1].name[2] = 'n';
	card[1].name[3] = 'g';
	card[1].name[4] = 0;
	
	struct Player player1;
	struct Player player2;
	struct Player player3;
	struct Player player4;
	
	player1.player_ID = 1;
	player2.player_ID = 2;
	player3.player_ID = 3;
	player4.player_ID = 4;
	
	player1.health = 1;
	player2.health = 4;
	player3.health = 3;
	player4.health = 0;
	
	player1.position = 1;
	player2.position = 2;
	player3.position = 3;
	player4.position = 3;
	
	player1.career = 1;
	player2.career = 2;
	player3.career = 3;
	player4.career = 4;
	
	player1.weapon = 0;
	player2.weapon = 5;
	player3.weapon = 0;
	player4.weapon = 0;
	
	player1.jail = 0;
	player2.jail = 1;
	player3.jail = 0;
	player4.jail = 0;
	
	player1.dynamic = 1;
	player2.dynamic = 1;
	player3.dynamic = 0;
	player4.dynamic = 0;
	
	player1.scope = 1;
	player2.scope = 1;
	player3.scope = 0;
	player4.scope = 0;
	
	player1.hourse = 3;
	player2.hourse = 0;
	player3.hourse = 0;
	player4.hourse = 0;
	
	player1.barrel = 1;
	player2.barrel = 0;
	player3.barrel = 1;
	player4.barrel = 0;
	
	for (int i=0; i<80; i++){
		player1.hand[i] = 0;
		player2.hand[i] = 0;
		player3.hand[i] = 0;
		player4.hand[i] = 0;
	}
	player1.hand[1] = 1;
	player1.hand[5] = 1;
	
	/*****************測試用END*****************/
	
	//印出現在狀態(公開的身分、每個人狀態) 
	print_allPlayers(player1, player2, player3, player4);
	//印出某1個玩家的手牌以供出牌選擇 
	print_hand(player1);
	
	return 0;
}

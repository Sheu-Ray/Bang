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
#include "Bang_structure.h"

extern struct CARD card[80];

char * getPosition(int position){
	switch(position){
		case 1:
			return "sheriff";
		case 3:
			return "outlaw";
		case 4:
			return "renegade";
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
	//printf("(Choose at least %d more cards.)\n", handcount-player.health);
	return;
}


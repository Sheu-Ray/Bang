#include <stdio.h>
#include <stdlib.h>
#include "Bang_structure.h"

extern struct CARD card[80];

char * getPosition(int position){
	switch(position){
		case 1:
			return "Sheriff";
		case 3:
			return "Outlaw";
		case 4:
			return "Renegade";
		default:
			return NULL;
	}
	return NULL;
}

char * getCareer(int career){
	switch(career){
		case 0:
			return "Bart Cassidy   ";
		case 1:
			return "Black Jack     ";
		case 2:
			return "Calamity Janet ";
		case 3:
			return "El Gringo      ";
		case 4:
			return "Jesse Jones    ";
		case 5:
			return "Jourdonnais    ";
		case 6: 
			return "Kit Carlson    ";
		case 7:
			return "Lucky Duke     ";
		case 8:
			return "Paul Regret    ";
		case 9:
			return "Pedro Ramirez  ";
		case 10:
			return "Rose Doolan    ";
		case 11:
			return "Sid Ketchum    ";
		case 12:
			return "Slab the Killer";
		case 13:
			return "Suzy Lafayette ";
		case 14:
			return "Vulture Sam    ";
		case 15:
			return "Willy the Kid  ";
		default:
			return NULL;
	}
		
	return NULL;
}

int right(int self){
	if (self == 1){
		return 4;
	}
	else{
		return self-1;
	}
}

int left(int self){
	if (self == 4){
		return 1;
	}
	else{
		return self+1;
	}
}

int distance(struct Player player[4], int self, int target){ //self&target: 1-4
	int dist = 0;
	
	//self to self: no distance
	if (self == target){
		return 0;
	}
	
	//self or target die: return -1
	if (player[self-1].health <= 0 || player[target-1].health <= 0){
		return -1;
	}
	
	//origin distance
	dist = abs(self-target);
	if (dist > 2){
		dist = 4-dist;
	}
	if(dist == 2){
		if (player[left(self)-1].health <= 0 || player[right(self)-1].health <= 0){
			dist --;
		}
	}
	
	//Paul Regret
	if (player[target-1].career == 8){
		dist ++;
	}
	
	//Rose Doolan
	if (player[self-1].career == 10){
		dist --;
	}
	
	//Mustang(horse)
	if (player[target-1].horse == 66){
		dist ++;
	}
	
	//Appolosa(hourse)
	if (player[self-1].horse == 67){
		dist --;
	}
	
	//Scope
	if (player[self-1].scope == 65){
		dist --;
	}
	
	return dist;
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

void print_player(struct Player player[4], int viewer){
	for (int i=0; i<4; i++){
		//player_ID
		printf("Player%d ", player[i].player_ID);
		
		//health
		printf("( ");
		if (player[i].health <= 0){
			printf("DEAD  ");
		}
		else{
			for (int j=0; j<player[i].health; j++){
				printf("1");
			}
			for (int j=0; j<6-player[i].health; j++){
				printf(" ");
			}
		}
		printf(") ");
		
		//card_count
		printf("%d cards | ", player[i].card_amount);
		
		//distance
		if (viewer > 0){
			if ( i == viewer-1 || player[i].health <= 0 ){
				printf("     | ");
			}
			else{
				printf("d: %d | ", distance(player, viewer, i+1));
			}
		}
		
		//career
		printf("%s ", getCareer(player[i].career));
		
		//dynamite
		if (player[i].dynamite == 1){
			printf("[D]");
		}
		else{
			printf("   ");
		}
		
		//jail
		if (player[i].jail == 1){
			printf("[J]");
		}
		else{
			printf("   ");
		}
		
		printf("|");
		
		//weapon
		if (player[i].weapon > 0){ 
			printf("[%s]", card[player[i].weapon].name);
		}
		
		//scope
		if (player[i].scope > 0){ 
			printf("[Scope]");
		}
		
		//horse
		if (player[i].horse > 0){ 
			printf("[%s]", card[player[i].horse].name);
		}
		
		//barrel
		if (player[i].barrel > 0){ 
			printf("[Barrel]");
		}
		
		printf("\n");
	} 
	
	return;
}

// viewer: playerID(1-4) , viewer <= 0 may not print distance part
void print_allPlayers(struct Player player[4], int viewer){
	//印出公開身分: 警長&死掉的人的身分(歹徒.叛徒...) 
	print_position(player[0], player[1], player[2], player[3]);
	//印每個人攤在桌上的牌(生命、職業、裝備、炸彈監獄酒桶...) 
	print_player(player, viewer);
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

void print_winner(struct Player player[4], int winner){ //1:sheriff 3:outlaw 4:renegade
	int printed = 0;
	printf("The winner is...... ");
	if (winner == 1){
		printf("SHERIFF !!\n");
	}
	else if (winner == 3){
		printf("OUTLAW !!\n");
	}
	else if (winner == 4){
		printf("RENEGADE !!\n");
	}
	else {
		printf("\n");
	}
	
	printf("(");
	for (int i=0; i<4; i++){
		if (player[i].position == winner){
			if (printed > 0){
				printf(" &");
			}
			printf(" Player%d", i+1);
			printed ++;
		}
	}
	printf(" won the game. )\n");
	return;
}

//use in the end of game
void print_allPosition(struct Player player[4]){
	printf("\n");
	for (int i=0; i<4; i++){
		printf("< Player%d is %s >\n", i+1, getPosition(player[i].position));
	}
	return;
}

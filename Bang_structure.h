#ifndef BANG_H
#define BANG_H

struct CARD{
    int ID;
    int suit;
    int rank;
    char name[20];
    char * description;
};

struct CAREER {
    int ID;
    int health;
    char name[20];
    char * description;
};

struct Player{
	int player_ID;
	int health;   //剩餘生命值
	int AI;       //是否為電腦
	int position; //身分
	int career;   //職業 
	int weapon;   //武器 
	int hourse;   //馬 
	int jail;     //監獄  
	int dynamic;  //炸彈 
	int scope;    //瞄準鏡   
	int barrel;   //酒桶 
	int hand[80]; //手牌
    int card_amount;
};

#endif // BANG_H#define MYMATCH_H

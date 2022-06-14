#ifndef BANG_H
#define BANG_H

#include <stdio.h>
#include <string.h>

struct CARD{
    int ID;
    char name[20];
    int suit;
    int rank;
};

struct CAREER {
    int ID;
    char name[20];
    int health;
};

struct Player {
    int player_ID;
    int health;
    int AI;
    int position;
    int career;
    int weapon;
    int hourse;
    int jail;
    int dynamite;
    int scope;
    int hand[80];
    int barrel;
};

int draw_card[80];
int fold_card[80];
int fold_card_flag = 0;

struct CARD card[80];

char BangCARDNAME[20] = "Bang";
card[0].ID = 0;
strcpy(card[0].name,BangCARDNAME);
card[0].suit = 0;
card[0].rank = 0;
//
card[1].ID = 1;
strcpy(card[1].name,BangCARDNAME);
card[1].suit = 1;
card[1].rank = 0;
//
card[2].ID = 2;
strcpy(card[2].name,BangCARDNAME);
card[2].suit = 1;
card[2].rank = 11;
//
card[3].ID = 3;
strcpy(card[3].name,BangCARDNAME);
card[3].suit = 1;
card[3].rank = 12;
//
card[4].ID = 4;
strcpy(card[4].name,BangCARDNAME);
card[4].suit = 2;
card[4].rank = 0;
//
card[5].ID = 5;
strcpy(card[5].name,BangCARDNAME);
card[5].suit = 2;
card[5].rank = 1;
//
card[6].ID = 6;
strcpy(card[6].name,BangCARDNAME);
card[6].suit = 2;
card[6].rank = 2;
//
card[7].ID = 7;
strcpy(card[7].name,BangCARDNAME);
card[7].suit = 2;
card[7].rank = 3;
//
card[8].ID = 8;
strcpy(card[8].name,BangCARDNAME);
card[8].suit = 2;
card[8].rank = 4;
//
card[9].ID = 9;
strcpy(card[9].name,BangCARDNAME);
card[9].suit = 2;
card[9].rank = 5;
//
card[10].ID = 10;
strcpy(card[10].name,BangCARDNAME);
card[10].suit = 2;
card[10].rank = 6;
//
card[11].ID = 11;
strcpy(card[11].name,BangCARDNAME);
card[11].suit = 2;
card[11].rank = 7;
//
card[12].ID = 12;
strcpy(card[12].name,BangCARDNAME);
card[12].suit = 2;
card[12].rank = 8;
//
card[13].ID = 13;
strcpy(card[13].name,BangCARDNAME);;
card[13].suit = 2;
card[13].rank = 9;
//
card[14].ID = 14;
strcpy(card[14].name,BangCARDNAME);
card[14].suit = 2;
card[14].rank = 10;
//
card[15].ID = 15;
strcpy(card[15].name,BangCARDNAME);
card[15].suit = 2;
card[15].rank = 11;
//
card[16].ID = 16;
strcpy(card[16].name,BangCARDNAME);
card[16].suit = 2;
card[16].rank = 12;
//
card[17].ID = 17;
strcpy(card[17].name,BangCARDNAME);
card[17].suit = 3;
card[17].rank = 1;
//
card[18].ID = 18;
strcpy(card[18].name,BangCARDNAME);
card[18].suit = 3;
card[18].rank = 2;
//
card[19].ID = 19;
strcpy(card[19].name,BangCARDNAME);
card[19].suit = 3;
card[19].rank = 3;
//
card[20].ID = 20;
strcpy(card[20].name,BangCARDNAME);
card[20].suit = 3;
card[20].rank = 4;
//
card[21].ID = 21;
strcpy(card[21].name,BangCARDNAME);
card[21].suit = 3;
card[21].rank = 5;
//
card[22].ID = 22;
strcpy(card[22].name,BangCARDNAME);
card[22].suit = 3;
card[22].rank = 6;
//
card[23].ID = 23;
strcpy(card[23].name,BangCARDNAME);
card[23].suit = 3;
card[23].rank = 7;
//
card[24].ID = 24;
strcpy(card[24].name,BangCARDNAME);
card[24].suit = 3;
card[24].rank = 8;
//
char MISSEDCARDNAME[20] = "Missed";
card[25].ID = 25;
strcpy(card[25].name,MISSEDCARDNAME);
card[25].suit = 0;
card[25].rank = 1;
//
card[26].ID = 26;
strcpy(card[26].name,MISSEDCARDNAME);
card[26].suit = 0;
card[26].rank = 2;
//
card[27].ID = 27;
strcpy(card[27].name,MISSEDCARDNAME);
card[27].suit = 0;
card[27].rank = 3;
//
card[28].ID = 28;
strcpy(card[28].name,MISSEDCARDNAME);
card[28].suit = 0;
card[28].rank = 4;
//
card[29].ID = 29;
strcpy(card[29].name,MISSEDCARDNAME);
card[29].suit = 0;
card[29].rank = 5;
//
card[30].ID = 30;
strcpy(card[30].name,MISSEDCARDNAME);
card[30].suit = 0;
card[30].rank = 6;
//
card[31].ID = 31;
strcpy(card[31].name,MISSEDCARDNAME);
card[31].suit = 0;
card[31].rank = 7;
//
card[32].ID = 32;
strcpy(card[32].name,MISSEDCARDNAME);
card[32].suit = 3;
card[32].rank = 0;
//
card[33].ID = 33;
strcpy(card[33].name,MISSEDCARDNAME);
card[33].suit = 3;
card[33].rank = 9;
//
card[34].ID = 34;
strcpy(card[34].name,MISSEDCARDNAME);
card[34].suit = 3;
card[34].rank = 10;
//
card[35].ID = 35;
strcpy(card[35].name,MISSEDCARDNAME);
card[35].suit = 3;
card[35].rank = 11;
//
card[36].ID = 36;
strcpy(card[36].name,MISSEDCARDNAME);
card[36].suit = 3;
card[36].rank = 12;
//
char GATKINGCARDNAME[20] = "Gatling";
card[37].ID = 37;
strcpy(card[37].name,GATKINGCARDNAME);
card[37].suit = 1;
card[37].rank = 9;
//
char INDIANSCARDNAME[20] = "Indians";
card[38].ID = 38;
strcpy(card[38].name,INDIANSCARDNAME);
card[38].suit = 2;
card[38].rank = 0;
//
card[39].ID = 39;
strcpy(card[39].name,INDIANSCARDNAME);
card[39].suit = 2;
card[39].rank = 12;
//
char DANICCARDNAME[20] = "Danic";
card[40].ID = 40;
strcpy(card[40].name,  DANICCARDNAME);
card[40].suit = 1;
card[40].rank = 0;
//
card[41].ID = 41;
strcpy(card[41].name,  DANICCARDNAME);
card[41].suit = 1;
card[41].rank = 10;
//
card[42].ID = 42;
strcpy(card[42].name,  DANICCARDNAME);
card[42].suit = 1;
card[42].rank = 11;
//
card[43].ID = 43;
strcpy(card[43].name,  DANICCARDNAME);
card[43].suit = 2;
card[43].rank = 7;
//
char CATBALOUCARDNAME[20] = "CatBalou";
card[44].ID = 44;
strcpy(card[44].name, CATBALOUCARDNAME);
card[44].suit = 1;
card[44].rank = 12;
//
card[45].ID = 45;
strcpy(card[45].name, CATBALOUCARDNAME);
card[45].suit = 2;
card[45].rank = 8;
//
card[46].ID = 46;
strcpy(card[46].name, CATBALOUCARDNAME);
card[46].suit = 2;
card[46].rank = 9;
//
card[47].ID = 47;
strcpy(card[47].name, CATBALOUCARDNAME);
card[47].suit = 2;
card[47].rank = 10;
//
char STAGECOACHCARDNAME[20] = "Stagecoach";
card[48].ID = 48;
strcpy(card[48].name, STAGECOACHCARDNAME);
card[48].suit = 0;
card[48].rank = 8;
//
card[49].ID = 49;
strcpy(card[49].name, STAGECOACHCARDNAME);
card[49].suit = 0;
card[49].rank = 8;
//
char WELLSFARGOCARDNAME[20] = "WellsFargo";
card[50].ID = 50;
strcpy(card[50].name, WELLSFARGOCARDNAME);
card[50].suit = 1;
card[50].rank = 2;
//
char GeneralStoreCARDNAME[20] = "GeneralStore";
card[51].ID = 51;
strcpy(card[51].name, GeneralStoreCARDNAME);
card[51].suit = 0;
card[51].rank = 11;
//
card[52].ID = 52;
strcpy(card[52].name, GeneralStoreCARDNAME);
card[52].suit = 3;
card[52].rank = 8;
//
char BeerCARDNAME[20] = "Beer";
card[53].ID = 53;
strcpy(card[53].name, BeerCARDNAME);
card[53].suit = 1;
card[53].rank = 5;
//
card[54].ID = 54;
strcpy(card[54].name, BeerCARDNAME);
card[54].suit = 1;
card[54].rank = 6;
//
card[55].ID = 55;
strcpy(card[55].name, BeerCARDNAME);
card[55].suit = 1;
card[55].rank = 7;
//
card[56].ID = 56;
strcpy(card[56].name, BeerCARDNAME);
card[56].suit = 1;
card[56].rank = 8;
//
card[57].ID = 57;
strcpy(card[57].name, BeerCARDNAME);
card[57].suit = 1;
card[57].rank = 9;
//
card[58].ID = 58;
strcpy(card[58].name, BeerCARDNAME);
card[58].suit = 1;
card[58].rank = 10;
//
char SaloowCARDNAME[20] = "Saloow";
card[59].ID = 59;
strcpy(card[59].name, SaloowCARDNAME);
card[59].suit = 1;
card[59].rank = 4;
//
char DuelCARDNAME[20] = "Duel";
card[60].ID = 60;
strcpy(card[60].name, DuelCARDNAME);
card[60].suit = 0;
card[60].rank = 10;
//
card[61].ID = 61;
strcpy(card[61].name, DuelCARDNAME);
card[61].suit = 2;
card[61].rank = 11;
//
card[62].ID = 62;
strcpy(card[62].name, DuelCARDNAME);
card[62].suit = 3;
card[62].rank = 7;
//
char BarrelCARDNAME[20] = "Barrel";
card[63].ID = 63;
strcpy(card[63].name, BarrelCARDNAME);
card[63].suit = 0;
card[63].rank = 11;
//
card[64].ID = 64;
strcpy(card[64].name, BarrelCARDNAME);
card[64].suit = 0;
card[64].rank = 12;
//
char ScopeCARDNAME[20] = "Scope";
card[65].ID = 65;
strcpy(card[65].name, ScopeCARDNAME);
card[65].suit = 0;
card[65].rank = 0;
//
char MustangCARDNAME[20] = "Mustang";
card[66].ID = 66;
strcpy(card[66].name, MustangCARDNAME);
card[66].suit = 1;
card[66].rank = 7;
//
char AppaloosaCARDNAME[20] = "Appaloosa";
card[67].ID = 67;
strcpy(card[67].name, AppaloosaCARDNAME);
card[67].suit = 1;
card[67].rank = 8;
//
char JailCARDNAME[20] = "Jail";
card[68].ID = 68;
strcpy(card[68].name, JailCARDNAME);
card[68].suit = 0;
card[68].rank = 9;
//
card[69].ID = 69;
strcpy(card[69].name, JailCARDNAME);
card[69].suit = 0;
card[69].rank = 10;
//
card[70].ID = 70;
strcpy(card[70].name, JailCARDNAME);
card[70].suit = 1;
card[70].rank = 3;
//
char DynamiteCARDNAME[20] = "Dynamite";
card[71].ID = 71;
strcpy(card[71].name, DynamiteCARDNAME);
card[71].suit = 1;
card[71].rank = 1;
//
char VolcanicCARDNAME[20] = "Volcanic";
card[72].ID = 72;
strcpy(card[72].name, VolcanicCARDNAME);
card[72].suit = 0;
card[72].rank = 9;
//
card[73].ID = 73;
strcpy(card[73].name, VolcanicCARDNAME);
card[73].suit = 3;
card[73].rank = 9;
//
char SchofieldCARDNAME[20] = "Schofield";
card[74].ID = 74;
strcpy(card[74].name, SchofieldCARDNAME);
card[74].suit = 0;
card[74].rank = 12;
//
card[75].ID = 75;
strcpy(card[75].name, SchofieldCARDNAME);
card[75].suit = 3;
card[75].rank = 10;
//
card[76].ID = 76;
strcpy(card[76].name, SchofieldCARDNAME);
card[76].suit = 3;
card[76].rank = 11;
//
char RemingtonCARDNAME[20] = "Remington";
card[77].ID = 77;
strcpy(card[77].name, RemingtonCARDNAME);
card[77].suit = 3;
card[77].rank = 12;
//
char RevCarabineCARDNAME[20] = "RevCarabine";
card[78].ID = 78;
strcpy(card[78].name, RevCarabineCARDNAME);
card[78].suit = 3;
card[78].rank = 0;
//
char WinchedterCARDNAME[20] = "Winchedter";
card[79].ID = 79;
strcpy(card[79].name, WinchedterCARDNAME);
card[79].suit = 0;
card[79].rank = 7;

struct CAREER career[16];
char Bart_CassidyCARDNAME[20] = "Bart_Cassidy";
career[0].ID = 0;
strcpy(career[0].name, Bart_CassidyCARDNAME);
career[0].health = 4;
//
char Black_JackCARDNAME[20] = "Black_Jack";
career[1].ID = 1;
strcpy(career[1].name, Black_JackCARDNAME);
career[1].health = 4;
//
char Calamity_JanetCARDNAME[20] = "Calamity_Janet";
career[2].ID = 2;
strcpy(career[2].name, Calamity_JanetCARDNAME);
career[2].health = 4;
//
char El_GringoCARDNAME[20] = "El_Gringo";
career[3].ID = 3;
strcpy(career[3].name, El_GringoCARDNAME);
career[3].health = 3;
//
char Jesse_JonesCARDNAME[20] = "Jesse_Jones";
career[4].ID = 4;
strcpy(career[4].name, Jesse_JonesCARDNAME);
career[4].health = 4;
//
char JourdonnaisCARDNAME[20] = "Jourdonnais";
career[5].ID = 5;
strcpy(career[5].name, JourdonnaisCARDNAME);
career[5].health = 4;
//
char Kit_CarlsonCARDNAME[20] = "Kit_Carlson";
career[6].ID = 6;
strcpy(career[6].name, Kit_CarlsonCARDNAME);
career[6].health = 4;
//
char Lucky_DukeCARDNAME[20] = "Lucky_Duke";
career[7].ID = 7;
strcpy(career[7].name, Lucky_DukeCARDNAME);
career[7].health = 4;
//
char Paul_RegretCARDNAME[20] = "Paul_Regret";
career[8].ID = 8;
strcpy(career[8].name, Paul_RegretCARDNAME);
career[8].health = 3;
//
char Pedro_RamirezCARDNAME[20] = "Pedro_Ramirez";
career[9].ID = 9;
strcpy(career[9].name, Pedro_RamirezCARDNAME);
career[9].health = 4;
//
char Rose_DoolanCARDNAME[20] = "Rose_Doolan";
career[10].ID = 10;
strcpy(career[10].name, Rose_DoolanCARDNAME);
career[10].health = 4;
//
char Sid_KetchumCARDNAME[20] = "Sid_Ketchum";
career[11].ID = 11;
strcpy(career[11].name, Sid_KetchumCARDNAME);
career[11].health = 4;
//
char Slab_the_KillerCARDNAME[20] = "Slab_the_Killer";
career[12].ID = 12;
strcpy(career[12].name, Slab_the_KillerCARDNAME);
career[12].health = 4;
//
char Suzy_LafayetteCARDNAME[20] = "Suzy_Lafayette";
career[13].ID = 13;
strcpy(career[13].name, Suzy_LafayetteCARDNAME);
career[13].health = 4;
//
char Vulture_SamCARDNAME[20] = "Vulture_Sam";
career[14].ID = 14;
strcpy(career[14].name, Vulture_SamCARDNAME);
career[14].health = 4;
//
char Willy_the_KidCARDNAME[20] = "Willy_the_Kid";
career[15].ID = 15;
strcpy(career[15].name, Willy_the_KidCARDNAME);
career[15].health = 4;
//

#endif // BANG_H#define MYMATCH_H

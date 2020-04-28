#ifndef SHOOTOUT_H
#define SHOOTOUT_H

#include <iostream>
using namespace std;

class Agent;

class Item {
protected:
    const int id;
    static int ID;
public:
    Item();
    virtual ~Item();
    int getID();
    
};


//armors
class Armor:public Item {
protected:
    int weigth;
    int protection;
public:
    Armor();
    Armor(int weigth, int protection);
    ~Armor();
    int getWeight();
    int getProtection();
    virtual void takeHit(Agent* owner,int hit);  // armura agentului owner este lovita cu hit hitpoints
};

class HealingShield : public Armor {
    int healRate;  // dupa fiecare lovitura, armura se vindeca cu healRate HP
    int maxProt;   // protectia pe care o primeste de la inceput
public:
    HealingShield(int _weigth,int _protection ,int heal);
    HealingShield(int _weigth,int _protection);
    HealingShield();
    ~HealingShield();
    void heal();  // vindecarea armurii cu healRate HP
    void takeHit(Agent* owner, int hit); // armura agentului owner este lovita cu hit hitpoints
};
class ForceShield : public Armor {
    int numOfStops;  //  numarul de ture in care poate opri oricefel de lovitura
public:
    ForceShield(int _weigth,int _protection,int stops);
    ForceShield(int _weigth,int _protection);
    ForceShield();
    ~ForceShield();
    void takeHit(Agent* owner, int hit); // armura agentului owner este lovita cu hit hitpoints
};
class Kevlar : public Armor {
    double dmgReduction;  // procentul care scade din puterea proiectilului pe care il incaseaza armura la o lovitura
public:
    Kevlar(int _weigth,int _protection,int dmgRed);
    Kevlar(int _weigth,int _protection);
    Kevlar();
    ~Kevlar();
    void takeHit(Agent* owner, int hit); // armura agentului owner este lovita cu hit hitpoints
};


//weapons
class Weapon:public Item {
protected:
    int dmg;  // puterea armei
public:
    Weapon(int damage);
    Weapon();
    ~Weapon();
    int getDmg();
    virtual int shootDead(Agent* owner, Agent* that ) = 0;  // Owner loveste that si intoarce 1 daca il omoara, 0 daca nu
    void getAffectedByArmor(Armor* armor);  // reduce puterea armei in functie de armura
};

class Revolver : public Weapon {
    int magazine;  // numarul de ture care poate lovi consecutiv fara sa reincarce arma
public:
    Revolver(int damage);
    Revolver();
    ~Revolver();
    int shootDead(Agent* owner , Agent* that); // Owner loveste that si intoarce 1 daca il omoara, 0 daca nu
};
class Shotgun : public Weapon {
    int barrels; // nuamrul de lovituri pe care le efectueaza intr o tura
public:
    Shotgun(int damage);
    Shotgun();
    ~Shotgun();
    int shootDead(Agent* owner, Agent* that); // Owner loveste that si intoarce 1 daca il omoara, 0 daca nu
};
class Bow : public Weapon {
    double dmgMultiplier;  // procentul cu care se mareste puterea armei la fiecare lovitura
public:
    Bow(int damage);
    Bow();
    ~Bow(); 
    int shootDead(Agent* owner, Agent* that); // Owner loveste that si intoarce 1 daca il omoara, 0 daca nu
};


//agents
class Agent {
protected:
    int reach, x, y, maxx, maxy, moveOpt, hp;  // reach = aria de vizibilitate, x,y = coordonateele in matrice, unde maxx si maxy capacitatea matricei, moveOpt dicteaza miscarea in matrice
    Armor* armura;
    Weapon* arma;
public:
    Agent(Armor* armor, Weapon* weapon, int _reach);  // un agnet este initializat cu armura, arma si arie de vizibilitate
    Agent(int _reach);
    Agent();
    virtual ~Agent();
    virtual void moveInPattern() = 0;   // deplasarea in matrice
    void setCoordinates(int _x, int _y, int _maxx,int _maxy);
    void setCoordinates(int _x, int _y);
    int getCoordinatesX();
    int getCoordinatesY();
    int shootWithWeaponDead(Agent* that);  //  loveste agentul that si intoarce 1 daca reuseste sa-l ucida
    void takeHitInArmor(int hit);          // directionarea loviturii din metoda de mai sus
    void takeHitInHp(int hit);             // -----------------||--------------------------
    bool haveArmor();                      // verifica daca agentul are armura
    void getShot(int hit);                 // agentul primeste lovitura de hit hitpoints
    int getHp();
    int getWeaponDmg();
    int getArmorProtection();
    int getReach();
};

class Krul :public Agent {
public:
    Krul(Armor* armor, Weapon* weapon, int _reach);  // Krul este intializat ca un agnet cu armura, arma si arie de vizibilitate
    Krul(int _reach);
    ~Krul();
    void moveInPattern();  // deplasarea in matrice specifica lui Krul
};

class Annesix :public Agent {
    int stepsTaken;
public:
    Annesix(Armor* armor, Weapon* weapon , int _reach); // Annesix este intializat ca un agnet cu armura, arma si arie de vizibilitate
    Annesix(int _reach);
    ~Annesix();
    void moveInPattern(); // deplasarea in matrice specifica lui Annesix
};

class Board {
    int n, m;  // capacitatea matricei
    int** matrix;  // matrice alocata dinamic
    int numOfAgents; // numarul maxim de agenti
    int activeAgents; // numarul de agenti inregistrati initial in joc
    int agentsLeft;  // numarul de agenti ramasi la un moment dat (utilizat pentru conditia de finalizare a jocului)
    typedef Agent* AgentPtr; //alias
    AgentPtr* agenti; // dupa initializarea din constructor, devine pointer la lista de pointeri AKA agent[i] e pointer la agentul i
public:
    Board(int n, int m, int numA);
    ~Board();
    void insertAgent(Agent* a, int _x, int _y ); // inserarea unui agent a in matrice pe pozitia x y
    void showBoard();
    bool nextRound();        // returneaza true daca a ramas un singur agent in viata, false daca nu 
    int getAgentsLeft(); 
    void simulateAllGame();  // simulare a jocului pana la terminarea acestuia
    void simulateByRounds(); // simulare a jocului pe runde cu decizia de a continua, continua si arata tabla si oprire 
};

#endif
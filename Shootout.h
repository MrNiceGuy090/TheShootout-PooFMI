#pragma once
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
    virtual void takeHit(Agent*,int);
};

class HealingShield : public Armor {
    int healRate;
    int maxProt;
public:
    HealingShield(int,int,int);
    HealingShield(int,int);
    HealingShield();
    ~HealingShield();
    void heal();
    void takeHit(Agent*, int);
};
class ForceShield : public Armor {
    int numOfStops;
public:
    ForceShield(int,int,int);
    ForceShield(int,int);
    ForceShield();
    ~ForceShield();
    void takeHit(Agent*, int);
};
class Kevlar : public Armor {
    double dmgReduction;
public:
    Kevlar(int,int,int);
    Kevlar(int,int);
    Kevlar();
    ~Kevlar();
    void takeHit(Agent*, int);
};


//weapons
class Weapon:public Item {
protected:
    int dmg;
public:
    Weapon(int);
    Weapon();
    ~Weapon();
    int getDmg();
    virtual int shootDead(Agent* , Agent* ) = 0;
    void getAffectedByArmor(Armor* armor);
};

class Revolver : public Weapon {
    int magazine;
public:
    Revolver(int);
    Revolver();
    ~Revolver();
    int shootDead(Agent* , Agent*);
};
class Shotgun : public Weapon {
    int barrels;
public:
    Shotgun(int);
    Shotgun();
    ~Shotgun();
    int shootDead(Agent*, Agent*);
};
class Bow : public Weapon {
    double dmgMultiplier;
public:
    Bow(int);
    Bow();
    ~Bow();
    int shootDead(Agent* , Agent*);
};


//agents
class Agent {
protected:
    int reach, x, y, maxx, maxy, moveOpt, hp;
    Armor* armura;
    Weapon* arma;
public:
    Agent(Armor*, Weapon*, int);
    Agent(int);
    Agent();
    virtual ~Agent();
    virtual void moveInPattern() = 0;
    void setCoordinates(int, int, int,int);
    void setCoordinates(int, int);
    int getCoordinatesX();
    int getCoordinatesY();
    int shootWithWeaponDead(Agent*);
    void takeHitInArmor(int);
    void takeHitInHp(int);
    bool haveArmor();
    void getShot(int);
    int getHp();
    int getWeaponDmg();
    int getArmorProtection();
    int getReach();
};

class Krul :public Agent {
public:
    Krul(Armor*, Weapon*, int);
    Krul(int);
    ~Krul();
    void moveInPattern();
};

class Annesix :public Agent {
    int stepsTaken;
public:
    Annesix(Armor*, Weapon*, int);
    Annesix(int);
    ~Annesix();
    void moveInPattern();
};

class Board {
    int n, m;
    int** matrix;
    int numOfAgents;
    int activeAgents;
    int agentsLeft;
    typedef Agent* AgentPtr; //alias
    AgentPtr* agenti; // dupa initializarea din constructor, devine pointer la lista de pointeri AKA agent[i] e pointer la agentul i
public:
    Board(int n, int m, int numA);
    ~Board();
    void insertAgent(Agent* a, int _x, int _y );
    void showBoard();
    void nextRound();
    int getAgentsleft();
};

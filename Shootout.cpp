#include "Shootout.h"
#include <iostream>
using namespace std;




// board
Board::Board(int _n, int _m, int numA): n(_n), m(_m), numOfAgents(numA) {
    matrix = new int* [n];
    for (int i = 1; i <= n; i++)
        matrix[i] = new int[m+1]();
    agenti = new AgentPtr[numOfAgents];
    activeAgents = 0;
}

Board::~Board() {
    for (int i = 0; i < n; ++i)
        delete[] matrix[i];
    delete[] matrix;
}
void Board::showBoard() {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++)
            cout << matrix[i][j] << " ";
        cout << endl;
    }
}
void Board::insertAgent(Agent* a, int _x, int _y) {
    activeAgents++;
    agentsLeft++;
    agenti[activeAgents] = a;
    agenti[activeAgents]->setCoordinates(_x, _y, this->n , this->m);
    this->matrix[_x][_y] = activeAgents;
    cout << "Inserted agent #" <<activeAgents<<"("<<a<<") at position "<<_x << "x"<<_y<< endl;
}
int Board::getAgentsleft() {
    return agentsLeft;
}
void Board::nextRound() {
    for (int k = 1; k <= activeAgents; k++) 
    if(agenti[k]){
        int x_ag = agenti[k]->getCoordinatesX();
        int y_ag = agenti[k]->getCoordinatesY();
        int r = agenti[k]->getReach();
        int rmaxx = x_ag + r; if (rmaxx > n)rmaxx = n;
        int rminx = x_ag - r; if (rminx < 1)rminx = 1;
        int rmaxy = y_ag + r; if (rmaxy > m)rmaxy = m;
        int rminy = y_ag - r; if (rminy < 1)rminy = 1;
        int enemy = 0, i, j;
        for (i = rminx; i <= rmaxx; i++) 
            for (j = rminy; j <= rmaxy; j++) {
                if (matrix[i][j] != 0 && matrix[i][j]!=k) {
                    enemy = matrix[i][j];
                    goto found;
                }
            }
    found:

        if(enemy){
            if (agenti[k]->shootWithWeaponDead(agenti[enemy]))
            {
                agenti[enemy] = 0;
                matrix[i][j] = 0;
                agentsLeft--;
            }
        }
        else {
            this->matrix[x_ag][y_ag] = 0;
            agenti[k]->moveInPattern();
            x_ag = agenti[k]->getCoordinatesX();
            y_ag = agenti[k]->getCoordinatesY();
            this->matrix[x_ag][y_ag] = k;
        }
    }

}

// items
int Item::ID = 0;
Item::Item():id(++ID) {};
Item::~Item() {};
int Item::getID() {
    return Item::id;
}

//armors
Armor::Armor(int weigth, int protection) : weigth(weigth), protection(protection){};
Armor::Armor(){
    weigth = 10;
    protection = 10;
}
Armor::~Armor() {};
int Armor::getWeight(){
    return weigth;
}
int Armor::getProtection() {
    return protection;
}
void Armor::takeHit(Agent* owner, int hit) {
    protection -= hit;
    cout << owner << " got shot, " <<protection<<" armor HP left"<< endl;
}

    //HealingShield
HealingShield::HealingShield(int w, int p, int h) : Armor(w, p), healRate(h), maxProt(p) {};
HealingShield::HealingShield(int w, int p) : Armor(w, p), maxProt(p) {
    healRate = 10;
};
HealingShield::~HealingShield() {};
HealingShield::HealingShield() {
    maxProt = this->protection;
    healRate = 20;
}
void HealingShield::heal() {
    if (this->protection > 0) {
        if (this->protection < maxProt - healRate) this->protection += healRate;
        else this->protection = maxProt;
    }
}
void HealingShield::takeHit(Agent* owner, int hit) {
    protection -= hit;
    if (protection > 0) {
        cout << owner << " got shot, " << protection << " armor HP left" << endl;
    }
    this->heal();
}

    //ForceShield
ForceShield::ForceShield(int w, int p, int stops): Armor(w,p), numOfStops(stops){}
ForceShield::ForceShield(int w, int p) : Armor(w, p){
    numOfStops = 2;
}
ForceShield::ForceShield(){
    numOfStops = 2; 
}
ForceShield::~ForceShield() {};
void ForceShield::takeHit(Agent* owner, int hit) {
    if (numOfStops > 0) {
        numOfStops--; cout << owner << " blocked a shot" << endl;
    }
    else {
        protection -= hit;
        if (protection > 0) {
            cout << owner << " got shot, " << protection << " armor HP left" << endl;
        }
    }
}

    //Kevlar
Kevlar::Kevlar(int w, int p, int dmgRed): Armor(w,p), dmgReduction(float(dmgRed)/100){}
Kevlar::Kevlar(int w, int p) : Armor(w, p) {
    dmgReduction = 0.2;
}
Kevlar::Kevlar() {
    dmgReduction = 0.2;
}
Kevlar::~Kevlar() {};
void Kevlar::takeHit(Agent* owner, int hit) {
    protection -= hit - int(hit * dmgReduction);
    if (protection > 0) {
        cout << owner << " got shot, " << protection << " armor HP left" << endl;
    }
}




//weapons
Weapon::Weapon() {
    dmg = 40;
}
Weapon::Weapon(int damage)  {
    dmg = damage;
}
Weapon::~Weapon() {};
int Weapon::getDmg() {
    return dmg;
}
void Weapon::getAffectedByArmor(Armor* armor) {
    dmg -= armor->getWeight() / 10;
}



    //Revolver
Revolver::Revolver(int dmg): Weapon(dmg) {
    magazine = 6;
}
Revolver::Revolver() : Weapon() {
    magazine = 6;
}
Revolver::~Revolver() {};
int Revolver::shootDead(Agent* owner,Agent* that) {
    if (magazine > 0) {
        cout << owner << " shot " << that << endl; magazine--;
        that->getShot(this->dmg);
        if (that->getHp() <= 0) {
            cout << owner << " killed " << that << endl;
            delete that;
            return 1;
        }
    }
    else {
        cout <<owner<< " reloaded his revolver" << endl;
        magazine = 6;
    }
    return 0;
}

    // Shotgun
Shotgun::Shotgun(int dmg) : Weapon(dmg) {
    barrels = 2;
}
Shotgun::Shotgun() : Weapon() {
    barrels = 2;
}
Shotgun::~Shotgun() {};
int Shotgun::shootDead(Agent* owner, Agent* that) {
    for (int i = 1; i <= barrels; i++) {
        cout << owner << " shot " << that << endl;
        that->getShot(this->dmg);
        if (that->getHp() <= 0) {
            cout << owner << " killed " << that << endl;
            delete that;
            return 1;
        }
    }
    return 0;
}

    //Bow
Bow::Bow(int dmg) : Weapon(dmg) {
    dmgMultiplier = 1.2;
}
Bow::Bow() : Weapon() {
    dmgMultiplier = 1.2;
}
Bow::~Bow() {};
int Bow::shootDead(Agent* owner, Agent* that) {
    cout << owner << " shot " << that << endl;
    that->getShot(this->dmg);
    this->dmg = int(this->dmg * dmgMultiplier);
    if (that->getHp() <= 0) {
        cout << owner << " killed " << that << endl;
        delete that;
        return 1;
    }
    return 0;
}


//Agents
Agent::Agent(Armor* armor, Weapon* weapon, int _reach) : reach(_reach), armura(armor), arma(weapon) {
    arma->getAffectedByArmor(armor);
    moveOpt = 1;
    hp = 100;
};
Agent::Agent(int reach) : reach(reach) {
    arma = new Revolver();
    armura = new Kevlar();
    arma->getAffectedByArmor(armura);
    moveOpt = 1;
    hp = 100;
}
Agent::Agent() {
    reach = 3;
    arma = new Revolver();
    armura = new Kevlar();
    arma->getAffectedByArmor(armura);
    moveOpt = 1;
    hp = 100;
}
Agent::~Agent() {
    if(armura)
    delete[] armura;
    delete[] arma; 
};
void Agent::setCoordinates(int _x, int _y, int _maxx, int _maxy) { 
    x = _x; y = _y; maxx = _maxx; maxy = _maxy;
};
void Agent::setCoordinates(int _x, int _y)  {
    x = _x; y = _y;
};
int Agent::getCoordinatesX() {
    return x;
}
int Agent::getCoordinatesY() {
    return y;
}
int Agent::shootWithWeaponDead(Agent* that) {
    return arma->shootDead(this,that);
}
void Agent::takeHitInArmor(int hit) {
    armura->takeHit(this,hit);
    if (armura->getProtection() <= 0) {
        delete armura; cout << this << "'s armor was destroyed" << endl;
        armura = 0;
    }
}
void Agent::takeHitInHp(int hit) {
    hp -= hit;
    if (hp > 0) {
        cout << this << " got shot, " << hp << " hp left" << endl;
    }
}
bool Agent::haveArmor() {
    if (armura) return true;
    else return false;
}
void Agent::getShot(int hit) {
    if (this->haveArmor())this->takeHitInArmor(hit);
    else this->takeHitInHp(hit);
}
int Agent::getHp() {
    return hp;
}
int Agent::getWeaponDmg() {
    return arma->getDmg();
}
int Agent::getArmorProtection() {
    return armura->getProtection();
}
int Agent::getReach() {
    return reach;
}

Krul::Krul(Armor* armor, Weapon* weapon, int _reach) : Agent(armor, weapon, _reach) {};
Krul::Krul(int _reach) : Agent(_reach) {};
Krul::~Krul() {};
void Krul::moveInPattern() {
    int x_next = x, y_next = y;
    if (moveOpt == 1) {
        x_next += 1;
        y_next += 1;
    }
    else if (moveOpt == 2) {
        y_next -= 1; 
    }
    else if (moveOpt == 3) {
        x_next -= 1;
    }

    if (x_next > maxx || y_next > maxy) {
        moveOpt = 2;
        y_next = y - 1;
        x_next = x;
    }
    else if (y_next < 1) {
        moveOpt = 3;
        y_next = y;
        x_next = x - 1;
    }
    else if (x_next < 1) {
        moveOpt = 1;
        y_next = y + 1;
        x_next = x + 1;
    }
    x = x_next;
    y = y_next;
}

Annesix::Annesix(Armor* armor, Weapon* weapon, int _reach) : Agent(armor, weapon, _reach) { stepsTaken = 0; };
Annesix::Annesix(int _reach) : Agent(_reach) { stepsTaken = 0; };
Annesix::~Annesix() {};
void Annesix::moveInPattern() {
    int x_next = x, y_next = y;
    if (moveOpt == 1) {
        y_next += 1;
    }
    else if (moveOpt == 2) {
        x_next -= 1;
    }
    else if (moveOpt == 3) {
        y_next -= 1;
    }
    else if (moveOpt == 4) {
        x_next += 1;
    }
    if (y_next > maxy) {
        moveOpt = 3;
        stepsTaken = 1;
        y_next = y - 1;
    }
    else if (y_next < 1) {
        moveOpt = 1;
        stepsTaken = 1;
        y_next = y + 1;
    }
    else if (x_next > maxx) {
        moveOpt = 2;
        stepsTaken = 1;
        x_next = x - 1;
    }
    else if (x_next < 0) {
        moveOpt = 4;
        stepsTaken = 1;
        x_next = x + 1;
    }
    if (stepsTaken == 5) {
        if (moveOpt < 4)moveOpt++;
        else moveOpt = 1;
        stepsTaken = 0;
    }
    x = x_next;
    y = y_next;
    stepsTaken++;
}
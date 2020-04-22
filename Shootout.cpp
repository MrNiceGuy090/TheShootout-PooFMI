#include "Shootout.h"
#include <iostream>
using namespace std;

// board
Board::Board(int _n, int _m, int numA): n(_n), m(_m), numOfAgents(numA) {
    matrix = new int* [n];
    for (int i = 1; i <= n; i++)       // alocare dinamica matrice
        matrix[i] = new int[m+1]();
    agenti = new AgentPtr[numOfAgents];  // alocare dinamica lista de pointeri de tip Agent*
    activeAgents = 0;
}

Board::~Board() {
    for (int i = 0; i < n; ++i)
        delete[] matrix[i];            // stergerea matricei
    delete[] matrix;
}
void Board::showBoard() {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++)
            cout << matrix[i][j] << " ";        // afisarea matricei
        cout << endl;
    }
}
void Board::insertAgent(Agent* a, int _x, int _y) {
    activeAgents++;
    agentsLeft++;
    agenti[activeAgents] = a;  // agentii sunt memorati intr o lista ordonata dupa momentul adaugarii. Acesta se reprezinta pe matricea jocului prin indicele din aceasta lista
    agenti[activeAgents]->setCoordinates(_x, _y, this->n , this->m);  // inserarea unui agent la coordonatele x,y 
    this->matrix[_x][_y] = activeAgents;
    cout << "Inserted agent #" <<activeAgents<<"("<<a<<") at position "<<_x << "x"<<_y<< endl;
}
int Board::getAgentsleft() {
    return agentsLeft;
}
void Board::nextRound() {
    int somethingHappend = 0;
    for (int k = 1; k <= activeAgents; k++)        // itereaza prin toti agentii declarati la inceput
    if(agenti[k]){                                 // daca agentul respectiv nu a fost ucis
        int x_ag = agenti[k]->getCoordinatesX();
        int y_ag = agenti[k]->getCoordinatesY();    // x_ag, y_ag sunt coordonatele agentului in acest moment
        int r = agenti[k]->getReach();
        int rmaxx = x_ag + r; if (rmaxx > n)rmaxx = n;
        int rminx = x_ag - r; if (rminx < 1)rminx = 1;
        int rmaxy = y_ag + r; if (rmaxy > m)rmaxy = m;
        int rminy = y_ag - r; if (rminy < 1)rminy = 1;      // rmaxx,rminx,rmaxy,rminy reprezinta limitele in care poate cauta un alt agent
        int enemy = 0, i, j;
        for (i = rminx; i <= rmaxx; i++) 
            for (j = rminy; j <= rmaxy; j++) {
                if (matrix[i][j] != 0 && matrix[i][j]!=k) {       // daca gaseste un alt agent in aria de vizibilitate, memoreaza indicele acestuia in variabila enemy
                    enemy = matrix[i][j];
                    goto found;
                }
            }
    found:

        if(enemy){
            somethingHappend = 1;
            if (agenti[k]->shootWithWeaponDead(agenti[enemy]))  // se incearca lovirea inmaicului, si daca acesta este eliminat, este sters din lista de agenti si din matrice
            {
                agenti[enemy] = 0;
                matrix[i][j] = 0;
                agentsLeft--;
            }
        }
        else {                                                  // daca nu se gaseste niciun agent, acesta va trebui sa avanseze in matrice
            this->matrix[x_ag][y_ag] = 0;
            agenti[k]->moveInPattern();
            x_ag = agenti[k]->getCoordinatesX();
            y_ag = agenti[k]->getCoordinatesY();
            this->matrix[x_ag][y_ag] = k;
        }
    }
    if (somethingHappend == 0)cout << "Nothing happend this round.\n";

}

void Board::simulateAllGame() {
    int num = 1;
    while (this->getAgentsleft() > 1)
    {
        this->nextRound(); cout << "End of Turn " << num << endl << endl;
        num++;
    }
    for (int i = 1; i <= activeAgents; i++)
        if (agenti[i]) cout << "Agentul #" << i << "(" << agenti[i] << ") este castigator!";
}

void Board::simulateByRounds() {
    int numAgentsLeft = 0;
    string input;
    cout << "La fiecare integorare 'urmatoarea runda?' raspundeti cu y/n/s \n y pentru da \n n pentru n \n s pentru da si sa se afiseze si tabla\n";
    cout << "urmatoarea runda?\n";
    cin >> input;
    while (input == "y" || input == "s") {
        numAgentsLeft = 0;
        for (int i = 1; i <= activeAgents; i++)
            if (agenti[i]) numAgentsLeft++;
        if (numAgentsLeft == 1)break;
        this->nextRound();
        if (input == "s") this->showBoard();
        cout << "urmatoarea runda? \n";
        cin >> input;
    }
    if (numAgentsLeft == 1) {
        for (int i = 1; i <= activeAgents; i++)
            if (agenti[i]) cout << "Agentul #" << i << "(" << agenti[i] << ") este castigator!";
    }
    else {
        for (int i = 1; i <= activeAgents; i++)
            if (agenti[i]) cout << "Agentul #" << i << "(" << agenti[i] << ") a ramas in competitie.\n";
    }
    cout << "\n";
}

// items
int Item::ID = 0;
Item::Item():id(++ID) {};
Item::~Item() {};
int Item::getID() {
    return Item::id;
}

//armors - base class
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
HealingShield::HealingShield(int _weigth, int _protection, int heal) : Armor(_weigth, _protection), healRate(heal), maxProt(_protection) {};
HealingShield::HealingShield(int _weigth, int _protection) : Armor(_weigth, _protection), maxProt(_protection) {
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
    this->heal();  // propietatea de baza a acestei armuri, dupa ce primeste o lovitura, se vindeca cu o anumita rata
}

    //ForceShield
ForceShield::ForceShield(int _weigth, int _protection, int stops): Armor(_weigth,_protection), numOfStops(stops){}
ForceShield::ForceShield(int _weigth, int _protection) : Armor(_weigth, _protection){
    numOfStops = 2;
}
ForceShield::ForceShield(){
    numOfStops = 2; 
}
ForceShield::~ForceShield() {};
void ForceShield::takeHit(Agent* owner, int hit) {
    if (numOfStops > 0) {
        numOfStops--; cout << owner << " blocked a shot" << endl;
    }            // armura blocheaza maxim numOfStops lovituri
    else {
        protection -= hit;
        if (protection > 0) {
            cout << owner << " got shot, " << protection << " armor HP left" << endl;
        }
    }
}

    //Kevlar
Kevlar::Kevlar(int _weigth, int _protection, int dmgRed): Armor(_weigth,_protection), dmgReduction(float(dmgRed)/100){}
Kevlar::Kevlar(int _weigth, int _protection) : Armor(_weigth, _protection) {
    dmgReduction = 0.2;
}
Kevlar::Kevlar() {
    dmgReduction = 0.2;
}
Kevlar::~Kevlar() {};
void Kevlar::takeHit(Agent* owner, int hit) {
    protection -= hit - int(hit * dmgReduction);  // fiecare lovitura este redusa cu un anumit procent
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
    dmg -= armor->getWeight() / 10;     // puterea armei este afectata de greutatea pe care o armura agentului ce poseda arma respectiva
}



    //Revolver
Revolver::Revolver(int damage): Weapon(damage) {
    magazine = 6;
}
Revolver::Revolver() : Weapon() {
    magazine = 6;
}
Revolver::~Revolver() {};
int Revolver::shootDead(Agent* owner,Agent* that) {  // owner - that reprezinta flow-ul unei lovituri ( de la owner catre that)
    if (magazine > 0) {
        cout << owner << " shot " << that << endl; magazine--;
        that->getShot(this->dmg);
        if (that->getHp() <= 0) {    // daca agentul care este lovit nu mai are hp pozitiv, acesta este declarat ucis si sters din joc
            cout << owner << " killed " << that << endl;
            delete that;
            return 1;  // returneaza 1 daca reuseste sa ucida agentul care primeste lovitura, 0 altfel
        }
    }
    else {
        cout <<owner<< " reloaded his revolver" << endl;
        magazine = 6;
    }              // Revolverul poate lovi de 6 ori consecutiv, dupa care necesita o tura penntru a reincarca arma, reincarcarea se poate realiza doar daca un agent este in
                   // aria de vizibilitate a agentului ce poseda arma
    return 0;
}

    // Shotgun
Shotgun::Shotgun(int damage) : Weapon(damage) {
    barrels = 2;
}
Shotgun::Shotgun() : Weapon() {
    barrels = 2;
}
Shotgun::~Shotgun() {};
int Shotgun::shootDead(Agent* owner, Agent* that) { // owner - that reprezinta flow-ul unei lovituri ( de la owner catre that)
    for (int i = 1; i <= barrels; i++) {
        cout << owner << " shot " << that << endl;
        that->getShot(this->dmg);
        if (that->getHp() <= 0) {  // daca agentul care este lovit nu mai are hp pozitiv, acesta este declarat ucis si sters din joc
            cout << owner << " killed " << that << endl;
            delete that;
            return 1;  // returneaza 1 daca reuseste sa ucida agentul care primeste lovitura, 0 altfel
        }  // Shotgun-ul loveste de barrels ori in aceeasi tura
    }
    return 0;
}

    //Bow
Bow::Bow(int damage) : Weapon(damage) {
    dmgMultiplier = 1.2;
}
Bow::Bow() : Weapon() {
    dmgMultiplier = 1.2;
}
Bow::~Bow() {};
int Bow::shootDead(Agent* owner, Agent* that) { // owner - that reprezinta flow-ul unei lovituri ( de la owner catre that)
    cout << owner << " shot " << that << endl;
    that->getShot(this->dmg);
    this->dmg = int(this->dmg * dmgMultiplier);
    if (that->getHp() <= 0) {  // daca agentul care este lovit nu mai are hp pozitiv, acesta este declarat ucis si sters din joc
        cout << owner << " killed " << that << endl;
        delete that;
        return 1; // returneaza 1 daca reuseste sa ucida agentul care primeste lovitura, 0 altfel
    }
    return 0;
}


//Agents
Agent::Agent(Armor* armor, Weapon* weapon, int _reach) : reach(_reach), armura(armor), arma(weapon) {
    // un agent este initializat cu o armura, arma si reach
    arma->getAffectedByArmor(armor); // arma este afectata de greutatea armurii de la inceput
    moveOpt = 1; // fiecare agent are un pattern in care se deplaseaza si incepe cu prima varianta de mers prin matrice
    hp = 100;
};
Agent::Agent(int reach) : reach(reach) {
    // se poate opta pentru initilizarea unui agent cu o arma si o armura basic
    arma = new Revolver();
    armura = new Kevlar();
    arma->getAffectedByArmor(armura);
    moveOpt = 1;
    hp = 100;
}
Agent::Agent() {
    // se poate opta pentru initilizarea unui agent cu o arma, o armura si un reach de 3 basic
    reach = 3;
    arma = new Revolver();
    armura = new Kevlar();
    arma->getAffectedByArmor(armura);
    moveOpt = 1;
    hp = 100;
}
Agent::~Agent() {
    if(armura)
    delete[] armura; // cel mai probabil armura va fi stearsa pana in momentul uciderii
    delete[] arma; 
};
void Agent::setCoordinates(int _x, int _y, int _maxx, int _maxy) { 
    x = _x; y = _y; maxx = _maxx; maxy = _maxy;
    // x,y = pozitita in matricea jocului
    // maxx, maxy = capacitatea matricei
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
    // cand un agent loveste un alt agent, arma lui il loveste defapt si returneza 1 daca acesta reuseste sa-l ucida
    return arma->shootDead(this,that);
}
void Agent::takeHitInArmor(int hit) {
    armura->takeHit(this,hit); // primeste lovitura in armura
    if (armura->getProtection() <= 0) { // daca aceasta nu mai are protectie pozitiva, este stearsa
        delete armura; cout << this << "'s armor was destroyed" << endl;
        armura = 0;
    }
}
void Agent::takeHitInHp(int hit) {
    hp -= hit;  // primeste lovitura direct
    if (hp > 0) { // daca agentul nu mai are hp pozitiv, acesta este declarat ucis
        cout << this << " got shot, " << hp << " hp left" << endl;
    }
}
bool Agent::haveArmor() {
    if (armura) return true;
    else return false;
}
void Agent::getShot(int hit) {
    // indica unde primeste agentul lovitura, pe baza existentei unei armuri
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
    // miscare in matrice intr un triunghi drepthungic, pana in extremele matricei
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
    // miscare in matrice intr un patrat de lungime 6, in limita posibilitatii
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


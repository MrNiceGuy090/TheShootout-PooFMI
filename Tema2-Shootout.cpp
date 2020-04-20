#include "Shootout.h"
#include <iostream>
using namespace std;
void simulateAllGame(Board* game) {
    while (game->getAgentsleft() > 1)
    {
        game->nextRound(); cout << endl;
    }
    game->showBoard();
}
int main()
{
    Bow* spear = new Bow(50);
    Shotgun* mini = new Shotgun(40);
    Revolver* rev = new Revolver(70);
    HealingShield* hshield = new HealingShield(200, 70, 10);
    ForceShield* fshield = new ForceShield(200, 100, 3);
    Kevlar* kshield = new Kevlar(200, 100, 30);
    HealingShield* h2shield = new HealingShield(300, 140);

    Krul* a1 = new Krul(hshield, spear, 5);
    Annesix* a2 = new Annesix(kshield, mini, 3);
    Krul* a3 = new Krul(fshield, spear, 7);
    Annesix* a4 = new Annesix(h2shield, rev, 6);

    Board* game = new Board(25,25,5);
    game->insertAgent(a1,1,1);
    game->insertAgent(a2, 13, 13);
    game->insertAgent(a3, 25, 25);
    game->insertAgent(a4, 18, 12);

    simulateAllGame(game);

    return 0;
}


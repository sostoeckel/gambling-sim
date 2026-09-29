#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <ctime>
#include <limits>
#include <unistd.h>
using namespace std;

//"I pledge my honor that I have abided by the Stevens Honor System."

int randIntRange (int start, int endInclusive) {
  return start + (rand() % (endInclusive));
  srand(time(0));
}

bool testRand (int generationThreshold) {
  int randRoll = randIntRange (1, 100);
  return (randRoll <= generationThreshold);
}

class Card {
  public:
    enum suit {spades = 0, hearts = 1, diamonds = 2, clubs = 3};
    enum value {ace = 1, two = 2, three = 3, four = 4, five = 5, six = 6, seven = 7, eight = 8, nine = 9, ten = 10, jack = 11, queen = 12, king = 13};

    suit thisSuit;
    value thisValue;

    int getIntVal () {
      return static_cast<int>(thisValue);
    }

    string getName () {
      string toReturn;
      switch (thisValue) {
        case (ace) :
          toReturn = "Ace";
          break;
        case (jack) :
          toReturn = "Jack";
          break;
        case (queen) :
          toReturn = "Queen";
          break;
        case (king) :
          toReturn = "King";
          break;
        default:
          toReturn = to_string(getIntVal());
          break;
      }
      return toReturn;
    }

    string getSuit () {
      string toReturn;
      switch (thisSuit) {
        case spades:
          toReturn = "Spades";
          break;
        case hearts:
          toReturn = "Hearts";
          break;
        case diamonds:
          toReturn = "Diamonds";
          break;
        case clubs:
          toReturn = "Clubs";
          break;
      }
      return toReturn;
    }

    int getBlackJackNum () {
      if (getIntVal() >= 11) {
        return 10;
      } else if (getIntVal() == 1) {
        return 11;
      } else {
        return getIntVal();
      }
    }

    /*
    Card(suit suitToSet, value valToSet) {
      thisSuit = suitToSet;
      thisValue = valToSet;
    }*/
};

class Deck {
  public:
    vector<Card> cards;

    Deck () {
      for (int i = 0; i < 4; i++) {
        for (int j = 1; j <= 13; j++) {
          //Card cardToPush = Card(static_cast<Card::suit>(i), static_cast<Card::value>(j));
          Card cardToPush;
          cardToPush.thisSuit = static_cast<Card::suit>(i);
          cardToPush.thisValue = static_cast<Card::value>(j);
          cards.push_back(cardToPush);
        }
      }
    }

    void shuffleCards () {
      srand(time(0));
      int numCards = cards.size();

      for (int i = 0; i < numCards - 1; i++) {
        int j = rand() % (numCards - i);
        Card cardi = cards.at(i);
        Card cardj = cards.at(rand() % (numCards - i));
        cards.at(i) = cardj;
        cards.at(j) = cardi;
      }
    }

    Card deal () {
      Card toReturn = cards.back();
      cards.pop_back();
      return toReturn;
    }
};

//had by both dealer and player; both implement diff. functions
class BlackJackHand {
  public:
    Card cards[5];
    int numInHand = 0;

    void takeCard (Card dealt) {
      cards[numInHand] = dealt;
      numInHand++;
    }

    int sumCards () {
      int sum = 0;
      int numAces = 0;

      for (int i = 0; i < numInHand; i++) {
        sum += cards[i].getBlackJackNum();
        if ((cards->thisValue) == Card::ace) {
          numAces++;
        }
      }
      //converts ace 11s to 1s until sum < 21 or runs out of aces
      if (sum > 21) {
        for (int i = 0; i < numAces; i++) {
          //sum - 10 <= 21
          sum -= 10;
          if (sum <= 21) {
            break;
          }
        }
      }

      return sum;
    }

    string printValsTail () {
      string toReturn = cards[1].getName();
      for (int i = 2; i < numInHand; i++) {
        toReturn = toReturn + ", " + cards[i].getName();
      }
      return toReturn;
    }

    //prints the faces of the cards
    string printAllVals () {
      return (cards[0].getName() + ", " + printValsTail());
    }
};

class Player {
  public:
    int stressPercent;
    int intoxPercent;
    int dollars; //in dollars, not dealing in cents
    bool recentlyDrank;
    bool havePlayed;
    bool willHeartAttack;//implemeneted in main/casino floor 
    //heart attack will occur on the floor of the casino
    bool willBlackout;
    bool willSnap;
    int numWins;
    int numLosses;

    void increaseStress (int times) {
      stressPercent += times;
    }

    void decreaseStress (int times) {
      stressPercent -= times;
      if (stressPercent < 0) {
        stressPercent = 0;
      }
    }

    void intox (int strength) {
      intoxPercent += strength; //strength inputs will be 5, 10, & 20
    }

    void detox () {
      //forms an approximation of an inverse graph
      int toxEchelon = (intoxPercent / 33) + 1;
      intoxPercent -= toxEchelon;
      if (intoxPercent < 0) {
        intoxPercent = 0;
      }
    }

    void detox (int times) {
      //forms an approximation of an inverse graph
      for (int i = 0; i < times; i++) {
        int toxEchelon = (intoxPercent / 33) + 1;
        intoxPercent -= toxEchelon;
        if (intoxPercent < 0) {
          intoxPercent = 0;
        }
      }
    }

    void adviseBar () {
      if (stressPercent > 50) {
        cout << "> A trip to the bar may help you.\n";
      }
    }

    //is used to signal the beginning of a rash decision
    bool testRash () {
      if (stressPercent <= 100) {
        return false;
      } else {
        return (testRand(stressPercent - 100));
      }
    }

    //is used to signal "drunk" reactions, like passing out or not being able to see your cards.
    bool testDrunk () {
      if (intoxPercent <= 100) {
        return false;
      } else {
        return (testRand(intoxPercent - 100));
      }
    }

    void stressMechs(){
      if (stressPercent >= 100) {
        cout << "> You can't remember how long you've been here. But you have to keep going. You can't stop. Not now.\n";
        if (intoxPercent >= 100) {
          cout << "> You feel like you could snap at any moment.\n\n";
          if (testRash()) {
            willSnap = true;
          }
        } else {
          cout << "> You feel a pain in your chest.\n\n";
          if (testRash()) {
            willHeartAttack = true;
          }
        }
      }
      if(stressPercent > 80) {
        cout << "> You feel like a nail is piercing your skull and you can't stop clenching your fists.\n\n";
      }
      if(stressPercent > 60)
      {
        cout << "> You feel your heart pounding in your chest.\n\n";
      }
      if(stressPercent > 40) {
        cout << "> Beads of sweat coat your forehead.\n\n";
      }
      if(stressPercent > 20) {
        cout << "> You feel on edge.\n\n";
      }
    }

    void intoxMechs(){
      if (intoxPercent >= 100) {
        cout << "> The world spins.  It is hard to think.\n";
        if (stressPercent >= 100) {
          cout << "> Anger and alcohol course through your veins. You feel like you could snap at any moment.\n\n";
          if (testDrunk()) {
            willSnap = true;
          }
        } else {
          cout << "> You feel cold. Very cold. Maybe more drinks will warm you up.\n\n";
          if (testRash()) {
            willBlackout = true;
          }
        }
      }
      if(intoxPercent > 80) {
        cout << "> The bright lights of the casino hurts to look at.\n\n";
      }
      if(intoxPercent > 60)
      {
        cout << "> You cough up a bit of vomit.\n\n";
      }
      if(intoxPercent > 40) {
        cout << "> You feel a lot woozier and a bit ill.\n\n";
      }
      if(intoxPercent > 20) {
        cout << "> Your hands feel clammy and you feel a bit woozy.\n\n";
      }
    }

    Player () {
      stressPercent = 5;
      intoxPercent = 0;
      dollars = 500;
      recentlyDrank = false;
      havePlayed = false;
      willHeartAttack = false;
      willBlackout = false;
      willSnap = false;
      numWins = 0;
      numLosses = 0;
  }
};

Player player1;

class BlackJackGame {
  public:   
    void runBlackJackRound () {
      srand(time(0));
      BlackJackHand playerHand;
      BlackJackHand dealerHand;
      Deck deck;
      player1.havePlayed = true;
      deck.shuffleCards();
      //deals 2 cards to both the player and the dealer
      playerHand.takeCard(deck.deal());
      playerHand.takeCard(deck.deal());
      dealerHand.takeCard(deck.deal());
      dealerHand.takeCard(deck.deal());
      bool playerBust = false;
      bool playerWin = false;
      bool dealerBust = false;
      bool dealerWin = false;
      int bet;
      bool firstTime = true;
      //int stressMult = 1;// allows for stress to be modified based on outcome
      int stressUp = 8;
      int stressDown = 4;

      cout << "> You walk up to one of the tables on the casino floor. The dealer greets you: \"Welcome " << ((player1.havePlayed) ? ("back ") : "") << "to the table.\"";
      if (player1.recentlyDrank) {
        cout << "  How was your drink?";
        player1.recentlyDrank = false;
      }
      cout << "  The game is blackjack. Place your bet.\"\n\n" << endl;

      cout << "> Total funds: $" << player1.dollars << "\n";
      cout << "> How many dollars would you like to bet?\n> ";
      cin >> bet;
      player1.dollars -= bet;
      cout << "\n> The dealer deals out 2 cards to you and themselves." << "\n\n";

      cout << "> Dealer's Hand:    " << "[Unshown], " << dealerHand.printValsTail() + "\n";
      cout << "> Your Hand:        " + playerHand.printAllVals() + "\n\n";
      cout << "> Optimal Card Sum: " + to_string(playerHand.sumCards()) + "\n\n";

      //catches starting blackjacks 
      playerWin = (playerHand.sumCards() == 21);
      dealerWin = (dealerHand.sumCards() == 21);

      //player turns hit/stand loop
      while (!(playerBust || playerWin)) {
        if (playerHand.numInHand >= 5) {
          break;
        }

        bool rashRound = player1.testRash();
        //bool drunkRound = player1.testDrunk();

        if (!firstTime) {
          if (player1.testDrunk()) {
            cout << "> You know that you and the dealer have cards on the table, but you can't make them out.\n";
            cout << "> You think one of your cards was " << playerHand.cards[(rand() % playerHand.numInHand)].getName() << "\n";
            cout << "> You think the total sum of your cards was " << to_string(playerHand.sumCards() + (rand() % 5) - 2) << "?\n\n";
          } else {
            cout << "> Dealer's Hand:    " << "[Unshown], " + dealerHand.printValsTail() + "\n";
            cout << "> Your Hand:        " + playerHand.printAllVals() + "\n";
            cout << "> Optimal Card Sum: " + to_string(playerHand.sumCards()) + "\n\n";
          }
        }

        int choice = 2;

        if (player1.testDrunk()) {
          choice = (rand() % 2) + 1;

          cout << "> You mumble and try to think of how to respond to the dealer's muddled words, placing your hands on the table" << ((choice==0) ? (", but slip and hit your head on the edge.") : (" to keep yourself vertical, sliding them in the process.")) << "\n";
          cout << "> You hear the dealer between the ringing in your ears say, \"" << ((choice == 0) ? ("I think that's a hit!") : ("Are you trying to stand?")) << ".\n\n";
        } else if (player1.testRash()) {
          cout << "> A ringing in your ears becomes more apparent.  You begin to grip at your hair and a thought comes to you: \"I can stop whenever I want to, but 90% of gamblers quit right before they hit it big.\nYou slam your fist on the table.\"\n\n";
          choice = 1;
        } else {
          cout << "> Would you like to hit or stand?" << endl;
          cout << "> Hit - 1\n> Stand - 2\n> Surrender - 3\n" << endl;
          cin >> choice;
        }

        if (choice == 1) {
          Card cardDealt = deck.deal();
          cout << "> You are dealt another card face-up, the " + cardDealt.getName() + " of " + cardDealt.getSuit() + "\n";
          playerHand.takeCard(cardDealt);
        } else if (choice == 3) {
          cout << "> You surrender your hand and choose to bust.\n\n";
          playerBust = true;
          stressUp = 4;
        } else {//player stands
          cout << "> You choose to stand.\n\n";
          break;//enters dealer accumulation phase
        }

        player1.detox();

        if (playerHand.sumCards() > 21) {
          cout << "Your cards total to " << playerHand.sumCards() << ".\n";
          cout << "You have bust.\n\n";
          playerBust = true;
          break;
        }
      }
      //dealer procedure
      if (!playerBust) {
        cout << "> The dealer flips over their first card.\n";
        cout << "> Dealer's Hand            :"+ dealerHand.printAllVals() + "\n";
        cout << "> Dealer's Optimal Card Sum: " + to_string(dealerHand.sumCards()) + "\n\n";

        while (dealerHand.sumCards() <= 16) {
          cout << "> The dealer draws another card. + \n";
          cout << "> Dealer's Hand            :"+ dealerHand.printAllVals() + "\n";
          cout << "> Dealer's Optimal Card Sum: " + to_string(dealerHand.sumCards()) + "\n\n";
        }
        dealerBust = (dealerHand.sumCards() > 21);
      }
      //cases to make: playerBust, dealerBust, Draw, playerwin, dealerwin
      if (playerBust) {//stressful
        cout << "> You have lost your bet. You feel a bit more stressed.\n";
        player1.adviseBar();
        player1.increaseStress(stressUp);
        if (player1.numWins > 0) {
          cout << "> The dealer says:\"Keep trying, you've already won before so you can do it again!\"\n\n";
        } else {
          cout << "> The dealer says:\"I'm sure you'll win at some point.  Keep trying.\"\n\n";
        }
        cout << "> Total funds: $" << player1.dollars << "\n\n";
        return;
      } else if (dealerBust) {//calming
        if (playerHand.sumCards() == 21) {
          cout << "> The dealer has bust. You receive 3:2 your bet back due to blackjack.\n";
          player1.dollars += (5*bet)/2;
        } else {
          cout << "> The dealer has bust. You receive twice your bet back.\n";
          player1.dollars += 2*bet;
        }
        cout << "> The dealer says:\"Great job! Keep at it. You could win so much more.\"\n\n";
        player1.decreaseStress(stressDown);
        cout << "> Total funds: $" << player1.dollars << "\n\n";
      } else if (playerHand.sumCards() == dealerHand.sumCards()) {//neutral
        cout << "> You and the dealer have drawn.  You receive your bet back.\n";
        player1.dollars += bet;
        cout << "> Total funds: $" << player1.dollars << "\n\n";
      } else if (playerHand.sumCards() > dealerHand.sumCards()) {//calming
        if (playerHand.sumCards() == 21) {
          cout << "> You have won by beating the dealer. You receive 3:2 your bet back due to blackjack.\n";
          player1.dollars += player1.dollars += (5*bet)/2;
        } else {
          cout << "> You have won by beating the dealer. You receive twice your bet back.\n";
          player1.dollars += player1.dollars += 2*bet;
        }
        cout << "> The dealer says:\"Stupendous. With this luck you could win it all.\"\n\n";
        player1.decreaseStress(stressDown);
        cout << "> Total funds: $" << player1.dollars << "\n\n";
      } else if (dealerHand.sumCards() > playerHand.sumCards()) {//stressful
        cout << "> You have lost to the dealer's hand. You have also lost your bet. You feel a bit more stressed.\n";
        player1.adviseBar();
        player1.increaseStress(stressUp);
        if (player1.numWins > 0) {
          cout << "> The dealer says:\"Keep trying, you've already won before so you can do it again!\"\n\n";
        } else {
          cout << "> The dealer says:\"I'm sure you'll win at some point.  Keep trying.\"\n\n";
        }
        cout << "> Total funds: $" << player1.dollars << "\n\n";
      }
    }
};

void barScene(bool greet){
  int drink;
  int sol;
  if (greet) {
    cout << "> Welcome to the bar. What would you like to drink?" << endl;
  }
  cout << "> Alcohol options:" << endl;
  cout << "> Miller Lite - 1\n> Rum - 2\n> Jungle Juice - 3\n> " << endl;
  cin >> drink;
  cout << "> He pours you a glass and sets it in front of you." << endl;
  cout << "> You drink it and look around at the casino." << endl;
  if(drink == 1){
    player1.decreaseStress(5);
    player1.intox(5);
    player1.dollars -= 5;
  } else if(drink == 2){
    player1.decreaseStress(10);
    player1.intox(10);
    player1.dollars -= 7;
  } else if(drink == 3){
    player1.decreaseStress(20);
    player1.intox(20);
    player1.dollars -= 7;
  }
  cout << "> The bartender looks at you with solemn eyes." << endl;
  cout << "> Do you want to drink another - 1\nOR\n Head back to keep playing - 2" << endl;
  cin >> sol;
  if(sol == 1) {
    barScene(false);
  } else if(sol == 2) {
    cout << "> The bartender bids you farewell.\n\n";
  }
}

// ending 1: (done)
// ending 2: // leaving voluntarily
void ending2() {
  cout << "> Despite all of the blaring sights and souds, you are able to navigate to one of the exits." << endl;
  cout << "> You see many wayward groups and individuals entering the casino and remember that you were once as excited as them." << endl;
  cout << "> However long you spent in the casino, it sure passed by fast.\n";
  unsigned int microsecond = 1000000;
  usleep(2 * microsecond);
  cout << "> Ending 2: Left." << endl;
}
// ending 3: // heart attack
void ending3(){
  cout << "> You feel your heart pounding and your chest feels tight. You feel a sharp pain in your left arm that spreads to your jaw. Your vision starts darkening." << endl;
  cout << "> You slowly tip off your chair and slide to the floor, but in your state you barely notice it." << endl;
  cout << "> The world goes black then a bright white as it all goes quiet.\n";
  unsigned int microsecond = 1000000;
  usleep(2 * microsecond);
  cout << "> Ending 3: Goodbye." << endl;
}
// ending 4: // blacking out
void ending4(){
  cout << "> You feel woozy and your vision spins and you try your best to grab onto anything--the table, the chair-- to stop yourself from falling, but your hands can't seem to hold onto anything." << endl;
  cout << "> As you slowly lose consciousness, you regret drinking so much, and why you hadn't left sooner." << endl;
  unsigned int microsecond = 1000000;
  usleep(2 * microsecond);
  cout << "> Ending 4: Blacked out." << endl;
}
// ending 5: // snapping
void ending5(){
  cout << "> Anger takes over your brain and you stand up suddenly, knocking your chair to the floor." << endl;
  cout << "> For a second you can't feel anything except your rage as you leap across the table and grab the dealer's neck, squeezing it as tightly as possible." << endl;
  cout << "> He freezes in surprise then in shock grabs your hands around his neck as he chokes." << endl;
  cout << "> Your fings claw at his neck as you feel security grabbing your shoulders, pulling you off of him." << endl;
  cout << "> They drag you outside to waiting police cars but all you can think about is the money you've lost and the terrible future you face." << endl;
  unsigned int microsecond = 1000000;
  usleep(2 * microsecond);
  cout << "> Ending 5: Snapped." << endl;
}
//ending 6: LOST ALL MONEY RAHHH
void ending6(){
  cout << "> You check your pockets and your heart drops as you realise you have nothing left. No money. Not even a penny." << endl;
  cout << "> You turn to the door and leave solemnly. It was fun while it lasted." << endl;
  unsigned int microsecond = 1000000;
  usleep(2 * microsecond);
  cout << "> Ending 6: Spent." << endl;
}

BlackJackGame bg;

int main() {
  //flavor text can change with >50 stress || intox
  bool hasLeftVoluntary = false;//2nd ending (leave before any negative consequences)
  bool hasHeartAttack = false;//3rd ending (snapping due to stress)
  bool hasBlackedOut = false;//4th ending (paddy wagon)
  bool hasSnapped = false;//5th ending (attack dealer)
  bool isDestitute = false;//6th ending

  char choice1;
  cout << "The courage to be vulnerable is not about winning or losing, it's about the courage to show up when you can't predict or control the outcome." << endl;
  unsigned int microsecond = 1000000;
  usleep(3 * microsecond);
  cout << "> Start?" << endl;
  cout << "> y/n" << endl;
  cin >> choice1;
  if(choice1 == 'y'){
    cout << "> Instructions:" << endl;
    cout << "> Gamble to try to win as much money as you can. Take a break at the bar and check out the blackjack table. Live your best life.\n\n";
    unsigned int microsecond = 1000000;
    usleep(3 * microsecond); //sleeps for 3 second
  } else {
    cout << "> Why did you start playing this game then.\n\n";
    unsigned int microsecond = 1000000;
    usleep(2 * microsecond);
    cout << "> Ending 1. The Quitter.\n\n";
    return(0);
  }

  //main "casino floor" base area - should house the entrances to the bar, blackjack, and exit of the casino
  while (!((hasLeftVoluntary) || (hasHeartAttack) || (hasBlackedOut) || (hasSnapped) || (isDestitute))) {
    int actChoice;
    cout << "> You stand on the casino floor.\n\n";
    unsigned int microsecond = 1000000;
    usleep(2 * microsecond);
    cout << "> You can select from the following options:\n";
    cout << "> 1 - Play blackjack at one of the tables\n> 2 - Walk over to the bar\n> 3 - Take moment to rest\n> 4 - Exit the casino\n\n> ";
    cin >> actChoice;

    switch (actChoice) {
      case 1 :
        bg.runBlackJackRound();
        break;
      case 2:
        barScene(true);
        break;
      case 4:
        cout << "You turn to face the doors of the casino.\n\n";
        hasLeftVoluntary = true;
        break;
      default:
        cout << "> You take a few minutes to sit down on the one of the couches.\n> You feel a bit refreshed.";
        player1.detox(2);
        player1.decreaseStress(4);
        break;
    }

    player1.stressMechs();
    player1.intoxMechs();
    hasHeartAttack = player1.willHeartAttack;
    hasBlackedOut = player1.willBlackout;
    hasSnapped = player1.willSnap;
    isDestitute = (player1.dollars <= 0);

    player1.detox();
  }

  if (hasLeftVoluntary) {
    ending2();
  } else if (hasHeartAttack) {
    ending3();
  } else if (hasBlackedOut) {
    ending4();
  } else if (hasSnapped) {
    ending5();
  } else if (isDestitute) {
    ending6();
  }

  return(0);
}
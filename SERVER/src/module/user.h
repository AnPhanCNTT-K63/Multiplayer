#pragma once
#include<iostream>
#include<stdio.h>
#include <algorithm>

#include "algorithms.h"
using namespace std;

vector<vector<Card>> history;

class User
{
private:
    vector<Card> userCards;

    // index of selected cards
    vector<int> selectedCards;

    int money = 0;
    int cardCount = 13;
    bool isFirst = false;
    bool isSpecial = false;
    bool isTurn = false;

public:
    User(PlayingCards &plCards)
    {
        for (int i = 0; i < 13; i++)
        {
            Card lastCard = plCards.get1Card();
            this->userCards.push_back(lastCard);
        }

        isFirstUser();
    }

    void sortCard()
    {
        mergeSort(this->userCards, 0, this->cardCount - 1);
    }

    vector<Card> getUserCards()
    {
        return userCards;
    }

    int getCardCount()
    {
        return this->cardCount;
    }

    void setCardCount(int num)
    {
        this->cardCount -= num;
    }

    void clearUserCards()
    {
        this->userCards.clear();
    }

    void printCards()
    {
        sortCard();
        for (int i = 0; i < cardCount; i++)
        {
            userCards[i].setX(i * 80 + 50);

            // selected cards
            if (count(selectedCards.begin(), selectedCards.end(), i))
            {
                userCards[i].setY(525);
            }
            else
            {
                userCards[i].setY(560);
            }

            // Render texture to screen
            SDL_RenderCopy(gRenderer, userCards[i].getTexture(), NULL, userCards[i].getDestinationRect());
        }
    }

    void changeSelected(int x)
    {

        // check if x in changeSelected of not?
        if (count(selectedCards.begin(), selectedCards.end(), x))
        {
            selectedCards.erase(remove(selectedCards.begin(), selectedCards.end(), x), selectedCards.end());
        }
        else
        {
            selectedCards.push_back(x);
        }
    }

    void hit()
    {
        int selectedCardLen = selectedCards.size();

        // check is valid cards?

        // change turn (no user's turn)
        isTurn = false;

        // sort selected cards
        sort(selectedCards.begin(), selectedCards.end());

        // append history
        int historyIndex = history.size();
        if (historyIndex > 2)
        {
            history.clear();
        }

        vector<Card> temp;
        for (int index : selectedCards)
        {
            temp.push_back(userCards[index]);
        }
        history.push_back(temp);

        for (int i = selectedCardLen - 1; i >= 0; i--)
        {
            userCards.erase(userCards.begin() + selectedCards[i]);
            cardCount--;
        }

        selectedCards.clear();
    }

    void isFirstUser()
    {
        for (Card card : userCards)
        {
            if (card.getSuits() == SPADES && card.getValue() == 3)
            {
                this->isTurn = true;
            }
        }
    }

    bool isUserTurn()
    {
        return isTurn;
    }

    void setUserTurn(bool value)
    {
        isTurn = value;
    }
    // int countSuits()
    // {
    //     int countBlack = 0;
    //     int countRed = 0;
    //     for (const Card &card : this->userCards)
    //     {
    //         if (card.getSuits() == 3 || card.getSuits() == 4)
    //         {
    //             countRed++;
    //         }
    //         else
    //         {
    //             countBlack++;
    //         }
    //     }
    //     return countBlack < countRed ? countRed : countBlack;
    // }
    // map<int, int> getSaveCards() // Create a map to save cards
    // {
    //     map<int, int> saveCards;
    //     for (const Card &card : this->userCards)
    //     {
    //         saveCards[card.getValue()]++;
    //     }
    //     return saveCards;
    // }
    // void isSpecialCards() // Check for a perfect hand
    // {
    //     map<int, int> saveCards = userCards.getSaveCards();
    //     int count = 0; // Count couple
    //     int countQuads = 0;
    //     int countJack = 0;
    //     for (pair<int, int> x : saveCards)
    //     {
    //         if (x.second >= 2)
    //         {
    //             if (x.second == 4)
    //             {
    //                 if (x.first == 2)
    //                 {
    //                     countJack++;
    //                     break;
    //                 }
    //                 else
    //                 {
    //                     countQuads++;
    //                 }
    //             }
    //             count++;
    //         }
    //     }
    //     if (count == 6) // Have 6 couple
    //     {
    //         this->isSpecial = true;
    //     }
    //     else if (count == 5)
    //     {
    //         if (countQuads == 1) // Have 4 couple and a quad
    //         {
    //             this->isSpecial = true;
    //         }
    //         else // Have five consecutive pairs
    //         {
    //             int flag = 1;
    //             int prevKey = -1;
    //             for (pair<int, int> x : saveCards)
    //             {
    //                 if (x.second >= 2)
    //                 {
    //                     if (prevKey != -1 && x.first != prevKey + 1)
    //                     {
    //                         flag = -1;
    //                         break;
    //                     }
    //                     prevKey = x.first;
    //                 }
    //             }
    //             if (flag == 1)
    //             {
    //                 this->isSpecial = true;
    //             }
    //         }
    //     }
    //     else if (count == 4) // Have 2 couple and 2 quads
    //     {
    //         if (countQuads >= 2)
    //         {
    //             this->isSpecial = true;
    //         }
    //     }
    //     else if (countJack == 1) // Have 4 Jack;
    //     {
    //         this->isSpecial = true;
    //     }
    //     else if (count <= 1) // 3-A
    //     {
    //         this->isSpecial = true;
    //     }
    //     else if (userCards.countSuits() >= 12) // Have 12 black cards or 12 red cards
    //     {
    //         this->isSpecial = true;
    //     }
    // }
};

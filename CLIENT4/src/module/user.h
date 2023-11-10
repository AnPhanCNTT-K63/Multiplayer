#pragma once
#include <iostream>
#include<vector>
#include<map>
#include <algorithm>
#include "algorithms.h"
using namespace std;

vector<vector<Card>> history;
vector<int> gameResult;
vector<string> sendCardList;

class User
{
protected:
    vector<Card> userCards;

private:
    // index of selected cards
    vector<int> selectedCards;

    int money = 0;

    bool isFirst = false;
    bool isSpecial = false;
    bool isSkip = false;
    bool isTurn = false;
    bool isFinish = false;

    SDL_Texture *winTexture = NULL;

public:
    User(PlayingCards &plCards)
    {
        initUser(plCards);
        this->setWinTexture();
    }

    void initUser(PlayingCards &plCards)
    {
        userCards.clear();
        for (int i = 0; i < 13; i++)
        {
            Card lastCard = plCards.get1Card();
            this->userCards.push_back(lastCard);
        }
        mergeSort(userCards, 0, 12);
        this->isFinish = false;
        this->isSkip = false;

        checkSpecialCards();

        isFirstUser();
    }

    int selectedCardsLen()
    {
        return selectedCards.size();
    }

    void setWinTexture(int id = -1)
    {
        if (id == -1)
        {
            winTexture = loadTexture("src/image/win.png");
        }
        if (id == 1)
        {
            winTexture = loadTexture("src/image/win1.png");
        }
        if (id == 2)
        {
            winTexture = loadTexture("src/image/win2.png");
        }
        if (id == 3)
        {
            winTexture = loadTexture("src/image/win3.png");
        }
    }

    int getCardCount()
    {
        return userCards.size();
    }

    void sortCard()
    {
        int cardCount = getCardCount();
        mergeSort(userCards, 0, cardCount - 1);
    }

    vector<Card> getUserCards()
    {
        return userCards;
    }

    void clearUserCards()
    {
        this->userCards.clear();
    }

    void printCards()
    {
        sortCard();

        int cardCount = getCardCount();

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

    void printWinner(int id = -1)
    {
        int len = gameResult.size(); // length of gamerResult

        // this opens a font style and sets a size
        TTF_Font *Lazy = TTF_OpenFont("src/fonts/Freedom-nZ4J.otf", 40);
        SDL_Color White = {255, 255, 255};

        for (int i = 0; i < len; i++)
        {
            SDL_Rect destinationRect, Message_rect;
            if (gameResult[i] == -1)
            {
                destinationRect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 100, 170, 170};
                Message_rect = {SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2 + 220, 100, 100};
            }

            if (gameResult[i] == id)
            {
                if (id == 1)
                {
                    destinationRect = {50, SCREEN_HEIGHT / 2 - 145, 170, 170};
                    Message_rect = {50, SCREEN_HEIGHT / 2, 100, 100};
                }
                else if (id == 2)
                {
                    destinationRect = {SCREEN_WIDTH / 2 - 100, 5, 170, 170};
                    Message_rect = {SCREEN_WIDTH / 2 - 70, SCREEN_HEIGHT / 2 - 195, 100, 100};
                }
                else if (id == 3)
                {
                    destinationRect = {SCREEN_WIDTH - 200, SCREEN_HEIGHT / 2 - 145, 170, 170};
                    Message_rect = {SCREEN_WIDTH - 150, SCREEN_HEIGHT / 2, 100, 100};
                }

                SDL_Surface *surfaceMessage = NULL;
                if (i == 0)
                {
                    surfaceMessage = TTF_RenderText_Solid(Lazy, "FIRST PLACE", White);
                }
                else if (i == 1)
                {
                    surfaceMessage = TTF_RenderText_Solid(Lazy, "SECOND PLACE", White);
                }
                else if (i == 2)
                {
                    surfaceMessage = TTF_RenderText_Solid(Lazy, "THIRD PLACE", White);
                }
                else if (i == 3)
                {
                    surfaceMessage = TTF_RenderText_Solid(Lazy, "FOURTH PLACE", White);
                }

                SDL_Texture *Message = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage);

                SDL_RenderCopy(gRenderer, Message, NULL, &Message_rect);
                SDL_RenderCopy(gRenderer, getWinTexture(), NULL, &destinationRect);
            }
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

    bool getIsFinish()
    {
        return isFinish;
    }

    bool checkWin()
    {
        if (getCardCount() == 0)
        {
            this->isFinish = true;
            return true;
        }
        if (gameResult.size() == 3)
        {
            this->isFinish = true;
            return true;
        }

        return false;
    }

    void setPlace()
    {
        // player has index -1
        gameResult.push_back(-1);
    }

    bool checkSequence(int value[], int selectedCardLen)
    {
        for (int i = 0; i < selectedCardLen; i++)
        {
            if (value[i] == 15)
                return 0;
        }
        selectionSort(value, selectedCardLen);
        for (int i = 0; i < selectedCardLen - 1; i++)
        {
            if (value[selectedCardLen - 1] != value[i] + selectedCardLen - i - 1)
            {
                return 0;
            }
        }
        return 1;
    }

    bool checkTriple(int selectedCard)
    {
        for (int i = 0; i < selectedCard; i++)
        {
            if (userCards[selectedCards[i]].getValue() == 15)
                return 0;
        }
        if (selectedCard % 2 == 1)
            return 0;

        int *tmp_1_value = new int[selectedCard / 2];
        int *tmp_2_value = new int[selectedCard / 2];
        int n1 = 0;
        int n2 = 0;
        for (int i = 0; i < selectedCard; i += 2)
        {
            tmp_1_value[n1] = userCards[selectedCards[i]].getValue();
            n1++;
        }
        for (int i = 1; i < selectedCard; i += 2)
        {
            tmp_2_value[n2] = userCards[selectedCards[i]].getValue();
            n2++;
        }
        if (!checkSequence(tmp_1_value, n1))
            return 0;

        for (int i = 0; i < n1; i++)
        {
            if (tmp_1_value[i] != tmp_2_value[i])
                return 0;
        }
        return 1;
    }

    bool checkDouble(int selectedCard)
    {
        for (int i = 1; i < selectedCard; i++)
            if (userCards[selectedCards[0]].getValue() != userCards[selectedCards[i]].getValue())
                return 0;
        return 1;
    }

    bool check(int selectedCardLen)
    {
        if (selectedCardLen == 1)
            return 1;
        if (selectedCardLen >= 6)
            if (checkTriple(selectedCardLen) == 1)
                return 1;
        if (selectedCardLen > 2)
        {
            int value[13];
            for (int i = 0; i < selectedCardLen; i++)
            {
                value[i] = userCards[selectedCards[i]].getValue();
            }
            if (checkSequence(value, selectedCardLen))
                return 1;
        }
        if (selectedCardLen >= 2)
            if (checkDouble(selectedCardLen))
                return 1;
        return 0;
    }

    void hit()
    {
        int selectedCardLen = selectedCards.size();

        // check is valid cards?
        if (!check(selectedCardLen))
        {
            return;
        }

        // change turn (if not user's turn)
        isTurn = false;
        isFirst = false;

        // sort selected cards
        sort(selectedCards.begin(), selectedCards.end());

        // append history
        int historyLen = history.size();
        /*if (historyLen > 2)
        {
            history.clear();
        }*/

        vector<Card> temp;
        for (int index : selectedCards)
        {
            string storage;
            storage = userCards[index].getPath();
            temp.push_back(userCards[index]);
            sendCardList.push_back(storage);
        }
        history.push_back(temp);

        for (int i = selectedCardLen - 1; i >= 0; i--)
        {
            userCards.erase(userCards.begin() + selectedCards[i]);
        }

        selectedCards.clear();
    }

    void isFirstUser()
    {
        isTurn = false;
        isFirst = false;
        for (Card card : userCards)
        {
            if (card.getSuits() == SPADES && card.getValue() == 3)
            {
                isTurn = true;
                isFirst = true;
            }
        }
    }

    void setIsFirst(bool value)
    {
        isFirst = value;
    }

    bool getIsFirst()
    {
        return isFirst;
    }

    bool isUserTurn()
    {
        return isTurn;
    }

    void setUserTurn(bool value)
    {
        isTurn = value;
    }

    void setSkip(bool value)
    {
        isSkip = value;
    }

    bool getSkip()
    {
        return isSkip;
    }

    void printSkipText(int id = 0)
    {
        // this opens a font style and sets a size
        TTF_Font *Lazy = TTF_OpenFont("src/fonts/Freedom-nZ4J.otf", 24);
        SDL_Color Red = {255, 0, 0};

        SDL_Surface *surfaceMessage = TTF_RenderText_Solid(Lazy, "SKIP!", Red);
        SDL_Texture *Message = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage);

        // user is default
        SDL_Rect Message_rect = {SCREEN_WIDTH / 2 - 110, SCREEN_HEIGHT / 2 + 70, 80, 80};
        if (id == 1)
        {
            Message_rect.x = 50;
            Message_rect.y = SCREEN_HEIGHT / 2 - 40;
        }
        else if (id == 2)
        {
            Message_rect.x = SCREEN_WIDTH / 2 - 100;
            Message_rect.y = SCREEN_HEIGHT / 2 - 240;
        }
        else if (id == 3)
        {
            Message_rect.x = SCREEN_WIDTH - 200;
            Message_rect.y = SCREEN_HEIGHT / 2 - 40;
        }

        SDL_RenderCopy(gRenderer, Message, NULL, &Message_rect);
    }

    int countSuits()
    {
        int countBlack = 0;
        int countRed = 0;
        for (auto &card : this->userCards)
        {
            if (card.getSuits() == 3 || card.getSuits() == 4)
            {
                countRed++;
            }
            else
            {
                countBlack++;
            }
        }
        return countBlack < countRed ? countRed : countBlack;
    }
    map<int, int> getSaveCards() // Create a map to save cards
    {
        map<int, int> saveCards;
        for (auto &card : this->userCards)
        {
            saveCards[card.getValue()]++;
        }
        return saveCards;
    }

    void checkSpecialCards() // Check for a perfect hand
    {
        map<int, int> saveCards = getSaveCards();
        int count = 0; // Count couple
        int countQuads = 0;
        int countJack = 0;
        for (pair<int, int> x : saveCards)
        {
            if (x.second >= 2)
            {
                if (x.second == 4)
                {
                    if (x.first == 2)
                    {
                        countJack = 1;
                        break;
                    }
                    else
                    {
                        countQuads++;
                    }
                }
                count++;
            }
        }
        if (count == 6 || count == 0 || countJack == 1 || countQuads == 3 || countSuits() >= 12)
        // Have 6 couple || 3->A  || Have 4 cards(2) || have 3 Quads || Have 12 black cards or 12 red cards
        {
            this->isSpecial = true;
        }
        else if (count == 5)
        {
            if (countQuads == 1) // Have 4 couple and a quad
            {
                this->isSpecial = true;
            }
            else // Have five consecutive pairs
            {
                int fleg = 1;
                int prevKey = -1;
                for (pair<int, int> x : saveCards)
                {
                    if (x.second >= 2)
                    {
                        if (prevKey != -1 && x.first != prevKey + 1)
                        {
                            fleg = -1;
                            break;
                        }
                        prevKey = x.first;
                    }
                }
                if (fleg == 1)
                {
                    this->isSpecial = true;
                }
            }
        }
        else if (count == 4) // Have 2 couple and 2 quads
        {
            if (countQuads >= 2)
            {
                this->isSpecial = true;
            }
        }
    }
    bool isSpecialCards()
    {
        return this->isSpecial;
    }

    SDL_Texture *getWinTexture()
    {
        return winTexture;
    }
};

class Computer : public User
{
private:
    int id;

public:
    Computer(int id, PlayingCards &plCards) : User(plCards)
    {
        this->id = id;
        this->sortCard();
        this->setWinTexture(id);
    }

    int getId()
    {
        return this->id;
    }
    int getFirstCards()
    {
        // key: value, value: quantiti
        map<int, int> saveCards = getSaveCards();
        int value[13];
        int i = 0;

        for (auto it : saveCards)
        {
            if (it.second == 1)
            {
                value[i] = it.first;
                i++;
            }
        }

        int minCardValue = userCards[0].getValue();

        if (saveCards[minCardValue] == 1)
        {
            while (!checkSequence(value, i) && i > 2)
                i--;
        }
        if (i > 2 && saveCards[minCardValue] == 1)
            return i;
        else
            return saveCards[minCardValue];
    }

    void printBackCard()
    {
        if (!getIsFinish())
        {
            SDL_Rect destinationRect;

            int backCardWidth = 80;
            int backCardHeight = 116;

            // init 3 cards

            if (id == 1)
            {
                destinationRect = {50, SCREEN_HEIGHT / 2 - 145, backCardWidth, backCardHeight};
                SDL_RenderCopy(gRenderer, backTexture, NULL, &destinationRect);
            }

            if (id == 2)
            {

                destinationRect = {SCREEN_WIDTH / 2 - 100, 5, backCardWidth, backCardHeight};
                SDL_RenderCopy(gRenderer, backTexture, NULL, &destinationRect);
            }

            if (id == 3)
            {

                destinationRect = {SCREEN_WIDTH - 200, SCREEN_HEIGHT / 2 - 145, backCardWidth, backCardHeight};
                SDL_RenderCopy(gRenderer, backTexture, NULL, &destinationRect);
            }
        }
    }

    void animationCard(int id)
    {
        // create a random card for performance
        vector<Card> demoCards = history.back();

        int initX, initY, padding;

        if (id == 1)
        {
            initX = 50;
            initY = SCREEN_HEIGHT / 2 - 145;

            padding = 0;
            for (Card demoCard : demoCards)
            {
                demoCard.setWidth(80);
                demoCard.setHeight(116);
                demoCard.normalColor();
                demoCard.setX(initX + padding);
                demoCard.setY(initY);

                SDL_RenderCopy(gRenderer, demoCard.getTexture(), NULL, demoCard.getDestinationRect());

                padding += 40;
            }
        }
        else if (id == 2)
        {
            initX = SCREEN_WIDTH / 2 - 100;
            initY = 5;

            padding = 0;
            for (Card demoCard : demoCards)
            {
                demoCard.setWidth(80);
                demoCard.setHeight(116);
                demoCard.normalColor();
                demoCard.setX(initX + padding);
                demoCard.setY(initY);

                SDL_RenderCopy(gRenderer, demoCard.getTexture(), NULL, demoCard.getDestinationRect());

                padding += 40;
            }
        }
        else
        {
            initX = SCREEN_WIDTH - 200;
            initY = SCREEN_HEIGHT / 2 - 145;

            padding = 0;
            for (Card demoCard : demoCards)
            {
                demoCard.setWidth(80);
                demoCard.setHeight(116);
                demoCard.normalColor();
                demoCard.setX(initX + padding);
                demoCard.setY(initY);

                SDL_RenderCopy(gRenderer, demoCard.getTexture(), NULL, demoCard.getDestinationRect());

                padding += 40;
            }
        }

        SDL_RenderPresent(gRenderer);
        SDL_Delay(400);
    }

    void setPlace()
    {
        gameResult.push_back(id);
    }
};

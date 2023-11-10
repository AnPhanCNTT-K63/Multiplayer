#include<iostream>
#include<vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <cstdio>
#include<sstream>
#include<enet/enet.h>

using namespace std;

#include "src/module/global.h"
#include "src/module/init.h"
#include "src/module/close.h"
#include "src/module/texture.h"
#include "src/module/card.h"
#include "src/module/user.h"
#include "src/module/render.h"
#include "src/module/Client.h"
#include "src/module/logic_game.h"
#include "src/module/control.h"

bool checkWinByDefault(User player, vector<Computer> computers)
{
    bool winByDefault = false;
    if (player.isSpecialCards())
    {
        renderPassWin(player.getWinTexture());
        winByDefault = true;
    }
    for (int i = 0; i < COMPUTER_NUM; i++)
    {
        if (computers[i].isSpecialCards())
        {
            renderPassWin(computers[i].getWinTexture());
            winByDefault = true;
        }
    }
    return winByDefault;
}

void playAgain(PlayingCards& plCards, User& player, vector<Computer>& computers)
{
    SDL_RenderClear(gRenderer);
    SDL_RenderCopy(gRenderer, backgroundTexture, NULL, NULL);

    history.clear();
    gameResult.clear();

    srand(time(0));

    themeCard = themesCard[rand() % THEME_NUM];

    plCards.createPlayingCards();
    plCards.shufflePlayingCards();

    player.initUser(plCards);

    for (Computer& computer : computers)
    {
        computer.initUser(plCards);
    }

    bool winByDefault = checkWinByDefault(player, computers);

    if (winByDefault)
    {
        SDL_RenderPresent(gRenderer);
        SDL_Delay(2000);
        playAgain(plCards, player, computers);
    }
    else
    {
        for (Computer computer : computers)
            computer.printBackCard();

        renderHitBtn();
        if (!player.getIsFirst())
        {
            renderSkipBtn();
        }
        player.printCards();

        SDL_RenderPresent(gRenderer);

        if (player.isUserTurn())
        {
            computers[0].setUserTurn(true);
        }
    }
}

int main(int argc, char* args[])
{

    if (enet_initialize() != 0) {
        cout << "ERROR\n";
        return EXIT_FAILURE;
    }
    atexit(enet_deinitialize);
    Client client;
    client.createHost();
    client.setHost("192.168.52.105");
    client.setPort(8888);
    client.setPeer();

    // check for connect to server 
    bool isConnectToServer = false;
    while (client.setHostService("CONNECT", 8000) > 0) {
        cout << "CONNECTING TO SERVER SUCCEED!" << endl << "PLEASE WAIT FOR ANOTHER PLAYERS TO PLAY...." << endl;
        Sleep(4000);
        isConnectToServer = true;
        break;
    }

    // Start up SDL and create window
    if (!init())
    {
        printf("Failed to initialize!\n");
    }
    else if (!isConnectToServer) {
        cout << "CAN'T CONNECT TO SERVER";
    }
    else
    {
        // Main loop flag
        bool quit = false;

        // Event handler
        SDL_Event e;

        // set background
        loadBackground();

        // set a seed
        srand(time(0));

        // theme of the game
        themeCard = themesCard[rand() % THEME_NUM];

        // init shuffle deck
        PlayingCards plCards;

        // init user
        User player(plCards);

        // init computers
        vector<Computer> computers;
        for (int i = 0; i < COMPUTER_NUM; i++)
        {
            // init id and 13 cards
            Computer temp(i + 1, plCards);
            computers.push_back(temp);
        }

        bool winByDefault = checkWinByDefault(player, computers);

        if (winByDefault)
        {
            SDL_RenderPresent(gRenderer);
            SDL_Delay(2000);

            quit = true;
            playAgain(plCards, player, computers);
        }

        string backPath = "src/cards/" + themeCard + "BACK.png";
        backTexture = loadTexture(backPath);
        for (Computer computer : computers)
            computer.printBackCard();

        hitBtnTexture = loadTexture("src/image/play.png");
        skipBtnTexture = loadTexture("src/image/skip.png");
        againBtnTexture = loadTexture("src/image/again.png");
        printTurnText(1);
        renderHitBtn();
        if (!player.getIsFirst())
        {
            renderSkipBtn();
        }
        player.printCards();

        SDL_RenderPresent(gRenderer);

        if (player.isUserTurn())
        {
            computers[0].setUserTurn(true);
        }

        // check if finished A GAME
        bool isGameFinish = false;

        // game loop
        while (!quit)
        {

            // user play -----------------------------------------------------------------
            while (SDL_PollEvent(&e) != 0)
            {

                if (e.type == SDL_QUIT)
                {
                    quit = true;
                }
                if (e.type == SDL_MOUSEBUTTONUP)
                {
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);

                    // handle events
                    if (!player.getIsFinish())
                    {
                        // event handler
                        cardSelectEvent(player, computers, mouseX, mouseY);

                        if (mouseX >= skipBtnArea.x && mouseX <= skipBtnArea.x + skipBtnArea.w &&
                            mouseY >= skipBtnArea.y && mouseY <= skipBtnArea.y + skipBtnArea.h)
                        {
                            client.sendDataWhenSkip();
                        }

                        if (mouseX >= hitBtnArea.x && mouseX <= hitBtnArea.x + hitBtnArea.w &&
                            mouseY >= hitBtnArea.y && mouseY <= hitBtnArea.y + hitBtnArea.h)
                        {
                            hitBtnEvent(player, computers);
                            check2++;

                        }

                        SDL_RenderPresent(gRenderer);
                    }

                    // play again btn event
                    if (isGameFinish)
                    {
                        if (mouseX >= againBtnArea.x && mouseX <= againBtnArea.x + againBtnArea.w &&
                            mouseY >= againBtnArea.y && mouseY <= againBtnArea.y + againBtnArea.h)
                        {
                            isGameFinish = false;
                            playAgain(plCards, player, computers);
                        }
                    }


                    // printTurnText(check2);
                }
                //client.sendCardDataToServer(sendCardList);
                // multiplayer(player, computers, History);
                // renderHistoryVer2(History);


                if (e.type == SDL_KEYUP)
                {
                    if (e.key.keysym.sym == SDLK_RETURN) // press Enter
                    {
                        hitBtnEvent(player, computers);

                    }

                    if (e.key.keysym.sym == SDLK_SPACE) // press Space
                    {
                        skipBtnEvent(player, computers);
                    }

                    if (e.key.keysym.sym >= SDLK_0 && e.key.keysym.sym <= SDLK_9) // 0 - 9 numbers
                    {
                        // The number key was pressed
                        int index = e.key.keysym.sym - SDLK_0 - 1; // Get the numeric value

                        player.changeSelected(index);

                        renderSelectEvent(player, computers);
                    }

                    SDL_RenderPresent(gRenderer);
                }
            }
            client.sendCardDataToServer(sendCardList);
            multiplayer(client, player, computers, History);
            renderHistoryVer2(History);


            //----------------------------------------------------------------------------

            // check if game finish
            isGameFinish = true;
            for (Computer computer : computers)
            {
                if (!computer.getIsFinish())
                {
                    isGameFinish = false;
                    break;
                }
            }

            if (isGameFinish)
            {
                SDL_RenderClear(gRenderer);
                SDL_RenderCopy(gRenderer, backgroundTexture, NULL, NULL);

                for (Computer computer : computers)
                {
                    computer.printWinner(computer.getId());
                }
                player.printWinner();

                renderAgainBtn();

                SDL_RenderPresent(gRenderer);
            }
        }

        // Free resources and close SDL
        close();
    }

    return 0;
}
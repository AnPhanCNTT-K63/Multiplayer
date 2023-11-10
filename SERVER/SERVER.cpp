//SERVER
#include<iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <cstdio>
#include<vector>
#include<ctime>
#include<enet/enet.h>   
#include<sstream>
#include<map>

using namespace std;

// Screen dimension constants
const int SCREEN_WIDTH = 1250;
const int SCREEN_HEIGHT = 700;
vector<string> cardDataList; // tao vector luu cac file path cua card da nhan (nhi phan)


#include "src/module/texture.h"
#include "src/module/card.h"
#include "src/module/render.h"
#include "src/module/user.h"

bool init()
{
    // Initialization flag
    bool success = true;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        // Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf("Warning: Linear texture filtering not enabled!");
        }

        // Create window
        gWindow = SDL_CreateWindow("Card game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)    
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            // Create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                // Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                // Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
            }
        }
    }

    return success;
}

void close()
{
    // Free loaded image
    // SDL_DestroyTexture(gTexture);
    // gTexture = NULL;

    // Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

//void sendDataToAllClient(ENetHost* server, vector<string> cardDataList,int id) {
//    string ID = to_string(id);
//    for (string &cardPath : cardDataList) {
//        cardPath = cardPath + ID;
//        ENetPacket* packet = enet_packet_create(cardPath.c_str(), cardPath.length() + 1, ENET_PACKET_FLAG_RELIABLE);
//        enet_host_broadcast(server, 0, packet);
//    }
//}



void sendDataToAllClient(ENetHost* server, vector<string> cardDataList) {
    for (string& cardPath : cardDataList) {
        ENetPacket* packet = enet_packet_create(cardPath.c_str(), cardPath.length() + 1, ENET_PACKET_FLAG_RELIABLE);
        enet_host_broadcast(server, 0, packet);
    }
}


void senDataToOneClient(ENetPeer* peer, vector<string> cardSendList){
    
        for (string cardPath : cardSendList) {
            ENetPacket* packet = enet_packet_create(cardPath.c_str(), cardPath.length() + 1, ENET_PACKET_FLAG_RELIABLE);
            enet_peer_send(peer, 0, packet);
        }
    
}
 
int main(int argc, char* args[])
{
    
    char buffer[1024] = { '\0' };
    // SERVER CONNECT TO CLIENT

    if (enet_initialize() != 0) {
        cout << "ERROR";
        return EXIT_FAILURE;
    }
    atexit(enet_deinitialize);  
    ENetHost* server;
    ENetAddress address;
    ENetEvent event;
    address.host = ENET_HOST_ANY;
    address.port = 8888;
    server = enet_host_create(&address, 32, 1, 0, 0);
    if (server == NULL) {
        cout << "ERROR TO CREATE";
        return EXIT_FAILURE;
    }
    cout << "SERVER HAS OPENED" << endl;
    while (true) {
        while (enet_host_service(server, &event, 5000) > 0) {
            switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
                cout << "CLIENT CONNECTED " << event.peer->address.host << " " << ":" << " "
                    << event.peer->address.port << endl;
                break;              
            case ENET_EVENT_TYPE_RECEIVE:               
                cout << "A PACKET WITH DATA : " << event.packet->data 
                    << " WAS RECEIVED FROM CLIENT   : " << event.peer->address.host << " "
                    << " " << event.peer->address.port << " ";                
                sprintf_s(buffer, "%s", event.packet->data);                    
                // CHECK LOGIC GAME HERE
                cardDataList.push_back(buffer);
                cout << endl << "VALID CARD - sending card to client...." << endl;
                cout << endl;              
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                cout << "DISCONNECTED " << event.peer->address.host << " " << ":" << " " << event.peer->address.port << endl;
                break;
            }
            /*for (auto client : connectedClient) {
                if (client != event.peer) {
                    senDataToClient(client, cardDataList);
                    cardDataList.clear();
                }
            }*/
            sendDataToAllClient(server, cardDataList);         
            cardDataList.clear();
            
        }
    }

    // SERVER CONNECT
        
    // Start up SDL and create window
    if (!init())
    {
        printf("Failed to initialize!\n");
    }
    
    // Free resources and close SDL
    

    return 0;
}
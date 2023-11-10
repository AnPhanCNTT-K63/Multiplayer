#pragma once
#include<iostream>
#include<vector>
#include<string>
#include <ctime>
using namespace std;

vector<string> History;
vector<SDL_Rect> destination;
vector<string> cardImage;
int Distance = 0;
int Distance2 = 50;
int Count = 0;
int check1 = 0;
int check2 = 3;

class Client {

private:

    ENetHost* client;
    ENetAddress address;
    ENetPeer* peer;
    ENetEvent event;

public:

    unsigned char* getPacketData() {
        return event.packet->data;
    }

    int getID() {
        return 2;
    }

    void setPort(int portID) {
        address.port = portID;
    }

    void createHost() {
        client = enet_host_create(NULL, 1, 1, 0, 0);

    }

    void setHost(string IP) {
        enet_address_set_host(&address, IP.c_str());
    }

    void setPeer() {
        peer = enet_host_connect(client, &address, 1, 0);
    }

    int setHostService(string types, int time) {
        if (types.c_str() == "CONNECT") {
            event.type = ENET_EVENT_TYPE_CONNECT;
        }
        else if (types.c_str() == "RECEIVE") {
            event.type = ENET_EVENT_TYPE_RECEIVE;
        }
        else if (types.c_str() == "DISCONNECT") {
            event.type = ENET_EVENT_TYPE_DISCONNECT;
        }
        return enet_host_service(client, &event, time);
    }

    void sendCardDataToServer(vector<string> cardSendList) {
        string ID = to_string(getID());
        for (string& cardPath : cardSendList) {
            cardPath += ID;
            string Path = "2" + cardPath;
            ENetPacket* packet = enet_packet_create(Path.c_str(), Path.length() + 1, ENET_PACKET_FLAG_RELIABLE);
            enet_peer_send(peer, 0, packet);
        }
    }

    void sendDataWhenSkip() {
        string message = "5aaaaaaaaaaaaaaaaaaaaaaaa5";
        ENetPacket* packet = enet_packet_create(message.c_str(), message.length() + 1, ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(peer, 0, packet);
    }

};

string getPath(string String) {
    string temp;
    for (auto it = String.begin() + 1; it != String.end() - 1;it++) {

        temp += *it;
    }
    return temp;
}

int getID(string String) {
    string temp;
    for (auto it = String.begin() + 24; it != String.end();it++) {
        if (isdigit(*it))
        {
            temp += *it;
        }
    }
    int id = stoi(temp);
    return id;
}

void printTurnText(int id)
{
    SDL_Texture* turnText = loadTexture("src/image/enemy.png");
    SDL_Rect destination;
    destination.w = 150;
    destination.h = 150;
    if (id == 1)
    {
        destination.x = 20;
        destination.y = SCREEN_HEIGHT / 2 - 40;
    }
    else if (id == 2)
    {
        destination.x = SCREEN_WIDTH / 2 - 140;
        destination.y = SCREEN_HEIGHT / 2 - 240;
    }
    else if (id == 3)
    {
        destination.x = SCREEN_WIDTH - 240;
        destination.y = SCREEN_HEIGHT / 2 - 40;
    }

    SDL_RenderCopy(gRenderer, turnText, NULL, &destination);

}

void printMyTurnText()
{
    SDL_Texture* turnText = loadTexture("src/image/turn.png");
    SDL_Rect destination = { SCREEN_WIDTH / 2 - 135, SCREEN_HEIGHT / 2 + 30, 180, 180 };
    SDL_RenderCopy(gRenderer, turnText, NULL, &destination);

}

void renderHistoryVer2(vector<string> history) {

    if (history.size() > 0) {
        for (int i = 0; i < history.size();i++) {
            SDL_Texture* temp = loadTexture(history[i]);
            SDL_SetTextureColorMod(temp, 120, 120, 120);
            SDL_RenderCopy(gRenderer, temp, NULL, &destination[i]);
        }
    }
}

void multiplayer(Client client, User& player, vector<Computer>& computers, vector<string>& historyTemp) {
        
    char buffer[1024] = { '\0' };

    while (client.setHostService("RECEIVE", 50) > 0) {
        if (Count >= 3) {
            history.clear();
            SDL_RenderClear(gRenderer);
            SDL_RenderCopy(gRenderer, backgroundTexture, NULL, NULL);
            for (Computer computer : computers) {
                computer.printBackCard();
            }
            historyTemp.clear();
            player.printCards();
            SDL_RenderPresent(gRenderer);
            Distance = 90;
            Count = 0;
        }
        sprintf_s(buffer, "%s", client.getPacketData());
        cardImage.push_back(buffer);
        SDL_Rect temp = { (SCREEN_WIDTH / 4) + 50 + Distance , 250 ,80,115 };
        destination.push_back(temp);
        Distance += 90;
    }
    for (string& card : cardImage) {
        string Card = getPath(card);
        int id = getID(card);
        string::iterator it = card.begin();
        if (*it == '2') {
            cout << "Receiving information from server.....VALID CARD, ready to print Card : " << Card << " on screen" << endl;
            historyTemp.push_back(Card);
            SDL_Texture* temp = loadTexture(Card);
            SDL_RenderCopy(gRenderer, temp, NULL, &destination[Count]);
            printTurnText(check2);
            renderHistory(history);
            Count++;
            SDL_RenderPresent(gRenderer);
        }
        else if (*it == '1' || *it == '3' || *it == '4') {
            check1++;
            if (check1 == 1) {
                check2++;
            }
            if (check2 == 4) {
                printMyTurnText();
                check2 = 0;
            }
            cout << "Receiving information from server.....VALID CARD, ready to print Card : " << Card << " on screen" << endl;
            historyTemp.push_back(Card);
            SDL_Texture* temp = loadTexture(Card);
            SDL_RenderCopy(gRenderer, temp, NULL, &destination[Count]);
            printTurnText(check2);
            renderHistory(history);
            Count++;          
            SDL_RenderPresent(gRenderer);

        } 
        else if (*it == '5') {
            SDL_RenderClear(gRenderer);
            SDL_RenderCopy(gRenderer, backgroundTexture, NULL, NULL);
            for (Computer computer : computers) {
                computer.printBackCard();
            }
            player.printCards();
            renderHitBtn();
            renderSkipBtn();
            check2++;
            if (check2 == 4) {
                printMyTurnText();
                check2 = 0;
            }
            printTurnText(check2);
            SDL_RenderPresent(gRenderer);
        }
    }
    check1 = 0;
    // xoa bo nho dem 
    cardImage.clear();
    sendCardList.clear();
}




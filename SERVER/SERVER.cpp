//SERVER
#include<iostream>
#include<vector>
#include <string>
#include<ctime>
#include<enet/enet.h>   
#include<sstream>

using namespace std;
vector<string> cardDataList; // tao vector luu cac file path cua card da nhan (nhi phan)

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
            sendDataToAllClient(server, cardDataList);         
            cardDataList.clear();          
        }
    }       
    return 0;
}
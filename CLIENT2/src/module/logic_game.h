#pragma once
#include <iostream>
#include <algorithm>
#include<map>
#include "card.h"
#include "user.h"
#include "algorithms.h"
using namespace std;
vector<int> cards_will_Hit_1(vector<Card> computer, vector<Card> history)
{
    vector<int> temp1;
    int temp = computer.size();
    mergeSort(computer, 0, temp - 1);
    map<int, int> mp, mp1;
    for (auto it : computer)
    {
        mp[it.getValue()]++;
    }
    int index = 0; // Biến này dùng để lấy giá trị của 3 lá cùng giá trị hoặc 4 lá cùng giá trị
    for (auto it : history)
    {
        index = it.getValue();
        mp1[it.getValue()]++;
    }
    if (mp1[15] == 1 || mp1[15] == 2)
    {
        for (int i = 0; i < computer.size(); i++)
        {
            if (mp[computer[i].getValue()] == 4)
            {
                temp1.push_back(i);
                temp1.push_back(i + 1);
                temp1.push_back(i + 2);
                temp1.push_back(i + 3);
                return temp1;
            }
        }
    }
    if ((int)history.size() == 1)
    {
        for (int i = 0; i < temp; ++i)
        {
            if (computer[i].getValue() > history[0].getValue())
            {
                temp1.push_back(i);
                return temp1;
            }
            else if ((computer[i].getValue() == history[0].getValue()) && (computer[i].getSuits() < history[0].getSuits()))
            {
                temp1.push_back(i);
                return temp1;
            }
        }
    }
    else if ((int)history.size() == 2)
    {
        bool ok = true;
        int suit_his1 = history[0].getSuits();
        int suit_his2 = history[1].getSuits();
        for (int i = 0; i < temp; i++)
        {
            if ((mp[computer[i].getValue()] == 2) && (computer[i].getValue() == history[0].getValue()))
            {
                if (suit_his1 != 3 && suit_his2 != 3)
                {
                    temp1.push_back(i);
                    temp1.push_back(i + 1);
                    return temp1;
                }
            }
            else if ((mp[computer[i].getValue()] == 2) && (computer[i].getValue() > history[0].getValue()))
            {
                temp1.push_back(i);
                temp1.push_back(i + 1);
                return temp1;
            }
        }
    }
    else
    {
        // Nếu có sáp hoặc tú quý của máy có không
        if (mp1[index] == 3 || mp1[index] == 4)
        {
            for (int i = index + 1; i <= 15; i++)
            {
                // Có thể là dấu lớn nma tao tiếc vụ máy có tứ quý mà đánh sáp(3 lá giống nhau)
                if (mp[i] == mp1[index])
                {
                    for (int i = 0; i < (int)computer.size(); i++)
                    {
                        if (computer[i].getValue() == index)
                        {
                            temp1.push_back(i);
                            temp1.push_back(i + 1);
                            temp1.push_back(i + 2);
                            return temp1;
                        }
                    }
                }
            }
        }

        vector<pair<int, int>> v;
        // Xu ly sanh
        v.push_back({computer[0].getValue(), 0});
        for (int i = 1; i < (int)computer.size(); i++)
        {
            if (computer[i].getValue() != computer[i - 1].getValue())
            {
                v.push_back({computer[i].getValue(), i});
            }
        }

        // Dùng sliding window
        int a[16];
        int k = (int)history.size();
        auto it = v.begin();
        for (int i = 0; i < k; i++)
        {
            a[i] = (*it).first;
            temp1.push_back((*it).second);
            it++;
        }
        if (a[0] > history[0].getValue() && a[k - 1] - a[0] + 1 == k)
        {
            return temp1;
        }
        else if (a[0] == history[0].getValue() && computer[(*(it--)).second].getSuits() < history[0].getSuits() && a[k - 1] - a[0] + 1 == k)
        {
            return temp1;
        }
        for (int i = k; i < (int)v.size(); i++)
        {
            temp1.erase(temp1.begin());
            a[i] = (*it).first;
            temp1.push_back((*it).second);
            if (a[i - k + 1] > history[0].getValue() && a[i] - a[i - k + 1] + 1 == k)
            {
                return temp1;
            }
            else if ((a[i - k + 1] == history[0].getValue()) && (computer[(*it).second].getSuits() < history[0].getSuits()) && (a[i] - a[i - k + 1] + 1 == k))
            {
                return temp1;
            }
            it++;
        }
        return temp1;
    }
}

// Trả về giá trị true là không đánh, trả về giá trị false là đánh
bool check_computer(vector<Card> computer, vector<Card> history)
{
    // Số lá bài trên tay ít hơn số lá bài trên bàn thì không đánh
    if ((int)computer.size() < (int)history.size())
    {
        return true;
    }
    // Sắp xếp lại lá bài trước khi check
    mergeSort(computer, 0, (int)computer.size() - 1);

    // Xử lý cóc
    if ((int)history.size() == 1)
    {
        // So sánh giá trị của bài trên bàn với bài trên tay
        for (int i = 0; i < (int)computer.size(); ++i)
        {
            // Nếu lá bài thứ i của máy lớn trên bài thì cho đánh
            if (computer[i].getValue() > history[0].getValue())
            {
                return false;
            }
            // Nếu bằng nhau thì xét chất cơ rô chuồn bích (cơ =3)
            else if ((computer[i].getValue() == history[0].getValue()) && (computer[i].getSuits() < history[0].getSuits()))
            {
                return false;
            }
        }
        return true;
    }
    // Xu ly doi
    else if ((int)history.size() == 2)
    {
        // Đánh dấu lá bài trên tay
        map<int, int> mp;
        // Biến check có tứ quý không
        bool check_Qua = false;
        for (auto it : computer)
        {
            mp[it.getValue()]++;
            if (mp[it.getValue()] == 4)
            {
                check_Qua = true;
            }
        }
        // Xử lý chặt heo
        if (check_Qua)
        {
            return false;
        }

        // Xử lý đôi có cùng giá trị
        if (mp[history[0].getValue()] == 2)
        {
            if (history[0].getSuits() != 3 && history[1].getSuits() != 3)
            {
                return false;
            }
        }
        for (int i = 0; i < (int)computer.size(); i++)
        {
            if (mp[computer[i].getValue()] == 2 && computer[i].getValue() > history[0].getValue())
            {
                return false;
            }
        }
        return true;
    }
    else
    {
        // Đánh dấu lá bài của máy với lá bài trên bàn
        map<int, int> mp, mp1;
        for (auto it : computer)
        {
            mp[it.getValue()]++;
        }
        int index = 0;
        for (auto it : history)
        {
            index = it.getValue();
            mp1[it.getValue()]++;
        }
        // Nếu có sáp hoặc tú quý của máy có không
        if (mp1[index] == 3 || mp1[index] == 4)
        {
            if (mp[index] == mp1[index])
            {
                return false;
            }
            return true;
        }
        int temp1 = (int)history.size(), temp2 = (int)computer.size();
        // Kiem tra tam cuoi cua la truoc
        Card the_last_history = history[temp1 - 1];
        Card the_first_history = history[0];
        // Nếu lá cuối sảnh là lá át cơ thì không đánh
        if (the_last_history.getValue() == 14 && the_last_history.getSuits() == 3)
        {
            return true;
        }
        else
        {
            vector<pair<int, int>> v;

            // Xu ly sanh
            v.push_back({computer[0].getValue(), 0});
            for (int i = 1; i < temp2; i++)
            {
                if (computer[i].getValue() != computer[i - 1].getValue())
                {
                    v.push_back({computer[i].getValue(), i});
                }
            }
            int count = 0;

            // Dùng sliding window
            int a[16];
            int k = (int)history.size();
            auto it = v.begin();
            for (int i = 0; i < k; i++)
            {

                a[i] = (*it).first;
                it++;
            }
            if (a[0] > history[0].getValue() && a[k - 1] - a[0] + 1 == k)
            {
                return false;
            }
            else if (a[0] == history[0].getValue() && computer[(*(it--)).second].getSuits() < history[0].getSuits() && a[k - 1] - a[0] + 1 == k)
            {
                return false;
            }
            for (int i = k; i < (int)v.size(); i++)
            {

                a[i] = (*it).first;
                if (a[i] == 15)
                {
                    return true;
                }
                if (a[i - k + 1] > history[0].getValue() && a[i] - a[i - k + 1] + 1 == k)
                {
                    return false;
                }
                else if ((a[i - k + 1] == history[0].getValue()) && (computer[(*it).second].getSuits() < history[0].getSuits()) && (a[i] - a[i - k + 1] + 1 == k))
                {
                    return false;
                }
                it++;
            }
            return true;
        }
    }
}
void doneTurn(User &player, vector<Computer> &computers)
{
    int count = 0;

    if (player.getSkip())
    {
        count++;
    }
    for (Computer computer : computers)
    {
        if (computer.getSkip())
        {
            count++;
        }
    }

    if (count == 3 - gameResult.size())
    {
        if (!player.getSkip())
        {
            player.setUserTurn(true);
            player.setIsFirst(true);
        }
        else
        {
            player.setUserTurn(false);
        }

        player.setSkip(false);

        for (Computer &computer : computers)
        {
            if (!computer.getSkip())
            {
                computer.setUserTurn(true);
                computer.setIsFirst(true);
            }
            else
            {
                computer.setUserTurn(false);
            }

            computer.setSkip(false);
        }

        if (player.isUserTurn())
        {
            for (int i = 0; i < COMPUTER_NUM; i++)
            {
                if (!computers[i].getIsFinish())
                {
                    computers[i].setUserTurn(true);
                    break;
                }
            }
        }

        SDL_RenderClear(gRenderer);
        SDL_RenderCopy(gRenderer, backgroundTexture, NULL, NULL);

        renderHistory(history);

        if (!player.getIsFinish())
        {
            renderHitBtn();
            if (!player.getIsFirst())
            {
                renderSkipBtn();
            }

            player.printCards();

            if (player.getSkip())
            {
                player.printSkipText();
            }
        }
        else
        {
            player.printWinner();
        }

        if (player.getSkip())
        {
            player.printSkipText();
        }

        // print skip text and back cards
        for (Computer computer : computers)
        {
            if (computer.getIsFinish())
            {
                computer.printWinner(computer.getId());
            }
            computer.printBackCard();

            if (computer.getSkip())
            {
                computer.printSkipText(computer.getId());
            }
        }

        SDL_Delay(500);
        SDL_RenderPresent(gRenderer);
    }
}
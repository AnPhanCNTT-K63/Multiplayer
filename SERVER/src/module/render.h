#pragma once
void renderBackCard()
{
    SDL_Rect destinationRect;

    // play btn
    destinationRect = {210, 450, 135, 59};
    SDL_RenderCopy(gRenderer, skipBtnTexture, NULL, &destinationRect);

    // skip btn
    destinationRect = {800, 450, 135, 59};
    SDL_RenderCopy(gRenderer, hitBtnTexture, NULL, &destinationRect);
}

void renderComputerCards()
{
    SDL_Rect destinationRect;

    // init 3 cards
    destinationRect = {50, SCREEN_HEIGHT / 2 - 145, 100, 145};
    SDL_RenderCopy(gRenderer, backTexture, NULL, &destinationRect);
    destinationRect = {SCREEN_WIDTH - 200, SCREEN_HEIGHT / 2 - 145, 100, 145};
    SDL_RenderCopy(gRenderer, backTexture, NULL, &destinationRect);
    destinationRect = {SCREEN_WIDTH / 2 - 100, 5, 100, 145};
    SDL_RenderCopy(gRenderer, backTexture, NULL, &destinationRect);
}

void renderHistory(vector<vector<Card>> history)
{
    int padding = 0;
    int len = history.size();
    for (int i = 0; i < len; i++)
    {
        padding += 50;
        for (Card card : history[i])
        {
            card.setX((SCREEN_HEIGHT / 2) + padding);
            card.setY(300);
            card.setWidth(80);
            card.setHeight(116);

            if (i != len - 1)
                card.blackColor();
            else
                card.normalColor();

            SDL_RenderCopy(gRenderer, card.getTexture(), NULL, card.getDestinationRect());

            padding += 40;
        }
    }
}
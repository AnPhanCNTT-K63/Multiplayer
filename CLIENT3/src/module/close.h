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
    TTF_Quit();
}
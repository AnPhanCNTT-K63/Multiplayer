#pragma once

SDL_Texture *backgroundTexture = NULL;
SDL_Texture *backTexture = NULL;
SDL_Texture *hitBtnTexture = NULL;
SDL_Texture *skipBtnTexture = NULL;
SDL_Texture *againBtnTexture = NULL;

SDL_Texture *loadTexture(string path)
{
    // The final texture
    SDL_Texture *newTexture = NULL;

    // Load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

void loadBackground()
{
    // tao bo dem backgroundTexture de luu tru background (SDL_PIXELFORMAT_RGBA8888 định dạng pixel cho texture, SDL_TEXTUREACCESS_TARGET vẽ lên texture, SDL_TEXTUREACCESS_)
    backgroundTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_Surface *backgroundSurface = IMG_Load("src/image/OIP1.png"); // Thay bằng đường dẫn đến hình ảnh của bạn
    if (backgroundSurface == nullptr)
    {
        printf("Failed to load image! SDL_image Error: %s\n", IMG_GetError());
    }
    else
    {
        backgroundTexture = SDL_CreateTextureFromSurface(gRenderer, backgroundSurface); // Chuyển đổi hình ảnh Surface sang Texture
        SDL_RenderCopy(gRenderer, backgroundTexture, NULL, NULL);
        // có NULL vì muốn thao tác với toàn bộ nội dung của texture hoặc renderer, mà không muốn áp đặt giới hạn về vị trí và kích thước.
        SDL_FreeSurface(backgroundSurface);
    }
}

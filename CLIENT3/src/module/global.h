// Screen dimension constants
const int SCREEN_WIDTH = 1250;
const int SCREEN_HEIGHT = 700;
// How many computer will play
const int COMPUTER_NUM = 3;

// The window we'll be rendering to / surface
SDL_Window *gWindow = NULL;

// The window renderer
SDL_Renderer *gRenderer = NULL;

// The theme of the game
const int THEME_NUM = 3;
string themesCard[THEME_NUM] = {"cards1/", "cards2/", "cards3/"};
string themeCard = "";

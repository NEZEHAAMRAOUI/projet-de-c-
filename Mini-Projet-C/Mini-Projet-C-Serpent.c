#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define TILE_SIZE 20
#define INITIAL_SNAKE_SIZE 3
#define MAX_OBSTACLES 11

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

SDL_Texture* fond_ecran_Texture = NULL;
SDL_Surface* fond_ecran_image  = NULL;

int SOR=200;

int obstaclesX[MAX_OBSTACLES], obstaclesY[MAX_OBSTACLES];
int obstaclesX1[MAX_OBSTACLES], obstaclesY1[MAX_OBSTACLES];
int obstaclesX2[MAX_OBSTACLES], obstaclesY2[MAX_OBSTACLES];
int obstaclesX3[MAX_OBSTACLES], obstaclesY3[MAX_OBSTACLES];

int snakeSize = INITIAL_SNAKE_SIZE;
int snakeDirection = 0;
int score = 0;
bool Verifier_food_obstacle();
bool enPause = true;

struct Cordonee{
    int x, y;
};
struct Cordonee snake[100];
struct Cordonee food;

void Changertitre() {
    char titre[100];
    sprintf(titre, "jeu de serpen- Score: %d", score);
    SDL_SetWindowTitle(window, titre);
}

void Jeux() {
    if (snake[0].x == food.x && snake[0].y == food.y) {
        snakeSize++;
        score += 10;
        SOR-=20;
        generateFood();
    }
}

void  initializeSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("makhdamch l sdl : %s\n", SDL_GetError());
    }

    window = SDL_CreateWindow("jeu de serent", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("probleme de creation de window : %s\n", SDL_GetError());
        SDL_Quit();
        }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("probleme de creation de renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

}

void Ajout_image() {
    fond_ecran_image= IMG_Load("gason.png");

    fond_ecran_Texture = SDL_CreateTextureFromSurface(renderer, fond_ecran_image );
    SDL_FreeSurface(fond_ecran_image);
}

void closeSDL() {
    SDL_DestroyTexture(fond_ecran_Texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();
}

void SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius) {
    for (int i = -radius; i <= radius; i++) {
        for (int j = -radius; j <= radius; j++) {
            if (i * i + j * j <= radius * radius) {
                SDL_RenderDrawPoint(renderer, x + i, y + j);
            }
        }
    }
}

void drawSnake_1(){
    for (int i = 0; i < snakeSize; i++) {
        if(i%2==0){
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillCircle(renderer, snake[i].x+TILE_SIZE/2, snake[i].y+TILE_SIZE/2, +TILE_SIZE/2);
        }
        if(i%2!=0){
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_RenderFillCircle(renderer, snake[i].x+TILE_SIZE/2, snake[i].y+TILE_SIZE/2, +TILE_SIZE/2);
        }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    int eyeSize = TILE_SIZE / 5;
    SDL_Rect leftEye = { snake[0].x+ eyeSize,  snake[0].y + eyeSize, eyeSize, eyeSize };
    SDL_Rect rightEye = { snake[0].x + 3 * eyeSize, snake[0].y + eyeSize, eyeSize, eyeSize };
    SDL_RenderFillRect(renderer, &leftEye);
    SDL_RenderFillRect(renderer, &rightEye);

    int mouthSize = TILE_SIZE / 8;
    SDL_Rect mouthRect = { snake[0].x+ eyeSize, snake[0].y  + 3 * eyeSize, TILE_SIZE- 2 * eyeSize, mouthSize };
    SDL_RenderFillRect(renderer, &mouthRect);

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderFillCircle(renderer, food.x+TILE_SIZE/2, food.y+TILE_SIZE/2, +TILE_SIZE/2);
}

void drawSnake() {
    drawSnake_1();
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        SDL_Rect obstacleRect = { obstaclesX[i], obstaclesY[i], TILE_SIZE, TILE_SIZE};
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
        SDL_RenderFillRect(renderer, &obstacleRect);

        SDL_Rect obstacleRect1 = { obstaclesX1[i], obstaclesY1[i], TILE_SIZE, TILE_SIZE};
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
        SDL_RenderFillRect(renderer, &obstacleRect1);

        SDL_Rect obstacleRect2 = { obstaclesX2[i], obstaclesY2[i], TILE_SIZE, TILE_SIZE};
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
        SDL_RenderFillRect(renderer, &obstacleRect2);

        SDL_Rect obstacleRect3 = { obstaclesX3[i], obstaclesY3[i], TILE_SIZE, TILE_SIZE};
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
        SDL_RenderFillRect(renderer, &obstacleRect3);
    }
}
void moveSnake() {
    for (int i = snakeSize - 1; i > 0; i--) {
        snake[i].x = snake[i - 1].x;
        snake[i].y = snake[i - 1].y;
    }

    if (snakeDirection == 0) {
        snake[0].x += TILE_SIZE;
    } else if (snakeDirection == 1) {
        snake[0].y -= TILE_SIZE;
    } else if (snakeDirection == 2) {
        snake[0].x -= TILE_SIZE;
    } else if (snakeDirection == 3) {
        snake[0].y += TILE_SIZE;
    }
}
void generateFood() {
    srand(time(NULL));
    do {
        food.x = (rand() % (SCREEN_WIDTH / TILE_SIZE)) * TILE_SIZE;
        food.y = (rand() % (SCREEN_HEIGHT / TILE_SIZE)) * TILE_SIZE;
    } while (!Verifier_food_obstacle());
}

bool Verifier_food_obstacle() {
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (food.x == obstaclesX[i] && food.y == obstaclesY[i]) {
            return false;
        }
        if (food.x == obstaclesX1[i] && food.y == obstaclesY1[i]) {
            return false;
        }
        if (food.x == obstaclesX2[i] && food.y == obstaclesY2[i]) {
            return false;
        }
        if (food.x == obstaclesX3[i] && food.y == obstaclesY3[i]) {
            return false;
        }
    }
    return true;
}

bool checkCollision_1(){
    if (snake[0].x < 0 || snake[0].x >= SCREEN_WIDTH || snake[0].y < 0 || snake[0].y >= SCREEN_HEIGHT)
        return true;

    for (int i = 1; i < snakeSize; i++) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
            return true;
    }
    return false;
}

bool checkCollision() {
    if (snake[0].x < 0 || snake[0].x >= SCREEN_WIDTH || snake[0].y < 0 || snake[0].y >= SCREEN_HEIGHT)
        return true;

    for (int i = 1; i < snakeSize; i++) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
            return true;
    }
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (snake[0].x == obstaclesX[i] &&snake[0].y == obstaclesY[i]) {
            return true;
        }
        if (snake[0].x  == obstaclesX1[i] && snake[0].y == obstaclesY1[i]) {
            return true;
        }
        if (snake[0].x  == obstaclesX2[i] && snake[0].y == obstaclesY2[i]) {
            return true;
        }
        if (snake[0].x == obstaclesX3[i] && snake[0].y == obstaclesY3[i]) {
            return true;
        }
    }
    return false;
}
void initialise_jeu_1(){
    initialise_jeu();
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        obstaclesX[i] =20*i+80;
        obstaclesY[i] =20*i+240;

        obstaclesX1[i] =20*i-20+360;
        obstaclesY1[i] =440-20*i;

        obstaclesX2[i] =20*i-20+100;
        obstaclesY2[i] =220-20*i;

        obstaclesX3[i] =20*i+340;
        obstaclesY3[i] =20*i+20;
    }
}
void initialise_jeu(){
    for (int i = 0; i < snakeSize; i++) {
        snake[i].x = SCREEN_WIDTH / 2 - i * TILE_SIZE;
        snake[i].y = SCREEN_HEIGHT / 2;
    }
    generateFood();
}

void gestionPause() {
    SDL_Event evenementPause;
    while (enPause) {
        while (SDL_PollEvent(&evenementPause) != 0) {
            if (evenementPause.type == SDL_KEYDOWN) {
                switch (evenementPause.key.keysym.sym) {
                    case SDLK_SPACE:
                        enPause = false;
                        break;
                    case SDLK_ESCAPE:
                        closeSDL();
                        exit(0);
                        break;
                }
            }
        }
    }
}
int main(int argc, char* args[]) {
    int N;
    printf("dans ce jeu il y a deux niveau entrer 1 pour le premier niveau et entrer 2 pour la deuxieme :");
    scanf("%d",&N);

    initializeSDL();
    Ajout_image();
    SDL_Event event;
    bool quit = false;

    if(N==1){
        initialise_jeu();
        while (quit==false) {
            moveSnake();
            Jeux();
            Changertitre();

            SDL_RenderCopy(renderer, fond_ecran_Texture, NULL, NULL);
            drawSnake_1();
            SDL_RenderPresent(renderer);
            SDL_Delay(SOR);
            gestionPause();

            while (SDL_PollEvent(&event) != 0) {
                if (event.type == SDL_QUIT) {
                    quit = true;
                }
                else if (event.type == SDL_KEYDOWN) {
                    switch (event.key.keysym.sym) {
                        case SDLK_UP:
                            snakeDirection = 1;
                            break;
                        case SDLK_DOWN:
                            snakeDirection = 3;
                            break;
                        case SDLK_LEFT:
                            snakeDirection = 2;
                            break;
                        case SDLK_RIGHT:
                            snakeDirection = 0;
                            break;
                        case SDLK_SPACE:
                            enPause = true;
                            break;
                    }
                }
            }
            if (checkCollision_1()) {
                printf("\n\t\tJeu termine !\n");
                printf("\t\t SCORE = %d \n",score);
                quit = true;
            }
        }
        closeSDL();
    }
    else if (N==2){
        initialise_jeu_1();
        while (quit==false) {
            moveSnake();
            Jeux();
            Changertitre();
            SDL_RenderCopy(renderer, fond_ecran_Texture, NULL, NULL);
            drawSnake();
            SDL_RenderPresent(renderer);
            SDL_Delay(SOR);
            gestionPause();
            while (SDL_PollEvent(&event) != 0) {
                if (event.type == SDL_QUIT) {
                    quit = true;
                }
                else if (event.type == SDL_KEYDOWN) {
                    switch (event.key.keysym.sym) {
                        case SDLK_UP:
                            snakeDirection = 1;
                            break;
                        case SDLK_DOWN:
                            snakeDirection = 3;
                            break;
                        case SDLK_LEFT:
                            snakeDirection = 2;
                            break;
                        case SDLK_RIGHT:
                            snakeDirection = 0;
                            break;
                        case SDLK_SPACE:
                            enPause = true;
                            break;
                    }
                }
            }
            if (checkCollision()) {
                printf("\n\t\tJeu termine !\n");
                printf("\t\t SCORE = %d \n",score);
                quit = true;
            }
        }
        closeSDL();
    }
    return 0;
}

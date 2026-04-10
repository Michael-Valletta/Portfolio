#include "raylib.h"
#include <iostream>
#include <vector>
#include "raymath.h"

using namespace std;

Texture2D playerTex;
Texture2D fireballTex;
Texture2D enemy1Tex; 
Texture2D enemy2Tex;
Texture2D xpTex;
Texture2D backgroundTex;
Sound fireballSound;
Sound xpSound;

class Weapons {
public:
    Vector2 pos;
    Vector2 direction;
    float speed;
    bool active;

    Weapons(Vector2 startPos, Vector2 dir) {
        pos = startPos;
        direction = dir;
        speed = 500.0f;
        active = true;
    }

    void Update() {
        pos.x += direction.x * speed * GetFrameTime();
        pos.y += direction.y * speed * GetFrameTime();

        if (pos.x < 0 || pos.x > GetScreenWidth() || pos.y < 0 || pos.y > GetScreenHeight()) {
            active = false;
        }
    }

    void Draw() {
        if (active) {
            DrawTexture(fireballTex, (int)pos.x, (int)pos.y, WHITE);
        }
    }
};

class Enemy {
public:
    Vector2 pos;
    float speed;
    int health;
    int type;
    bool active;
    float hitTimer = 0.0f;

    Enemy(Vector2 startPos, int enemyType) {
        pos = startPos;
        type = enemyType;
        active = true;

        if (type == 1) { 
            speed = 80.0f;  
            health = 20; 
        } else { 
            speed = 150.0f; 
            health = 10; 
        } 
    }

    void TriggerHit() { hitTimer = 0.5f; }

    void Update(Vector2 playerPos) {
        float dt = GetFrameTime();
        if (hitTimer > 0) hitTimer -= dt;
        if (pos.x < playerPos.x) pos.x += speed * dt;
        if (pos.x > playerPos.x) pos.x -= speed * dt;
        if (pos.y < playerPos.y) pos.y += speed * dt;
        if (pos.y > playerPos.y) pos.y -= speed * dt;
        
        if (health <= 0) active = false;
    }

    void Draw() {
        if (!active) return;
        Color tint = (hitTimer > 0) ? RED : WHITE;
        if (type == 1) DrawTexture(enemy1Tex, (int)pos.x, (int)pos.y, tint);
        else DrawTexture(enemy2Tex, (int)pos.x, (int)pos.y, tint);
    }
};

class Experience {
public:
    Vector2 pos;
    bool active;

    Experience(Vector2 startPos) {
        pos = startPos;
        active = true;
    }

    void Draw() {
        if (active) DrawTexture(xpTex, (int)pos.x, (int)pos.y, WHITE);
    }
};


class Player {
private:
    Vector2 position;
    int health;
    float speed;
    float hitTimer = 0.0f;

    vector<Weapons> Fireballs;
    float shootTimer = 0.0f;
    float shootRate = 0.5f;

public:
    Vector2 GetPosition() { return position; }
    void UpgradeSpeed(float amt) { speed += amt; }
    void UpgradeFireRate(float amt) { shootRate -= amt; if(shootRate < 0.1f) shootRate = 0.1f; }
    void TriggerHit() { hitTimer = 1.0f; }
    void Reset() {
        position = { 400, 300 };
        Fireballs.clear();
    }
    Player() {
        position = { 400, 300 };
        speed = 250.0f;
        health = 100;
    }

    void Update(vector<Enemy>& enemies) {
        float dt = GetFrameTime();
        if (hitTimer > 0) hitTimer -= dt;
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    position.y -= speed * GetFrameTime();
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))  position.y += speed * GetFrameTime();
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  position.x -= speed * GetFrameTime();
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) position.x += speed * GetFrameTime();
        
        if (position.x < 0) position.x = 0;
        if (position.y < 0) position.y = 0;
        if (position.x > GetScreenWidth() - playerTex.width) position.x = (float)GetScreenWidth() - playerTex.width;
        if (position.y > GetScreenHeight() - playerTex.height) position.y = (float)GetScreenHeight() - playerTex.height;

        shootTimer += dt;
        if (shootTimer >= shootRate) {
            float closestDist = 1000.0f; 
            int targetIndex = -1;

            for (int i = 0; i < (int)enemies.size(); i++) {
                float dist = Vector2Distance(position, enemies[i].pos);
                if (dist < closestDist) {
                    closestDist = dist;
                    targetIndex = i;
                }
            }
            if (targetIndex != -1) {
                Vector2 dir = Vector2Subtract(enemies[targetIndex].pos, position);
                dir = Vector2Normalize(dir);
                
                Fireballs.push_back(Weapons({ position.x + 20, position.y + 20 }, dir));
                PlaySound(fireballSound);
                shootTimer = 0;
            }
        }
        
        for (int i = 0; i < (int)Fireballs.size(); i++) {
            Fireballs[i].Update();

            for (int j = 0; j < (int)enemies.size(); j++) {
                if (CheckCollisionCircles(Fireballs[i].pos, 10, enemies[j].pos, 20)) {
                    enemies[j].health -= 10;
                    enemies[j].TriggerHit();
                    Fireballs[i].active = false;
                }
            }

            if (!Fireballs[i].active) {
                Fireballs.erase(Fireballs.begin() + i);
                i--;
            }
        }
    }

    void Draw() {
        for (auto& b : Fireballs) {
            b.Draw();
        }
        Color tint = (hitTimer > 0) ? RED : WHITE;
        DrawTexture(playerTex, (int)position.x, (int)position.y, tint);
    }
};

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1000;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Home Assigenemnt");
    InitAudioDevice();

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    playerTex = LoadTexture("Images/Player.png");
    fireballTex = LoadTexture("Images/Fireball.png");
    enemy1Tex = LoadTexture("Images/enemy1.png");
    enemy2Tex = LoadTexture("Images/enemy2.png");
    xpTex = LoadTexture("Images/xp.png");
    backgroundTex = LoadTexture("Images/background.png"); 
    Music bgMusic = LoadMusicStream("Audio/BackgroundMusic.mp3"); 
    PlayMusicStream(bgMusic);
    fireballSound = LoadSound("Audio/Fireball.mp3");
    SetSoundVolume(fireballSound, 5.0f);
    xpSound = LoadSound("Audio/XpSound.mp3");
    SetSoundVolume(xpSound,12.5f);

    Player player;
    vector<Enemy> enemies;
    
    float spawnTimer = 0;
    float spawnInterval = 2.0f;

    vector<Experience> xpDrops;
    int playerXP = 0;
    int xpToLevel = 50; 
    int level = 1;       
    bool levelingUp = false;

    int playerHealth = 100;
    bool gameOver = false;
    bool won = false;
    float timer = 600.0f;
    bool gameStarted = false;


    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateMusicStream(bgMusic);
        float dt = GetFrameTime();
        
        if (playerXP >= xpToLevel) {
            levelingUp = true;
        }

        if(!gameStarted){
            if(IsKeyPressed(KEY_SPACE)){
                gameStarted = true;
            }
        }else{
            if(!gameOver && !won){
                if (!levelingUp) {
                    timer -= dt;
                    if(timer <=0 ) timer = 0;
                    if(timer <=0 && !gameOver){
                        won = true;
                    }
                    for(int i = 0; i< (int)enemies.size();i++){
                        if(CheckCollisionCircles(player.GetPosition(),20,enemies[i].pos,20)){
                            playerHealth -= 1;
                            player.TriggerHit();

                            if(playerHealth <=0){
                                playerHealth = 0;
                                gameOver = true;
                            }
                        }
                    }
                    spawnTimer += dt;
                    if (spawnTimer >= spawnInterval) {
                        int edge = GetRandomValue(0, 3);
                        Vector2 sPos;
                        if (edge == 0) sPos = {(float)GetRandomValue(0, 800), -40}; 
                        else if (edge == 1) sPos = {(float)GetRandomValue(0, 800), 640}; 
                        else if (edge == 2) sPos = {-40, (float)GetRandomValue(0, 600)}; 
                        else sPos = {840, (float)GetRandomValue(0, 600)};

                        enemies.push_back(Enemy(sPos, GetRandomValue(1, 2)));
                        spawnTimer = 0;
                        
                        if (spawnInterval > 0.5f) spawnInterval -= 0.05f; 
                    }

                    // Update
                    player.Update(enemies);

                    for (int i = 0; i < (int)enemies.size(); i++) {
                        enemies[i].Update(player.GetPosition());
                        if (!enemies[i].active) {
                            xpDrops.push_back(Experience(enemies[i].pos));
                            enemies.erase(enemies.begin() + i);
                            i--;
                        }
                    }      

                    for (int i = 0; i < (int)xpDrops.size(); i++) {
                        if (CheckCollisionCircles(player.GetPosition(), 40, xpDrops[i].pos, 15)) {
                            playerXP += 10;
                            PlaySound(xpSound);
                            xpDrops.erase(xpDrops.begin() + i);
                            i--;
                        }
                    }
                } else {
                    if (IsKeyPressed(KEY_ONE)) {
                        player.UpgradeSpeed(50); 
                        levelingUp = false;
                        playerXP = 0;
                        level++;
                        xpToLevel += 25;
                    }
                    if (IsKeyPressed(KEY_TWO)) {
                        player.UpgradeFireRate(0.05f); 
                        levelingUp = false;
                        playerXP = 0;
                        level++;
                        xpToLevel += 25;
                    }
                }
            }else{
                if(gameOver || won){
                    if(IsKeyPressed(KEY_SPACE)){
                        playerHealth = 100;
                        playerXP = 0;
                        level = 1;
                        enemies.clear();
                        xpDrops.clear();
                        gameOver = false;
                        timer = 600.f;
                        won = false;
                        xpToLevel = 50;
                        player.Reset();
                    }
                }
            }
        }

        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawTexture(backgroundTex, 0, 0, WHITE);

            if(!gameStarted){
                DrawRectangle(0, 0, 1000, 800, Fade(BLACK, 0.7f));
                DrawText("Dragonic", 350, 300, 60, ORANGE);
                DrawText("Use WASD/Arrow keys to Move", 350, 400, 20, WHITE);
                DrawText("Survive for 10 minutes", 350, 430, 20, WHITE);
                DrawText("Press Space to Start", 350, 520, 25, YELLOW);
            }else{
                int minutes = (int)timer /60;
                int seconds = (int)timer % 60;
                DrawText(TextFormat("TIME: %02i:%02i", minutes, seconds), 800, 20, 30, WHITE);

                for (auto& drop : xpDrops) drop.Draw();
                for (int i = 0; i < (int)enemies.size(); i++) enemies[i].Draw();
                player.Draw();


                if (levelingUp) {
                    DrawRectangle(0, 0, 1000, 800, Fade(BLACK, 0.5f)); 
                    DrawText("LEVEL UP!", 400, 300, 40, YELLOW);
                    DrawText("Press 1 for +Movement Speed", 350, 380, 20, WHITE);
                    DrawText("Press 2 for +Attack Speed", 350, 410, 20, WHITE);
                }

                DrawText(TextFormat("Level: %i  XP: %i/%i", level, playerXP, xpToLevel), 10, 10, 20, WHITE);
                DrawRectangle(10, 40, 200, 20, RED); 
                DrawRectangle(10, 40, (int)(playerHealth * 2), 20, GREEN); 
                DrawText(TextFormat("HP: %i", playerHealth), 15, 42, 18, WHITE);

                if (gameOver) {
                    DrawRectangle(0, 0, 1000, 800, Fade(BLACK, 0.8f));
                    DrawText("GAME OVER", 380, 350, 50, RED);
                    DrawText("Press Space to Restart", 390, 420, 20, RAYWHITE);
                }

                if (won) {
                    DrawRectangle(0, 0, 1000, 800, Fade(DARKBLUE, 0.8f));
                    DrawText("You Won!", 350, 300, 60, GOLD);
                    DrawText(TextFormat("Final Level: %i", level), 420, 380, 25, WHITE);
                    DrawText("Press Space to Play Again", 350, 450, 20, RAYWHITE);
                }
            }

         EndDrawing();
    } 
    //----------------------------------------------------------------------------------

    // De-Initialization
    UnloadTexture(playerTex);
    UnloadTexture(fireballTex);
    UnloadTexture(enemy1Tex);
    UnloadTexture(enemy2Tex);
    UnloadTexture(xpTex);
    UnloadTexture(backgroundTex);
    UnloadMusicStream(bgMusic);
    CloseAudioDevice();
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
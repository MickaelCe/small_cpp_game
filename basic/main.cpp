#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "Prop.h"
#include "Enemy.h"
#include <string>

int main()
{
    // window creation
    int windowWidth{450};
    int windowHeight{450};
    InitWindow(windowWidth, windowHeight, "Survivor");

    // set background
    Texture2D background = LoadTexture("nature_tileset/OpenWorldMap24x24.png");
    Vector2 mapPos{0.0, 0.0};
    const float mapScale{4.0f};

    Character knight{windowWidth, windowHeight};

    Prop props[2]{
        Prop{Vector2{600.f, 300.f}, LoadTexture("nature_tileset/Rock.png")},
        Prop{Vector2{400.f, 500.f}, LoadTexture("nature_tileset/Log.png")}};

    Enemy goblin{Vector2{800.f, 300.f}, LoadTexture("characters/goblin_idle_spritesheet.png"), LoadTexture("characters/goblin_run_spritesheet.png")};

    Enemy slime{Vector2{500.f, 700.f}, LoadTexture("characters/slime_idle_spritesheet.png"), LoadTexture("characters/slime_run_spritesheet.png")};

    Enemy *enemies[]{
        &goblin,
        &slime};

    for (auto enemy : enemies)
    {
        enemy->setTarget(&knight);
    }

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        mapPos = Vector2Scale(knight.getWorldPos(), -1.f);

        // DrawMap
        DrawTextureEx(background, mapPos, 0.0, mapScale, WHITE);

        for (auto prop : props)
        {
            prop.Render(knight.getWorldPos());
        }

        if (!knight.getAlive())
        {
            DrawText("Game Over!", 55.f, 45.f, 40, RED);
            EndDrawing();
            continue;
        }
        else
        {
            std::string knightHealth = "Health: ";
            knightHealth.append(std::to_string(knight.getHealth()), 0, 5);
            DrawText(knightHealth.c_str(), 55.f, 45.f, 40, RED);
        }

        for (auto enemy : enemies)
        {
            enemy->tick(GetFrameTime());
        }

        knight.tick(GetFrameTime());
        // checkmap bound
        if (knight.getWorldPos().x < 0.f ||
            knight.getWorldPos().y < 0.f ||
            knight.getWorldPos().x + windowWidth > background.width * mapScale ||
            knight.getWorldPos().y + windowHeight > background.height * mapScale)
        {
            knight.undoMovement();
        }
        // check collision
        for (auto prop : props)
        {
            if (CheckCollisionRecs(prop.getCollisionRec(knight.getWorldPos()), knight.getCollisionRec()))
            {
                knight.undoMovement();
            }
        }

        goblin.tick(GetFrameTime());

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            for (auto enemy : enemies)
            {
                if (CheckCollisionRecs(goblin.getCollisionRec(), knight.getWeaponCollisionRec()))
                {
                    enemy->setAlive(false);
                }
            }
        }
        else
        {
        }

        EndDrawing();
    }
    UnloadTexture(background);
    CloseWindow();
}
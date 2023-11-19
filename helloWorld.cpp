
#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "dynamicsFunction.h"   

#define MAX_BUILDINGS   100

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------

    srand(time(NULL));
    int track = rand() % 2;

    const int screenWidth = 800;
    const int screenHeight = 450;

    int currentFrame = 0;

    int framesCounter = 0;
    int framesSpeed = 8;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 2d camera");

    InitAudioDevice();              // Initialize audio device

    Music music;
    if(track==0) music = LoadMusicStream("resources/SaintSeans_sym3.mp3");
    else         music = LoadMusicStream("resources/SaintSeans_2.mp3");

    PlayMusicStream(music);



    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
    Texture2D scarfy = LoadTexture("resources/scarfy.png");        // Texture loading
    Texture2D scarfyF = LoadTexture("resources/scarfyflipped2.png");
    Texture2D scarfyIdle = LoadTexture("resources/scarfyIdle2.png");
    Texture2D scarfyJump = LoadTexture("resources/scarfyJump.png");
    Texture2D mossyTile = LoadTexture("resources/Mossy_Ruin_Tiles124.png");

    Texture2D curScarfy;
    Rectangle frameRec = { 0.0f, 0.0f, (float)scarfy.width/6, (float)scarfy.height };
    Rectangle idleFrameRec { 0.0f, 0.0f, (float)scarfyIdle.width/2, (float)scarfy.height };
    printf("Scarfy height:%d\tWidth:%d\n", scarfyIdle.height, scarfyIdle.width);
    Vector2 texturePos;
    Rectangle mossyTileFrameRec = {0,0, (float)mossyTile.width, (float)mossyTile.height};


    float defaultHeight = 118;
    float defaultWidth = 42;

    float defaultXPos   = 400;
    float defaultYPos   = 0;
    Rectangle player    = { defaultXPos, defaultYPos, defaultWidth, defaultHeight };

    int runningOffset = defaultWidth/4;

    Rectangle buildings[MAX_BUILDINGS] = { 0 };
    Color buildColors[MAX_BUILDINGS] = { 0 };

    Vector2 acceleration = {0, 0.2};
    PosVel playerPosVel;

    std::vector<Rectangle>* collisionBoxes = new std::vector<Rectangle>;

    playerPosVel.Pos.x = player.x;
    playerPosVel.Pos.y = player.y;
    playerPosVel.Vel.x = 0;
    playerPosVel.Vel.y = 0;

    bool isAirborne = false;
    bool isMovingLeft = false;
    bool isMovingRight = false;

    // 1 for Right, 0 for left
    int directionFacing = 0;
    bool isIdle          = true;

    int spacePressedDuration = 0;
    char* spaceString = (char*)malloc(100*sizeof(char));

    int spacing = 0;

    for (int i = 0; i < MAX_BUILDINGS; i++)
    {
        buildings[i].width = (float)GetRandomValue(50, 200);
        buildings[i].height = (float)GetRandomValue(100, 800);
        buildings[i].y = screenHeight - 130.0f - buildings[i].height;
        buildings[i].x = -6000.0f + spacing;

        spacing += (int)buildings[i].width;

        buildColors[i] = { static_cast<unsigned char>(GetRandomValue(200, 240)),
                           static_cast<unsigned char>(GetRandomValue(200, 240)),
                           static_cast<unsigned char>(GetRandomValue(200, 250)), 255 };
    }

    Rectangle floorCollisionBox = {-6000, 320, 13000, 8000};
    collisionBoxes->push_back(floorCollisionBox);

    Rectangle ledge1CollisionBox = {-400, 50, (float)mossyTile.width, (float)mossyTile.height};
    collisionBoxes->push_back(ledge1CollisionBox);

    Camera2D camera = { 0 };
    camera.target = { static_cast<float>(player.x + 20.0f), static_cast<float>(player.y + 20.0f )};
    camera.offset = { static_cast<float>(screenWidth/2.0f), static_cast<float>(screenHeight/2.0f)};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        UpdateMusicStream(music);   // Update music buffer with new stream data
        //----------------------------------------------------------------------------------
        
        // Player movement

        if (isAirborne == false) {
            // reset x velocity
            playerPosVel.Vel.x = 0;
            isMovingLeft = false;
            isMovingRight = false;
        }

        if (IsKeyDown(KEY_SPACE)){

            spacePressedDuration += 1;
        }
        else if(!IsKeyDown(KEY_SPACE)){
            player.height = defaultHeight;
            player.y      = player.y;
            if(spacePressedDuration != 0){
                playerPosVel.Vel.y -= computeJumpStrength(spacePressedDuration);
                if (IsKeyDown(KEY_RIGHT)) {
                    playerPosVel.Vel.x = 10;
                    directionFacing = 1;
                }
                else if (IsKeyDown(KEY_LEFT)) {
                    playerPosVel.Vel.x = -10;
                    directionFacing = 0;
                }

                isAirborne = true;
            }
            spacePressedDuration = 0;

            

            if (IsKeyDown(KEY_RIGHT) && !isAirborne && !IsKeyDown(KEY_SPACE)) {
                isMovingLeft = false;
                isMovingRight = true;
                directionFacing = isMovingRight;
                playerPosVel.Vel.x = 5;
                //player.x += 5;
            }
            else if (IsKeyDown(KEY_LEFT) && !isAirborne && !IsKeyDown(KEY_SPACE)) {
                //player.x -= 5;
                isMovingLeft = true;
                isMovingRight = false;
                directionFacing = isMovingRight;
                playerPosVel.Vel.x = -5;
            }
            else {
                isIdle = true;
                isMovingLeft  = false;
                isMovingRight = false;
            }

            //if (IsKeyDown(KEY_UP)) player.y -= 2;
            //else if (IsKeyDown(KEY_DOWN)) player.y += 2;
        }

        //printf("player.Vel.x before: %f\t", playerPosVel.Vel.x);

        // compute Dynamics
        playerPosVel.Pos.x = player.x;
        playerPosVel.Pos.y = player.y;



        playerPosVel = applyAcceleration(playerPosVel, acceleration);

        player.x = playerPosVel.Pos.x;
        player.y = playerPosVel.Pos.y;

        isAirborne  = checkAllCollisionBoxes(collisionBoxes, player, &playerPosVel) ? false : true;

        // apply Dyanmics
        player.x = playerPosVel.Pos.x;
        player.y = playerPosVel.Pos.y;

        //printf("player.Vel.x after: %f\n", playerPosVel.Vel.x);
        

        // Camera target follows player
        camera.target = {static_cast<float>(player.x + 20), static_cast<float>(player.y + 20) };

        // Camera rotation controls
        if (IsKeyDown(KEY_A)) camera.rotation--;
        else if (IsKeyDown(KEY_S)) camera.rotation++;

        // Limit camera rotation to 80 degrees (-40 to 40)
        if (camera.rotation > 40) camera.rotation = 40;
        else if (camera.rotation < -40) camera.rotation = -40;

        // Camera zoom controls
        camera.zoom += ((float)GetMouseWheelMove()*0.05f);

        if (camera.zoom > 3.0f) camera.zoom = 3.0f;
        else if (camera.zoom < 0.1f) camera.zoom = 0.1f;

        // Camera reset (zoom and rotation)
        if (IsKeyPressed(KEY_R))
        {
            camera.zoom = 1.0f;
            camera.rotation = 0.0f;
        }
        //----------------------------------------------------------------------------------

        // Texture location update
        framesCounter++;
        texturePos = playerPosVel.Pos;
        texturePos.x = playerPosVel.Pos.x + ((float)player.width - (float)scarfy.width/6)/2;
        texturePos.y = playerPosVel.Pos.y - (scarfy.height - player.height);
        if(isMovingRight) {
            texturePos.x -=runningOffset;
            curScarfy = scarfy;
            
        }
        else if(isMovingLeft) {
            texturePos.x +=runningOffset;
            curScarfy = scarfyF;           
        }
        else
        {
            frameRec.x = directionFacing * (float)scarfyIdle.width/2;
            frameRec.width = (float)scarfyIdle.width/2;
            
            //texturePos.y = playerPosVel.Pos.y;
            if(spacePressedDuration == 0) {
                curScarfy = scarfyIdle;
                texturePos.x = playerPosVel.Pos.x + directionFacing * (player.width - (float)scarfyIdle.width/2);
            }
            else {
                curScarfy = scarfyJump;
                texturePos.x = playerPosVel.Pos.x + directionFacing * (player.width - (float)scarfyJump.width/2);
            }
            // force animation update for next frame
            framesCounter = (60/framesSpeed) - 1;
        }

        // Animation Frame update
        if (framesCounter >= (60/framesSpeed))
        {
            framesCounter = 0;
            currentFrame++;
            

            if (currentFrame > 5) currentFrame = 0;

            if(isMovingRight){
                frameRec.x = (float)currentFrame*(float)scarfy.width/6;
                frameRec.width = (float)scarfy.width/6;
                //curScarfy = scarfy;
                
            }
            else if (isMovingLeft){
                frameRec.x = ((float)scarfy.width - (float)currentFrame*(float)scarfy.width/6);
                frameRec.width = (float)scarfy.width/6;
                //curScarfy = scarfyF;
            }
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode2D(camera);

                DrawRectangleRec(floorCollisionBox, DARKGRAY);
                
                for (int i = 0; i < MAX_BUILDINGS; i++) DrawRectangleRec(buildings[i], buildColors[i]);

                DrawRectangleRec(ledge1CollisionBox, DARKBLUE);

                //DrawRectangleRec(player, RED);

                DrawLine((int)camera.target.x, -screenHeight*10, (int)camera.target.x, screenHeight*10, GREEN);
                DrawLine(-screenWidth*10, (int)camera.target.y, screenWidth*10, (int)camera.target.y, GREEN);

                DrawTextureRec(curScarfy, frameRec, texturePos, WHITE);  // Draw part of the texture
                DrawTextureRec(mossyTile, mossyTileFrameRec,{ledge1CollisionBox.x, ledge1CollisionBox.y}, WHITE );


            EndMode2D();

            DrawText("SCREEN AREA", 640, 10, 20, RED);

            DrawRectangle(0, 0, screenWidth, 5, RED);
            DrawRectangle(0, 5, 5, screenHeight - 10, RED);
            DrawRectangle(screenWidth - 5, 5, 5, screenHeight - 10, RED);
            DrawRectangle(0, screenHeight - 5, screenWidth, 5, RED);

            DrawRectangle( 10, 10, 250, 113, Fade(SKYBLUE, 0.5f));
            DrawRectangleLines( 10, 10, 250, 113, BLUE);

            DrawText("Free 2d camera controls:", 20, 20, 10, BLACK);
            DrawText("- Right/Left to move Offset", 40, 40, 10, DARKGRAY);
            DrawText("- Mouse Wheel to Zoom in-out", 40, 60, 10, DARKGRAY);
            DrawText("- A / S to Rotate", 40, 80, 10, DARKGRAY);
            DrawText("- R to reset Zoom and Rotation", 40, 100, 10, DARKGRAY);

            sprintf(spaceString, "- Space is held down for: %d", spacePressedDuration);
            DrawText(spaceString, 40, 120, 10, DARKGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    UnloadMusicStream(music);   // Unload music stream buffers from RAM

    CloseAudioDevice();         // Close audio device (music streaming is automatically stopped)


    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

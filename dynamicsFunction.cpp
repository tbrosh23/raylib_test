#include <stdio.h>
#include "raylib.h"
#include "dynamicsFunction.h"

#define ACCEL_2ORDER -0.001f
#define ACCEL_1ORDER 0.3f
#define ACCEL_0ORDER 0.04f


float computeJumpStrength(int time) {
    time = time > 80 ? 80 : time;
    return ACCEL_2ORDER * time*time + ACCEL_1ORDER * time + ACCEL_0ORDER;
}

PosVel applyAcceleration(PosVel curPosVel, Vector2 acceleration ) {
    PosVel newPosVel;

    newPosVel.Pos.x = curPosVel.Pos.x + curPosVel.Vel.x + acceleration.x;
    newPosVel.Pos.y = curPosVel.Pos.y + curPosVel.Vel.y + acceleration.y;

    newPosVel.Vel.x = curPosVel.Vel.x + acceleration.x;
    newPosVel.Vel.y = curPosVel.Vel.y + acceleration.y;

    return newPosVel;
}


int checkAllCollisionBoxes(std::vector<Rectangle>* collisionBoxes, Rectangle player, PosVel* curPosVel) {
    Rectangle collisionRectangle;
    int collisionStatus = 0;
    for(int i = 0; i < collisionBoxes->size(); i++) {
        if(CheckCollisionRecs(player, (*collisionBoxes)[i])) {
            collisionRectangle = GetCollisionRec(player, (*collisionBoxes)[i]);
            // Compare the width to the height. The smaller of the 2 will be applied
            if(collisionRectangle.height < collisionRectangle.width) {
                // Check if the collision is top or bottom
                if(collisionRectangle.y == (*collisionBoxes)[i].y) { //upper
                    curPosVel->Pos.y = curPosVel->Pos.y - collisionRectangle.height;
                    collisionStatus = 1;
                }
                else curPosVel->Pos.y = curPosVel->Pos.y + collisionRectangle.height;
                // Reset the Velocity
                curPosVel->Vel.y = 0;
            }
            else {
                // Check if the collision is left or right
                if(collisionRectangle.x == (*collisionBoxes)[i].x) // left
                    curPosVel->Pos.x = curPosVel->Pos.x - collisionRectangle.width;
                else
                    curPosVel->Pos.x = curPosVel->Pos.x + collisionRectangle.width;
                
                // Bounce off the wall at 1/5 speed
                curPosVel->Vel.x = -curPosVel->Vel.x/4;
                collisionStatus = 2;
            }
        }
    }
    return collisionStatus;
}

int checkCameraBounds(Panel curPanel, Vector2 playerPos) {
    int retVal = eNoCHANGE;
    if(playerPos.y > (curPanel.camera.target.y - curPanel.camera.offset.y + SCREEN_HEIGHT/2)){
        retVal = eLOWER;
    }
    else if (playerPos.y < (curPanel.camera.target.y - curPanel.camera.offset.y-SCREEN_HEIGHT/2))
    {
        retVal = eUPPER;
    }
    else if (playerPos.x < (curPanel.camera.offset.x - SCREEN_WIDTH / 2))
    {
        retVal = eLEFT;
    }
    else if (playerPos.x > (curPanel.camera.offset.x + SCREEN_WIDTH / 2))
    {
        retVal = eRIGHT;
    }

    return retVal;    
}
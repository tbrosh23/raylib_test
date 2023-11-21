#include <vector>
#include "rayLib.h"

#define JUMP_SPEED 7

struct {
    Vector2 Pos;
    Vector2 Vel;
}typedef PosVel;

struct {
    Vector2 cameraPos;
    std::vector<Rectangle>* collisionBoxes;
}typedef Panel;

float computeJumpStrength(int time);

PosVel applyAcceleration(PosVel curPosVel, Vector2 acceleration );

// Checks player against all collision boxes. Modifies curPosVel based collisions
// returns:
// 1 if collision on an upper edge
// 2 if collision on side-wall.
// 0 all other times
int checkAllCollisionBoxes(std::vector<Rectangle>* collisionBoxes, Rectangle player, PosVel* curPosVel);
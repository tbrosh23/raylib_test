#include <vector>
#include "rayLib.h"

struct {
    Vector2 Pos;
    Vector2 Vel;
}typedef PosVel;

float computeJumpStrength(int time);

PosVel applyAcceleration(PosVel curPosVel, Vector2 acceleration );

// Checks player against all collision boxes. If a vertical collision on an upper edge occurs,
// return true.
bool checkAllCollisionBoxes(std::vector<Rectangle>* collisionBoxes, Rectangle player, PosVel* curPosVel);
#include <vector>
#include "rayLib.h"

#define JUMP_SPEED 7

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

enum cameraStat {
    eNoCHANGE = 0,
    eUPPER    = 1,
    eLOWER    = 2,
    eLEFT     = 3,
    eRIGHT    = 4
};

struct {
    Vector2 Pos;
    Vector2 Vel;
}typedef PosVel;

struct {
    Camera2D camera;
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

// @\briefChecks the player against the current panel's camera bounds.
// Returns:
// 0 if the player is still within the current panel camera bounds (cameraStat eNOCHANGE)
// corresponding cameraStat if player is outside current panel camera bounds
int checkCameraBounds(Panel curPanel, Vector2 playerPos);

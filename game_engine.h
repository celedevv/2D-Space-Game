#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "qpoint.h"
#include <cstdlib>
#include <config.h>

class game_engine
{
private:

    float randSpeed();
    QPointF randDirection();

public:
    game_engine();

    float getRandSpeed();
    QPointF getRandDirection();

    float newRotationAngle = 0.0f;
};

#endif // GAME_ENGINE_H

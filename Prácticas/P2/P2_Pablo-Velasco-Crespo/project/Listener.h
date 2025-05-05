#ifndef LISTENER_H
#define LISTENER_H

#include <AL/al.h>

class Listener 
{
public:
    static void setPosition(float x, float y, float z);

    static void setVelocity(float x, float y, float z);

    static void setOrientation(float forwardX, float forwardY, float forwardZ,
        float upX, float upY, float upZ);
};

#endif // LISTENER_H

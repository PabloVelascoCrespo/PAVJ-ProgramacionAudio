#ifndef LISTENER_H
#define LISTENER_H

#include <AL/al.h>

class Listener 
{
public:
    // Configura la posici�n del oyente
    static void setPosition(float x, float y, float z);

    // Configura la velocidad del oyente
    static void setVelocity(float x, float y, float z);

    // Configura la orientaci�n del oyente
    // El vector de orientaci�n incluye direcci�n hacia adelante (x, y, z)
    // y hacia arriba (upX, upY, upZ).
    static void setOrientation(float forwardX, float forwardY, float forwardZ,
        float upX, float upY, float upZ);
};

#endif // LISTENER_H

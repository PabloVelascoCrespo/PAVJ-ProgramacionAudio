#ifndef LISTENER_H
#define LISTENER_H

#include <AL/al.h>

class Listener 
{
public:
    // Configura la posición del oyente
    static void setPosition(float x, float y, float z);

    // Configura la velocidad del oyente
    static void setVelocity(float x, float y, float z);

    // Configura la orientación del oyente
    // El vector de orientación incluye dirección hacia adelante (x, y, z)
    // y hacia arriba (upX, upY, upZ).
    static void setOrientation(float forwardX, float forwardY, float forwardZ,
        float upX, float upY, float upZ);
};

#endif // LISTENER_H

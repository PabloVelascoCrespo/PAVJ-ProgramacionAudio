#include "../openal/AL/al.h"
#include "../openal/AL/alc.h"


#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#define LITE_GFX_IMPLEMENTATION
#include <glfw3.h>
#include <sstream>
#include <vector>

#include <fstream>     
#include <iterator>
#include <iostream>
#include <cmath>

#include "AudioBuffer.h"
#include "AudioSource.h"
#include "Listener.h"
#include <litegfx.h>

// ============ GLOBAL VALUES ============
// window
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// listener
float listenerRadius = 20.0f;
const float listenerSpeed = 300.0f;

// source
const float sourceRadius = 15.0f;
const float orbitRadius = WINDOW_HEIGHT / 4.0f;
const float orbitSpeed = 1.0f;
AudioSource* source = nullptr;

// math
const float PI = 3.1416f;

using namespace std;

bool initOpenAL(ALCdevice*& device, ALCcontext*& context)
{
  device = alcOpenDevice(nullptr);
  if (!device) return false;
  context = alcCreateContext(device, nullptr);
  if (!context) {
    alcCloseDevice(device);
    return false;
  }
  alcMakeContextCurrent(context);
  return true;
}

void cleanupOpenAL(ALCdevice* device, ALCcontext* context)
{
  alcMakeContextCurrent(nullptr);
  alcDestroyContext(context);
  alcCloseDevice(device);
}

int main()
{

  // Init OpenAL
  ALCdevice* device = nullptr;
  ALCcontext* context = nullptr;
  if (!initOpenAL(device, context))
  {
    std::cerr << "Error al inicializar OpenAL\n";
    return -1;
  }

  // ListenerSettings
  Listener::setPosition(0.0f, 0.0f, 0.0f);
  Listener::setVelocity(0.0f, 0.0f, 0.0f);
  Listener::setOrientation(0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

  // Load Audio Buffer
  AudioBuffer* buffer = AudioBuffer::load("data/engine.wav");
  if (!buffer)
  {
    std::cerr << "Error al cargar el buffer de audio\n";
    cleanupOpenAL(device, context);
    return -1;
  }

  // Create Audio Source
  source = new AudioSource(buffer);
  source->play();

  // Doppler Settings
  alDopplerFactor(1.0f);   
  alDopplerVelocity(343.3f);

  // Init GLFW
  if (!glfwInit())
  {
    std::cerr << "Error al inicializar GLFW\n";
    delete buffer;
    delete source;
    cleanupOpenAL(device, context);
    return -1;
  }

  // Create GLFW Window
  GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Efecto Doppler", nullptr, nullptr);
  if (!window)
  {
    std::cerr << "Error al crear la ventana de GLFW\n";
    glfwTerminate();
    delete buffer;
    delete source;
    cleanupOpenAL(device, context);
    return -1;
  }

  glfwMakeContextCurrent(window);
  lgfx_setup2d(WINDOW_WIDTH, WINDOW_HEIGHT);


  float listenerX = WINDOW_WIDTH / 2.0f;
  float listenerY = WINDOW_HEIGHT - 50.0f;
  float lastListenerX = listenerX;

  float angle = 0.0f;
  float lastAngle = angle;

  float deltaTime = 0.0f;
  float lastTime = glfwGetTime();
  

  // Main Loop
  while (!glfwWindowShouldClose(window))
  {
    float currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    glfwPollEvents();

    // == INPUT ==
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
      listenerX -= listenerSpeed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
      listenerX += listenerSpeed * deltaTime;
    }
    // == LISTENER POSITION ==
    if (listenerX < listenerRadius) 
    {
      listenerX = listenerRadius;
    }
    if (listenerX > WINDOW_WIDTH - listenerRadius)
    {
      listenerX = WINDOW_WIDTH - listenerRadius;
    }

    // == SOURCE POSITION == 
    lastAngle = angle;
    angle += orbitSpeed * deltaTime;
    if (angle > 2.0f * PI) angle -= 2.0f * PI;

    float centerX = WINDOW_WIDTH / 2.0f;
    float centerY = WINDOW_HEIGHT / 2.0f;

    float sourceX = centerX + cosf(angle) * orbitRadius;
    float sourceY = centerY + sinf(angle) * orbitRadius;

    float lastSourceX = centerX + cosf(lastAngle) * orbitRadius;
    float lastSourceY = centerY + sinf(lastAngle) * orbitRadius;


    float listenerVelocityX = (listenerX - lastListenerX) / deltaTime;
    lastListenerX = listenerX;

    Listener::setPosition(listenerX, 0.0f, listenerY);
    Listener::setVelocity(listenerVelocityX, 0.0f, 0.0f);

    float sourceVelocityX = (sourceX - lastSourceX) / deltaTime;
    float sourceVelocityY = (sourceY - lastSourceY) / deltaTime;
    source->setPosition(sourceX, 0.0f, sourceY);
    source->setVelocity(sourceVelocityX, 0.0f, sourceVelocityY);

    lgfx_clearcolorbuffer(0.0f, 0.0f, 0.0f);

    lgfx_setcolor(1.0f, 0.0f, 0.0f, 1.0f);
    lgfx_drawoval(sourceX, sourceY, sourceRadius, sourceRadius);

    lgfx_setcolor(1.0f, 1.0f, 1.0f, 1.0f);
    lgfx_drawoval(listenerX, listenerY, listenerRadius, listenerRadius);
    glfwSwapBuffers(window);
  }

  // Cleaning Resources
  delete source;
  delete buffer;
  glfwDestroyWindow(window);
  glfwTerminate();
  cleanupOpenAL(device, context);
  return 0;
}

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

#include "AudioBuffer.h"
#include "AudioSource.h"
#include "Listener.h"

using namespace std;

// Variables globales
AudioSource* source = nullptr;

// Callback para manejo de teclas
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (!source) return;

	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key) 
		{
		case GLFW_KEY_UP: 
		{ // Aumentar pitch
			float pitch = source->getPitch();
			source->setPitch(pitch + 0.1f);
			std::cout << "Pitch aumentado a " << pitch + 0.1f << "\n";
			break;
		}
		case GLFW_KEY_DOWN:
		{ // Disminuir pitch
			float pitch = source->getPitch();
			source->setPitch(std::max(0.1f, pitch - 0.1f)); // Evita valores negativos
			std::cout << "Pitch disminuido a " << pitch - 0.1f << "\n";
			break;
		}
		case GLFW_KEY_LEFT:
		{ // Mover fuente a la izquierda
			float x, y, z;
			source->getPosition(x, y, z);
			source->setPosition(x - 1.0f, y, z);
			std::cout << "Fuente movida a la izquierda: (" << x - 1.0f << ", " << y << ", " << z << ")\n";
			break;
		}
		case GLFW_KEY_RIGHT: 
		{ // Mover fuente a la derecha
			float x, y, z;
			source->getPosition(x, y, z);
			source->setPosition(x + 1.0f, y, z);
			std::cout << "Fuente movida a la derecha: (" << x + 1.0f << ", " << y << ", " << z << ")\n";
			break;
		}
		case GLFW_KEY_SPACE: 
		{ // Reproducir/pausar
			if (source->isPlaying()) 
			{
				source->pause();
				std::cout << "Audio pausado.\n";
			}
			else 
			{
				source->play();
				std::cout << "Audio reproduciéndose.\n";
			}
			break;
		}
		case GLFW_KEY_Q:
		{
			glfwSetWindowShouldClose(window, true);
			break;
		}
		}
	}
}

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
	// Inicializar OpenAL
	ALCdevice* device = nullptr;
	ALCcontext* context = nullptr;
	if (!initOpenAL(device, context)) 
	{
		std::cerr << "Error al inicializar OpenAL\n";
		return -1;
	}

	// Configurar el Listener
	Listener::setPosition(0.0f, 0.0f, 0.0f);
	Listener::setVelocity(0.0f, 0.0f, 0.0f);
	Listener::setOrientation(0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

	// Cargar el buffer de audio
	AudioBuffer* buffer = AudioBuffer::load("data/music.wav");
	if (!buffer)
	{
		std::cerr << "Error al cargar el buffer de audio\n";
		cleanupOpenAL(device, context);
		return -1;
	}

	// Crear la fuente de audio
	source = new AudioSource(buffer);

	// Inicializar GLFW
	if (!glfwInit())
	{
		std::cerr << "Error al inicializar GLFW\n";
		delete buffer;
		cleanupOpenAL(device, context);
		return -1;
	}

	// Crear la ventana de GLFW
	GLFWwindow* window = glfwCreateWindow(800, 600, "Audio Playback Test", nullptr, nullptr);
	if (!window)
	{
		std::cerr << "Error al crear la ventana de GLFW\n";
		glfwTerminate();
		delete buffer;
		cleanupOpenAL(device, context);
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keyCallback);

	std::cout << "Controles:\n"
		<< " - Flecha arriba: Aumentar pitch\n"
		<< " - Flecha abajo: Disminuir pitch\n"
		<< " - Flecha izquierda: Mover fuente a la izquierda\n"
		<< " - Flecha derecha: Mover fuente a la derecha\n"
		<< " - Espacio: Reproducir/Pausar\n";

	// Bucle principal
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}

	// Limpiar recursos
	delete source;
	delete buffer;
	glfwDestroyWindow(window);
	glfwTerminate();
	cleanupOpenAL(device, context);
	return 0;
}

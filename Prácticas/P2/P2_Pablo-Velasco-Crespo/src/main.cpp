#define LITE_GFX_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION

#include <iostream>
#include <string>
#include <vector>

#include "stb_image.h"

#include "../project/Vec2.h"
#include "../project//Sprite.h"
#include "../project/CircleCollider.h"
#include "../project/PixelsCollider.h"
#include "../project/RectCollider.h"
#include "../project/Font.h"


// Variables globales
double xMouseLocation, yMouseLocation;
const Vec2 windowResolution(1600, 1000);

// CALLBACK PARA LAS TECLAS
void inputCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

struct MovingText {
	Font* font;
	Vec2 position;
	float speed;
	unsigned int r, g, b, a;

	MovingText(Font* f, const Vec2& pos, float spd, const unsigned int _r, const unsigned int _g, const unsigned int _b, const unsigned int _a)
		: font(f), position(pos), speed(spd), r(_r), g(_g), b(_b), a(_a) {
	}
};

int main() {
	if (!glfwInit())
	{
		return 0;
	}

	Vec2* mouseLocation;

	GLFWwindow* window = glfwCreateWindow(windowResolution.getX(), windowResolution.getY(), "PVC: P5", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	lgfx_setup2d(windowResolution.getX(), windowResolution.getY());

	Font* font1 = Font::load("Jhon Cream.ttf", 32.0f);
	Font* font2 = Font::load("Orange.ttf", 48.0f);
	Font* font3 = Font::load("SFSlapstickComic.ttf", 64.0f);

	if (!font1 || !font2 || !font3) {
		std::cerr << "Error al cargar las fuentes.\n";
		glfwTerminate();
		return -1;
	}

	std::vector<Font*> fonts = { font1, font2, font3 };
	std::vector<MovingText> movingTexts;

	srand(static_cast<unsigned>(time(nullptr)));

	double previousTime = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		double currentTime = glfwGetTime();
		double elapsed_seconds = currentTime - previousTime;
		glfwGetCursorPos(window, &xMouseLocation, &yMouseLocation);


		if (rand() % 101 == 0) {
			Font* randomFont = fonts[rand() % fonts.size()];
			float randomY = static_cast<float>(rand() % static_cast<int>(windowResolution.getY()));
			float randomSpeed = 20.0f + static_cast<float>(rand() % 181); // Velocidad entre 20 y 200

			// Generar color aleatorio
			unsigned int randomR = rand() % 256;
			unsigned int randomG = rand() % 256;
			unsigned int randomB = rand() % 256;
			unsigned int randomA = 255; // Alfa completo

			movingTexts.emplace_back(randomFont, Vec2(windowResolution.getX(), randomY), randomSpeed, randomR, randomG, randomB, randomA);
		}

		lgfx_clearcolorbuffer(0, 0, 0);

		for (auto it = movingTexts.begin(); it != movingTexts.end();) {
			MovingText& text = *it;

			// Establecer el color antes de dibujar
			lgfx_setcolor(text.r / 255.0f, text.g / 255.0f, text.b / 255.0f, text.a / 255.0f);

			// Dibujar texto
			text.font->draw("Hello, world!", text.position);

			// Actualizar posición
			text.position.setX(text.position.getX() - text.speed * elapsed_seconds);

			// Eliminar texto si sale de la pantalla
			if (text.position.getX() < -500.0f) { // Considera un margen para texto ancho
				it = movingTexts.erase(it);
			}
			else {
				++it;
			}
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
		glfwSetKeyCallback(window, inputCallback);

		previousTime = currentTime;
	}
	delete font1;
	delete font2;
	delete font3;

	glfwTerminate();
}
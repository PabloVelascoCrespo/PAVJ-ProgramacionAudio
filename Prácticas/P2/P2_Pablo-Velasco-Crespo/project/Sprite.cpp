#include "Sprite.h"
#include "CircleCollider.h"
#include "RectCollider.h"
#include "PixelsCollider.h"
#include <glfw3.h>
#include <utility>

// Constructor with texture and frame parameters
Sprite::Sprite(const ltex_t* _tex, int _hframes, int _vframes)
    : texture(_tex), hFrames(_hframes), vFrames(_vframes),
    red(1), green(1), blue(1), alpha(1), angle(0),
    currentFrame(0), fps(24), mode(lblend_t::BLEND_SOLID),
    callback(nullptr), userData(nullptr), total_time(0),
    collider(nullptr), type(CollisionType::COLLISION_NONE) 
{}

// Default constructor
Sprite::Sprite()
    : Sprite(nullptr, 1, 1) 
{}

// Destructor
Sprite::~Sprite()
{
    delete collider;
}

// Callback setters and getters
void Sprite::setCallback(CallbackFunc _func) 
{
    callback = _func;
}

void Sprite::setUserData(void* _data) 
{
    userData = _data;
}

void* Sprite::getUserData() const
{
    return userData;
}

// Texture setters and getters
const ltex_t* Sprite::getTexture() const
{
    return texture;
}

void Sprite::setTexture(const ltex_t* _tex, int _hframes, int _vframes) 
{
    texture = _tex;
    hFrames = _hframes;
    vFrames = _vframes;
}

// Blend mode setters and getters
lblend_t Sprite::getBlend() const 
{
    return mode;
}

void Sprite::setBlend(lblend_t _mode)
{
    mode = _mode;
}

// Color setters and getters
float Sprite::getRed() const { return red; }
float Sprite::getGreen() const { return green; }
float Sprite::getBlue() const { return blue; }
float Sprite::getAlpha() const { return alpha; }

void Sprite::setColor(float _r, float _g, float _b, float _a) 
{
    red = _r;
    green = _g;
    blue = _b;
    alpha = _a;
}

// Position setters and getters
const Vec2& Sprite::getPosition() const
{
    return position;
}

void Sprite::setPosition(const Vec2& _pos) 
{
    position = _pos;
    if (collider) 
    {
        collider->setPosition(_pos);
    }
}

// Angle setters and getters
float Sprite::getAngle() const { return angle; }
void Sprite::setAngle(float _angle) { angle = _angle; }

// Scale setters and getters
const Vec2& Sprite::getScale() const { return scale; }
void Sprite::setScale(const Vec2& _scale) 
{ 
    scale = _scale;
    if (collider)
    {
        if (type == CollisionType::COLLISION_CIRCLE)
        {
            float radius = std::max(texture->width, texture->height) / 2 * scale.getX();
            collider->setSize(Vec2(radius,0));
        }
        else if (type == CollisionType::COLLISION_RECT)
        {
            Vec2 rectSize(texture->width * scale.getX(), texture->height * scale.getY());
            collider->setSize(rectSize);
        }
        // DE MOMENTO NO TENEMOS EN CUENTA EL CAMBIO DE ESCALA DE LAS COLISIONES A NIVEL DE PÍXEL
    }
}

// Size calculation
Vec2 Sprite::getSize() const
{
    return Vec2(texture->width / hFrames * scale.getX(), texture->height / vFrames * scale.getY());
}

// Pivot setters and getters
const Vec2& Sprite::getPivot() const { return pivot; }
void Sprite::setPivot(const Vec2& _pivot) { pivot = _pivot; }

// Frame setters and getters
int Sprite::getHframes() const { return hFrames; }
int Sprite::getVframes() const { return vFrames; }
int Sprite::getFps() const { return fps; }
void Sprite::setFps(int _fps) { fps = _fps; }

float Sprite::getCurrentFrame() const { return currentFrame; }
void Sprite::setCurrentFrame(int _frame) { currentFrame = _frame; }

// Collision management
void Sprite::setCollisionType(CollisionType _type)
{
    type = _type;
    delete collider;
    collider = nullptr;

    if (type == CollisionType::COLLISION_CIRCLE) 
    {
        float radius = std::max(texture->width, texture->height) / 2 * scale.getX();
        collider = new CircleCollider(position, radius);
    }
    else if (type == CollisionType::COLLISION_RECT) 
    {
        Vec2 rectSize(texture->width * scale.getX(), texture->height * scale.getY());
        collider = new RectCollider(position, rectSize);
    }
    else if (type == CollisionType::COLLISION_PIXELS)
    {
        size_t pixelCount = texture->width * texture->height * 4;
        uint8_t* pixels = new uint8_t[pixelCount];
        ltex_getpixels(texture, pixels);
        collider = new PixelsCollider(position, Vec2(texture->width, texture->height), pixels);
    }
}

Sprite::CollisionType Sprite::getCollisionType() const { return type; }
const Collider* Sprite::getCollider() const { return collider; }

bool Sprite::collides(const Sprite& _other) const 
{
    if (!collider || !_other.getCollider()) 
    {
        return false;
    }
    return collider->collides(*_other.getCollider());
}

// Update logic
void Sprite::update(float _deltaTime) 
{
    total_time += _deltaTime;

    if (fps != 0 && total_time >= 1.f / fabs(fps))
    {
        total_time -= 1.f / fabs(fps);
        currentFrame = (fps > 0)
            ? (currentFrame + 1) % (hFrames * vFrames)
            : (currentFrame - 1 + (hFrames * vFrames)) % (hFrames * vFrames);

        // En el caso de que el sprite tenga animación y tenga colisión a nivel de píxel 
        // se tendrán que actualizar los píxeles cada frame
        if (collider)
        {
            if (type == CollisionType::COLLISION_PIXELS)
            {
                size_t pixelCount = texture->width * texture->height * 4;
                uint8_t* pixels = new uint8_t[pixelCount];
                ltex_getpixels(texture, pixels);
                static_cast<PixelsCollider*>(collider)->setPixels(pixels);
            }
        }
    }

    if (callback) 
    {
        callback(*this, _deltaTime);
    }
}

// Drawing logic
void Sprite::draw() const 
{
    lgfx_setblend(mode);

    float frameWidth = texture->width / hFrames;
    float frameHeight = texture->height / vFrames;

    int frameX = currentFrame % hFrames;
    int frameY = currentFrame / hFrames;

    float u0 = frameX * frameWidth / texture->width;
    float v0 = frameY * frameHeight / texture->height;
    float u1 = (frameX + 1) * frameWidth / texture->width;
    float v1 = (frameY + 1) * frameHeight / texture->height;

    lgfx_setcolor(red, green, blue, alpha);
    Vec2 size = getSize();
    ltex_drawrotsized(texture, position.getX(), position.getY(), angle, pivot.getX(), pivot.getY(), size.getX(), size.getY(), u0, v0, u1, v1);
}

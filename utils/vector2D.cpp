#include <cmath>
#include "./vector2D.h"

Vector2D::Vector2D()
    : x(0.0f), y(0.0f)
{
}

Vector2D::Vector2D(float single)
    : x(single), y(single)
{
}


Vector2D::Vector2D(float x, float y)
    : x(x), y(y)
{
}

Vector2D::Vector2D(const Vector2D& vec)
    : x(vec.x), y(vec.y)
{
}


Vector2D::Vector2D(Vector2D&& vec)
    : x(vec.x), y(vec.y)
{
}


auto Vector2D::operator=(const Vector2D& vec) -> Vector2D& {
    this->x = vec.x;
    this->y = vec.y;

    return *this;
}


auto Vector2D::operator=(Vector2D&& vec) -> Vector2D& {
    this->x = vec.x;
    this->y = vec.y;

    return *this;
}


Vector2D::~Vector2D() {
    this->x = 0.f;
    this->y = 0.f;
}


auto Vector2D::operator+(Vector2D& other) -> Vector2D { 
    return Vector2D(this->x + other.x, this->y + other.y);
}


auto Vector2D::operator-(Vector2D& other) -> Vector2D { 
    return Vector2D(this->x - other.x, this->y - other.y);
}


auto Vector2D::operator*(float scalar) -> Vector2D { 
    return Vector2D(this->x * scalar, this->y * scalar);
}


auto Vector2D::operator/(float scalar) -> Vector2D {
    return Vector2D(this->x / scalar, this->y / scalar);
}


auto Vector2D::operator+=(Vector2D& other) -> Vector2D& {
    this->x += other.x;
    this->y += other.y;
    return *this;
}


auto Vector2D::operator-=(Vector2D& other) -> Vector2D& {
    this->x -= other.x;
    this->y -= other.y;
    return *this;
}


auto Vector2D::operator*=(float scalar) -> Vector2D& {
    this->x *= scalar;
    this->y *= scalar;
    return *this;
}


auto Vector2D::operator/=(float scalar) -> Vector2D& {
    this->x /= scalar;
    this->y /= scalar;
    return *this;
}


[[nodiscard]] auto Vector2D::magnitude() const -> float {
    return sqrtf(this->x * this->x + this->y * this->y);
}


[[nodiscard]] auto Vector2D::normalized() const -> Vector2D {
    Vector2D cpy(*this);
    cpy /= cpy.magnitude();
    return cpy;
}

auto Vector2D::map(std::function<float (float)> function) -> void {
    this->x = function(this->x);
    this->y = function(this->y);
}

auto Vector2D::to_string() -> std::string {
    char buffer[50];
    sprintf(buffer, "[%.2f, %.2f]", this->x, this->y);
    return std::string(buffer);
}

auto operator<< (std::ostream& output, Vector2D& vec) -> std::ostream& {
    output << vec.to_string();
}
#pragma once

#include <nlohmann/json.hpp>

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdexcept>
#include <string>

#define PI_F float(M_PI)
#define PI_D double(M_PI)
#define MAGIC_CONSTANT 2654435769LL //https://en.wikipedia.org/wiki/Tiny_Encryption_Algorithm

#undef min
#undef max

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

template<typename T>
struct Vector1
{
    T x = 0;
    Vector1() = default;
    Vector1(T x) : x(x) { }
    template<typename R> Vector1(Vector1<R> other) { this->x = T(other.x); }
	T distanceTo(Vector1<T>& other) const { return other.x - this->x; }
    T magnitude() const { return sqrt(pow(this->x, 2)); }
    Vector1<T> normalize() const { float mag = this->magnitude(); return Vector1<T>(this->x / mag); }
	std::string toString() const { return std::to_string(this->x); }
    Vector1<T> interpolate(Vector1<T>& other, float delta) const { return Vector1<T>(this->x + ((other.x - this->x) * delta)); }
    bool isZero() const { return this->x == 0; }
    Vector1<T> toRadians() { return this->x * (PI_F / 180.0f); }
    Vector1<T> toDegrees() { return this->x * (180.0f / PI_F); }
    Vector1<T> adjust() { return Vector1<T>(this->x + 90); }
    Vector1<T> cos() { return std::cos(this->x); }
    Vector1<T> sin() { return std::sin(this->x); }
    operator nlohmann::json() const { return nlohmann::json{ { "x", this->x } }; }
    template<typename R> void operator=(R other) { this->x = other.x; }
    template<typename R> bool operator==(R other) { return this->x == other.x; }
    Vector1<T> operator+(Vector1<T> other) const { return Vector1<T>(this->x + other.x); }
    Vector1<T> operator-(Vector1<T> other) const { return Vector1<T>(this->x - other.x); }
    Vector1<T> operator*(Vector1<T> other) const { return Vector1<T>(this->x * other.x); }
    Vector1<T> operator/(Vector1<T> other) const { return Vector1<T>(this->x / other.x); }
    Vector1<T>& operator+=(Vector1<T> other) { this->x += other.x; return *this; }
    Vector1<T>& operator-=(Vector1<T> other) { this->x -= other.x; return *this; }
    Vector1<T>& operator*=(Vector1<T> other) { this->x *= other.x; return *this; }
    Vector1<T>& operator/=(Vector1<T> other) { this->x /= other.x; return *this; }
    Vector1<T> operator+(T other) const { return Vector1<T>(this->x + other); }
    Vector1<T> operator-(T other) const { return Vector1<T>(this->x - other); }
    Vector1<T> operator*(T other) const { return Vector1<T>(this->x * other); }
    Vector1<T> operator/(T other) const { return Vector1<T>(this->x / other); }
    Vector1<T>& operator+=(T other) { this->x += other; return *this; }
    Vector1<T>& operator-=(T other) { this->x -= other; return *this; }
    Vector1<T>& operator*=(T other) { this->x *= other; return *this; }
    Vector1<T>& operator/=(T other) { this->x /= other; return *this; }
    T& operator[](int index) { *(this + (index * sizeof(T))); }
};

template<typename T>
struct Vector3;

template<typename T>
struct Vector2
{
    T x = 0;
    T y = 0;
    Vector2() = default;
    Vector2(T x, T y) : x(x), y(y) { }
    template<typename R> Vector2(Vector1<R> other) { this->x = T(other.x); this->y = T(other.y); }
    Vector2(glm::vec2 other) { this->x = other.x; this->y = other.y; }
	T distanceTo(Vector2<T>& other) const { return sqrt(pow(other.x - this->x, 2) + pow(other.y - this->y, 2)); }
    T magnitude() const { return sqrt(pow(this->x, 2) + pow(this->y, 2)); };
    Vector2<T> normalize() const { float mag = this->magnitude(); return Vector2<T>(this->x / mag, this->y / mag); }
	std::string toString() const { return std::to_string(x) + "," + std::to_string(y); }
    Vector2<T> interpolate(Vector2<T> other, float delta) const { return Vector2<T>(this->x + ((other.x - this->x) * delta), this->y + ((other.y - this->y) * delta)); }
    bool isZero() const { return this->x == 0 && this->y == 0; }
    Vector2<T> toRadians() { return { this->x * (PI_F / 180.0f), this->y * (PI_F / 180.0f) }; }
    Vector2<T> toDegrees() { return { this->x * (180.0f / PI_F), this->y * (180.0f / PI_F) }; }
    Vector2<T> adjust() { return Vector2<T>(this->x, this->y + 90); }
    Vector3<T> toCartesian() { return Vector3<T>(std::cos(this->x) * std::sin(this->y), -std::sin(this->x), std::cos(this->x) * std::cos(this->y)); }
    Vector1<T> invert() { return Vector1<T>(-this->x); }
    Vector1<T> min() { return this->x < this->y ? this->x : this->y; }
    Vector1<T> max() { return this->x > this->y ? this->x : this->y; }
    Vector1<T> size() { return this->max() - this->min(); }
    Vector1<T> center() { return this->min() + (this->size() / 2.0f); }
    size_t hash() const {
        size_t val = 0;
        if constexpr (std::is_integral_v<T>)
        {
            val ^= this->x + MAGIC_CONSTANT + (val << 6LL) + (val >> 2LL);
            val ^= this->y + MAGIC_CONSTANT + (val << 6LL) + (val >> 2LL);
        }
        else
        {
            val ^= std::hash<T>(this->x) + MAGIC_CONSTANT + (val << 6LL) + (val >> 2LL);
            val ^= std::hash<T>(this->y) + MAGIC_CONSTANT + (val << 6LL) + (val >> 2LL);
        }
        return val;
    }
    operator nlohmann::json() const { return nlohmann::json{ { "x", this->x }, { "y", this->y } }; }
    operator glm::vec2() const { return glm::vec2(this->x, this->y); }
    template<typename R> void operator=(R other) { this->x = other.x; this->y = other.y; }
    template<typename R> bool operator==(const R& other) const { return this->x == other.x && this->y == other.y; }
    Vector2<T> operator+(Vector2<T> other) const { return Vector2<T>(this->x + other.x, this->y + other.y); }
    Vector2<T> operator-(Vector2<T> other) const { return Vector2<T>(this->x - other.x, this->y - other.y); }
    Vector2<T> operator*(Vector2<T> other) const { return Vector2<T>(this->x * other.x, this->y * other.y); }
    Vector2<T> operator/(Vector2<T> other) const { return Vector2<T>(this->x / other.x, this->y / other.y); }
    Vector2<T>& operator+=(Vector2<T> other) { this->x += other.x; this->y += other.y; return *this; }
    Vector2<T>& operator-=(Vector2<T> other) { this->x -= other.x; this->y -= other.y; return *this; }
    Vector2<T>& operator*=(Vector2<T> other) { this->x *= other.x; this->y *= other.y; return *this; }
    Vector2<T>& operator/=(Vector2<T> other) { this->x /= other.x; this->y /= other.y; return *this; }
    Vector2<T> operator+(T other) const { return Vector2<T>(this->x + other, this->y + other); }
    Vector2<T> operator-(T other) const { return Vector2<T>(this->x - other, this->y - other); }
    Vector2<T> operator*(T other) const { return Vector2<T>(this->x * other, this->y * other); }
    Vector2<T> operator/(T other) const { return Vector2<T>(this->x / other, this->y / other); }
    Vector2<T>& operator+=(T other) { this->x += other; this->y += other; return *this; }
    Vector2<T>& operator-=(T other) { this->x -= other; this->y -= other; return *this; }
    Vector2<T>& operator*=(T other) { this->x *= other; this->y *= other; return *this; }
    Vector2<T>& operator/=(T other) { this->x /= other; this->y /= other; return *this; }
    T& operator[](int index) { *(this + (index * sizeof(T))); }
};
template<typename T>
struct std::hash<Vector2<T>>
{
    size_t operator()(const Vector2<T>& obj) const {
        return obj.hash();
    }
};

template<typename T>
struct Vector3
{
    T x = 0;
    T y = 0;
    T z = 0;
    Vector3() = default;
    Vector3(T x, T y, T z) : x(x), y(y), z(z) { }
    Vector3(Vector1<T> x, Vector1<T> y, Vector1<T> z) : x(x.x), y(y.x), z(z.x) { }
    Vector3(glm::ivec3 ivec) : x(ivec.x), y(ivec.y), z(ivec.z) {}
    Vector3(glm::vec3 vec) : x(vec.x), y(vec.y), z(vec.z) {}
    template<typename R> Vector3(const Vector3<R> other) { this->x = T(other.x); this->y = T(other.y); this->z = T(other.z); }
	template<typename R> float distanceTo(const Vector3<R>& other) const { return sqrt(pow(other.x - this->x, 2) + pow(other.y - this->y, 2) + pow(other.z - this->z, 2)); }
    T magnitude() const { return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2)); };
    Vector3<T> normalize() const { float mag = this->magnitude(); return Vector3<T>(this->x / mag, this->y / mag, this->z / mag); }
	std::string toString() const { return std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z); }
    Vector3<T> interpolate(const Vector3<T>& other, float delta) const { return Vector3<T>(this->x + ((other.x - this->x) * delta), this->y + ((other.y - this->y) * delta), this->z + ((other.z - this->z) * delta)); }
    bool isZero() const { return this->x == 0 && this->y == 0 && this->z == 0; }
    bool operator<(const Vector3<T>& rhs) const {
        return this->x < rhs.x || (this->x == rhs.x && this->y < rhs.y) ||
            (this->x == rhs.x && this->y == rhs.y && this->z < rhs.z);
    };
    operator nlohmann::json() const { return nlohmann::json{ { "x", this->x }, { "y", this->y }, { "z", this->z } }; }
    operator glm::vec3() const { return glm::vec3(this->x, this->y, this->z); }
    template<typename R> void operator=(R other) { this->x = other.x; this->y = other.y; this->z = other.z; }
    template<typename R> bool operator==(R other) const { return this->x == other.x && this->y == other.y && this->z == other.z; }
    Vector3<T> operator+(Vector3<T> other) const { return Vector3<T>(this->x + other.x, this->y + other.y, this->z + other.z); }
    Vector3<T> operator-(Vector3<T> other) const { return Vector3<T>(this->x - other.x, this->y - other.y, this->z - other.z); }
    Vector3<T> operator*(Vector3<T> other) const { return Vector3<T>(this->x * other.x, this->y * other.y, this->z * other.z); }
    Vector3<T> operator/(Vector3<T> other) const { return Vector3<T>(this->x / other.x, this->y / other.y, this->z / other.z); }
    Vector3<T>& operator+=(Vector3<T> other) { this->x += other.x; this->y += other.y; this->z += other.z; return *this; }
    Vector3<T>& operator-=(Vector3<T> other) { this->x -= other.x; this->y -= other.y; this->z -= other.z; return *this; }
    Vector3<T>& operator*=(Vector3<T> other) { this->x *= other.x; this->y *= other.y; this->z *= other.z; return *this; }
    Vector3<T>& operator/=(Vector3<T> other) { this->x /= other.x; this->y /= other.y; this->z /= other.z; return *this; }
    Vector3<T> operator+(T other) const { return Vector3<T>(this->x + other, this->y + other, this->z + other); }
    Vector3<T> operator-(T other) const { return Vector3<T>(this->x - other, this->y - other, this->z - other); }
    Vector3<T> operator*(T other) const { return Vector3<T>(this->x * other, this->y * other, this->z * other); }
    Vector3<T> operator/(T other) const { return Vector3<T>(this->x / other, this->y / other, this->z / other); }
    Vector3<T>& operator+=(T other) { this->x += other; this->y += other; this->z += other; return *this; }
    Vector3<T>& operator-=(T other) { this->x -= other; this->y -= other; this->z -= other; return *this; }
    Vector3<T>& operator*=(T other) { this->x *= other; this->y *= other; this->z *= other; return *this; }
    Vector3<T>& operator/=(T other) { this->x /= other; this->y /= other; this->z /= other; return *this; }
    T& operator[](int index) { *(this + (index * sizeof(T))); }
	//Returns in RADIANS
	Vector2<T> calcAngleRad(Vector3<T> target) const
	{
		Vector2<T> vec2;
		T dX = this->x - target.x;
		T dY = this->y - target.y;
		T dZ = this->z - target.z;
		double distance = sqrt(dX * dX + dY * dY + dZ * dZ);
		vec2.x = ((T)atan2(dY, (float)distance) * (float)3.13810205 / (float)3.141592653589793);
		vec2.y = ((T)atan2(dZ, dX) * (float)3.13810205 / (float)3.141592653589793) + (float)1.569051027;
		return vec2;
	}
	//Returns in DEGREES
	Vector2<T> calcAngleDeg(Vector3<T> target) const
	{
        return calcAngleRad(target).toDegrees();
	}
	//Also returns in Degrees
	Vector2<T> calcAngle(Vector3<T> target) const {
		return calcAngleDeg(target);
	}
    Vector3<T> rotateXYZ(Vector2<T> angles) const {
        Vector2<T> anglesRad = angles.adjust().toRadians();
        Vector2<T> vecXZ(this->x, this->z);
        T speed = vecXZ.magnitude();

        Vector3<T> rotated;
        rotated.x = cos(anglesRad.y) * speed;
        rotated.z = sin(anglesRad.y) * speed;
        rotated.y = anglesRad.invert().sin().x * speed;
        return rotated;
    }
    size_t hash() const {
        size_t val = 0;
        if constexpr (std::is_integral_v<T>)
        {
            val ^= this->x + MAGIC_CONSTANT + (val << 6LL) + (val >> 2LL);
            val ^= this->y + MAGIC_CONSTANT + (val << 6LL) + (val >> 2LL);
            val ^= this->z + MAGIC_CONSTANT + (val << 6LL) + (val >> 2LL);
        }
        else
        {
            val ^= std::hash<T>(this->x) + MAGIC_CONSTANT + (val << 6LL) + (val >> 2LL);
            val ^= std::hash<T>(this->y) + MAGIC_CONSTANT + (val << 6LL) + (val >> 2LL);
            val ^= std::hash<T>(this->z) + MAGIC_CONSTANT + (val << 6LL) + (val >> 2LL);
        }
        return val;
    }
};
template<typename T>
struct std::hash<Vector3<T>>
{
    size_t operator()(const Vector3<T>& obj) const {
        return obj.hash();
    }
};

template<typename T>
struct Vector4
{
    T x = 0;
    T y = 0;
    T z = 0;
    T w = 0;
    Vector4() = default;
    Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) { }
    Vector4(Vector2<T> min, Vector2<T> max) { this->x = min.x; this->y = min.y; this->z = max.x, this->w = max.y; }
    template<typename R> Vector4(Vector4<R> other) { this->x = T(other.x); this->y = T(other.y); this->z = T(other.z); this->w = T(other.w); }
    Vector4(const nlohmann::json& data) { this->x = data["x"]; this->y = data["y"]; this->z = data["z"]; this->w = data["w"]; }
    Vector4(glm::vec4 other) { this->x = other.x; this->y = other.y; this->z = other.z; this->w = other.w; }
	T distanceTo(Vector4<T>& other) const { return sqrt(pow(other.x - this->x, 2) + pow(other.y - this->y, 2) + pow(other.z - this->z, 2) + pow(other.w - this->w, 2)); }
    T magnitude() const { return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2) + pow(this->w, 2)); };
    Vector4<T> normalize() const { float mag = this->magnitude(); return Vector4<T>(this->x / mag, this->y / mag, this->z / mag, this->w / mag); }
	std::string toString() const { return std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + "," + std::to_string(w); }
    Vector4<T> interpolate(Vector4<T> other, float delta) const { return Vector4<T>(this->x + ((other.x - this->x) * delta), this->y + ((other.y - this->y) * delta), this->z + ((other.z - this->z) * delta), this->w + ((other.w - this->w) * delta)); }
    bool isZero() const { return this->x == 0 && this->y == 0 && this->z == 0 && this->w == 0; }
    Vector2<T> min() { return Vector2<T>(x, y); }
    Vector2<T> max() { return Vector2<T>(z, w); }
    Vector2<T> size() { return this->max() - this->min(); }
    Vector2<T> center() { Vector2<T> size = this->size(); return Vector2<T>(this->x + (size.x / 2.0f), this->y + (size.y / 2.0f)); }
    bool contains(Vector2<T> point) const { return this->x <= point.x && this->y <= point.y && this->z >= point.x && this->w >= point.y; }
    operator nlohmann::json() const { return { { "x", this->x }, { "y", this->y }, { "z", this->z }, { "w", this->w } }; }
    operator glm::vec4() const { return glm::vec4(this->x, this->y, this->z, this->w); }
    template<typename R> void operator=(R other) { this->x = other.x; this->y = other.y; this->z = other.z; this->w = other.w; }
    template<typename R> bool operator==(R other) { return this->x == other.x && this->y == other.y && this->z == other.z && this->w == other.w; }
    Vector4<T> operator+(Vector4<T> other) const { return Vector4(this->x + other.x, this->y + other.y, this->z + other.z, this->w + other.w); }
    Vector4<T> operator-(Vector4<T> other) const { return Vector4(this->x - other.x, this->y - other.y, this->z - other.z, this->w - other.w); }
    Vector4<T> operator*(Vector4<T> other) const { return Vector4(this->x * other.x, this->y * other.y, this->z * other.z, this->w * other.w); }
    Vector4<T> operator/(Vector4<T> other) const { return Vector4(this->x / other.x, this->y / other.y, this->z / other.z, this->w / other.w); }
    Vector4<T>& operator+=(Vector4<T> other) { this->x += other.x; this->y += other.y; this->z += other.z; this->w += other.w; return *this; }
    Vector4<T>& operator-=(Vector4<T> other) { this->x -= other.x; this->y -= other.y; this->z -= other.z; this->w -= other.w; return *this; }
    Vector4<T>& operator*=(Vector4<T> other) { this->x *= other.x; this->y *= other.y; this->z *= other.z; this->w *= other.w; return *this; }
    Vector4<T>& operator/=(Vector4<T> other) { this->x /= other.x; this->y /= other.y; this->z /= other.z; this->w /= other.w; return *this; }
    Vector4<T> operator+(T other) const { return Vector4<T>(this->x + other, this->y + other, this->z + other, this->w + other); }
    Vector4<T> operator-(T other) const { return Vector4<T>(this->x - other, this->y - other, this->z - other, this->w - other); }
    Vector4<T> operator*(T other) const { return Vector4<T>(this->x * other, this->y * other, this->z * other, this->w * other); }
    Vector4<T> operator/(T other) const { return Vector4<T>(this->x / other, this->y / other, this->z / other, this->w / other); }
    Vector4<T>& operator+=(T other) { this->x += other; this->y += other; this->z += other; this->w += other; return *this; }
    Vector4<T>& operator-=(T other) { this->x -= other; this->y -= other; this->z -= other; this->w -= other; return *this; }
    Vector4<T>& operator*=(T other) { this->x *= other; this->y *= other; this->z *= other; this->w *= other; return *this; }
    Vector4<T>& operator/=(T other) { this->x /= other; this->y /= other; this->z /= other; this->w /= other; return *this; }
    T& operator[](int index) { *(this + (index * sizeof(T))); }
};

template<typename T>
using Color = Vector4<T>;

#include <sstream>
#include <vector>
struct ColorF : public Color<float>
{
    ColorF(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f)
    {
        this->x = r;
        this->y = g;
        this->z = b;
        this->w = a;
    }

    ColorF(const Vector4<float>& other)
    {
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
        this->w = other.w;
    }

    static ColorF parse(const std::string& data)
    {
        std::string hex = data;
        // Remove the "#" prefix if present
        if (hex[0] == '#') {
            hex = hex.substr(1);
        }

        // Parse the hex values into an array of integers
        std::vector<int> hexValues;
        for (size_t i = 0; i < hex.size(); i += 2) {
            std::string hexByte = hex.substr(i, 2);
            std::stringstream ss;
            ss << std::hex << hexByte;
            int value;
            ss >> value;
            hexValues.push_back(value);
        }

        // Set the RGBA values based on the parsed hex values
        float r = hexValues.size() > 0 ? hexValues[0] / 255.0f : 0.0f;
        float g = hexValues.size() > 1 ? hexValues[1] / 255.0f : 0.0f;
        float b = hexValues.size() > 2 ? hexValues[2] / 255.0f : 0.0f;
        float a = hexValues.size() > 3 ? hexValues[3] / 255.0f : 1.0f;

        return ColorF(r, g, b, a);
    }

    static ColorF fromHSV(float hue, float saturation, float brightness, float alpha = 1.0f)
    {
        int colorSpace = static_cast<int>(hue * 6);
        float fractional = hue * 6 - colorSpace;
        float chroma = brightness * (1 - saturation);
        float q = brightness * (1 - fractional * saturation);
        float t = brightness * (1 - (1 - fractional) * saturation);
        switch (colorSpace % 6)
        {
        case 0: return ColorF(brightness, t, chroma, alpha);
        case 1: return ColorF(q, brightness, chroma, alpha);
        case 2: return ColorF(chroma, brightness, t, alpha);
        case 3: return ColorF(chroma, q, brightness, alpha);
        case 4: return ColorF(t, chroma, brightness, alpha);
        case 5: return ColorF(brightness, chroma, q, alpha);
        default: return ColorF(0, 0, 0, alpha);
        }
    }

    ColorF interpolate(ColorF other, float delta) const
    {
        return ColorF(static_cast<Vector4<float>>(*this).interpolate(static_cast<Vector4<float>>(other), delta));
    }

    operator nlohmann::json() const
    {
        std::stringstream ss;
        ss << "#"   << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(x * 0xFF)
                    << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(y * 0xFF)
                    << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(z * 0xFF)
                    << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(w * 0xFF);

        return ss.str();
    }

    uint32_t asInt() const
    {
        uint8_t r = static_cast<uint8_t>(x * 0xFF);
        uint8_t g = static_cast<uint8_t>(y * 0xFF);
        uint8_t b = static_cast<uint8_t>(z * 0xFF);
        uint8_t a = static_cast<uint8_t>(w * 0xFF);

        return (a << 24) | (b << 16) | (g << 8) | r;
    }
};

template<typename T = float>
struct AABB {
    /* Fields */
    Vector3<T> firstCorner = Vector3<T>(0,0,0);
    Vector3<T> secondCorner = Vector3<T>(0,0,0);
    /* Virtuals */
    /* Functions */

    AABB() = default;
    AABB(const Vector3<T>& firstCorner, const Vector3<T>& secondCorner)
    {
        this->firstCorner = firstCorner;
        this->secondCorner = secondCorner;
    }
    template<typename R>
    AABB(const Vector3<R>& firstCorner, const Vector3<R>& secondCorner)
    {
        this->firstCorner = Vector3<T>(firstCorner);
        this->secondCorner = Vector3<T>(secondCorner);
    }


    Vector3<T> closestPoint(Vector3<T> source)
    {
        Vector3<T> closest; // The closest point on the box to the source

        // For each dimension (x, y, z), find the closest point on the box
        closest.x = std::max(this->firstCorner.x, std::min(source.x, this->secondCorner.x));
        closest.y = std::max(this->firstCorner.y, std::min(source.y, this->secondCorner.y));
        closest.z = std::max(this->firstCorner.z, std::min(source.z, this->secondCorner.z));

        return closest;
    }

    bool isVectorOn(const Vector3<float>& vector, float tolerance) {
        return ((std::abs(vector.x - this->firstCorner.x) < tolerance) || (std::abs(vector.x - this->secondCorner.x) < tolerance)) ||
                ((std::abs(vector.y - this->firstCorner.y) < tolerance) || (std::abs(vector.y - this->secondCorner.y) < tolerance)) ||
                ((std::abs(vector.z - this->firstCorner.z) < tolerance) || (std::abs(vector.z - this->secondCorner.z) < tolerance));
    }

    AABB<T> flatten()
    {
        Vector3<T> newFirst = firstCorner;
        Vector3<T> newSecond = secondCorner;
        Vector3<T> middle = newFirst.interpolate(newSecond, 0.5f);
        newFirst.y = middle.y;
        newSecond.y = middle.y;
        return AABB<T>(
            newFirst,
            newSecond
            );
    }
};

template<typename T>
struct Matrix16 {
    union {
        struct {
            T a;
            T b;
            T c;
            T d;

            T e;
            T f;
            T g;
            T h;

            T i;
            T j;
            T k;
            T l;

            T m;
            T n;
            T o;
            T p;
        } val;
        T arr[16];
    };

    Matrix16() {

    }

    float dotProdX(const Vector3<float>& p) const {
        return p.x * arr[0] + p.y * arr[4] + p.z * arr[8] + arr[12];
    }

    float dotProdY(const Vector3<float>& p) const {
        return p.x * arr[1] + p.y * arr[5] + p.z * arr[9] + arr[13];
    }

    float dotProdZ(const Vector3<float>& p) const {
        return p.x * arr[2] + p.y * arr[6] + p.z * arr[10] + arr[14];
    }

    float dotProdW(const Vector3<float>& p) const {
        return p.x * arr[3] + p.y * arr[7] + p.z * arr[11] + arr[15];
    }

    Matrix16<T> reorder() {
        Matrix16<T> result;
        for (int i = 0; i < 4; i++) {
            result.arr[i * 4 + 0] = arr[0 + i];
            result.arr[i * 4 + 1] = arr[4 + i];
            result.arr[i * 4 + 2] = arr[8 + i];
            result.arr[i * 4 + 3] = arr[12 + i];
        }
        return result;
    }

    bool worldToScreenOld(Vector3<T> origin, Vector3<T> pos, Vector2<T>& screen, Vector2<T> fov, Vector2<T> displaySize) {
        pos = pos - origin;

        float x = this->dotProdX(pos);
        float y = this->dotProdY(pos);
        float z = this->dotProdZ(pos);

        if (z > 0)
            return false;

        float mX = (float)displaySize.x / 2.0F;
        float mY = (float)displaySize.y / 2.0F;

        screen.x = mX + (mX * x / -z * fov.x);
        screen.y = mY - (mY * y / -z * fov.y);

        return true;
    }

    Vector4<T> operator*(const Vector4<T>& vector) const {
        Vector4<T> result;

        result.x = val.a * vector.x + val.b * vector.y + val.c * vector.z + val.d * vector.w;
        result.y = val.e * vector.x + val.f * vector.y + val.g * vector.z + val.h * vector.w;
        result.z = val.i * vector.x + val.j * vector.y + val.k * vector.z + val.l * vector.w;
        result.w = val.m * vector.x + val.n * vector.y + val.o * vector.z + val.p * vector.w;

        return result;
    }

    Matrix16<T> operator*(const Matrix16<T>& other) const {
        Matrix16<T> result;

        result.val.a = val.a * other.val.a + val.b * other.val.e + val.c * other.val.i + val.d * other.val.m;
        result.val.b = val.a * other.val.b + val.b * other.val.f + val.c * other.val.j + val.d * other.val.n;
        result.val.c = val.a * other.val.c + val.b * other.val.g + val.c * other.val.k + val.d * other.val.o;
        result.val.d = val.a * other.val.d + val.b * other.val.h + val.c * other.val.l + val.d * other.val.p;

        result.val.e = val.e * other.val.a + val.f * other.val.e + val.g * other.val.i + val.h * other.val.m;
        result.val.f = val.e * other.val.b + val.f * other.val.f + val.g * other.val.j + val.h * other.val.n;
        result.val.g = val.e * other.val.c + val.f * other.val.g + val.g * other.val.k + val.h * other.val.o;
        result.val.h = val.e * other.val.d + val.f * other.val.h + val.g * other.val.l + val.h * other.val.p;

        result.val.i = val.i * other.val.a + val.j * other.val.e + val.k * other.val.i + val.l * other.val.m;
        result.val.j = val.i * other.val.b + val.j * other.val.f + val.k * other.val.j + val.l * other.val.n;
        result.val.k = val.i * other.val.c + val.j * other.val.g + val.k * other.val.k + val.l * other.val.o;
        result.val.l = val.i * other.val.d + val.j * other.val.h + val.k * other.val.l + val.l * other.val.p;

        result.val.m = val.m * other.val.a + val.n * other.val.e + val.o * other.val.i + val.p * other.val.m;
        result.val.n = val.m * other.val.b + val.n * other.val.f + val.o * other.val.j + val.p * other.val.n;
        result.val.o = val.m * other.val.c + val.n * other.val.g + val.o * other.val.k + val.p * other.val.o;
        result.val.p = val.m * other.val.d + val.n * other.val.h + val.o * other.val.l + val.p * other.val.p;

        return result;
    }
};
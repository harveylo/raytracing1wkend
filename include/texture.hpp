#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "perlin.hpp"
#include "util.hpp"
#include "vec3.hpp"
#include <memory>

class Texture{
public:
    virtual Color value(double u, double v, const Point3& p) const = 0;
    virtual ~Texture() = default;
};

class SolidColor : public Texture {
private:
    Color color_value;

public:
    SolidColor() {}
    SolidColor(Color c): color_value(c) {}
    SolidColor(double red, double green, double blue): color_value(red,green,blue) {}

    virtual Color value(double u, double v, const Point3& p) const override{
        return color_value;
    }
    virtual ~SolidColor() = default;
};

class CheckerTexture: public Texture{
public:
    std::shared_ptr<Texture> odd;
    std::shared_ptr<Texture> even;

    CheckerTexture(){}
    CheckerTexture(std::shared_ptr<Texture> even, std::shared_ptr<Texture> odd): odd(odd), even(even){}
    CheckerTexture(Color even, Color odd):odd (std::make_shared<SolidColor>(odd)), even(std::make_shared<SolidColor>(even)){}

    virtual Color value(double u, double v, const Point3& p) const override;
    virtual ~CheckerTexture() = default;
};

class NoiseTexture: public Texture{
public:
    Perlin noise;
    double scale;

    NoiseTexture(){}

    virtual Color value(double u, double v, const Point3& p) const override{
        return Color(1,1,1) * noise.noise(p);
    }

    virtual ~NoiseTexture() = default;
};


#endif
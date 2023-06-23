#define STB_IMAGE_IMPLEMENTATION
#include "texture.hpp"
#include <cmath>
#include "util.hpp"


Color CheckerTexture::value(double u, double v, const Point3& p) const{
    auto sines = sin(10*p.x())*sin(10*p.y())*sin(10*p.z());
    if(sines<0) return odd->value(u, v, p);
    else return even->value(u, v, p);
}


ImageTexture::ImageTexture(const char* filename){
    auto componets_per_pixel = bytes_per_pixel;

    data = stbi_load(
        filename, &width,&height,&componets_per_pixel,componets_per_pixel
    );

    if(!data) {
        std::cerr << "\e[1;31m ERROR: Could not load texture image file \e[0m'" << filename << "'.\n";
        width = height = 0;
    }

    // std::cerr<<"Image loaded with width: "<<width<<" height: "<<height<<" size: "<<sizeof(data)<<std::endl;

    bytes_per_scanline = bytes_per_pixel * width;
}


Color ImageTexture::value(double u, double v, const Vec3& p) const{
    // If no image loaded, return solid cyan for debug
    if(data == nullptr) return Color(0,1,1);

    // Clamp input texture coordinate to [0,1] x [1,0]
    u = clamp(u, 0.0, 1.0);
    // Scree coordinate starts from the upleft corner, thus flip the v
    v = 1.0 - clamp(v, 0.0, 1.0); 
    auto i = static_cast<int>(u*width);
    auto j = static_cast<int>(v * height);

    // guard, in case i and  j exceed the bound
    i = i >= width? width-1:i;
    j = j >= height? height-1:j;

    // std::cerr<<"i: "<<i<<" j: "<<j<<std::endl;

    const auto color_scale = 1.0/255.0;
    auto pixel = data + j*bytes_per_scanline + i*bytes_per_pixel;

    return Color(color_scale* pixel[0], color_scale* pixel[1], color_scale * pixel[2]);

}
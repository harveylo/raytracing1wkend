#ifndef __VEC3_H__
#define __VEC3_H__

#include<cmath>
#include<iostream>
#include "util.hpp"


class Vec3{
public:
    double e[3];
    Vec3(): e{0,0,0} {}
    Vec3(double x,double y, double z):e{x,y,z} {}
    double x() const {return e[0];}
    double y() const {return e[1];}
    double z() const {return e[2];}
    
    Vec3 operator-() const {return Vec3(-e[0],-e[1],-e[2]);}
    const double& operator[](int i) const {return e[i];}
    double& operator[](int i) {return e[i];}

    Vec3& operator +=(const Vec3& v){
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    Vec3& operator*=(const double t){
        e[0]*=t;
        e[1]*=t;
        e[2]*=t;
        return *this;
    }
    Vec3& operator/=(const double t){
        return *this *= 1/t;
    }
    bool near_zero(){
        const auto s = 1e-8;
        return ((fabs(e[0])<s)&&(fabs(e[1])<s)&&(fabs(e[2]<s)));
    }
    double length() const{
        return std::sqrt(length_squared());
    }
    double length_squared() const{
        return e[0]*e[0]+e[1]*e[1] + e[2]*e[2];
    }
    Vec3 operator + (const Vec3& v) const{
        return Vec3(e[0]+v.e[0],e[1]+v.e[1],e[2]+v.e[2]);
    }
    Vec3 operator - (const Vec3& v) const{
        return Vec3(e[0]-v.e[0],e[1]-v.e[1],e[2]-v.e[2]);
    }
    Vec3 operator * (const Vec3& v) const{
        return Vec3(e[0]*v.e[0],e[1]*v.e[1],e[2]*v.e[2]);
    }
    Vec3 operator * (const double& t) const{
        return Vec3(e[0]*t,e[1]*t,e[2]*t);
    }
    Vec3 operator / (const double& t) const{
        return *this*(1/t);
    }
    Vec3 reflect(const Vec3& normal) const{
        // Reflect the vector accroding to a normal
        return *this - normal*2*(this->dot(normal));
    }
    Vec3 refract(const Vec3& normal, const double etafrac) const {
        // ! If something goes wrong, check here
        auto cos_theta = fmin((-*this).dot(normal),1.0);
        // auto cos_theta = unit_vector((-*this)).dot(normal);
        auto rv = (*this+(normal*cos_theta))*etafrac;
        auto rp = (normal*(-sqrt(fabs(1.0-rv.length_squared()))));
        return rp+rv;
    }

    double dot(const Vec3& v) const{
        return e[0]*v.e[0]+e[1]*v.e[1]+e[2]*v.e[2];
    }

    Vec3 cross(const Vec3& v) const{
        return Vec3(e[1]*v.e[2]-e[2]*v.e[1],
                    e[2]*v.e[0]-e[0]*v.e[2],
                    e[0]*v.e[1]-e[1]*v.e[0]);
    }

    // Static tool functions
    static Vec3 unit_vector(const Vec3& v) {
        return v/v.length();
    }

    static Vec3 random() {
        return Vec3(random_double(),random_double(),random_double());
    }
    static Vec3 random(double min, double max) {
        return Vec3(random_double(min,max),random_double(min,max),random_double(min,max));
    }

    static Vec3 random_in_unit_sphere(){
        while(true){
            auto tem = random(-1.0,1.0);
            if(tem.length_squared() <= 1) return tem;
        }
    }

    static Vec3 random_unit_vector(){
        return unit_vector(random_in_unit_sphere());
    }

    static Vec3 random_in_hemisphere(const Vec3& normal){
        Vec3 in_unit_sphere = random_in_unit_sphere();
        // In the top hemisphere
        if(normal.dot(in_unit_sphere)>0.0) return in_unit_sphere;
        else return -in_unit_sphere;
    }
    static Vec3 random_in_unit_disk(){
        while(true){
            Vec3 tem(random_double(-1,1),random_double(-1,1),0);
            if(tem.length_squared()<=1) return tem;
        }
    }
    
};


inline std::ostream& operator<<(std::ostream &out, const Vec3& v){
    return out <<"{x="<<v.e[0]<<", y="<<v.e[1]<<", z="<<v.e[2]<<'}';
}
inline Vec3 operator*(const double t, const Vec3& v){
    return v*t;
}

using Point3 = Vec3;
using Color = Vec3;

#endif
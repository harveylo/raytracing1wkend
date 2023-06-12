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

    double dot(const Vec3& v) const{
        return e[0]*v.e[0]+e[1]*v.e[1]+e[2]*v.e[2];
    }

    Vec3 cross(const Vec3& v) const{
        return Vec3(e[1]*v.e[2]-e[2]*v.e[1],
                    e[2]*v.e[0]-e[0]*v.e[2],
                    e[0]*v.e[1]-e[1]*v.e[0]);
    }
    static Vec3 unit_vector(const Vec3& v){
        return v/v.length();
    }

    static Vec3 random(double min, double max){
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
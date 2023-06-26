#ifndef __HITTABLE_H__
#define __HITTABLE_H__

#include "ray.hpp"
#include <memory>
#include "aabb.hpp"
#include "vec3.hpp"


class Material;
class HittableList;
class Texture;

struct HitRecord {
    Point3 p;
    Vec3 normal;
    double t;
    bool front_face;
    std::shared_ptr<Material> mat_ptr;
    double u;
    double v;


    HitRecord():p(),normal(),t(),front_face(){}
    HitRecord(const Point3& p, const Vec3& normal, double t):p(p),normal(normal),t(t){}
    void set_record(const Point3& p, double t){
        this->p = p;
        this->t = t;
    }
    void set_face_normal(const Ray& r, const Vec3& outward_normal){
        front_face = outward_normal.dot(r.direction())<0;
        normal = front_face?outward_normal:-outward_normal;
    }
    void set_material(std::shared_ptr<Material> m){
        mat_ptr = m;
    }
};

class Hittable {
public:
    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const = 0;
    virtual bool bounding_box(double time0, double time1, AABB& output_box) const = 0;
    virtual ~Hittable() = default;
};

class Sphere: public Hittable{
public:
    Point3 center;
    double radius;
    std::shared_ptr<Material> mat_ptr;

    Sphere(){}
    Sphere(Point3 cen, double r):center(cen),radius(r){}
    Sphere(Point3 cen, double r, std::shared_ptr<Material> m):center(cen),radius(r),mat_ptr(m){}
    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;
    virtual bool bounding_box(double time0, double time1, AABB& output_box) const override;
    virtual ~Sphere() = default;
    static void get_sphere_uv(const Point3& p, double& u, double& v);
};


class MovingSphere: public Hittable{
public:
    Point3 scenter, ecenter;
    double radius;
    std::shared_ptr<Material> mat_ptr;
    double stime, etime;

    MovingSphere(){}
    MovingSphere(Point3 scenter, Point3 ecenter,double radius, std::shared_ptr<Material> mat_ptr, double stime, double etime):scenter(scenter),ecenter(ecenter),radius(radius),mat_ptr(mat_ptr),stime(stime),etime(etime){}
    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;
    virtual bool bounding_box(double time0, double time1, AABB& output_box) const override;
    virtual ~MovingSphere() = default;

    Point3 center(double time) const{
        return scenter + ((time - stime)/(etime-stime)*(ecenter - scenter));
    }
};

class XYRect : public Hittable{
public:
    std::shared_ptr<Material> mat_ptr;
    double x0,x1,y0,y1,k;
    Vec3 orientation;

    XYRect(): orientation(0,0,1){}
    XYRect(double x0, double x1, double y0, double y1, double k, std::shared_ptr<Material> mat): mat_ptr(mat),x0(x0), x1(x1), y0(y0), y1(y1),k(k),orientation(0,0,1) {}
    XYRect(double x0, double x1, double y0, double y1, double k, const Vec3& ori, std::shared_ptr<Material> mat): mat_ptr(mat), x0(x0), x1(x1), y0(y0), y1(y1),k(k),orientation(Vec3::unit_vector(Vec3(0,0,1)*ori)) {}

    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;
    virtual bool bounding_box(double time0, double time1, AABB& output_box) const override{
        // Padding z dimension a small amount to avoid infinite thin box
        output_box = AABB(Point3(x0,y0,k-0.0001),Point3(x1,y1,k+0.0001));
        return true;
    }

    virtual ~XYRect() = default;
};


class XZRect : public Hittable{
public:
    std::shared_ptr<Material> mat_ptr;
    double x0,x1,z0,z1,k;
    Vec3 orientation;

    XZRect(): orientation(0,1,0){}
    XZRect(double x0, double x1, double z0, double z1, double k, std::shared_ptr<Material> mat): mat_ptr(mat),x0(x0), x1(x1), z0(z0), z1(z1),k(k),orientation(0,1,0) {}
    XZRect(double x0, double x1, double z0, double z1, double k, const Vec3& ori, std::shared_ptr<Material> mat): mat_ptr(mat), x0(x0), x1(x1), z0(z0), z1(z1),k(k),orientation(Vec3::unit_vector(Vec3(0,0,1)*ori)) {}

    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;
    virtual bool bounding_box(double time0, double time1, AABB& output_box) const override{
        // Padding y dimension a small amount to avoid infinite thin box
        output_box = AABB(Point3(x0,k-0.0001,z0),Point3(x1,k+0.0001,z1));
        return true;
    }

    virtual ~XZRect() = default;
};


class YZRect : public Hittable{
public:
    std::shared_ptr<Material> mat_ptr;
    double y0,y1,z0,z1,k;
    Vec3 orientation;

    YZRect(): orientation(1,0,0){}
    YZRect(double y0, double y1, double z0, double z1, double k, std::shared_ptr<Material> mat): mat_ptr(mat),y0(y0), y1(y1), z0(z0), z1(z1), k(k),orientation(1,0,0) {}
    YZRect(double y0, double y1, double z0, double z1, double k, const Vec3& ori, std::shared_ptr<Material> mat): mat_ptr(mat), y0(y0), y1(y1),z0(z0),z1(z1),k(k),orientation(Vec3::unit_vector(Vec3(0,0,1)*ori)) {}

    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;
    virtual bool bounding_box(double time0, double time1, AABB& output_box) const override{
        // Padding z dimension a small amount to avoid infinite thin box
        output_box = AABB(Point3(k-0.0001,y0,z0),Point3(k+0.0001,y1,z1));
        return true;
    }

    virtual ~YZRect() = default;
};

class Translate : public Hittable{
public:
    std::shared_ptr<Hittable> ptr;
    Vec3 offset;

    Translate(std::shared_ptr<Hittable> p, const Vec3& offset) : ptr(p), offset(offset) {}

    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;
    virtual bool bounding_box(double time0, double time1, AABB& output_box) const override;
    virtual ~Translate() = default;
};

class RotateY : public Hittable {
public:
    std::shared_ptr<Hittable> ptr;
    double sin_theta;
    double cos_theta;
    bool hasbox;
    AABB bbox;

    RotateY(std::shared_ptr<Hittable> p, double angle);
    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;
    virtual bool bounding_box(double time0, double time1, AABB& output_box) const override;

    virtual ~RotateY() = default;
};


#endif
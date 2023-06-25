#ifndef __BOX_H__
#define __BOX_H__

#include "bvh.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "vec3.hpp"
#include <memory>

class Box : public Hittable {
public:
    Point3 box_min;
    Point3 box_max;
    BVHNode root;
    // HittableList root;

    Box(){}
    Box(const Point3& p0, const Point3& p1, std::shared_ptr<Material> ptr);

    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

    virtual bool bounding_box(double time0, double time1, AABB &output_box) const override{
        output_box = AABB(box_min,box_max);
        return true;
    }

    virtual ~Box() = default;
};

#endif
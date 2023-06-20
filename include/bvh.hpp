#ifndef __BVH_H__
#define __BVH_H__

#include "util.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include <iostream>
#include <memory>


class BVHNode : public Hittable{
public:
    std::shared_ptr<Hittable> left;
    std::shared_ptr<Hittable> right;
    AABB box;
    
    BVHNode(){}

    BVHNode(const HittableList& list, double time0, double time1):BVHNode(list.objects, 0, list.objects.size(),time0,time1){}

    BVHNode(
        const std::vector<std::shared_ptr<Hittable>>& src_objects,
        size_t start, size_t end, double time0, double time1 
    );

    virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const override;
    virtual bool bounding_box(double time0, double time1, AABB &output_box) const override;

    
};

inline bool box_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b, int axis){
    AABB box_a;
    AABB box_b;

    // Comparition is not time-sensitive, it's just used to distinguish two objects with certain order
    if(!a->bounding_box(0,0,box_a) || !b->bounding_box(0, 0, box_a))
        std::cerr<< "\e[1;31mNo bounding box in BVHNode constructor [in comparator].\e[0m\n";
    
    return box_a.min()[axis] < box_b.min()[axis];
}

bool box_x_compare(const std::shared_ptr<Hittable> , std::shared_ptr<Hittable>);
bool box_y_compare(const std::shared_ptr<Hittable> , std::shared_ptr<Hittable>);
bool box_z_compare(const std::shared_ptr<Hittable> , std::shared_ptr<Hittable>);

#endif
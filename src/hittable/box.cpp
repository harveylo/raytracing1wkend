#include "box.hpp"
#include "bvh.hpp"
#include "hittable.hpp"
#include <memory>

Box::Box(const Point3& p0, const Point3& p1, std::shared_ptr<Material> ptr){
    box_min = p0;
    box_max = p1;

    HittableList tem;
    tem.add(std::make_shared<XYRect>(p0.x(),p1.x(),p0.y(),p1.y(),p0.z(),ptr));
    tem.add(std::make_shared<XYRect>(p0.x(),p1.x(),p0.y(),p1.y(),p1.z(),ptr));


    tem.add(std::make_shared<YZRect>(p0.y(),p1.y(),p0.z(),p1.z(),p0.x(),ptr));
    tem.add(std::make_shared<YZRect>(p0.y(),p1.y(),p0.z(),p1.z(),p1.x(),ptr));

    tem.add(std::make_shared<XZRect>(p0.x(),p1.x(),p0.z(),p1.z(),p0.y(),ptr));
    tem.add(std::make_shared<XZRect>(p0.x(),p1.x(),p0.z(),p1.z(),p1.y(),ptr));

    root = BVHNode(tem,0,0);
    // root = tem;
}

bool Box::hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const {
    return root.hit(r, t_min, t_max, rec);
}
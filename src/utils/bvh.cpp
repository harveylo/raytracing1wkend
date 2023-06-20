#include "bvh.hpp"
#include "hittable.hpp"
#include "util.hpp"
#include <memory>
#include <vector>

bool BVHNode::bounding_box(double time0, double time1, AABB &output_box) const {
  output_box = box;
  return true;
}

bool box_x_compare(const std::shared_ptr<Hittable> a, std::shared_ptr<Hittable> b){
    return box_compare(a, b, 0);
}
bool box_y_compare(const std::shared_ptr<Hittable> a, std::shared_ptr<Hittable> b){
    return box_compare(a, b, 1);
}
bool box_z_compare(const std::shared_ptr<Hittable> a, std::shared_ptr<Hittable> b){
    return box_compare(a, b, 2);
}

bool BVHNode::hit(const Ray &r, double t_min, double t_max,
                  HitRecord &rec) const {
  if (!box.hit(r, t_min, t_max))
    return false;
  bool hit_left = left->hit(r, t_min, t_max, rec);
  bool hit_right = right->hit(r, t_min, hit_left ? rec.t : t_max, rec);

  return hit_left || hit_right;
}

BVHNode::BVHNode(const std::vector<std::shared_ptr<Hittable>> &src_objects,
                 size_t start, size_t end, double time0, double time1) {
    auto objects = src_objects;
    int axis = random_int_inc(0, 2);

    auto comparator = (axis == 0)?box_x_compare:
                      (axis == 1)?box_y_compare:
                      box_z_compare;
    size_t size = end - start;

    if(size == 1){
        left = right = objects[start];
    }else if (size == 2) {
        if(comparator(objects[start],objects[end-1])){
            left = objects[start];
            right = objects[end-1];
        }else {
            left = objects[end-1];
            right = objects[start];
        }
    }else{
        std::sort(objects.begin()+start, objects.begin()+end, comparator);
        
        auto mid = start + (size>>1);
        left = std::make_shared<Hittable>(BVHNode(src_objects,start,mid,time0,time1));
        right = std::make_shared<Hittable>(BVHNode(src_objects,mid,end,time0,time1));
    }

    AABB box_left, box_right;

    if(!left->bounding_box(time0, time1, box_left) || !right->bounding_box(time0, time1, box_right))
        std::cerr<< "\e[1;31mNo bounding box in BVHNode constructor.\e[0m\n";

    box = AABB::surrounding_box(box_left, box_right);
}

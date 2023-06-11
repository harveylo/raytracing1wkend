#ifndef __HITTABLE_LIST_H__
#define __HITTABLE_LIST_H__

#include "hittable.hpp"
#include <vector>   
#include <memory>

class HittableList: public Hittable{
public:
    std::vector<std::shared_ptr<Hittable>> objects;
    HittableList(){}
    HittableList(std::shared_ptr<Hittable> object){
        add(object);
    }
    // Get the closest object that the ray hits
    HittableList(std::vector<std::shared_ptr<Hittable>> objects):objects(objects){}
    void add(std::shared_ptr<Hittable> object){
        objects.emplace_back(object);
    }
    void clear(){
        objects.clear();
    }
    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;
};


#endif
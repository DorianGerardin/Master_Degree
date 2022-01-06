#ifndef RAY_H
#define RAY_H
#include "Line.h"
class Ray : public Line {
public:
    Ray() : Line() {}
    Ray( Vec3 const & o , Vec3 const & d ) : Line(o,d) {}

    Ray refract(Vec3 point, Vec3 normal, float refractionIndex){

    bool inside = Vec3::dot(normal,direction())>0;
    float eta;
    float cosi;
    float k;

    float sinIlim ;

    Vec3 dir;


    if(inside){
        eta =refractionIndex;
        sinIlim = 1.0 / refractionIndex;

        cosi = Vec3::dot(normal,direction());

        if(acos(cosi) > asin(sinIlim)){
            dir = 2 * Vec3::dot(normal,1*direction())*normal + direction();
            //dir = normal;
        }
        else{
            k = 1 - eta * eta * (1 - cosi * cosi); 
            dir = direction() * eta + normal * (eta *  cosi - sqrt(k)); 
        }



    }else{
        eta = 1/refractionIndex;
        sinIlim = refractionIndex;

        cosi = Vec3::dot(-1*normal,direction());

        if(acos(cosi) > asin(sinIlim)){
            dir = 2 * Vec3::dot(normal,-1*direction())*normal + direction();
            //dir = Vec3(1,1,1);
        }
        else{
            k = 1 - eta * eta * (1 - cosi * cosi); 
            dir = direction() * eta + normal * (eta *  cosi - sqrt(k)); 
        }
    }
    dir.normalize();

    return Ray(point+0.0001*dir,dir);
}
};
#endif

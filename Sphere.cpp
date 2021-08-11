
#include <cmath>
#include "Sphere.h"

using namespace std;



bool Sphere::hit( const Ray &r, double tmin, double tmax, SurfaceHitRecord &rec ) const 
{
    //***********************************************
    //*********** WRITE YOUR CODE HERE **************
    //***********************************************
    //Tranform ray to coordinates of Sphere

    
    Vector3d newVOrigin;
    newVOrigin.x() =  r.origin().x() - center.x();
    newVOrigin.y() =  r.origin().y() - center.y();
    newVOrigin.z() =  r.origin().z() - center.z();
   
    double a = dot(r.direction(), r.direction());
    double b = 2 * dot(r.direction(), newVOrigin);
    double c = dot(newVOrigin, newVOrigin) - (radius * radius);
    double d = b * b - 4 * a * c;
    double t;
    //update SurfaceHItRecord if discrimant is more than or equals to 0
    if(d == 0){
        t = (-b + sqrt(d))/(2 * a);
        if ( t < tmin || t > tmax ) return false;
        rec.t = t;
        rec.p = r.pointAtParam(t);
        rec.normal = (rec.p - center).makeUnitVector();
        rec.mat_ptr = matp;
    } else if(d > 0) {
        double t1 = (-b + sqrt(d))/(2 * a);
        double t2 = (-b - sqrt(d))/(2 * a);
        if(t2 > 0 && (t1 < 0 || t1 > t2)){
            t = t2;
        } else if(t1 > 0 && (t2 < 0 || t2 > t1)){
            t = t1;
        } else {
            return false;
        }
        if ( t < tmin || t > tmax ) return false;
        rec.t = t;
        rec.p = r.pointAtParam(t);
        rec.normal = (rec.p - center).makeUnitVector();
        rec.mat_ptr = matp;
        
    } else{
        return false;
    }
    return true;
}



bool Sphere::shadowHit( const Ray &r, double tmin, double tmax ) const 
{
    //***********************************************
    //*********** WRITE YOUR CODE HERE **************
    //***********************************************
    //Tranform ray to coordinates of Sphere

    Vector3d newVOrigin;
    newVOrigin.x() =  r.origin().x() - center.x();
    newVOrigin.y() =  r.origin().y() - center.y();
    newVOrigin.z() =  r.origin().z() - center.z();
   
    double a = dot(r.direction(), r.direction());
    double b = 2 * dot(r.direction(), newVOrigin);
    double c = dot(newVOrigin, newVOrigin) - (radius * radius);
    double d = b * b - 4 * a * c;
    double t;
      //update SurfaceHItRecord if discrimant is more than or equals to 0
    if(d == 0){
        t = (-b + sqrt(d))/(2 * a);
        if ( t < tmin || t > tmax ) return false;
    } else if(d > 0) {
        double t1 = (-b + sqrt(d))/(2 * a);
        double t2 = (-b - sqrt(d))/(2 * a);
        if(t2 > 0 && (t1 < 0 || t1 > t2)){
            t = t2;
        } else if(t1 > 0 && (t2 < 0 || t2 > t1)){
            t = t1;
        } else {
            return false;
        }
        if ( t < tmin || t > tmax ) return false;
    } else{
        return false;
    }
    return true;
}

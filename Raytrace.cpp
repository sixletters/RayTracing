
#include <cmath>
#include <cfloat>
#include "Vector3d.h"
#include "Color.h"
#include "Ray.h"
#include "Material.h"
#include "Surface.h"
#include "Plane.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Light.h"
#include "Scene.h"
#include "Raytrace.h"

using namespace std;


// This is for avoiding the "epsilon problem" or the shadow acne problem.
#define DEFAULT_TMIN    10e-6       

// Use this for tmax for non-shadow ray intersection test.
#define DEFAULT_TMAX    DBL_MAX



//////////////////////////////////////////////////////////////////////////////
// Compute the outgoing mirror reflection vector.
// Input incoming vector L is pointing AWAY from surface point.
// Assume normal vector N is unit vector.
// The output reflection vector is pointing AWAY from surface point, and
// has same length as incoming vector L.
//////////////////////////////////////////////////////////////////////////////

static Vector3d mirrorReflect( const Vector3d &L, const Vector3d &N )
{
    return ( 2.0 * dot( N, L ) ) * N - L;
}



//////////////////////////////////////////////////////////////////////////////
// Compute I_source * [ k_d * (N.L) + k_r * (R.V)^n ].
// Input vectors L, N and V are pointing AWAY from surface point.
// Assume all vector L, N and V are unit vectors.
//////////////////////////////////////////////////////////////////////////////

static Color computePhongLighting( const Vector3d &L, const Vector3d &N, const Vector3d &V,
                                   const Material &mat, const PointLightSource &ptLight )
{
    Vector3d R = mirrorReflect(L, N);

    float N_dot_L = (float)dot(N, L);
    if (N_dot_L < 0.0f) N_dot_L = 0.0f;

    float R_dot_V = (float)dot(R, V);
    if (R_dot_V < 0.0f) R_dot_V = 0.0f;
    float R_dot_V_pow_n = powf(R_dot_V, (float)mat.n);

    return ptLight.I_source * (mat.k_d * N_dot_L + mat.k_r * R_dot_V_pow_n);
}



//////////////////////////////////////////////////////////////////////////////
// Traces a ray into the scene.
// reflectLevels: specfies number of levels of reflections (0 for no reflection).
// hasShadow: specifies whether to generate shadows.
//////////////////////////////////////////////////////////////////////////////

Color Raytrace::TraceRay( const Ray &ray, const Scene &scene, 
                          int reflectLevels, bool hasShadow )
{
    Ray uRay( ray );
    uRay.makeUnitDirection();  // Normalize ray direction.


// Find whether and where the ray hits some surface. 
// Take the nearest hit point.

    bool hasHitSomething = false;
    double nearest_t = DEFAULT_TMAX;
    SurfaceHitRecord nearestHitRec;

    for ( int i = 0; i < scene.numSurfaces; i++ )
    {
        SurfaceHitRecord tempHitRec;
        bool hasHit = scene.surfacep[i]->hit( uRay, DEFAULT_TMIN, DEFAULT_TMAX, tempHitRec );

        if ( hasHit && tempHitRec.t < nearest_t )
        {
            hasHitSomething = true;
            nearest_t = tempHitRec.t;
            nearestHitRec = tempHitRec;
        }
    }

    if ( !hasHitSomething ) return scene.backgroundColor;

    nearestHitRec.normal.makeUnitVector();
    Vector3d N = nearestHitRec.normal;  // Unit vector.
    Vector3d V = -uRay.direction();     // Unit vector.

    Color result( 0.0f, 0.0f, 0.0f );   // The result will be accumulated here.


    ////////////////////////////////////
    ////////////////////////////////////


// Add to result the phong lighting contributed by each point light source.
// Compute for shadow if hasShadow is true.

    //***********************************************
    //*********** WRITE YOUR CODE HERE **************
    //***********************************************
    
    if(hasShadow){
        for(int i = 0;i < scene.numPtLights; i++){
            bool hitChecker = false;
            Vector3d L = scene.ptLight[i].position - nearestHitRec.p;
            double Tmax  = L.length()/(L.makeUnitVector().length());
            L = L.makeUnitVector();
            for(int k = 0; k < scene.numSurfaces;k++){
                hitChecker = scene.surfacep[k]->shadowHit(Ray(nearestHitRec.p, L), DEFAULT_TMIN, Tmax);
                if(hitChecker){
                break;
                }
            }
            if(!hitChecker){
                result += computePhongLighting(L, N, V.makeUnitVector(), *nearestHitRec.mat_ptr, scene.ptLight[i]);
            }
        }
    }
    else{
        for(int i = 0; i < scene.numPtLights; i++){
            Vector3d L = scene.ptLight[i].position - nearestHitRec.p;
            L = L.makeUnitVector();
            result += computePhongLighting(L, N, V, *nearestHitRec.mat_ptr, scene.ptLight[i]);
        }
    }

    // Add to result the global ambient lighting.

    //***********************************************
    //*********** WRITE YOUR CODE HERE **************
    //***********************************************
   result += nearestHitRec.mat_ptr->k_a * scene.amLight.I_a;



    // Add to result the reflection of the scene.

    //***********************************************
    //*********** WRITE YOUR CODE HERE **************
    //***********************************************
    if(reflectLevels != 0){
    Vector3d reflectedRay = mirrorReflect(V, N);
    result += TraceRay(Ray(nearestHitRec.p, reflectedRay.makeUnitVector()), scene, reflectLevels - 1, hasShadow) * nearestHitRec.mat_ptr->k_rg;
    }



    return result;
}

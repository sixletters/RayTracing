
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include "Util.h"
#include "Vector3d.h"
#include "Color.h"
#include "Image.h"
#include "Ray.h"
#include "Camera.h"
#include "Material.h"
#include "Light.h"
#include "Surface.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "Scene.h"
#include "Raytrace.h"
#include <iostream>
#include <fstream>
#include <vector>


using namespace std;

// Constants for Scene 1.
static const int imageWidth1 = 640;
static const int imageHeight1 = 480;
static const int reflectLevels1 = 2;  // 0 -- object does not reflect scene.
static const int hasShadow1 = true;
static const char outImageFile1[] = "out1.png";

// Constants for Scene 2.
static const int imageWidth2 = 640;
static const int imageHeight2 = 480;
static const int reflectLevels2 = 2;  // 0 -- object does not reflect scene.
static const int hasShadow2 = true;
static const char outImageFile2[] = "out2.png";


// vertex definition for 3D objects
struct vertex{
       double x;
       double y;
       double z;
  };

// face definition for 3D objectts
struct face{
      unsigned int v1,v2,v3;
};

//obj class
class Obj
{
public:
   std::vector<vertex> vertexes;
   std::vector<face> faces;
    
  void readfile(const char* filename);
  void draw(Scene &scene);
};

//method to read in Obj files
void Obj::readfile(const char *filename)
{
   string s;
   ifstream fin(filename);
   if(!fin)
         return;
   while(fin>>s)
   {
         switch(*s.c_str())
         {
         case 'v':
              {
                    vertex v;
                    fin>>v.x>>v.y>>v.z;
                    this->vertexes.push_back(v);
              }
              break;
         case 'f':
              {
                    face f;
                    fin>>f.v1>>f.v2>>f.v3;
               
                    faces.push_back(f);
              }
              break;
         }
   }
}


///////////////////////////////////////////////////////////////////////////
// Raytrace the whole image of the scene and write it to a file.
///////////////////////////////////////////////////////////////////////////

void RenderImage( const char *imageFilename, const Scene &scene, int reflectLevels, bool hasShadow )
{
    int imgWidth = scene.camera.getImageWidth();
    int imgHeight = scene.camera.getImageHeight();
    Image image( imgWidth, imgHeight ); // To store the result of ray tracing.

    double startTime = Util::GetCurrRealTime();
    double startCPUTime = Util::GetCurrCPUTime();

    // Generate image.
    for ( int y = 0; y < imgHeight; y++ )
    {
        double pixelPosY = y + 0.5;

        for ( int x = 0; x < imgWidth; x++ )
        {
            double pixelPosX = x + 0.5;
            Ray ray = scene.camera.getRay( pixelPosX, pixelPosY );
            Color pixelColor = Raytrace::TraceRay( ray, scene, reflectLevels, hasShadow );
            pixelColor.clamp();
            image.setPixel( x, y, pixelColor );
        }
        // printf( "%d ", y );
    }

    double stopCPUTime = Util::GetCurrCPUTime();
    double stopTime = Util::GetCurrRealTime();
    printf( "CPU time taken = %.1f sec\n", stopTime - startTime ); 
    printf( "Real time taken = %.1f sec\n", stopTime - startTime ); 

    // Write image to file.
    image.writeToFile( imageFilename );
}



// Forward declarations. These functions are defined later in the file.
void DefineScene1( Scene &scene, int imageWidth, int imageHeight );
void DefineScene2( Scene &scene, int imageWidth, int imageHeight );



void WaitForEnterKeyBeforeExit( void )
{
    fflush( stdin );
    getchar();
}



int main()
{
    atexit( WaitForEnterKeyBeforeExit );


// Define Scene 1.

    Scene scene1;
    DefineScene1( scene1, imageWidth1, imageHeight1 );

// Render Scene 1.

    printf( "Render Scene 1...\n" );
    RenderImage( outImageFile1, scene1, reflectLevels1, hasShadow1 );
    printf( "Image completed.\n" );



// Define Scene 2.

    Scene scene2;
    DefineScene2( scene2, imageWidth2, imageHeight2 );

// Render Scene 2.
 
    printf( "Render Scene 2...\n" );
    RenderImage( outImageFile2, scene2, reflectLevels2, hasShadow2 );
    printf( "Image completed.\n" );


    printf( "All done.\n" );
    return 0;
}



///////////////////////////////////////////////////////////////////////////
// Modeling of Scene 1.
///////////////////////////////////////////////////////////////////////////

void DefineScene1( Scene &scene, int imageWidth, int imageHeight )
{
       scene.backgroundColor = Color( 0.2f, 0.3f, 0.5f );

         scene.amLight.I_a = Color( 1.0f, 1.0f, 1.0f ) * 0.25f;

     // Define materials.

         scene.numMaterials = 5;
         scene.material = new Material[ scene.numMaterials ];

         // Light red.
         scene.material[0].k_d = Color( 0.8f, 0.4f, 0.4f );
         scene.material[0].k_a = scene.material[0].k_d;
         scene.material[0].k_r = Color( 0.8f, 0.8f, 0.8f ) / 1.5f;
         scene.material[0].k_rg = Color( 0.8f, 0.8f, 0.8f ) / 3.0f;
         scene.material[0].n = 64.0f;

         // Light green.
         scene.material[1].k_d = Color( 0.4f, 0.8f, 0.4f );
         scene.material[1].k_a = scene.material[0].k_d;
         scene.material[1].k_r = Color( 0.8f, 0.8f, 0.8f ) / 1.5f;
         scene.material[1].k_rg = Color( 0.8f, 0.8f, 0.8f ) / 3.0f;
         scene.material[1].n = 64.0f;

         // Light blue.
         scene.material[2].k_d = Color( 0.4f, 0.4f, 0.8f ) * 0.9f;
         scene.material[2].k_a = scene.material[0].k_d;
         scene.material[2].k_r = Color( 0.8f, 0.8f, 0.8f ) / 1.5f;
         scene.material[2].k_rg = Color( 0.8f, 0.8f, 0.8f ) / 2.5f;
         scene.material[2].n = 64.0f;

         // Yellow.
         scene.material[3].k_d = Color( 0.6f, 0.6f, 0.2f );
         scene.material[3].k_a = scene.material[0].k_d;
         scene.material[3].k_r = Color( 0.8f, 0.8f, 0.8f ) / 1.5f;
         scene.material[3].k_rg = Color( 0.8f, 0.8f, 0.8f ) / 3.0f;
         scene.material[3].n = 64.0f;

         // Gray.
         scene.material[4].k_d = Color( 0.6f, 0.6f, 0.6f );
         scene.material[4].k_a = scene.material[0].k_d;
         scene.material[4].k_r = Color( 0.6f, 0.6f, 0.6f );
         scene.material[4].k_rg = Color( 0.8f, 0.8f, 0.8f ) / 3.0f;
         scene.material[4].n = 128.0f;


     // Define point light sources.

         scene.numPtLights = 2;
         scene.ptLight = new PointLightSource[ scene.numPtLights ];

         scene.ptLight[0].I_source = Color( 1.0f, 1.0f, 1.0f ) * 0.6f;
         scene.ptLight[0].position = Vector3d( 100.0, 120.0, 10.0 );

         scene.ptLight[1].I_source = Color( 1.0f, 1.0f, 1.0f ) * 0.6f;
         scene.ptLight[1].position = Vector3d( 5.0, 80.0, 60.0 );


     // Define surface primitives.

         scene.numSurfaces = 15;
         scene.surfacep = new SurfacePtr[ scene.numSurfaces ];

         scene.surfacep[0] = new Plane( 0.0, 1.0, 0.0, 0.0, &(scene.material[2]) ); // Horizontal plane.
         scene.surfacep[1] = new Plane( 1.0, 0.0, 0.0, 0.0, &(scene.material[4]) ); // Left vertical plane.
         scene.surfacep[2] = new Plane( 0.0, 0.0, 1.0, 0.0, &(scene.material[4]) ); // Right vertical plane.
         scene.surfacep[3] = new Sphere( Vector3d( 40.0, 20.0, 42.0 ), 22.0, &(scene.material[0]) ); // Big sphere.
         scene.surfacep[4] = new Sphere( Vector3d( 75.0, 10.0, 40.0 ), 12.0, &(scene.material[1]) ); // Small sphere.

         // Cube +y face.
         scene.surfacep[5] = new Triangle( Vector3d( 50.0, 20.0, 90.0 ), Vector3d( 50.0, 20.0, 70.0 ),
                                           Vector3d( 30.0, 20.0, 70.0 ), &(scene.material[3]) );
         scene.surfacep[6] = new Triangle( Vector3d( 50.0, 20.0, 90.0 ), Vector3d( 30.0, 20.0, 70.0 ),
                                           Vector3d( 30.0, 20.0, 90.0 ), &(scene.material[3]) );

         // Cube +x face.
         scene.surfacep[7] = new Triangle( Vector3d( 50.0, 0.0, 70.0 ), Vector3d( 50.0, 20.0, 70.0 ),
                                           Vector3d( 50.0, 20.0, 90.0 ), &(scene.material[3]) );
         scene.surfacep[8] = new Triangle( Vector3d( 50.0, 0.0, 70.0 ), Vector3d( 50.0, 20.0, 90.0 ),
                                           Vector3d( 50.0, 0.0, 90.0 ), &(scene.material[3]) );

         // Cube -x face.
         scene.surfacep[9] = new Triangle( Vector3d( 30.0, 0.0, 90.0 ), Vector3d( 30.0, 20.0, 90.0 ),
                                           Vector3d( 30.0, 20.0, 70.0 ), &(scene.material[3]) );
         scene.surfacep[10] = new Triangle( Vector3d( 30.0, 0.0, 90.0 ), Vector3d( 30.0, 20.0, 70.0 ),
                                           Vector3d( 30.0, 0.0, 70.0 ), &(scene.material[3]) );

         // Cube +z face.
         scene.surfacep[11] = new Triangle( Vector3d( 50.0, 0.0, 90.0 ), Vector3d( 50.0, 20.0, 90.0 ),
                                           Vector3d( 30.0, 20.0, 90.0 ), &(scene.material[3]) );
         scene.surfacep[12] = new Triangle( Vector3d( 50.0, 0.0, 90.0 ), Vector3d( 30.0, 20.0, 90.0 ),
                                           Vector3d( 30.0, 0.0, 90.0 ), &(scene.material[3]) );

         // Cube -z face.
         scene.surfacep[13] = new Triangle( Vector3d( 30.0, 0.0, 70.0 ), Vector3d( 30.0, 20.0, 70.0 ),
                                           Vector3d( 50.0, 20.0, 70.0 ), &(scene.material[3]) );
         scene.surfacep[14] = new Triangle( Vector3d( 30.0, 0.0, 70.0 ), Vector3d( 50.0, 20.0, 70.0 ),
                                           Vector3d( 50.0, 0.0, 70.0 ), &(scene.material[3]) );
     
     
     
         


     // Define camera.

         scene.camera = Camera( Vector3d( 150.0, 120.0, 150.0 ), Vector3d( 45.0, 22.0, 55.0 ), Vector3d( 0.0, 1.0, 0.0 ),
                        (-1.0 * imageWidth) / imageHeight, (1.0 * imageWidth) / imageHeight, -1.0, 1.0, 3.0,
                        imageWidth, imageHeight );
}



///////////////////////////////////////////////////////////////////////////
// Modeling of Scene 2.
///////////////////////////////////////////////////////////////////////////

void DefineScene2( Scene &scene, int imageWidth, int imageHeight )
{
    //***********************************************
    //*********** WRITE YOUR CODE HERE **************
    //***********************************************
    
    
        scene.backgroundColor = Color( 0.5f, 0.5f, 0.9f );
        scene.amLight.I_a = Color( 0.5f, 0.5f, 1.0f ) * 0.25f;
    
        //Intilize 3D Objects
        Obj teddy;
        //Obj Cow;
        Obj teaPot;
        teddy.readfile("Teddy.obj");
       // Cow.readfile("Cow.obj");
        teaPot.readfile("Teapot.obj");
    
        // Define materials.

        scene.numMaterials = 9;
        scene.material = new Material[ scene.numMaterials ];

        // Light red.
        scene.material[0].k_d = Color( 0.8f, 0.4f, 0.4f );
        scene.material[0].k_a = scene.material[0].k_d;
        scene.material[0].k_r = Color( 0.8f, 0.8f, 0.8f ) / 1.5f;
        scene.material[0].k_rg = Color( 0.8f, 0.8f, 0.8f ) / 3.0f;
        scene.material[0].n = 64.0f;

        // Light green.
        scene.material[1].k_d = Color( 0.4f, 0.8f, 0.4f );
        scene.material[1].k_a = scene.material[0].k_d;
        scene.material[1].k_r = Color( 0.8f, 0.8f, 0.8f ) / 1.5f;
        scene.material[1].k_rg = Color( 0.8f, 0.8f, 0.8f ) / 3.0f;
        scene.material[1].n = 64.0f;

        // Light blue.
        scene.material[2].k_d = Color( 0.4f, 0.4f, 0.8f ) * 0.9f;
        scene.material[2].k_a = scene.material[0].k_d;
        scene.material[2].k_r = Color( 0.8f, 0.8f, 0.8f ) / 1.5f;
        scene.material[2].k_rg = Color( 0.8f, 0.8f, 0.8f ) / 2.5f;
        scene.material[2].n = 64.0f;

        // Yellow.
        scene.material[3].k_d = Color( 0.6f, 0.6f, 0.2f );
        scene.material[3].k_a = scene.material[0].k_d;
        scene.material[3].k_r = Color( 0.8f, 0.8f, 0.8f ) / 1.5f;
        scene.material[3].k_rg = Color( 0.8f, 0.8f, 0.8f ) / 3.0f;
        scene.material[3].n = 64.0f;
    
        // dull yellow
        scene.material[8].k_d = Color( 0.6f, 0.6f, 0.2f );
        scene.material[8].k_a = scene.material[0].k_d;
        scene.material[8].k_r = Color( 0.8f, 0.8f, 0.8f ) / 1.5f;
        scene.material[8].k_rg = Color( 0.8f, 0.8f, 0.8f ) / 3.0f;
        scene.material[8].n = 128.0f;

        // Gray.
        scene.material[4].k_d = Color( 0.3f, 0.3f, 0.6f );
        scene.material[4].k_a = scene.material[0].k_d;
        scene.material[4].k_r = Color( 0.3f, 0.3f, 0.6f );
        scene.material[4].k_rg = Color( 0.3f, 0.3f, 0.7f ) / 3.0f;
        scene.material[4].n = 10.0f;
        
        //white-ish
        scene.material[5].k_d = Color( 0.9f, 0.9f, 0.9f );
        scene.material[5].k_a = scene.material[0].k_d;
        scene.material[5].k_r = Color( 0.9f, 0.9f, 0.9f );
        scene.material[5].k_rg = Color( 0.9f, 0.9f, 0.9f ) / 3.0f;
        scene.material[5].n = 10.0f;
    
        //black-ish
        scene.material[6].k_d = Color( 0.1f, 0.1f, 0.1f );
        scene.material[6].k_a = scene.material[0].k_d;
        scene.material[6].k_r = Color( 0.1f, 0.1f, 0.1f );
        scene.material[6].k_rg = Color( 0.1f, 0.1f, 0.1f ) / 3.0f;
        scene.material[6].n = 10.0f;
        
    
        //Pink
        scene.material[7].k_d = Color( 1.0f, 0.4f, 0.7f );
        scene.material[7].k_a = scene.material[0].k_d;
        scene.material[7].k_r = Color( 1.0f, 0.4f, 0.7f  );
        scene.material[7].k_rg = Color(1.0f, 0.4f, 0.7f  ) / 3.0f;
        scene.material[7].n = 120.0f;


    // Define point light sources.

        scene.numPtLights = 2;
        scene.ptLight = new PointLightSource[ scene.numPtLights ];

        scene.ptLight[0].I_source = Color( 1.0f, 1.0f, 1.0f ) * 0.6f;
        scene.ptLight[0].position = Vector3d( 100.0, 120.0, 30.0 );

        scene.ptLight[1].I_source = Color( 1.0f, 1.0f, 1.0f ) * 0.6f;
        scene.ptLight[1].position = Vector3d( 15.0, 80.0, 60.0 );


        // Define surface primitives.

        scene.numSurfaces = 19 ;
        scene.surfacep = new SurfacePtr[ teddy.faces.size() +teaPot.faces.size() + scene.numSurfaces ];
    
        //define the room
    
        scene.surfacep[0] = new Plane( 0.0, 1.0, 0.0, 0.0, &(scene.material[2]) ); // Horizontal plane.
        scene.surfacep[1] = new Plane(  1.0, 0.0, 0.0, 10.0, &(scene.material[4]) ); // Left vertical plane.
        scene.surfacep[2] = new Plane( 0.0, 0.0, 1.0, 0.0, &(scene.material[4]) ); // Right vertical plane.

        // Cube
        scene.surfacep[3] = new Triangle( Vector3d( 50.0, 20.0, 80 ), Vector3d( 50.0, 20.0, 60 ),
                                          Vector3d( 30.0, 20.0, 60 ), &(scene.material[1]) );
        scene.surfacep[4] = new Triangle( Vector3d( 50.0, 20.0,  80 ), Vector3d( 30.0, 20.0, 60 ),
                                          Vector3d( 30.0, 20.0,  80 ), &(scene.material[1]) );
        scene.surfacep[5] = new Triangle( Vector3d( 50.0, 0.0, 60 ), Vector3d( 50.0, 20.0, 60 ),
                                          Vector3d( 50.0, 20.0,  80 ), &(scene.material[1]) );
        scene.surfacep[6] = new Triangle( Vector3d( 50.0, 0.0, 60 ), Vector3d( 50.0, 20.0,  80 ),
                                          Vector3d( 50.0, 0.0,  80 ), &(scene.material[1]) );
        scene.surfacep[7] = new Triangle( Vector3d( 30.0, 0.0,  80 ), Vector3d( 30.0, 20.0,  80 ),
                                          Vector3d( 30.0, 20.0, 60 ), &(scene.material[1]) );
        scene.surfacep[8] = new Triangle( Vector3d( 30.0, 0.0,  80 ), Vector3d( 30.0, 20.0, 60 ),
                                          Vector3d( 30.0, 0.0, 60 ), &(scene.material[1]) );
        scene.surfacep[9] = new Triangle( Vector3d( 50.0, 0.0,  80 ), Vector3d( 50.0, 20.0,  80 ),
                                          Vector3d( 30.0, 20.0,  80 ), &(scene.material[3]) );
        scene.surfacep[10] = new Triangle( Vector3d( 50.0, 0.0,  80 ), Vector3d( 30.0, 20.0,  80 ),
                                          Vector3d( 30.0, 0.0,  80 ), &(scene.material[1]) );
        scene.surfacep[11] = new Triangle( Vector3d( 30.0, 0.0, 60 ), Vector3d( 30.0, 20.0, 60 ),
                                          Vector3d( 50.0, 20.0, 60 ), &(scene.material[1]) );
        scene.surfacep[12] = new Triangle( Vector3d( 30.0, 0.0, 60 ), Vector3d( 50.0, 20.0, 60 ),
                                          Vector3d( 50.0, 0.0, 60 ), &(scene.material[1]) );
    
        // teddy bear eyes
        scene.surfacep[13] = new Sphere( Vector3d( 27.5 , 30.0, 15 ), 1.0, &(scene.material[6]) );
        scene.surfacep[14] = new Sphere( Vector3d( 32.5, 30.0, 15 ), 1.0, &(scene.material[6]) );
    
        // Hovering spheres
        scene.surfacep[15] = new Sphere( Vector3d( 20 , 35, 70 ), 3.0, &(scene.material[0]) );
        scene.surfacep[16] = new Sphere( Vector3d( 60, 35, 70 ), 3.0, &(scene.material[7]) );
        scene.surfacep[17] = new Sphere( Vector3d( 40 , 35, 50 ), 3.0, &(scene.material[2]) );
        scene.surfacep[18] = new Sphere( Vector3d( 40, 35, 90 ), 3.0, &(scene.material[3]) );
    
        
        //Teddy bear using triangles
       for(int i=0;i<teddy.faces.size();i++)
       {
          vertex v1 = teddy.vertexes[teddy.faces[i].v1 - 1];
          vertex v2 =  teddy.vertexes[teddy.faces[i].v2 - 1];
          vertex v3 =  teddy.vertexes[teddy.faces[i].v3 - 1];

          scene.surfacep[ i + 19 ] = new Triangle( Vector3d( v1.x+ 30.0, v1.y + 20.0, v1.z + 6), Vector3d( v2.x+ 30.0, v2.y + 20.0, v2.z + 6 ), Vector3d( v3.x + 30.0, v3.y+  20.0, v3.z + 6), &(scene.material[8]) );
           scene.numSurfaces++;
           
       }
    
        
        //teap pot with triangles
       for(int i=0;i<teaPot.faces.size();i++)
        {
           vertex v1 = teaPot.vertexes[teaPot.faces[i].v1 - 1];
           vertex v2 =  teaPot.vertexes[teaPot.faces[i].v2 - 1];
           vertex v3 =  teaPot.vertexes[teaPot.faces[i].v3 - 1];

           scene.surfacep[ i + 19 + teddy.faces.size()] = new Triangle( Vector3d( 4*v1.x + 40, 4*v1.y + 21 , 4*v1.z + 70), Vector3d( 4*v2.x + 40, 4*v2.y + 21, 4*v2.z + 70), Vector3d( 4*v3.x + 40, 4*v3.y + 21, 4*v3.z + 70), &(scene.material[5]) );
            scene.numSurfaces++;
            
        }
         

     

    // Define camera.
        scene.camera = Camera( Vector3d( 130.0, 50.0, 130 ), Vector3d( 45.0, 22.0, 55.0 ), Vector3d( 0.0, 1.0, 0.0 ),
                       (-1.0 * imageWidth) / imageHeight, (1.0 * imageWidth) / imageHeight, -1.0, 1.0, 3.0,
                       imageWidth, imageHeight );
    
}


#ifndef CAMERAH
#define CAMERAH

#include "algebra3.h"
#include "Ray.h"
#include "Sampler.h"
#include "randomc.h"
#include <vector>

/* Camera objects store information about the viewer and
   the viewing plane. They also produce viewing rays. */
class Camera {

protected:

	/* Instance vars */
	double rayStart;					// (Parametrically) where rays start considering intersections
	vec3 u, v, w;						// Local coordinate frame
	vec3 viewplaneLL;					// Lower left viewplane corner.
	vec3 viewplaneLR;					// Lower right viewplane corner.
	vec3 viewplaneUL;					// Upper left viewplane corner.	
	vec3 viewplaneUR;					// Upper right viewplane corner.

public:

	/* Constructors */
    Camera();
	Camera(const vec3& ul, const vec3& ll, const vec3& ur, const vec3& lr);
	Camera(const vec3& lookFrom, const vec3& lookAt, const vec3& up, double fovx, double fovy, double rayStart);

    virtual ~Camera();
    
	/* Instance methods */
	virtual Ray createViewingRay(const Sample& normSamp) = 0;
};

class PinholeCamera: public Camera {
    public:
        PinholeCamera(const vec3& viewPos, const vec3& ul, const vec3& ll, const vec3& ur, const vec3& lr);
		PinholeCamera(const vec3& lookFrom, const vec3& lookAt, const vec3& up, double fovx, double fovy, double rayStart);
        
        Ray createViewingRay(const Sample& normSamp);

	private:
		vec3 viewerPos;
};

class LensCamera: public Camera {
    public:
        LensCamera(const vec3& point, const vec3& edge1, const vec3& edge2,
                   const vec3& ul, const vec3& ll, const vec3& ur, const vec3& lr,
                   int sampleRate);
		LensCamera(const vec3& lookFrom, const vec3& lookAt, const vec3& up, double fovx, double fovy, double rayStart,
			double lenssize, int sampleRate);
        Ray createViewingRay(const Sample& normSamp);
    private:
        vec3 position;
        vec3 edge1;
        vec3 edge2;
        vector< vector<vec3> > jitteredPoints;
        CRandomMersenne* randGen;

		void initPointGrid(int sampleRate);
};

#endif
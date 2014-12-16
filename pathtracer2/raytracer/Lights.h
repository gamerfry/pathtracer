/*
 *  Light.h
 *  Classes relating to all the different lights. 
 *
 *  Created by Lita Cho on 9/22/08.
 * 
 */

#ifndef LIGHTSH
#define LIGHTSH

#include "Ray.h"
#include "rgb.h"
#include "algebra3.h"
#include "randomc.h"
#include <vector>

using namespace std;

/*****************************************************************************/
/*                      Abstract Light Class								 */
/******************* **********************************************************/
/* General abstract light class used to dervie all other lights for the ray
 * tracer.
 */
class Light {
    public:
        // Default Constructor
        Light();
        
        // Default Destructor
        virtual ~Light();
        
        // Constructs a light given the position and the color.
        // Parameters: 
        //      x, y, z - Position of the light.
		//		*fall - const, linear, quadratic falloff terms
        Light(const rgb color, double cfall=1.0, double lfall=0.0, double qfall=0.0);
        
        // Returns the color of the light.
        virtual rgb getIntensity(const vec3& refPoint);

		// Gets the shadow ray from STARTPOINT to this light object given a
		// certain shadow BIAS.
		virtual Ray getShadowRay(const vec3& startPoint, double bias, Ray& viewRay) = 0;

		// For pathtracing. Implemented only for area lights.
		virtual void sample(const vec3& fromPoint, const vec3& fromNormal, rgb& intensity, vec3& incidence);
    
    protected:
        rgb illumination;
		double falloff[3];		// const, linear quadratic
};

/*****************************************************************************/
/*                          Point Light Class                                */
/*****************************************************************************/
class PointLight: public Light {
    // Constructs a point light given the position and the color.
    // Parameters: 
    //      x, y, z - Position of the light.
    //      color - The RGB values regarding the color of the light
    public:
		PointLight(double x, double y, double z, const rgb color, double cfall=1.0, double lfall=0.0, double qfall=0.0);
		Ray getShadowRay(const vec3& startPoint, double bias, Ray& viewRay);
		rgb getIntensity(const vec3& refPoint);

	private:
		vec3 position;
		vec3 getIncidence(const vec3 &point);
};

/*****************************************************************************/
/*                     Directional Light Class                               */
/*****************************************************************************/
class DirectionalLight: public Light {
    // Constructs a point light given the position and the color.
    // Parameters: 
    //      x, y, z - Position of the light.
    //      color - The RGB values regarding the color of the light
    public:
		DirectionalLight(double x, double y, double z, const rgb color);
		Ray getShadowRay(const vec3& startPoint, double bias, Ray& viewRay);

	private:
		vec3 direction;
		vec3 getIncidence(const vec3 &point);
};

/*****************************************************************************/
/*						  Area Light Class                                   */
/*****************************************************************************/

class AreaLight: public Light {

public:
	
	AreaLight(const vec3& position, const vec3& edge1, const vec3& edge2, const rgb color, int sampleRate, 
		double cfall=1.0, double lfall=0.0, double qfall=0.0);
	~AreaLight();
	Ray getShadowRay(const vec3& startPoint, double bias, Ray& viewRay);
	rgb getIntensity(const vec3& refPoint);

	// For pathtracing
	virtual void sample(const vec3& fromPoint, const vec3& fromNormal, rgb& intensity, vec3& incidence);

protected:

	vec3 position;
	vec3 edge1;
	vec3 edge2;
	vec3 lightNormal;
	double area;
	CRandomMersenne* randGen;
	vec3 lastSampledPoint;
	vector< vector<vec3> > jitteredPoints;
	vec3 getIncidence(const vec3 &point, Ray& viewRay);

};


/*****************************************************************************/
/*					   Textured Area Light Class                             */
/*****************************************************************************/

// Forward declaration
class Texture;

class TexAreaLight: public AreaLight {

public:
	
	TexAreaLight(const vec3& position, const vec3& edge1, const vec3& edge2, const rgb color, int sampleRate, 
		Texture* t, double cfall=1.0, double lfall=0.0, double qfall=0.0);

	// For pathtracing
	void sample(const vec3& fromPoint, const vec3& fromNormal, rgb& intensity, vec3& incidence);

private:

	Texture* tex;

};



#endif
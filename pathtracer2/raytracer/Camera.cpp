#include "Camera.h"
#include <cfloat>
#include <time.h>


/* Constructors */
Camera::Camera(){}

Camera::Camera(const vec3& ul, const vec3& ll,
			   const vec3& ur, const vec3& lr) {

	viewplaneUL = ul;
	viewplaneLL = ll;
	viewplaneUR = ur;
	viewplaneLR = lr;
	rayStart = 0;

}

Camera::Camera(const vec3& lookFrom, const vec3& lookAt, const vec3& up, double fovx, double fovy, double rayStart)
{
	// Construct local coordinate frame
	double focaldist;
	w = lookFrom - lookAt;
	focaldist = w.length();
	w.normalize();
	u = up ^ w;
	u.normalize();
	v = w ^ u;
	v.normalize();	// Not really necessary, but what the hell.

	// Build the image plane
	double tanfx = tan(fovx/2 * M_PI/180)*focaldist;
	double tanfy = tan(fovy/2 * M_PI/180)*focaldist;
	viewplaneUL = lookFrom - focaldist*w + tanfy*v - tanfx*u;
	viewplaneLL = lookFrom - focaldist*w - tanfy*v - tanfx*u;
	viewplaneUR = lookFrom - focaldist*w + tanfy*v + tanfx*u;
	viewplaneLR = lookFrom - focaldist*w - tanfy*v + tanfx*u;

	this->rayStart = rayStart;
}

/* Destructor */
Camera::~Camera() {}


//////////////////////////
// Pinhole Camera       //
//////////////////////////

/* Constructors */
PinholeCamera::PinholeCamera(const vec3& viewPos, const vec3& ul, const vec3& ll, const vec3& ur, const vec3& lr)
: Camera(ul, ll, ur, lr) 
{
    viewerPos = viewPos;
}

PinholeCamera::PinholeCamera(const vec3& lookFrom, const vec3& lookAt, const vec3& up, double fovx, double fovy, double rayStart)
: Camera(lookFrom, lookAt, up, fovx, fovy, rayStart)
{
	viewerPos = lookFrom;
}


/* Instance methods */

// Requires that normSamp be normalized.
Ray PinholeCamera::createViewingRay(const Sample& normSamp) {

	double u = normSamp.horiz;
	double v = normSamp.vert;

	vec3 leftVertInterp = v*viewplaneUL + (1-v)*viewplaneLL;
	vec3 rightVertInterp = v*viewplaneUR + (1-v)*viewplaneLR;
	vec3 rayEnd = u*rightVertInterp + (1-u)*leftVertInterp;

	return Ray(viewerPos, rayEnd, rayStart, DBL_MAX, normSamp, NULL);
}

//////////////////////////
// Lens Camera          //
//////////////////////////

/* Constructors */
LensCamera::LensCamera(const vec3& postion, const vec3& edge1, const vec3& edge2,
                       const vec3& ul, const vec3& ll, const vec3& ur, const vec3& lr, 
                       int sampleRate):
    Camera(ul, ll, ur, lr) {
    this->position = postion;
    this->edge1 = edge1;
    this->edge2 = edge2;

	randGen = new CRandomMersenne(2);

	rayStart = 0;

	//initPointGrid(sampleRate);
}

LensCamera::LensCamera(const vec3& lookFrom, const vec3& lookAt, const vec3& up, double fovx, double fovy, double rayStart,
					   double lenssize, int sampleRate)
: Camera(lookFrom, lookAt, up, fovx, fovy, rayStart)
{
	randGen = new CRandomMersenne(2);
	edge1 = lenssize*v;
	edge2 = lenssize*u;
	position = lookFrom - 0.5*edge1 - 0.5*edge2;

	//initPointGrid(sampleRate);
	
}

void LensCamera::initPointGrid(int sampleRate)
{
	// Initialize point grid.
	for (int i = 0; i < sampleRate; i++) {
		jitteredPoints.push_back(vector<vec3>());
	}

	// Generate jittered points.
	double stepSize = 1.0 / sampleRate;
	vector<vec3> shuffledPoints;
	for (int i = 0; i < sampleRate; i++) {
		for (int j = 0; j < sampleRate; j++) {
			vec3 point = position + (i*stepSize + randGen->Random()*stepSize)*edge1 +
				(j*stepSize + randGen->Random()*stepSize)*edge2;
			shuffledPoints.push_back(point);
		}
	}

	// Shuffle points
	int size = (int)shuffledPoints.size();
	for (int i = size - 1; i >= 0; i--) {
		int randomIndex = randGen->IRandom(0, i);
		vec3 temp = shuffledPoints[i];
		shuffledPoints[i] = shuffledPoints[randomIndex];
		shuffledPoints[randomIndex] = temp;
	}

	// Fill point grid with shuffled points
	unsigned int shuffledIndex = 0;
	for (int i = 0; i < sampleRate; i++) {
		for (int j = 0; j < sampleRate; j++) {
			jitteredPoints[i].push_back(shuffledPoints[shuffledIndex]);
			shuffledIndex++;
		}
	}
}

Ray LensCamera::createViewingRay(const Sample& normSamp) {
    double u = normSamp.horiz;
    double v = normSamp.vert;
    
    vec3 leftVertInterp = v*viewplaneUL + (1-v)*viewplaneLL;
	vec3 rightVertInterp = v*viewplaneUR + (1-v)*viewplaneLR;
	vec3 rayEnd = u*rightVertInterp + (1-u)*leftVertInterp;

    //int p = normSamp.p;
    //int q = normSamp.q;
    //vec3 viewerPos = jitteredPoints[p][q];
    //return Ray(viewerPos, rayEnd, rayStart, DBL_MAX, normSamp, NULL);

	double f1 = randGen->Random();
	double f2 = randGen->Random();
	vec3 sampPoint = f1*edge1 + f2*edge2 + position;
	return Ray(sampPoint, rayEnd, rayStart, DBL_MAX, normSamp, NULL);
}


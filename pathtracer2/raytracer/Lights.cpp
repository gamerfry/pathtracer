#include <cfloat>
#include "Lights.h"
#include <time.h>
#include "Material.h"

// Default Constructor
Light::Light() {}

Light::~Light() {}

Light::Light(rgb color, double cfall, double lfall, double qfall) {
    illumination = color;
	falloff[0] = cfall;
	falloff[1] = lfall;
	falloff[2] = qfall;
}

rgb Light::getIntensity(const vec3& refPoint)
{
    return illumination;
}

void Light::sample(const vec3& fromPoint, const vec3& fromNormal, rgb& intensity, vec3& incidence)
{
	// Do nothing (Implemented only for area lights)
}

PointLight::PointLight(double x, double y, double z, const rgb color, double cfall, double lfall, double qfall):
    Light(color, cfall, lfall, qfall) {
		position = vec3(x,y,z);
	}

// Returns an incidence vector.
vec3 PointLight::getIncidence(const vec3 &point) {
	vec3 incidence = position - point;
    return incidence;
}

Ray PointLight::getShadowRay(const vec3& startPoint, double bias, Ray& viewRay) {

	return Ray(startPoint, bias, 1, getIncidence(startPoint), viewRay.getSample(), NULL);
}

rgb PointLight::getIntensity(const vec3& refPoint)
{
	double c, l, q;
	c = falloff[0];
	l = 0;
	q = 0;
	if (falloff[1] != 0 || falloff[2] != 0)
	{
		double r = (position - refPoint).length();
		l = falloff[1]*r;
		q = falloff[2]*r*r;
	}
	return illumination / (c+l+q);
}

DirectionalLight::DirectionalLight(double x, double y, double z, const rgb color):
    Light(color) {
		direction = vec3(x,y,z);
		direction.normalize();
}

// Returns an incidence vector.
vec3 DirectionalLight::getIncidence(const vec3 &point) {
    //vec3 incidence = position - origin;
    //incidence.normalize();
    return -direction;
}

Ray DirectionalLight::getShadowRay(const vec3& startPoint, double bias, Ray& viewRay) {

	return Ray(startPoint, bias, DBL_MAX, getIncidence(startPoint), viewRay.getSample(), viewRay.getLastHitPrim());

}


AreaLight::AreaLight(const vec3& position, const vec3& edge1, const vec3& edge2, const rgb color, int sampleRate,
					 double cfall, double lfall, double qfall) : Light(color, cfall, lfall, qfall) {

	this->position = position;
	this->edge1 = edge1;
	this->edge2 = edge2;

	lightNormal = edge1 ^ edge2;
	lightNormal.normalize();
	area = (edge1 ^ edge2).length();

	randGen = new CRandomMersenne(1);

	//getIncidence(vec3());		// Init "lastSampledPoint"

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

AreaLight::~AreaLight() {

	delete randGen;

}

vec3 AreaLight::getIncidence(const vec3 &point, Ray& viewRay)
{
	//int rI1 = randGen->IRandom(0, jitteredPoints.size()-1);
	//int rI2 = randGen->IRandom(0, jitteredPoints[0].size()-1);
	//vec3 incidence = jitteredPoints[rI1][rI2] - point;
	//lastSampledPoint = jitteredPoints[rI1][rI2];
	//return incidence;

	double f1 = randGen->Random();
	double f2 = randGen->Random();
	vec3 sampPoint = f1*edge1 + f2*edge2 + position;
	lastSampledPoint = sampPoint;
	return sampPoint - point;

	//int p = viewRay.getSample().p;
	//int q = viewRay.getSample().q;
	//vec3 incidence = jitteredPoints[p][q] - point;
	//lastSampledPoint = jitteredPoints[p][q];
	//return incidence;
}

Ray AreaLight::getShadowRay(const vec3& startPoint, double bias, Ray& viewRay) {

	return Ray(startPoint, bias, 1, getIncidence(startPoint, viewRay), viewRay.getSample(), viewRay.getLastHitPrim());
}

rgb AreaLight::getIntensity(const vec3& refPoint)
{
	double c, l, q;
	c = falloff[0];
	l = 0;
	q = 0;
	if (falloff[1] != 0 || falloff[2] != 0)
	{
		double r = (lastSampledPoint - refPoint).length();
		l = falloff[1]*r;
		q = falloff[2]*r*r;
	}
	return (area * illumination) / (c+l+q);		// Divide by 1/area to weight by probability of choosing this point.
}

void AreaLight::sample(const vec3& fromPoint, const vec3& fromNormal, rgb& intensity, vec3& incidence)
{
	// Generate sample point
	double f1 = randGen->Random();
	double f2 = randGen->Random();
	vec3 sampPoint = f1*edge1 + f2*edge2 + position;
	lastSampledPoint = sampPoint;

	// Fill in incidence vector
	incidence = sampPoint - fromPoint;

	// Fill in intensity (weight by area, ingoin/outgoing angle, and distance squared)
	double inlen = incidence.length();
	double cosThetaIn = MAX((fromNormal * incidence) / inlen, 0);
	double cosThetaOut = MAX((-incidence * lightNormal) / inlen, 0);
	//if (cosThetaOut < 0) cosThetaOut *= -1;
	double geoFactor = cosThetaIn*cosThetaOut / incidence.length2();
	// Probablity: 1/area.
	intensity = geoFactor * area * illumination;
}





TexAreaLight::TexAreaLight(const vec3& position, const vec3& edge1, const vec3& edge2, const rgb color, int sampleRate, 
		Texture* t, double cfall, double lfall, double qfall)
: AreaLight(position,edge1,edge2,color,sampleRate,cfall,lfall,qfall), tex(t)
{
}

void TexAreaLight::sample(const vec3& fromPoint, const vec3& fromNormal, rgb& intensity, vec3& incidence)
{
	// Generate sample point
	double f1 = randGen->Random();
	double f2 = randGen->Random();
	vec3 sampPoint = f1*edge1 + f2*edge2 + position;
	lastSampledPoint = sampPoint;

	// Fill in incidence vector
	incidence = sampPoint - fromPoint;

	// Fill in intensity (weight by area, ingoin/outgoing angle, and distance squared)
	double inlen = incidence.length();
	double cosThetaIn = MAX((fromNormal * incidence) / inlen, 0);
	double cosThetaOut = MAX((-incidence * lightNormal) / inlen, 0);
	//if (cosThetaOut < 0) cosThetaOut *= -1;
	double geoFactor = cosThetaIn*cosThetaOut / incidence.length2();
	// Probablity: 1/area.
	intensity = geoFactor * area * illumination;

	// Modulate by texture
	intensity = tex->getColor(vec2(f1,f2)) * intensity;
}
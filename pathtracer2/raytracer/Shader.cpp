/*
 *  Shader.cpp
 *  RayTracer
 *
 *  Created by Lita Cho on 9/23/08.
 *
 */

#include "Shader.h"

Shader::Shader() {}

Shader::~Shader() {}
Shader::Shader(rgb kA, rgb kD, rgb kS, double phong, vec3 viewer):
    kA(kA),
    kD(kD),
    kS(kS),
    phong(phong),
    viewer(viewer) {}

void Shader::addLight(Light *light) {
    myLights.push_back(light);
}

void Shader::removeLight(Light *light) {
    for (int i = 0; i < myLights.size(); i++) {
        if (myLights[i] == light) {
            // Erase the pointer within the vector
            myLights.erase(myLights.begin() + (i-1));
            break;
        }
    }
}

rgb Shader::diffuseComp(const vec3& point, const vec3& origin) {
    rgb diffuse;
    for (int i = 0; i < myLights.size(); i++) {
        vec3 light = myLights[i]->getIncidence(point, origin);
        diffuse = diffuse + (kD * myLights[i]->getColor() * MAX(point * light, 0));
    }
    return diffuse;
}

rgb Shader::specularComp(const vec3& point, const vec3& origin) {
    rgb specular;
    for (int i = 0; i < myLights.size(); i++) {
        vec3 light = myLights[i]->getIncidence(point, origin);
        vec3 reflectVec = -light + (2 * (light * point) * point);
        double scalarTerm = MAX(reflectVec * viewer, 0);
        specular = specular + (kS * myLights[i]->getColor() * pow(scalarTerm, phong));
    }
    return specular;
}

rgb Shader::ambientComp() {
    rgb ambient;
    for (unsigned int k = 0; k < myLights.size(); k++)
		ambient += myLights[k]->getColor();
	return ambient * (kA / myLights.size());
}

rgb Shader::getColor(const vec3& point, const vec3& origin) {
    return diffuseComp(point, origin) + specularComp(point, origin) +
           ambientComp();
}
/*
 *  Shader.h
 *  RayTracer
 *
 *  Created by Lita Cho on 9/23/08.
 *
 */
#include <vector>
#include "rgb.h"
#include "algebra3.h"
#include "Lights.h"

class Shader {
    public:
        Shader();
        virtual ~Shader();
        Shader(rgb kA, rgb kD, rgb kS, double phong, vec3 viewer);
        void addLight(Light *light);
        void removeLight(Light *light);
        rgb getColor(const vec3& point, const vec3& origin);
    private:
        rgb diffuseComp(const vec3& point, const vec3& origin);
        rgb specularComp(const vec3& point, const vec3& origin);
        rgb ambientComp();

    private:
        // Ambient Compontent
        rgb kA;
        // Diffuse Compontent
        rgb kD;
        // Specular Compontent
        rgb kS;
        // Phong Expontent
        double phong;
        //Lights
        std::vector<Light*> myLights;
        //Viewer
        vec3 viewer;
};
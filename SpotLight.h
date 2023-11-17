//
//  spotLight.h
//  test
//
//  Created by Nazirul Hasan on 22/9/23.
//

#ifndef spotLight_h
#define spotLight_h

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "shader.h"
#include <iostream>
#include <string>
using namespace std;

class SpotLight {
public:
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 direction;
    float k_c;
    float k_l;
    float k_q;
    float inner_circle;
    float outer_circle;
    int Number;

   

    SpotLight(float posX, float posY, float posZ, float ambR, float ambG, float ambB, float diffR, float diffG, float diffB, float specR, float specG, float specB, float constant, float linear, float quadratic, int num, float in_circle, float out_circle, float dirX, float dirY, float dirZ) {

        position = glm::vec3(posX, posY, posZ);
        ambient = glm::vec3(ambR, ambG, ambB);
        diffuse = glm::vec3(diffR, diffG, diffB);
        specular = glm::vec3(specR, specG, specB);
        k_c = constant;
        k_l = linear;
        k_q = quadratic;
        inner_circle = in_circle;
        outer_circle = out_circle;
        direction = glm::vec3(dirX, dirY, dirZ);
        Number = num - 1;
        
    }
    SpotLight() {
        position = glm::vec3(0, 0, 0);
        ambient = glm::vec3(1.0f, 1.0f, 1.0f);
        diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
        specular = glm::vec3(1.0f, 1.0f, 1.0f);
        k_c = 1.0f;
        k_l = 0.09f;
        k_q = 0.032f;
        inner_circle = glm::cos(glm::radians(20.5f));
        outer_circle = glm::cos(glm::radians(50.0f));
        direction = glm::vec3(0, 0, -2);
        Number = 1;
        
    }
    void setUpspotLight(Shader& lightingShader)
    {
        lightingShader.use();
        string lightNumber = to_string(Number);
        lightingShader.setVec3("spotLight[" + lightNumber + "].position", position);
        lightingShader.setVec3("spotLight[" + lightNumber + "].ambient", ambient);
        lightingShader.setVec3("spotLight[" + lightNumber + "].diffuse", diffuse);
        lightingShader.setVec3("spotLight[" + lightNumber + "].specular", specular);
        lightingShader.setFloat("spotLight[" + lightNumber + "].k_c", k_c);
        lightingShader.setFloat("spotLight[" + lightNumber + "].k_l", k_l);
        lightingShader.setFloat("spotLight[" + lightNumber + "].k_q", k_q);
        lightingShader.setFloat("spotLight[" + lightNumber + "].inner_circle", inner_circle);
        lightingShader.setFloat("spotLight[" + lightNumber + "].outer_circle", outer_circle);
        lightingShader.setVec3("spotLight[" + lightNumber + "].direction", direction);



    }
    
};

#endif /* spotLight_h */




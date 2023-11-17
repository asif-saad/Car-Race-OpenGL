//
//  pointLight.h
//  test
//
//  Created by Nazirul Hasan on 22/9/23.
//

#ifndef directionLight_h
#define directionLight_h

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "shader.h"
#include <string>
#include <iostream>


using namespace std;

class DirectionLight {
public:
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    int Number;
    DirectionLight() {
        // doing nothing
    }
    DirectionLight(float dirX, float dirY, float dirZ, float ambR, float ambG, float ambB, float diffR, float diffG, float diffB, float specR, float specG, float specB, int num) {

        direction = glm::vec3(dirX, dirY, dirZ);
        ambient = glm::vec3(ambR, ambG, ambB);
        diffuse = glm::vec3(diffR, diffG, diffB);
        specular = glm::vec3(specR, specG, specB);
        Number = num-1;
    }
  
   void setUpDirectLight(Shader & lightingShader)
        {
            lightingShader.use();
            string lightNumber = to_string(Number);
            lightingShader.setVec3("dirLight[" + lightNumber + "].direction", direction);
            lightingShader.setVec3("dirLight[" + lightNumber + "].ambient", ambientOn * ambient);
            lightingShader.setVec3("dirLight[" + lightNumber + "].diffuse", diffuseOn* diffuse);
            lightingShader.setVec3("dirLight[" + lightNumber + "].specular", specularOn*specular);



        }
   void turnOff()
   {
       ambientOn = 0.3;
       diffuseOn = 0.3;
       specularOn = 0.3;
   }
   void turnOn()
   {
       ambientOn = 1.0;
       diffuseOn = 1.0;
       specularOn = 1.0;
   }
  
private:
    float ambientOn = 1.0;
    float diffuseOn = 1.0;
    float specularOn = 1.0;
};

#endif /* pointLight_h */

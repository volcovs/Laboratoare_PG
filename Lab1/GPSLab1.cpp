//
//  GPSLab1.cpp
//
//  Copyright © 2017 CGIS. All rights reserved.
//

#include "GPSLab1.hpp"

namespace gps {
    glm::vec4 TransformPoint(const glm::vec4 &point)
    {
        glm::mat4 transformMatrix(1.0f);
        transformMatrix = glm::rotate(transformMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        transformMatrix = glm::translate(transformMatrix, glm::vec3(2.0f, 0.0f, 1.0f));

        return transformMatrix * point;
    }
    
    float ComputeAngle(const glm::vec3 &v1, const glm::vec3 &v2)
    {
        float cos = glm::dot(v1, v2);
        cos = cos / (glm::length(v1) * glm::length(v2));
        
        return glm::degrees(acos(cos));
    }
    
    bool IsConvex(const std::vector<glm::vec2> &vertices)
    {
        std::vector<glm::vec3> vert3D(vertices.size());
        for (int i = 0; i < vertices.size(); i++) {
            vert3D.at(i).x = vertices.at(i).x;
            vert3D.at(i).y = vertices.at(i).y;
            vert3D.at(i).z = 1.0f;
        }

        //all z-components of all cross products are of the same sign
        float firstSign = glm::cross(vert3D.at(0), vert3D.at(1)).z;

        for (int i = 1; i < vertices.size() - 1; i++) {
            if (((glm::cross(vert3D.at(i), vert3D.at(i + 1)).z) * firstSign) <= 0) {
                return false;
            }
        }

        if (((glm::cross(vert3D.at(vertices.size() - 1), vert3D.at(0)).z) * firstSign) <= 0) {
            return false;
        }
        else {
            return true;
        }
    }
    
    std::vector<glm::vec2> ComputeNormals(const std::vector<glm::vec2> &vertices)
    {
        std::vector<glm::vec2> normalsList(vertices.size());
        glm::vec3 normal3D;
        glm::vec3 zAxis(0.0f, 0.0f, 1.0f);

        std::vector<glm::vec3> vert3D(vertices.size());
        for (int i = 0; i < vertices.size(); i++) {
            vert3D.at(i).x = vertices.at(i).x;
            vert3D.at(i).y = vertices.at(i).y;
            vert3D.at(i).z = 1.0f;
        }

        //n = (V1-V2)x(0, 0, 1)
        for (int i = 0; i < vertices.size() - 1; i++) {
            normal3D = glm::cross(vert3D.at(i) - vert3D.at(i+1), zAxis);
            normal3D = glm::normalize(normal3D);
            normalsList.at(i).x = normal3D.x;
            normalsList.at(i).y = normal3D.y;
        }

        normal3D = glm::cross(vert3D.at(vertices.size()-1) - vert3D.at(0), zAxis);
        normal3D = glm::normalize(normal3D);
        normalsList.at(vertices.size()-1).x = normal3D.x;
        normalsList.at(vertices.size()-1).y = normal3D.y;

        return normalsList;
    }
}

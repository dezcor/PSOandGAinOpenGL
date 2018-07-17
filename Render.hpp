#ifndef _RENDER_HPP_
#define _RENDER_HPP_

#include <stdio.h>
#include <cmath>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <iosfwd>
#include <sstream>
#include <fstream>
// Incluir GLEW.
#include <GL/glew.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class CamaraView
{
    public:
        CamaraView(const glm::vec3& pos,float fow,float aspect,float zNear,float zFar)
        {
            mProjection = glm::perspective(fow,aspect,zNear,zFar);
            mPos = pos;
            mEye = -pos+glm::vec3(0,0,0);
            mWordUp = glm::vec3(0,1,0);
            this->Pitch = 0;
            this->Yaw = -90;
            updateCameraVectors();
        }

        CamaraView(const glm::vec3& pos,float left, float right, float bottom, float top, float zNear, float zFar)
        {
            mProjection = glm::ortho(left,right,bottom,top,zNear,zFar);
            mPos = pos;
            mEye = -pos+glm::vec3(0,0,0);
            mWordUp = glm::vec3(0,1,0);

            this->Pitch = 0;
            this->Yaw = -90;
        }

        inline glm::mat4 GetViewPerpetive() const
        {
           return mProjection * glm::lookAt(mPos,mPos+mEye,mUp);
        }

        inline glm::vec3 & GetPos()
        {
            return mPos;
        }

        enum class Movement {
                FORWARD,
                BACKWARD,
                LEFT,
                RIGHT
            };

        inline void Move(const Movement move)
        {
            glm::vec3 Front = glm::normalize(mEye);
            float velocity = MovementSpeed;
            if (move == Movement::FORWARD)
                mPos += Front * velocity;
            if (move == Movement::BACKWARD)
                mPos -= Front * velocity;
            if (move == Movement::LEFT)
                mPos -= Right * velocity;
            if (move == Movement::RIGHT)
                mPos += Right * velocity;
        }

        // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
        void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
        {
            xoffset *= MouseSensitivity;
            yoffset *= MouseSensitivity;

            Yaw   += xoffset;
            Pitch += yoffset;

            // Make sure that when pitch is out of bounds, screen doesn't get flipped
            if (constrainPitch)
            {
                if (Pitch > 89.0f)
                    Pitch = 89.0f;
                if (Pitch < -89.0f)
                    Pitch = -89.0f;
            }

            // Update Front, Right and Up Vectors using the updated Euler angles
            updateCameraVectors();
        }

        
    private:
        glm::mat4 mModel;
        glm::mat4 mProjection;
        glm::mat4 mView;
        glm::vec3 mPos;
        glm::vec3 mUp;
        glm::vec3 mWordUp;
        glm::vec3 mEye;
        glm::vec3 Right;
        //angulo
        float Yaw;
        float Pitch;
        // Camera options
        float MovementSpeed = 0.05f;
        float MouseSensitivity = 0.5f;


        void updateCameraVectors()
        {
            // Calculate the new Front vector
            glm::vec3 front;
            front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
            front.y = sin(glm::radians(Pitch));
            front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
            mEye = glm::normalize(front);
            // Also re-calculate the Right and Up vector
            Right = glm::normalize(glm::cross(mEye, mWordUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
            mUp    = glm::normalize(glm::cross(Right, mEye));
        }
};

// class Trasformada
// {
//     public:
//         Trasformada();
//     private:
//         glm::mat4 tras;
// };

class Render
{
    public:
    virtual void dcRender(glm::mat4 const &m) = 0;
    virtual void dcUpdate() = 0;
    void SetMatrizID(GLuint matrixid)
    {
        MatrixID = matrixid;
    }
    protected:
    GLuint MatrixID;
};

struct vect3
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
    vect3()
    {
        x=0;
        y=0;
        z=0;
    }

    vect3(GLfloat x,GLfloat y,GLfloat z)
    {
        this->x=x;
        this->y=y;
        this->z=z;
    }
};

struct vect2
{
    GLfloat x;
    GLfloat y;
    vect2()
    {
        x=0;
        y=0;
    }

    vect2(GLfloat x,GLfloat y)
    {
        this->x=x;
        this->y=y;
    }
};


struct ColorRGB
{
    GLfloat r;
    GLfloat g;
    GLfloat b;
    ColorRGB()
    {
        r=0;
        g=0;
        b=0;
    }

    ColorRGB(GLfloat r,GLfloat g,GLfloat b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    bool operator==(const ColorRGB & B)
    {
        if(r != B.r) return false;
        if(g != B.g) return false;
        if(b != B.b) return false;
        return true;
    }
};
struct Vertex3d
{
    vect3 Pos;
    ColorRGB color;
    vect2 coorText;
};

struct Vertex2d
{
    vect2 Pos;
    ColorRGB color;
};



#endif //_RENDER_HPP_
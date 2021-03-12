#pragma once

#include <Core/Engine.h>
#include <string>

class Platform {
    public: 
        Platform();
        ~Platform();
        void setX(float x );
        void setY(float y);
        void setZ(float z);
        void setInitialZ(float zInitial);
        float getX();
        float getY();
        float getZ();
        float getInitialZ();
        void setColor(int color);
        int  getColor();
        void setCollision(bool x);
        bool getCollision();
    
    private:
        float x,y,z;
        float initialZ;
        int color; //0 va insemna ca platforma e rosie
                    //1 "-" e galbena
                    //2 "- " e portocalie
                    //3 - " " verde
        bool collision;
};
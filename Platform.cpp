#include "Platform.h"
#include<stdio.h>
#include<stdlib.h>
#include<ctime>
#include <string>

Platform::Platform() {

}
Platform::~Platform() {

}

float Platform::getX() {
    return x;
}
float Platform::getY() {
    return y;
}
float Platform::getZ() {
    return z;
}
float Platform::getInitialZ() {
    return initialZ;
}
int Platform::getColor() {
    return color;
}
bool Platform::getCollision() {
    return collision;
}
void Platform::setX(float xs) {
    x = xs;
}
void Platform::setY(float ys) {
    y = ys;
}
void Platform::setZ(float zs) {
    z = zs;
}
void Platform::setInitialZ(float initialZs) {
    initialZ = initialZs;
}
void Platform::setColor(int scolor) {
    color = scolor;
}
void Platform::setCollision(bool x) {
    collision = x;
}
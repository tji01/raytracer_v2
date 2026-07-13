

/*

lin_alg.c 

this file defines functions used for linear algebra

*/


#include <math.h>
#ifndef STRUCTS
#define STRUCTS
#include "structs.h"
#endif

/*
    computes dot product 

    args:   vector* v1, v2

    return:  float
*/
float dot(vector* v1, vector* v2)
{
    float a = (v1->x) * (v2->x);
    float b = (v1->y) * (v2->y);
    float c = (v1->z) * (v2->z);
    return (a + b + c); 
}

/*
    computes the cross product of the vetors in place. 

    args:   vector* product, v2
    return: void
*/
void cross(vector* product, vector* v2)
{
    float x = (product->y) * (v2->z) - (product->z) * (v2->y);
    float y = (product->z) * (v2->x) - (product->x) * (v2->z);
    float z = (product->x) * (v2->y) - (product->y) * (v2->x);

    newVector(x, y, z, product);
}

/*
    scales given vector in place
    
    args:   float c
            vector* v
    returns: void
*/
void scale(float c, vector* v)
{
    float x = (v->x) * c;
    float y = (v->y) * c;
    float z = (v->z) * c;

    newVector(x, y, z, v);
}

/*
    ensures a number is in range [mn, mx] and sets to nearest value
    if not

    args:   float min, max, val

    return: float
*/
float clamp(float min, float max, float val)
{
    if(val < min) return min;
    if(val > max) return max;
    return val;
}

/*
    find the distance down the argument ray intersects a sphere

    args:   ball* ball
            ray* ray

    return  float
*/
float intersectBall(ball* ball, ray* ray)
{
    /*
    
        t = (-d * (e-c) +/- sqrt{ (d*(e-c))^2 - (d*d)[(e-c) * (e-c)] - R^2 } ) / (d*d)
        where   e = camera location
                d = ray direction
                c = sphere location
                t = distance from e in terms of d vector magnitude


    */

    //Discriminant 

    //(d*(e-c))^2 : a

    vector e; newVector(ray->o->x, ray->o->y, ray->o->z, &e);
    vector e_c; newVector(  (e.x - ball->position->x),
                            (e.y - ball->position->y),
                            (e.z - ball->position->z),
                            &e_c
                        );
    float dot_dec = dot(ray->v, &e_c);
    float a = (dot_dec * dot_dec);

    //(d*d)[(e-c) * (e-c)] : b

    float dot_dd = dot(ray->v, ray->v);
    float dot_ecec = dot(&e_c, &e_c);
    float b = dot_dd * dot_ecec;

    b = b - ((ball->radius) * (ball->radius));

    float discriminant = a - b;
    if(discriminant < 0) return -1;

    //Radical
    float root = sqrt(discriminant);

    //Pre radical
    vector neg_d; 
    newVector(ray->v->x, ray->v->y, ray->v->z, &neg_d);
    scale(-1, &neg_d);
    float c = dot(&neg_d, &e_c);

    //numerator
    float numeratorPlus = c + root;
    float numeratorMinus = c - root;
    float t;

    float quotientPlus = numeratorPlus / dot_dd;
    float quotientMinus = numeratorMinus / dot_dd;

    if(quotientMinus >= quotientPlus)
    {
        t = quotientPlus;
    }
    else
    {
        t = quotientMinus;
    }

    if(t<0) return -1;

    return t;


}

/*
    creates a vector in provided space from the given points

    args:   point* initial, terminal
            vector* v

    return: void
*/
void vectorFromPoints(point* initial, point* terminal, vector* v)
{
    newVector(  (terminal->x) - (initial->x),
                (terminal->y) - (initial->y), 
                (terminal->z) - (initial->z), 
                &v
            );
}

/*

    sums two vectors in place, sum placed in a

    args: vector* a, b

    return: void
*/
void vectorSum(vector* a, vector* b)
{
    newVector(
        a->x + b->x,
        a->y + b->y,
        a->z + b->z,
        a
    );
}

/*
    finds difference of points, difference placed in provided 
    vector space

    args: point* a, b
            vector* v


    return void
*/
void pointDifference(point* a, point* b, vector* v)
{
    newVector(
        a->x - b->x, 
        a->y - b->y,
        a->z - b->z,
        v
    );
}

/*
    returns the scalar magnatude of the argument vector

    args:   vector* a

    return: float
*/

float getMagnitude(vector* a)
{
    return (float) sqrt( (double) 
                        ((a->x) * (a->x)) + 
                        ((a->y) * (a->y)) + 
                        ((a->z) * (a->z)) );
}

/*
    modifies the provided vector so it is normalized

    args:   ray* r

    return: void
*/
void normalize(vector* v)
{
    float mag = getMagnitude(v);

    v->x = v->x / mag;
    v->y = v->y / mag;
    v->z = v->z / mag;
}

//Trig Wizardry

/*
    triangleM
*/
float triangleM(tri* t, ray* r)
{
    // M = A(EI - HF) + B(GF - DI) + C(DH - EG)

    float A = (t->a->x) - (t->b->x);
    float B = (t->a->y) - (t->b->y);
    float C = (t->a->z) - (t->b->z);

    float D = (t->a->x) - (t->c->x);
    float E = (t->a->y) - (t->c->y);
    float F = (t->a->z) - (t->c->z);

    float G = (r->v->x);
    float H = (r->v->y);
    float I = (r->v->z);

    return ((A * ((E*I)-(H*F))) + (B * ((G*F) - (D*I))) + (C * ((D*H) - (E*G))));

}

/* triangleBeta








*/
float triangleBeta(tri* t, ray* r)
{
    float M = triangleM(t, r);
    //if(M == 0) return -1;
    float D = (t->a->x) - (t->c->x);
    float E = (t->a->y) - (t->c->y);
    float F = (t->a->z) - (t->c->z);

    float G = (r->v->x);
    float H = (r->v->y);
    float I = (r->v->z);

    float J = (t->a->x) - (r->o->x);
    float K = (t->a->y) - (r->o->y);
    float L = (t->a->z) - (r->o->z);

    return ( ((J * ((E*I) - (H*F))) + (K * ((G*F) - (D*I))) + (L * ((D*H) - (E*G)))) / M);
}

/* triangleGamma








*/
float triangleGamma(tri* t, ray* r)
{
    float M = triangleM(t, r);
    //if(M == 0) return -1;

    float A = (t->a->x) - (t->b->x);
    float B = (t->a->y) - (t->b->y);
    float C = (t->a->z) - (t->b->z);

    float G = (r->v->x);
    float H = (r->v->y);
    float I = (r->v->z);

    float J = (t->a->x) - (r->o->x);
    float K = (t->a->y) - (r->o->y);
    float L = (t->a->z) - (r->o->z);

    return ( ((I * ((A*K) - (J*B))) + (H * ((J*C) - (A*L))) + (G * ((B*L) - (K*C)))) / M);
}

/* triangleT








*/
float triangleT(tri* t, ray* r)
{
    float M = triangleM(t, r);
    //if(M == 0) return -1;

    float A = (t->a->x) - (t->b->x);
    float B = (t->a->y) - (t->b->y);
    float C = (t->a->z) - (t->b->z);

    float D = (t->a->x) - (t->c->x);
    float E = (t->a->y) - (t->c->y);
    float F = (t->a->z) - (t->c->z);

    float J = (t->a->x) - (r->o->x);
    float K = (t->a->y) - (r->o->y);
    float L = (t->a->z) - (r->o->z);

    return ( -1 * ((F * ((A*K) - (J*B))) + (E * ((J*C) - (A*L))) + (D * ((B*L) - (K*C)))) / M);
}

/*
    Find distance at which given ray intersects the given triangle
    
    args:   tri* t
            ray* r

    returns -1 if no intersection
*/
float intersectTriangle(tri* tri, ray* r)
{

    float t = triangleT(tri, r);
    
    if(t < 0) return -1;

    float gamma = triangleGamma(tri, r);
    
    if((gamma < 0) | (gamma > 1)) return -1;

    float beta = triangleBeta(tri, r);

    if((beta < 0) | (beta > (1 - gamma))) return -1;


    return t;
}

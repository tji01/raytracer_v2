



/*
    Math LOL
*/

#include <math.h>
#include "structs.h"


/*
    Returns the dot product of two vector structs
*/
float dot(vector* v1, vector* v2)
{
    float a = (*(v1->x) * *(v2->x));
    float b = (*(v1->y) * *(v2->y));
    float c = (*(v1->z) * *(v2->z));
    return (a + b + c); 
}

/*  cross
    Args:   vector* v1, v2
    Return: vector* 

    returns the cross product of the vectors

    Free the returned address using freeVector


*/
vector* cross(vector* v1, vector* v2)
{
    float x = (*(v1->y) * *(v2->z)) - (*(v1->z) * *(v2->y));
    float y = (*(v1->z) * *(v2->x)) - (*(v1->x) * *(v2->z));
    float z = (*(v1->x) * *(v2->y)) - (*(v1->y) * *(v2->x));

    return newVector(x, y, z);

}
/*
    returns a vector struct scaled to a float c given a vector struct v.

    Must free using freeVector(addr)
*/
vector* scale(float c, vector* v)
{
    float x = (*(v->x) * c);
    float y = (*(v->y) * c);
    float z = (*(v->z) * c);
    return newVector(x, y, z);
    
}

/* clamp
    Args:   float min, max, value
    return: float

    Ensures that a number is in the rage [min, max] and if it is outside'
    it is set to the closest valid value


*/
float clamp(float min, float max, float value)
{
    if(value > max) return max;
    if(value < min) return min;
    return value;
}

/* intersectBall

    Args:   ball* ball (Sphere struct pointer)
            ray* ray (Ray struct pointer)

    Return: float closest value for how far away arg Ray intersects Sphere struct. Return value is in terms of the magnitude of the Ray vector. If Ray vector is normalized, return value is in unit terms. 


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
    vector* e = newVector(*(ray->o->x), *(ray->o->y), *(ray->o->z));
    vector* e_c  = newVector(//camera_vector - ball_position
                                (*(e->x)-*(ball->position->x)),
                                (*(e->y)-*(ball->position->y)), 
                                (*(e->z)-*(ball->position->z)) 
                            );

    float dot_dec = dot(ray->v, e_c);
    float a = (dot_dec * dot_dec);

    //(d*d)[(e-c) * (e-c)] : b
    float dot_dd = dot(ray->v, ray->v);
    float dot_ecec = dot(e_c, e_c);
    float b = dot_dd * dot_ecec;
    // ... - R^2 
    b = b - (*(ball->radius) * *(ball->radius));

    float discriminant = a - b;
    if (discriminant < 0) 
    {
        freeVector(e);
        freeVector(e_c);
        return -1;
    }

    //Radical
    float root = sqrt(discriminant);

    //pre radical (-d * (e-c) ): c 
    vector* neg_d = scale(-1, ray->v);
    float c = dot(neg_d, e_c);

    //numerator
    float numeratorPlus = (c + root);
    float numeratorMinus = (c - root);
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
    
    
    
    freeVector(neg_d);
    freeVector(e);
    freeVector(e_c);
    
    if (t<0) return -1;
   // if (isnan(t))return -1;
    return t;
}
/*
    Takes two point structs and returns the vector between them. 

    use freeVector() on returned address
*/
vector* vectorFromPoints(point* initial, point* terminal)
{
    float X = (*(terminal->x) - *(initial->x));
    float Y = (*(terminal->y) - *(initial->y));
    float Z = (*(terminal->z) - *(initial->z));

    return newVector(X, Y, Z);
}

/*

    Sums two vectors and returns a new vector struct's address
    input: vector* a, b
    output: vector* sum(a,b)

    use freeVector() on address after use

*/
vector* vectorSum(vector* a, vector* b)
{
    return newVector(
        *(a->x) + *(b->x),
        *(a->y) + *(b->y),
        *(a->z) + *(b->z)
    );
}


vector* pointDifference(point* a, point* b)
{
    return newVector(
        *(a->x) - *(b->x),
        *(a->y) - *(b->y),
        *(a->z) - *(b->z)
    );
}

/*
    Returns the scalar magnitude of the argument vector
*/
float getMagnitude(vector* a)
{
    return (float) sqrt( (double) ((*(a->x) * *(a->x)) + (*(a->y) * *(a->y)) + (*(a->z) * *(a->z))) );
}


/* normalizeRay

    Args:   ray* r (address of Ray struct to normalize)

    Destructive
    Modifies the values of the vector struct in the ray to normalize the vector. Calculates the magnitude to do so.
*/
void normalize(vector* v)
{
    float mag = getMagnitude(v);
    *(v->x) = (*(v->x)/mag);
    *(v->y) = (*(v->y)/mag);
    *(v->z) = (*(v->z)/mag);
}

/* triangleM








*/
float triangleM(tri* t, ray* r)
{
    // M = A(EI - HF) + B(GF - DI) + C(DH - EG)

    float A = *(t->a->x) - *(t->b->x);
    float B = *(t->a->y) - *(t->b->y);
    float C = *(t->a->z) - *(t->b->z);

    float D = *(t->a->x) - *(t->c->x);
    float E = *(t->a->y) - *(t->c->y);
    float F = *(t->a->z) - *(t->c->z);

    float G = *(r->v->x);
    float H = *(r->v->y);
    float I = *(r->v->z);

    return ((A * ((E*I)-(H*F))) + (B * ((G*F) - (D*I))) + (C * ((D*H) - (E*G))));
}


/* triangleBeta








*/
float triangleBeta(tri* t, ray* r)
{
    float M = triangleM(t, r);
    //if(M == 0) return -1;
    float D = *(t->a->x) - *(t->c->x);
    float E = *(t->a->y) - *(t->c->y);
    float F = *(t->a->z) - *(t->c->z);

    float G = *(r->v->x);
    float H = *(r->v->y);
    float I = *(r->v->z);

    float J = *(t->a->x) - *(r->o->x);
    float K = *(t->a->y) - *(r->o->y);
    float L = *(t->a->z) - *(r->o->z);

    return ( ((J * ((E*I) - (H*F))) + (K * ((G*F) - (D*I))) + (L * ((D*H) - (E*G)))) / M);
}

/* triangleGamma








*/
float triangleGamma(tri* t, ray* r)
{
    float M = triangleM(t, r);
    //if(M == 0) return -1;

    float A = *(t->a->x) - *(t->b->x);
    float B = *(t->a->y) - *(t->b->y);
    float C = *(t->a->z) - *(t->b->z);

    float G = *(r->v->x);
    float H = *(r->v->y);
    float I = *(r->v->z);

    float J = *(t->a->x) - *(r->o->x);
    float K = *(t->a->y) - *(r->o->y);
    float L = *(t->a->z) - *(r->o->z);

    return ( ((I * ((A*K) - (J*B))) + (H * ((J*C) - (A*L))) + (G * ((B*L) - (K*C)))) / M);
}

/* triangleT








*/
float triangleT(tri* t, ray* r)
{
    float M = triangleM(t, r);
    //if(M == 0) return -1;

    float A = *(t->a->x) - *(t->b->x);
    float B = *(t->a->y) - *(t->b->y);
    float C = *(t->a->z) - *(t->b->z);

    float D = *(t->a->x) - *(t->c->x);
    float E = *(t->a->y) - *(t->c->y);
    float F = *(t->a->z) - *(t->c->z);

    float J = *(t->a->x) - *(r->o->x);
    float K = *(t->a->y) - *(r->o->y);
    float L = *(t->a->z) - *(r->o->z);

    return ( -1 * ((F * ((A*K) - (J*B))) + (E * ((J*C) - (A*L))) + (D * ((B*L) - (K*C)))) / M);
}


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


void reflect_ray(ray* r, ray* norm)
{   
    
}

#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <cstdlib>
#include <iostream>

class Vec3;
static inline Vec3 operator + (Vec3 const & a , Vec3 const & b);
static inline Vec3 operator * (float a , Vec3 const & b);

class Vec3 {
private:
    float mVals[3];
public:
    Vec3() {mVals[0] = mVals[1] = mVals[2] = 0.f;}
    Vec3( float x , float y , float z ) {
       mVals[0] = x; mVals[1] = y; mVals[2] = z;
    }
    float & operator [] (unsigned int c) { return mVals[c]; }
    float operator [] (unsigned int c) const { return mVals[c]; }
    Vec3 operator = (Vec3 const & other) {
       mVals[0] = other[0] ; mVals[1] = other[1]; mVals[2] = other[2];
       return *this;
    }
    float squareLength() const {
       return mVals[0]*mVals[0] + mVals[1]*mVals[1] + mVals[2]*mVals[2];
    }
    float length() const { return sqrt( squareLength() ); }
    inline
    float norm() const { return length(); }
    inline
    float squareNorm() const { return squareLength(); }
    void normalize() { float L = length(); mVals[0] /= L; mVals[1] /= L; mVals[2] /= L; }
    static float dot( Vec3 const & a , Vec3 const & b ) {
       return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
    }
    static Vec3 cross( Vec3 const & a , Vec3 const & b ) {
       return Vec3( a[1]*b[2] - a[2]*b[1] ,
                    a[2]*b[0] - a[0]*b[2] ,
                    a[0]*b[1] - a[1]*b[0] );
    }
    void operator += (Vec3 const & other) {
        mVals[0] += other[0];
        mVals[1] += other[1];
        mVals[2] += other[2];
    }
    void operator -= (Vec3 const & other) {
        mVals[0] -= other[0];
        mVals[1] -= other[1];
        mVals[2] -= other[2];
    }
    void operator *= (float s) {
        mVals[0] *= s;
        mVals[1] *= s;
        mVals[2] *= s;
    }
    void operator /= (float s) {
        mVals[0] /= s;
        mVals[1] /= s;
        mVals[2] /= s;
    }
    static Vec3 compProduct(Vec3 const & a , Vec3 const & b) {
        return Vec3(a[0]*b[0] , a[1]*b[1] , a[2]*b[2]);
    }

    unsigned int getMaxAbsoluteComponent() const {
        if( fabs(mVals[0]) > fabs(mVals[1]) ) {
            if( fabs(mVals[0]) > fabs(mVals[2]) ) {
                return 0;
            }
            return 2;
        }
        if( fabs(mVals[1]) > fabs(mVals[2]) ) {
            return 1;
        }
        return 2;
    }
    Vec3 getOrthogonal() const {
        unsigned int c1 = getMaxAbsoluteComponent();
        unsigned int c2 = (c1+1)%3;
        Vec3 res( 0,0,0 );
        res[c1] = mVals[c2];
        res[c2] = -mVals[c1];
        return res;
    }
};

static inline Vec3 operator + (Vec3 const & a , Vec3 const & b) {
   return Vec3(a[0]+b[0] , a[1]+b[1] , a[2]+b[2]);
}
static inline Vec3 operator - (Vec3 const & a , Vec3 const & b) {
   return Vec3(a[0]-b[0] , a[1]-b[1] , a[2]-b[2]);
}
static inline Vec3 operator * (float a , Vec3 const & b) {
   return Vec3(a*b[0] , a*b[1] , a*b[2]);
}
static inline Vec3 operator * (Vec3 const & b , float a ) {
   return Vec3(a*b[0] , a*b[1] , a*b[2]);
}
static inline Vec3 operator / (Vec3 const &  a , float b) {
   return Vec3(a[0]/b , a[1]/b , a[2]/b);
}
static inline std::ostream & operator << (std::ostream & s , Vec3 const & p) {
    s << p[0] << " " << p[1] << " " << p[2];
    return s;
}
static inline std::istream & operator >> (std::istream & s , Vec3 & p) {
    s >> p[0] >> p[1] >> p[2];
    return s;
}

#endif

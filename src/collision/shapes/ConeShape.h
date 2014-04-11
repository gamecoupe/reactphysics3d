/********************************************************************************
* ReactPhysics3D physics library, http://code.google.com/p/reactphysics3d/      *
* Copyright (c) 2010-2013 Daniel Chappuis                                       *
*********************************************************************************
*                                                                               *
* This software is provided 'as-is', without any express or implied warranty.   *
* In no event will the authors be held liable for any damages arising from the  *
* use of this software.                                                         *
*                                                                               *
* Permission is granted to anyone to use this software for any purpose,         *
* including commercial applications, and to alter it and redistribute it        *
* freely, subject to the following restrictions:                                *
*                                                                               *
* 1. The origin of this software must not be misrepresented; you must not claim *
*    that you wrote the original software. If you use this software in a        *
*    product, an acknowledgment in the product documentation would be           *
*    appreciated but is not required.                                           *
*                                                                               *
* 2. Altered source versions must be plainly marked as such, and must not be    *
*    misrepresented as being the original software.                             *
*                                                                               *
* 3. This notice may not be removed or altered from any source distribution.    *
*                                                                               *
********************************************************************************/

#ifndef REACTPHYSICS3D_CONE_SHAPE_H
#define REACTPHYSICS3D_CONE_SHAPE_H

// Libraries
#include "CollisionShape.h"
#include "../../mathematics/mathematics.h"

/// ReactPhysics3D namespace
namespace reactphysics3d {

// Class ConeShape
/**
 * This class represents a cone collision shape centered at the
 * origin and alligned with the Y axis. The cone is defined
 * by its height and by the radius of its base. The center of the
 * cone is at the half of the height. The "transform" of the
 * corresponding rigid body gives an orientation and a position
 * to the cone. This collision shape uses an extra margin distance around
 * it for collision detection purpose. The default margin is 4cm (if your
 * units are meters, which is recommended). In case, you want to simulate small
 * objects (smaller than the margin distance), you might want to reduce the margin
 * by specifying your own margin distance using the "margin" parameter in the
 * constructor of the cone shape. Otherwise, it is recommended to use the
 * default margin distance by not using the "margin" parameter in the constructor.
 */
class ConeShape : public CollisionShape {

    private :

        // -------------------- Attributes -------------------- //

        /// Radius of the base
        decimal mRadius;

        /// Half height of the cone
        decimal mHalfHeight;

        /// sine of the semi angle at the apex point
        decimal mSinTheta;

        // -------------------- Methods -------------------- //

        /// Private copy-constructor
        ConeShape(const ConeShape& shape);

        /// Private assignment operator
        ConeShape& operator=(const ConeShape& shape);
        
    public :

        // -------------------- Methods -------------------- //

        /// Constructor
        ConeShape(decimal mRadius, decimal height, decimal margin = OBJECT_MARGIN);

        /// Destructor
        virtual ~ConeShape();

        /// Allocate and return a copy of the object
        virtual ConeShape* clone(void* allocatedMemory) const;

        /// Return the radius
        decimal getRadius() const;

        /// Return the height
        decimal getHeight() const;

        /// Return the number of bytes used by the collision shape
        virtual size_t getSizeInBytes() const;

        /// Return a local support point in a given direction with the object margin
        virtual Vector3 getLocalSupportPointWithMargin(const Vector3& direction) const;

        /// Return a local support point in a given direction without the object margin
        virtual Vector3 getLocalSupportPointWithoutMargin(const Vector3& direction) const;

        /// Return the local bounds of the shape in x, y and z directions
        virtual void getLocalBounds(Vector3& min, Vector3& max) const;

        /// Return the local inertia tensor of the collision shape
        virtual void computeLocalInertiaTensor(Matrix3x3& tensor, decimal mass) const;

        /// Test equality between two cone shapes
        virtual bool isEqualTo(const CollisionShape& otherCollisionShape) const;

        /// Create a proxy collision shape for the collision shape
        virtual ProxyShape* createProxyShape(MemoryAllocator& allocator, CollisionBody* body,
                                             const Transform& transform, decimal mass) const;
};

// Class ProxyConeShape
/**
 * The proxy collision shape for a cone shape.
 */
class ProxyConeShape : public ProxyShape {

    private:

        // -------------------- Attributes -------------------- //

        /// Pointer to the actual collision shape
        const ConeShape* mCollisionShape;


        // -------------------- Methods -------------------- //

        /// Private copy-constructor
        ProxyConeShape(const ProxyConeShape& proxyShape);

        /// Private assignment operator
        ProxyConeShape& operator=(const ProxyConeShape& proxyShape);

    public:

        // -------------------- Methods -------------------- //

        /// Constructor
        ProxyConeShape(const ConeShape* shape, CollisionBody* body,
                       const Transform& transform, decimal mass);

        /// Destructor
        ~ProxyConeShape();

        /// Return the collision shape
        virtual const CollisionShape* getCollisionShape() const;

        /// Return the number of bytes used by the proxy collision shape
        virtual size_t getSizeInBytes() const;

        /// Return a local support point in a given direction with the object margin
        virtual Vector3 getLocalSupportPointWithMargin(const Vector3& direction);

        /// Return a local support point in a given direction without the object margin
        virtual Vector3 getLocalSupportPointWithoutMargin(const Vector3& direction);

        /// Return the current collision shape margin
        virtual decimal getMargin() const;
};

// Allocate and return a copy of the object
inline ConeShape* ConeShape::clone(void* allocatedMemory) const {
    return new (allocatedMemory) ConeShape(*this);
}

// Return the radius
inline decimal ConeShape::getRadius() const {
    return mRadius;
}

// Return the height
inline decimal ConeShape::getHeight() const {
    return decimal(2.0) * mHalfHeight;
}

// Return the number of bytes used by the collision shape
inline size_t ConeShape::getSizeInBytes() const {
    return sizeof(ConeShape);
}

// Return the local bounds of the shape in x, y and z directions
inline void ConeShape::getLocalBounds(Vector3& min, Vector3& max) const {

    // Maximum bounds
    max.x = mRadius + mMargin;
    max.y = mHalfHeight + mMargin;
    max.z = max.x;

    // Minimum bounds
    min.x = -max.x;
    min.y = -max.y;
    min.z = min.x;
}

// Return the local inertia tensor of the collision shape
inline void ConeShape::computeLocalInertiaTensor(Matrix3x3& tensor, decimal mass) const {
    decimal rSquare = mRadius * mRadius;
    decimal diagXZ = decimal(0.15) * mass * (rSquare + mHalfHeight);
    tensor.setAllValues(diagXZ, 0.0, 0.0,
                        0.0, decimal(0.3) * mass * rSquare,
                        0.0, 0.0, 0.0, diagXZ);
}

// Test equality between two cone shapes
inline bool ConeShape::isEqualTo(const CollisionShape& otherCollisionShape) const {
    const ConeShape& otherShape = dynamic_cast<const ConeShape&>(otherCollisionShape);
    return (mRadius == otherShape.mRadius && mHalfHeight == otherShape.mHalfHeight);
}

// Create a proxy collision shape for the collision shape
inline ProxyShape* ConeShape::createProxyShape(MemoryAllocator& allocator, CollisionBody* body,
                                               const Transform& transform, decimal mass) const {
    return new (allocator.allocate(sizeof(ProxyConeShape))) ProxyConeShape(this, body,
                                                                           transform, mass);
}

// Return the collision shape
inline const CollisionShape* ProxyConeShape::getCollisionShape() const {
    return mCollisionShape;
}

// Return the number of bytes used by the proxy collision shape
inline size_t ProxyConeShape::getSizeInBytes() const {
    return sizeof(ProxyConeShape);
}

// Return a local support point in a given direction with the object margin
inline Vector3 ProxyConeShape::getLocalSupportPointWithMargin(const Vector3& direction) {
    return mCollisionShape->getLocalSupportPointWithMargin(direction);
}

// Return a local support point in a given direction without the object margin
inline Vector3 ProxyConeShape::getLocalSupportPointWithoutMargin(const Vector3& direction) {
    return mCollisionShape->getLocalSupportPointWithoutMargin(direction);
}

// Return the current object margin
inline decimal ProxyConeShape::getMargin() const {
    return mCollisionShape->getMargin();
}

}

#endif

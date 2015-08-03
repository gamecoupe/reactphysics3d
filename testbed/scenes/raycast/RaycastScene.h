/********************************************************************************
* ReactPhysics3D physics library, http://www.reactphysics3d.com                 *
* Copyright (c) 2010-2015 Daniel Chappuis                                       *
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

#ifndef RAYCAST_SCENE_H
#define RAYCAST_SCENE_H

// Libraries
#define _USE_MATH_DEFINES
#include <cmath>
#include "openglframework.h"
#include "reactphysics3d.h"
#include "SceneDemo.h"
#include "Sphere.h"
#include "Box.h"
#include "Cone.h"
#include "Cylinder.h"
#include "Capsule.h"
#include "Line.h"
#include "ConvexMesh.h"
#include "Dumbbell.h"
#include "VisualContactPoint.h"
#include "../common/Viewer.h"

namespace raycastscene {

// Constants
const float SCENE_RADIUS = 30.0f;
const openglframework::Vector3 BOX_SIZE(4, 2, 1);
const float SPHERE_RADIUS = 3.0f;
const float CONE_RADIUS = 3.0f;
const float CONE_HEIGHT = 5.0f;
const float CYLINDER_RADIUS = 3.0f;
const float CYLINDER_HEIGHT = 5.0f;
const float CAPSULE_RADIUS = 3.0f;
const float CAPSULE_HEIGHT = 5.0f;
const float DUMBBELL_HEIGHT = 5.0f;
const int NB_RAYS = 100;
const float RAY_LENGTH = 30.0f;
const int NB_BODIES = 7;

// Raycast manager
class RaycastManager : public rp3d::RaycastCallback {

    private:

        /// All the visual contact points
        std::vector<VisualContactPoint*> mHitPoints;

        /// All the normals at hit points
        std::vector<Line*> mNormals;

        /// Shader
        openglframework::Shader& mShader;

        /// Contact point mesh folder path
        std::string mMeshFolderPath;

   public:

        RaycastManager(openglframework::Shader& shader,
                       const std::string& meshFolderPath)
            : mShader(shader), mMeshFolderPath(meshFolderPath) {

        }

        virtual rp3d::decimal notifyRaycastHit(const rp3d::RaycastInfo& raycastInfo) {
            rp3d::Vector3 hitPos = raycastInfo.worldPoint;
            openglframework::Vector3 position(hitPos.x, hitPos.y, hitPos.z);
            VisualContactPoint* point = new VisualContactPoint(position, mMeshFolderPath);
            mHitPoints.push_back(point);

            // Create a line to display the normal at hit point
            rp3d::Vector3 n = raycastInfo.worldNormal;
            openglframework::Vector3 normal(n.x, n.y, n.z);
            Line* normalLine = new Line(position, position + normal);
            mNormals.push_back(normalLine);

            return raycastInfo.hitFraction;
        }

        void render(const openglframework::Matrix4& worldToCameraMatrix,
                    bool showNormals) {

            // Render all the raycast hit points
            for (std::vector<VisualContactPoint*>::iterator it = mHitPoints.begin();
                 it != mHitPoints.end(); ++it) {
                (*it)->render(mShader, worldToCameraMatrix);
            }

            if (showNormals) {

                // Render all the normals at hit points
                for (std::vector<Line*>::iterator it = mNormals.begin();
                     it != mNormals.end(); ++it) {
                    (*it)->render(mShader, worldToCameraMatrix);
                }
            }
        }

        void resetPoints() {

            // Destroy all the visual contact points
            for (std::vector<VisualContactPoint*>::iterator it = mHitPoints.begin();
                 it != mHitPoints.end(); ++it) {
                delete (*it);
            }
            mHitPoints.clear();

            // Destroy all the normals
            for (std::vector<Line*>::iterator it = mNormals.begin();
                 it != mNormals.end(); ++it) {
                delete (*it);
            }
            mNormals.clear();
        }
};

// Class RaycastScene
class RaycastScene : public SceneDemo {

    private :

        // -------------------- Attributes -------------------- //

        /// Contact point mesh folder path
        std::string mMeshFolderPath;

        /// Raycast manager
        RaycastManager mRaycastManager;

        /// All the raycast lines
        std::vector<Line*> mLines;

        /// Current body index
        int mCurrentBodyIndex;

        /// True if the hit points normals are displayed
        bool mAreNormalsDisplayed;

        /// Raycast manager

        /// All objects on the scene
        Box* mBox;
        Sphere* mSphere;
        Cone* mCone;
        Cylinder* mCylinder;
        Capsule* mCapsule;
        ConvexMesh* mConvexMesh;
        Dumbbell* mDumbbell;

        /// Collision world used for the physics simulation
        rp3d::CollisionWorld* mCollisionWorld;

        /// All the points to render the lines
        std::vector<openglframework::Vector3> mLinePoints;

        /// Vertex Buffer Object for the vertices data
        openglframework::VertexBufferObject mVBOVertices;

        /// Vertex Array Object for the vertex data
        openglframework::VertexArrayObject mVAO;

        /// Create the raycast lines
        void createLines();

        // Create the Vertex Buffer Objects used to render with OpenGL.
        void createVBOAndVAO(openglframework::Shader& shader);


    public:

        // -------------------- Methods -------------------- //

        /// Constructor
        RaycastScene(const std::string& name);

        /// Destructor
        virtual ~RaycastScene();

        /// Update the physics world (take a simulation step)
        /// Can be called several times per frame
        virtual void updatePhysics();

        /// Take a step for the simulation
        virtual void update();

        /// Render the scene in a single pass
        virtual void renderSinglePass(openglframework::Shader& shader,
                                      const openglframework::Matrix4& worldToCameraMatrix);

        /// Reset the scene
        virtual void reset();

        /// Change the body to raycast
        void changeBody();

        /// Display or not the surface normals at hit points
        void showHideNormals();

        /// Called when a keyboard event occurs
        virtual void keyboardEvent(int key, int scancode, int action, int mods);
};

// Display or not the surface normals at hit points
inline void RaycastScene::showHideNormals() {
    mAreNormalsDisplayed = !mAreNormalsDisplayed;
}

}

#endif

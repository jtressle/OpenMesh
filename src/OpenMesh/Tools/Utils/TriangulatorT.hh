/* ========================================================================= *
 *                                                                           *
 *                               OpenMesh                                    *
 *           Copyright (c) 2001-2015, RWTH-Aachen University                 *
 *           Department of Computer Graphics and Multimedia                  *
 *                          All rights reserved.                             *
 *                            www.openmesh.org                               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * This file is part of OpenMesh.                                            *
 *---------------------------------------------------------------------------*
 *                                                                           *
 * Redistribution and use in source and binary forms, with or without        *
 * modification, are permitted provided that the following conditions        *
 * are met:                                                                  *
 *                                                                           *
 * 1. Redistributions of source code must retain the above copyright notice, *
 *    this list of conditions and the following disclaimer.                  *
 *                                                                           *
 * 2. Redistributions in binary form must reproduce the above copyright      *
 *    notice, this list of conditions and the following disclaimer in the    *
 *    documentation and/or other materials provided with the distribution.   *
 *                                                                           *
 * 3. Neither the name of the copyright holder nor the names of its          *
 *    contributors may be used to endorse or promote products derived from   *
 *    this software without specific prior written permission.               *
 *                                                                           *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       *
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED *
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A           *
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER *
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,  *
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,       *
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR        *
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    *
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING      *
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS        *
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.              *
 *                                                                           *
 * ========================================================================= */

/*===========================================================================*\
 *                                                                           *
 *   $Revision$                                                         *
 *   $Date$                   *
 *                                                                           *
\*===========================================================================*/

#ifndef OPENMESH_TRIANGUALTORT_HH
#define OPENMESH_TRIANGUALTORT_HH


//== INCLUDES =================================================================


//#include <OpenMesh/Core/System/config.h>
#include <OpenMesh/Core/Geometry/VectorT.hh>
#include <OpenMesh/Core/Mesh/PolyConnectivity.hh>
#include <algorithm>    // std::for_each

// ------------------------------------------------------------- namespace ----

namespace OpenMesh {
namespace Utils {

// ----------------------------------------------------------------------------

//== CLASS DEFINITION =========================================================


/** Trinagulate vec2 and vec3 meshes
 *
 *  -
 */
template <class Mesh>
class TrinagulatorT
{

public:

    /// constructor
    TrinagulatorT(Mesh& _mesh) : mesh_(_mesh) {}

    /// destructor
    ~TrinagulatorT() {}

private:

    // ref to mesh
    Mesh& mesh_;
    float sign(OpenMesh::Vec2f p1, OpenMesh::Vec2f p2, OpenMesh::Vec2f p3)
    {
        return (p1[0] - p3[0]) * (p2[1] - p3[1]) - (p2[0] - p3[0]) * (p1[1] - p3[1]);
    }


    bool pointInTriangle(OpenMesh::Vec2f pt, OpenMesh::Vec2f v1, OpenMesh::Vec2f v2, OpenMesh::Vec2f v3)
    {
        bool b1, b2, b3;

        b1 = sign(pt, v1, v2) < 0.0f;
        b2 = sign(pt, v2, v3) < 0.0f;
        b3 = sign(pt, v3, v1) < 0.0f;

        return ((b1 == b2) && (b2 == b3));
    }

    bool triangleIntersectingVertex(OpenMesh::VertexHandle vh1, OpenMesh::VertexHandle vh2, OpenMesh::VertexHandle vh3, HalfedgeHandle he, OpenMesh::VPropHandleT<typename Mesh::Point> point2D)
    {

        VertexHandle vh = mesh_.to_vertex_handle(he);

        while(vh != vh1){
            Vec2f p = mesh_.property(point2D,vh);
            if(pointInTriangle(p, mesh_.property(point2D,vh1), mesh_.property(point2D,vh2), mesh_.property(point2D,vh3))) return true;
        }
        return false;
    }

    bool isKonkav(OpenMesh::VertexHandle vh1, OpenMesh::VertexHandle vh2, OpenMesh::VertexHandle vh3, OpenMesh::VPropHandleT<typename Mesh::Point> point2D)
    {       
        Vec2f v1 = mesh_.property(point2D,vh2)-mesh_.property(point2D,vh1);
        Vec2f v2 = mesh_.property(point2D,vh3)-mesh_.property(point2D,vh2);
        float dot = v1[0]*v2[0] + v1[1]*v2[1]; //dot product
        float det = v1[0]*v2[1] - v1[1]*v2[0]; //determinant

        if(std::atan2(det, dot)<0){
            return false;
        }
        else {
            return true;
        }
    }


    void project3dTo2d(HalfedgeHandle he, uint edges, OpenMesh::Vec3f normal, OpenMesh::VPropHandleT<typename Mesh::Point> point2D)
    {

        //lazy 3d to 2d projection by acg:
        Vec3f axis[3] = { Vec3f(1.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f), normal };
        // orthonormalize projection axes
        axis[2].normalize();
        // make sure first axis is linearly independent from the normal

        while (std::abs(axis[0] | axis[2]) > 0.95f || (axis[0].sqrnorm() < 0.001f))
        {
          for (int i = 0; i < 3; ++i)
            axis[0][i] = float(rand()) / float(RAND_MAX) * 2.0f - 1.0f;

          axis[0].normalize();

        }
        // make axis[0] orthogonal to normal
        axis[0] = axis[0] - axis[2] * (axis[0] | axis[2]);
        axis[0].normalize();
        axis[1] = axis[2] % axis[0];

        for(uint i = edges; i!=0; i--){

            VertexHandle vh = mesh_.to_vertex_handle(he);

            Vec2f projectedPoint = Vec2f(axis[0] | Vec3f(mesh_.point(vh)), axis[1] | Vec3f(mesh_.point(vh)));
            mesh_.property(point2D,vh) = projectedPoint;

            he = mesh_.next_halfedge_handle(he);
        }
    }

    bool triangulateT(OpenMesh::FaceHandle& fh_, uint edges, OpenMesh::VPropHandleT<typename Mesh::Point> point2D)
    {

        HalfedgeHandle he(mesh_.halfedge_handle(fh_));
        HalfedgeHandle he_plus1(mesh_.next_halfedge_handle(he));
        HalfedgeHandle he_plus2(mesh_.next_halfedge_handle(he_plus1));
        HalfedgeHandle he_plus3(mesh_.next_halfedge_handle(he_plus2));
        int maxIter = edges*edges+10;

        std::cout<<"Polygon has "<< edges <<" edges."<<std::endl;

        while(edges>3){
            if(isKonkav(mesh_.to_vertex_handle(he), mesh_.to_vertex_handle(he_plus1), mesh_.to_vertex_handle(he_plus2), point2D)){

                if(!triangleIntersectingVertex(mesh_.to_vertex_handle(he), mesh_.to_vertex_handle(he_plus1), mesh_.to_vertex_handle(he_plus2), he_plus3, point2D)){

                    FaceHandle new_fh = mesh_.new_face();
                    HalfedgeHandle new_he_triangle  = mesh_.new_edge(mesh_.to_vertex_handle(he_plus2), mesh_.to_vertex_handle(he));
                    HalfedgeHandle new_he_face = mesh_.opposite_halfedge_handle(new_he_triangle);

                    mesh_.set_halfedge_handle(new_fh, he_plus1);

                    mesh_.set_next_halfedge_handle(he_plus2, new_he_triangle);
                    mesh_.set_next_halfedge_handle(new_he_triangle, he_plus1);

                    mesh_.set_next_halfedge_handle(he, new_he_face);
                    mesh_.set_next_halfedge_handle(new_he_face, he_plus3);

                    mesh_.set_face_handle(he_plus1, new_fh);
                    mesh_.set_face_handle(he_plus2, new_fh);
                    mesh_.set_face_handle(new_he_triangle, new_fh);

                    mesh_.copy_all_properties(fh_, new_fh, true);
                    mesh_.copy_all_properties(he_plus1, new_he_triangle, true);
                    mesh_.copy_all_properties(he, new_he_face, true);

                    edges--;
                }
            }
            he = mesh_.next_halfedge_handle(he);
            he_plus1 = mesh_.next_halfedge_handle(he);
            he_plus2 = mesh_.next_halfedge_handle(he_plus1);
            he_plus3 = mesh_.next_halfedge_handle(he_plus2);

            if(maxIter==0)return false;
            else maxIter--;
        }

        return true;
    }

public:


    bool triangulateTVec2(OpenMesh::FaceHandle fh_)
    {

        OpenMesh::VPropHandleT<typename Mesh::Point> point2D;
        mesh_.add_property(point2D);

        HalfedgeHandle he(mesh_.halfedge_handle(fh_));
        HalfedgeHandle he_iterate(mesh_.next_halfedge_handle(he));

        VertexHandle vh = mesh_.to_vertex_handle(he);
        uint edges = 2;
        //count halfedges and read points / calculate face normal
        while(he!=he_iterate)
        {
            vh = mesh_.to_vertex_handle(he_iterate);
            mesh_.property(point2D,vh) = Vec2f(mesh_.point(vh));

            edges++;
            he_iterate = mesh_.next_halfedge_handle(he_iterate);
        }

        if(edges <= 3) return true;

        //project 3d points to 2d

        return triangulateT(fh_, edges, point2D);

    }

    bool triangulateTVec2()
    {

        std::vector<PolyConnectivity::FaceIter> faces;

        for (PolyConnectivity::FaceIter faceI = mesh_.faces_begin(); faceI != mesh_.faces_end(); ++faceI)
        {
            faces.push_back(faceI);
        }

        bool succeeded = true;

        while(!faces.empty()){

            succeeded &= triangulateTVec2(*(faces.back()));
            faces.pop_back();
        }

        return succeeded;

    }

    bool triangulateTVec3(OpenMesh::FaceHandle fh_)
    {
        OpenMesh::VPropHandleT<typename Mesh::Point> point2D;
        mesh_.add_property(point2D);

        HalfedgeHandle he(mesh_.halfedge_handle(fh_));
        HalfedgeHandle he_iterate(mesh_.next_halfedge_handle(he));

        VertexHandle vh = mesh_.to_vertex_handle(he);
        Vec3f lastPoint = Vec3f(mesh_.point(vh));
        vh = mesh_.to_vertex_handle(he_iterate);

        Vec3f normal = lastPoint % Vec3f(mesh_.point(vh));;
        lastPoint = Vec3f(mesh_.point(vh));

        uint edges = 2;
        //count halfedges and read points / calculate face normal
        while(he!=he_iterate)
        {
            vh = mesh_.to_vertex_handle(he_iterate);
            Vec3f thispoint = Vec3f(mesh_.point(vh));
            normal += lastPoint % thispoint;
            lastPoint = thispoint;

            edges++;
            he_iterate = mesh_.next_halfedge_handle(he_iterate);
        }

        vh = mesh_.to_vertex_handle(he);
        normal += lastPoint % Vec3f(mesh_.point(vh));
        if(edges <= 3) return true;

        //project 3d points to 2d

        project3dTo2d(he, edges, normal, point2D);

        return triangulateT(fh_, edges, point2D);
    }


    bool triangulateTVec3()
    {
        std::vector<PolyConnectivity::FaceIter> faces;

        for (PolyConnectivity::FaceIter faceI = mesh_.faces_begin(); faceI != mesh_.faces_end(); ++faceI)
        {
            faces.push_back(faceI);
        }

        bool succeeded = true;

        while(!faces.empty()){

            succeeded &= triangulateTVec3(*(faces.back()));
            faces.pop_back();
        }

        return succeeded;
    }


};


//=============================================================================
}} //namespaces
#endif
//=============================================================================


//typename Mesh::Point::value_type


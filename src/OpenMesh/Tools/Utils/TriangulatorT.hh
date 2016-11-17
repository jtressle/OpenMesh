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

    bool triangleIntersectingVertex(OpenMesh::VertexHandle vh1, OpenMesh::VertexHandle vh2, OpenMesh::VertexHandle vh3, std::map<OpenMesh::VertexHandle, OpenMesh::Vec2f> &points)
    {
        Vec2f p1 = points[vh1];
        Vec2f p2 = points[vh2];
        Vec2f p3 = points[vh3];

        typedef std::map<VertexHandle, Vec2f>::iterator it_type;
        for(it_type iterator = points.begin(); iterator != points.end(); iterator++) {

            VertexHandle vh = iterator->first;

            if(vh!=vh1 && vh!=vh2 && vh!= vh3){

                Vec2f p = iterator->second;
                if(pointInTriangle(p, p1, p2, p3)) return true;
            }
        }

        return false;
    }

    bool isKonkav(OpenMesh::VertexHandle vh1, OpenMesh::VertexHandle vh2, OpenMesh::VertexHandle vh3, std::map<OpenMesh::VertexHandle, OpenMesh::Vec2f> &points)
    {
        Vec2f v1 = points[vh2]-points[vh1];
        Vec2f v2 = points[vh3]-points[vh2];
        float dot = v1[0]*v2[0] + v1[1]*v2[1]; //dot product
        float det = v1[0]*v2[1] - v1[1]*v2[0]; //determinant
        if(angle = std::atan2(det, dot))return false;
        else return true;
    }


    OpenMesh::Vec2f project3dTo2d(OpenMesh::Vec3f pos, OpenMesh::Vec3f normal)
    {
        Vec3f projection = pos - mesh_.cross( mesh_.dot(pos, normal), normal);
        return Vec2f(projection); //x,y are x,y in 2d or x,z?
    }

    bool triangulateT(OpenMesh::FaceHandle& fh_, std::map<OpenMesh::VertexHandle, OpenMesh::Vec2f> &points)
    {
        int edges = points.size();

        HalfedgeHandle he(mesh_.halfedge_handle(fh_));
        HalfedgeHandle he_plus1(mesh_.next_halfedge_handle(he));
        HalfedgeHandle he_plus2(mesh_.next_halfedge_handle(he_plus1));
        HalfedgeHandle he_plus3(mesh_.next_halfedge_handle(he_plus2));

        while(edges>3){
            if(isKonkav(mesh_.to_vertex_handle(he), mesh_.to_vertex_handle(he_plus1), mesh_.to_vertex_handle(he_plus2), points)){

                if(!triangleIntersectingVertex(mesh_.to_vertex_handle(he), mesh_.to_vertex_handle(he_plus1), mesh_.to_vertex_handle(he_plus2), points)){

                    FaceHandle new_fh = mesh_.new_face();
                    HalfedgeHandle new_he_triangle  = new_edge(mesh_.to_vertex_handle(he_plus2), mesh_.to_vertex_handle(he));
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
        }

        return;
    }

public:


    bool triangulateTVec2(OpenMesh::FaceHandle& fh_)
    {
        HalfedgeHandle he(mesh_.halfedge_handle(fh_));
        HalfedgeHandle he_iterate(mesh_.next_halfedge_handle(he));

        std::map<VertexHandle, Vec2f> points;

        VertexHandle vh = mesh_.to_vertex_handle(he);
        points[vh]=Vec2f(mesh_.point(vh));

        while(he!=he_iterate){
            vh = mesh_.to_vertex_handle(he_iterate);
            points[vh]=Vec2f(mesh_.point(vh));
            he_iterate = mesh_.next_halfedge_handle(he_iterate);
        }

        triangulateT(fh_, points);

        return true;
    }

    bool triangulateTVec2()
    {
        bool ret = true;
        for (PolyConnectivity::FaceIter faceIter = mesh_.faces_begin(); faceIter != mesh_.faces_end(); ++faceIter) {
            ret = ret && mesh_.triangulateTVec2(*faceIter);
        }
        return ret;
    }

    bool triangulateTVec3(OpenMesh::FaceHandle& fh_)
    {

        HalfedgeHandle he(mesh_.halfedge_handle(fh_));
        HalfedgeHandle he_iterate(mesh_.next_halfedge_handle(he));

        std::map<VertexHandle, Vec3f> points3d;
        std::map<VertexHandle, Vec2f> points;

        VertexHandle vh = mesh_.to_vertex_handle(he);
        points3d[vh]=Vec3f(mesh_.point(vh));

        //count halfedges and read points

        while(he!=he_iterate)
        {
            vh = mesh_.to_vertex_handle(he_iterate);
            points3d[vh]=Vec3f(mesh_.point(vh));
            he_iterate = mesh_.next_halfedge_handle(he_iterate);
        }

        //calculate face normal

        Vec3f normal;

        {
            typedef std::map<VertexHandle, Vec3f>::iterator it_type;
            it_type iterator = points3d.begin();

            Vec3f lastPoint = iterator->second;
            iterator++;

            for(iterator; iterator != points3d.end(); iterator++)
            {
                Vec3f thispoint = iterator->second;
                normal += lastPoint % thispoint;
                lastPoint = thispoint;
            }

            normal += lastPoint % points3d.begin()->second;
        }

        //project 3d points to 2d

        typedef std::map<VertexHandle, Vec3f>::iterator it_type;
        for(it_type iterator = points3d.begin(); iterator != points3d.end(); iterator++)
        {
            VertexHandle vh = iterator->first;
            points[vh]=project3dTo2d(iterator->second, normal);
        }

        triangulateT(fh_, points);

        return true;
    }


    bool triangulateTVec3()
    {
        bool ret = true;

        for (PolyConnectivity::FaceIter faceIter = mesh_.faces_begin(); faceIter != mesh_.faces_end(); ++faceIter) {
            ret = ret && mesh_.triangulateTVec3(*faceIter);
        }
        return ret;
    }


};


//=============================================================================
}} //namespaces
#endif
//=============================================================================

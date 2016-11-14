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


#define OPENMESH_TRIANGUALTORT_CC


//== INCLUDES =================================================================


#include <OpenMesh/Tools/Utils/TriangulatorT.hh>
#include <math.h> //atan2
//#include <list>

//== NAMESPACES ============================================================== 


namespace OpenMesh {


//== IMPLEMENTATION ========================================================== 

template <class Mesh> //Typ z.B. OpenMesh::PolyMesh_ArrayKernelT<OpenMesh::Vec2i>


float TriangulatorT<Mesh>::sign (vec2f p1, vec2f p2, vec2f p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool TriangulatorT<Mesh>::pointInTriangle (vec2f pt, vec2f v1, vec2f v2, vec2f v3)
{
    bool b1, b2, b3;

    b1 = sign(pt, v1, v2) < 0.0f;
    b2 = sign(pt, v2, v3) < 0.0f;
    b3 = sign(pt, v3, v1) < 0.0f;

    return ((b1 == b2) && (b2 == b3));
}

bool TriangulatorT<Mesh>::triangleIntersectingVertex(VertexHandle vh1, VertexHandle vh2, VertexHandle vh3, std::map<VertexHandle, vec2f> &points)
{

    vec2f p1 = points(vh1);
    vec2f p2 = points(vh2);
    vec2f p3 = points(vh3);

    typedef std::map<VertexHandle, vec2f>::iterator it_type;
    for(it_type iterator = points.begin(); iterator != points.end(); iterator++) {

        VertexHandle vh = iterator->first;

        if(vh!=vh1 && vh!=vh2 && vh!= vh3){

            vec2f p = iterator->second;
            if(pointInTriangle(p, p1, p2, p3)) return true;
        }
    }

    return false;
}


bool TriangulatorT<Mesh>::isKonkav(VertexHandle vh1, VertexHandle vh2, VertexHandle vh3, std::map<VertexHandle, vec2f> &points)
{

    vec2f v1 = points(vh2)-points(vh1);
    vec2f v2 = points(vh3)-points(vh2);
    float dot = v1.x*v2.x + v1.y*v2.y; //dot product
    float det = v1.x*v2.y - v1.y*v2.x; //determinant
    if(angle = std::atan2(det, dot))return false;
    else return true;
}

vec2f TriangulatorT<Mesh>::project3dTo2d(vec3f pos, vec3f normal){

    vec3f projection = pos - (pos | normal) % normal;
    return vec2f(projection); //x,y are x,y in 2d or x,z?
}

bool TriangulatorT<Mesh>::triangulateTVec2(FaceHandle& fh_)
{
    HalfedgeHandle he(halfedge_handle(fh_));
    HalfedgeHandle he_iterate(next_halfedge_handle(he));

    std::map<VertexHandle, vec2f> points;

    VertexHandle vh = to_vertex_handle(he);
    points[vh]=vec2f(0.0,0.0);//TODO get position

    while(he!=he_iterate){
        vh = to_vertex_handle(he_iterate);
        points[vh]=vec2f(0.0,0.0);//TODO get position
        he_iterate = next_halfedge_handle(he_iterate);
    }

    triangulateT(fh_, points);
}

bool TriangulatorT<Mesh>::triangulateTVec3(FaceHandle& fh_)
{

    HalfedgeHandle he(halfedge_handle(fh_));
    HalfedgeHandle he_iterate(next_halfedge_handle(he));

    std::map<VertexHandle, vec3f> points3d;
    std::map<VertexHandle, vec2f> points;

    VertexHandle vh = to_vertex_handle(he);
    points3d[vh]=vec3f(0.0,0.0,0.0);//TODO get position

    //count halfedges and read points

    while(he!=he_iterate)
    {
        vh = to_vertex_handle(he_iterate);
        points3d[vh]=vec3f(0.0,0.0,0.0);//TODO get position
        he_iterate = next_halfedge_handle(he_iterate);
    }

    //calculate face normal

    vec3f normal;

    {
        typedef std::map<VertexHandle, vec3f>::iterator it_type;
        it_type iterator = points3d.begin();

        vec3f lastPoint = iterator->second;
        iterator++;

        for(iterator; iterator != points3d.end(); iterator++)
        {
            vec3f thispoint = iterator->second;
            normal += lastPoint % thispoint;
            lastPoint = thispoint;
        }

        normal += lastPoint % points3d.begin()->second;
    }

    //project 3d points to 2d

    typedef std::map<VertexHandle, vec3f>::iterator it_type;
    for(it_type iterator = points3d.begin(); iterator != points3d.end(); iterator++)
    {
        VertexHandle vh = iterator->first;
        points[vh]=project3dTo2d(iterator->second, normal);
    }

    triangulateT(fh_, points);
}

bool TriangulatorT<Mesh>::triangulateT(FaceHandle& fh_, std::map<VertexHandle, vec2f> &points)
{
    int edges = points.size();

    HalfedgeHandle he(halfedge_handle(fh_));
    HalfedgeHandle he_plus1(next_halfedge_handle(he));
    HalfedgeHandle he_plus2(next_halfedge_handle(he_plus1));
    HalfedgeHandle he_plus3(next_halfedge_handle(he_plus2));

    while(edges>3){
        if(isKonkav(to_vertex_handle(he), to_vertex_handle(he_plus1), to_vertex_handle(he_plus2), points)){

            if(!triangleIntersectingVertex(to_vertex_handle(he), to_vertex_handle(he_plus1), to_vertex_handle(he_plus2), points)){

                FaceHandle new_fh = new_face();
                HalfedgeHandle new_he_triangle  = new_edge(to_vertex_handle(he_plus2), to_vertex_handle(he));
                HalfedgeHandle new_he_face = opposite_halfedge_handle(new_he_triangle);

                set_halfedge_handle(new_fh, he_plus1);

                set_next_halfedge_handle(he_plus2, new_he_triangle);
                set_next_halfedge_handle(new_he_triangle, he_plus1);

                set_next_halfedge_handle(he, new_he_face);
                set_next_halfedge_handle(new_he_face, he_plus3);

                set_face_handle(he_plus1, new_fh);
                set_face_handle(he_plus2, new_fh);
                set_face_handle(new_he_triangle, new_fh);

                copy_all_properties(fh_, new_fh, true);
                copy_all_properties(he_plus1, new_he_triangle, true);
                copy_all_properties(he, new_he_face, true);

                edges--;
            }

        }

        he = next_halfedge_handle(he);
        he_plus1 = next_halfedge_handle(he);
        he_plus2 = next_halfedge_handle(he_plus1);
        he_plus3 = next_halfedge_handle(he_plus2);
    }

    return;


}





//=============================================================================
} // namespace OpenMesh
//=============================================================================

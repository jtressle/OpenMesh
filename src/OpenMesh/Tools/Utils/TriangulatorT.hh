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




//== NAMESPACES ===============================================================


namespace OpenMesh {
namespace Utils {

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
  TrinagulatorT(const Mesh& _mesh) : mesh_(_mesh) {}
 
  /// destructor
  ~TrinagulatorT() {}

    bool triangulateTVec2(FaceHandle& fh_);
    bool triangulateTVec2();
    bool triangulateTVec3(FaceHandle& fh_);
    bool triangulateTVec3();


private:

    // ref to mesh
    const Mesh& mesh_;

    float sign(vec2f p1, vec2f p2, vec2f p3);
    bool pointInTriangle(vec2f pt, vec2f v1, vec2f v2, vec2f v3);
    bool triangleIntersectingVertex(VertexHandle vh1, VertexHandle vh2, VertexHandle vh3, std::map<VertexHandle, vec2f> &points);
    bool isKonkav(VertexHandle vh1, VertexHandle vh2, VertexHandle vh3, std::map<VertexHandle, vec2f> &points);
    vec2f project3dTo2d(vec3f pos, vec3f normal);

    bool triangulateT(FaceHandle& fh_, std::map<VertexHandle, vec2f> &points);


};

//=============================================================================
#endif
//=============================================================================

#include <gtest/gtest.h>
#include <Unittests/unittests_common.hh>

#include <iostream>

struct CustomTraitsVec2i : OpenMesh::DefaultTraits
{
    typedef OpenMesh::Vec2i Point;
};



typedef OpenMesh::PolyMesh_ArrayKernelT<CustomTraitsVec2i> PolyMeshVec2i;



/*
 * OpenMesh Poly with Vec2i
 */

class OpenMeshBasePolyVec2i : public testing::Test {

    protected:

        // This function is called before each test is run
        virtual void SetUp() {

            // Do some initial stuff with the member data here...
        }

        // This function is called after all tests are through
        virtual void TearDown() {

            // Do some final stuff with the member data here...
        }

    // This member will be accessible in all tests
   PolyMeshVec2i mesh_;
};



namespace {

/*
 * ====================================================================
 * Define tests below
 * ====================================================================
 */


/*
 * Checking for feature edges based on angle
 */
TEST_F(OpenMeshBasePolyVec2i, Instance_Vec2i_Mesh) {

  mesh_.clear();

  // Add some vertices
  PolyMeshVec2i::VertexHandle vhandle[4];

  vhandle[0] = mesh_.add_vertex(PolyMeshVec2i::Point(0, 0));
  vhandle[1] = mesh_.add_vertex(PolyMeshVec2i::Point(0, 1));
  vhandle[2] = mesh_.add_vertex(PolyMeshVec2i::Point(1, 1));

  // Add face
  std::vector<PolyMeshVec2i::VertexHandle> face_vhandles;

  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[2]);
  mesh_.add_face(face_vhandles);

  // ===============================================
  // Setup complete
  // ===============================================

  // Check one Request only vertex normals
  // Face normals are required for vertex and halfedge normals, so
  // that prevent access to non existing properties are in place

  mesh_.request_vertex_normals();
  mesh_.request_halfedge_normals();
  mesh_.request_face_normals();

}

/* Triangulates a polygon
 */
TEST_F(OpenMeshBasePolyVec2i, TriangualtePolygon) {

  mesh_.clear();

  // Add some vertices
  PolyMeshVec2i::VertexHandle vhandle[6];

  vhandle[0] = mesh_.add_vertex(Mesh::Point(0, 0));
  vhandle[1] = mesh_.add_vertex(Mesh::Point(2, 0));
  vhandle[2] = mesh_.add_vertex(Mesh::Point(1, 2));
  vhandle[3] = mesh_.add_vertex(Mesh::Point(2, 4));
  vhandle[4] = mesh_.add_vertex(Mesh::Point(0, 1));
  vhandle[5] = mesh_.add_vertex(Mesh::Point(1, 1));

  // Add two faces
  std::vector<PolyMeshVec2i::VertexHandle> face_vhandles;

  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[3]);
  face_vhandles.push_back(vhandle[4]);
  face_vhandles.push_back(vhandle[5]);

  mesh_.add_face(face_vhandles);
  face_vhandles.clear();



  //TODO: triangulate face

  // Test setup:
  //        3
  //       //
  //      //
  //     //
  //    /2
  //   /  \
  //  4--5 \
  //    /   \
  //  0-----1



  // Check setup
  EXPECT_EQ(4u, mesh_.n_faces() )    << "Wrong number of faces";

  //TODO: test if triangualtion works. Following triangles are allowed: 015,125,234,245

}

}

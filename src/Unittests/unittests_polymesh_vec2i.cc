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
TEST_F(OpenMeshBasePolyVec2i, Triangualte2DPolygon) {

  mesh_.clear();

  // Add some vertices
  PolyMeshVec2i::VertexHandle vhandle[6];

  vhandle[0] = mesh_.add_vertex(PolyMeshVec2i::Point(0, 0));
  vhandle[1] = mesh_.add_vertex(PolyMeshVec2i::Point(2, 0));
  vhandle[2] = mesh_.add_vertex(PolyMeshVec2i::Point(1, 2));
  vhandle[3] = mesh_.add_vertex(PolyMeshVec2i::Point(2, 4));
  vhandle[4] = mesh_.add_vertex(PolyMeshVec2i::Point(0, 1));
  vhandle[5] = mesh_.add_vertex(PolyMeshVec2i::Point(1, 1));

  // Add two faces
  std::vector<PolyMeshVec2i::VertexHandle> face_vhandles;

  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[3]);
  face_vhandles.push_back(vhandle[4]);
  face_vhandles.push_back(vhandle[5]);

  PolyMeshVec2i::HalfedgeHandle hedge_vhandles[6];
  hedge_vhandles[0]= mesh_.halfedge_handle(mesh_.add_face(face_vhandles));  //5-0
  hedge_vhandles[1]=mesh_.next_halfedge_handle(hedge_vhandles[0]);          //4-5
  hedge_vhandles[2]=mesh_.next_halfedge_handle(hedge_vhandles[1]);          //3-4
  hedge_vhandles[3]=mesh_.next_halfedge_handle(hedge_vhandles[2]);          //2-3
  hedge_vhandles[4]=mesh_.next_halfedge_handle(hedge_vhandles[3]);          //1-2
  hedge_vhandles[5]=mesh_.next_halfedge_handle(hedge_vhandles[4]);          //0-1

  //mesh_.add_face(face_vhandles);
  face_vhandles.clear();

  if(vhandle[1]==mesh_.to_vertex_handle(hedge_vhandles[0])&&vhandle[0]==mesh_.from_vertex_handle(hedge_vhandles[0])){
      std::cout<<"!!!Handle returns first edge!!!"<<std::endl;
  }else if(vhandle[0]==mesh_.to_vertex_handle(hedge_vhandles[0])&&vhandle[5]==mesh_.from_vertex_handle(hedge_vhandles[0])){
      //std::cout<<"!!!Handle returns last edge!!!"<<std::endl;
  }else{
      std::cout<<"!!!Handle returns shit!!!"<<std::endl;
  }


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


  mesh_.triangulate();

  if(mesh_.next_halfedge_handle(mesh_.next_halfedge_handle(mesh_.next_halfedge_handle(hedge_vhandles[0]))) == hedge_vhandles[0]
          && mesh_.next_halfedge_handle(mesh_.next_halfedge_handle(mesh_.next_halfedge_handle(hedge_vhandles[2]))) == hedge_vhandles[2]
          && mesh_.next_halfedge_handle(mesh_.next_halfedge_handle(mesh_.next_halfedge_handle(hedge_vhandles[3]))) == hedge_vhandles[3]
          && mesh_.next_halfedge_handle(mesh_.next_halfedge_handle(mesh_.next_halfedge_handle(hedge_vhandles[5]))) == hedge_vhandles[5]){

        if(mesh_.next_halfedge_handle(hedge_vhandles[0]) == hedge_vhandles[1]
          && mesh_.next_halfedge_handle(hedge_vhandles[1]) != hedge_vhandles[2]
          && mesh_.next_halfedge_handle(hedge_vhandles[2]) != hedge_vhandles[3]
          && mesh_.next_halfedge_handle(hedge_vhandles[3]) == hedge_vhandles[4]
          && mesh_.next_halfedge_handle(hedge_vhandles[4]) != hedge_vhandles[5]
          && mesh_.next_halfedge_handle(hedge_vhandles[5]) != hedge_vhandles[0]){

            EXPECT_TRUE(true);
        }else{
            std::cout<<"[          ] bad triangulation"<<std::endl;
            EXPECT_TRUE(false);
        }

  }else{
      std::cout<<"[          ] no triangulation"<<std::endl;
      EXPECT_TRUE(false);
  }


}

}

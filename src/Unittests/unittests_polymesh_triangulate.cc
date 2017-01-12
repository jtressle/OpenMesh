#include <gtest/gtest.h>
#include <Unittests/unittests_common.hh>
#include <time.h>
#include <iostream>

#include <OpenMesh/Tools/Utils/TriangulatorT.hh>


typedef OpenMesh::PolyMesh_ArrayKernelT<OpenMesh::DefaultTraits> PolyMeshVec3f;



/*
 * OpenMesh Poly with Vec2i
 */

class OpenMeshBasePolyVec3f : public testing::Test {

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
   PolyMeshVec3f mesh_;
};



namespace {

/*
 * ====================================================================
 * Define tests below
 * ====================================================================
 */


/* Triangulates a polygon
 */
TEST_F(OpenMeshBasePolyVec3f, Triangualte3DPolygon) {

  mesh_.clear();

  // Add some vertices
  PolyMeshVec3f::VertexHandle vhandle[6];

  vhandle[0] = mesh_.add_vertex(PolyMeshVec3f::PolyMeshT::Point(0.0, 0.0, 0.0));
  vhandle[1] = mesh_.add_vertex(PolyMeshVec3f::PolyMeshT::Point(2.0, 0.0, 0.0));
  vhandle[2] = mesh_.add_vertex(PolyMeshVec3f::PolyMeshT::Point(1.0, 0.0, 2.0));
  vhandle[3] = mesh_.add_vertex(PolyMeshVec3f::PolyMeshT::Point(2.0, 0.0, 4.1));
  vhandle[4] = mesh_.add_vertex(PolyMeshVec3f::PolyMeshT::Point(0.0, 0.0, 1.0));
  vhandle[5] = mesh_.add_vertex(PolyMeshVec3f::PolyMeshT::Point(1.0, 0.0, 1.1));

  // Add two faces
  std::vector<PolyMeshVec3f::VertexHandle> face_vhandles;

  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[3]);
  face_vhandles.push_back(vhandle[4]);
  face_vhandles.push_back(vhandle[5]);

  PolyMeshVec3f::HalfedgeHandle hedge_vhandles[6];
  hedge_vhandles[0]= mesh_.halfedge_handle(mesh_.add_face(face_vhandles));  //5-0
  hedge_vhandles[1]=mesh_.next_halfedge_handle(hedge_vhandles[0]);          //4-5
  hedge_vhandles[2]=mesh_.next_halfedge_handle(hedge_vhandles[1]);          //3-4
  hedge_vhandles[3]=mesh_.next_halfedge_handle(hedge_vhandles[2]);          //2-3
  hedge_vhandles[4]=mesh_.next_halfedge_handle(hedge_vhandles[3]);          //1-2
  hedge_vhandles[5]=mesh_.next_halfedge_handle(hedge_vhandles[4]);          //0-1

  //mesh_.add_face(face_vhandles);
  face_vhandles.clear();

  /*if(vhandle[1]==mesh_.to_vertex_handle(hedge_vhandles[0])&&vhandle[0]==mesh_.from_vertex_handle(hedge_vhandles[0])){
      std::cout<<"!!!Handle returns first edge!!!"<<std::endl;
  }else if(vhandle[0]==mesh_.to_vertex_handle(hedge_vhandles[0])&&vhandle[5]==mesh_.from_vertex_handle(hedge_vhandles[0])){
      //std::cout<<"!!!Handle returns last edge!!!"<<std::endl;
  }else{
      std::cout<<"!!!Handle returns shit!!!"<<std::endl;
  }*/


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

  clock_t start=clock();

  bool succeeded = OpenMesh::Utils::TrinagulatorT<PolyMeshVec3f>(mesh_).triangulateTVec3();

  clock_t finish=clock();

  if(!succeeded){
      std::cout<<"[          ] triangulation did not stop!"<<std::endl;
      EXPECT_TRUE(false);
      return;
  }

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

            std::cout<<"[          ] triangualtion succeeded in "<<(float)(finish-start)/(CLOCKS_PER_SEC /1000.0 )<<" ms."<<std::endl;

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

/* Triangulates a polygon
 */
TEST_F(OpenMeshBasePolyVec3f, Triangualte3DPolygonNoSurface) {

  mesh_.clear();

  // Add some vertices
  PolyMeshVec3f::VertexHandle vhandle[4];

  vhandle[0] = mesh_.add_vertex(PolyMeshVec3f::PolyMeshT::Point(0.0, 0.0, 0.0));
  vhandle[1] = mesh_.add_vertex(PolyMeshVec3f::PolyMeshT::Point(1.0, 0.0, 0.0));
  vhandle[2] = mesh_.add_vertex(PolyMeshVec3f::PolyMeshT::Point(3.0, 0.0, 0.0));
  vhandle[3] = mesh_.add_vertex(PolyMeshVec3f::PolyMeshT::Point(2.0, 0.0, 0.0));

  // Add two faces
  std::vector<PolyMeshVec3f::VertexHandle> face_vhandles;

  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[3]);

  PolyMeshVec3f::HalfedgeHandle hedge_vhandles[4];
  hedge_vhandles[0]= mesh_.halfedge_handle(mesh_.add_face(face_vhandles));  //3-0
  hedge_vhandles[1]=mesh_.next_halfedge_handle(hedge_vhandles[0]);          //2-3
  hedge_vhandles[2]=mesh_.next_halfedge_handle(hedge_vhandles[1]);          //1-2
  hedge_vhandles[3]=mesh_.next_halfedge_handle(hedge_vhandles[2]);          //0-1

  //mesh_.add_face(face_vhandles);
  face_vhandles.clear();


  // Test setup:
  //  0-1-3-2

  bool succeeded = OpenMesh::Utils::TrinagulatorT<PolyMeshVec3f>(mesh_).triangulateTVec3();

  if(!succeeded){
      std::cout<<"[          ] triangulation does not stop on meshes withoutsurface!"<<std::endl;
      EXPECT_TRUE(false);
      return;
  }else{
      EXPECT_TRUE(true);
  }

}

}

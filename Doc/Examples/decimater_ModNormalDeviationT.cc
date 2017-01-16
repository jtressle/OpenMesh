using namespace OpenMesh;

typedef TriMesh_ArrayKernelT<>               		Mesh;
typedef Decimater::DecimaterT<Mesh>          		Decimater;
typedef Decimater::ModNormalDeviationT<Mesh>::Handle 	HModNormalDeviation;

Mesh        mesh;             // a mesh object
Decimater   decimater(mesh);  // a decimater object, connected to a mesh

HModNormalDeviation hModNormalDeviation;				// use a normal deviation module
decimater.add(hModNormalDeviation); 					// register deviation module at the decimater
decimater.module(hModNormalDeviation).set_binary(false); 		// if the normal deviation module is not the primary module set binary to true;
decimater.module(hModNormalDeviation).set_normal_deviation(15.0);	// set max angle between normals in degrees

decimater.initialize();
decimater.decimate();
// after decimation: remove decimated elements from the mesh
mesh.garbage_collection();

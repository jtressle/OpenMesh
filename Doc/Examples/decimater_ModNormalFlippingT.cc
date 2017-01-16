using namespace OpenMesh;

typedef TriMesh_ArrayKernelT<>               		Mesh;
typedef Decimater::DecimaterT<Mesh>          		Decimater;
typedef Decimater::ModNormalFlippingT<Mesh>::Handle 	HModNormalFlipping;
typedef Decimater::ModNormalDeviationT<Mesh>::Handle 	HModNormalDeviation;

Mesh        mesh;             // a mesh object
Decimater   decimater(mesh);  // a decimater object, connected to a mesh

HModNormalFlipping hModNormalFlipping;  				// use a normal flipping module
decimater.add(hModNormalFlipping); 					// register the normal flipping module
decimater.module(hModNormalFlipping).set_max_normal_deviation(45.0);	// set the maximum normal deviation after collapse

//note that ModNormalFlipping only supports binary mode and has to be used in combination with a non-binary module
HModNormalDeviation hModNormalDeviation;				// use a non-binary module primarily
decimater.add(hModNormalDeviation); 					// register the module at the decimater
decimater.module(hModNormalDeviation).set_binary(false); 		// choose non-binary mode

decimater.initialize();
decimater.decimate();
// after decimation: remove decimated elements from the mesh
mesh.garbage_collection();

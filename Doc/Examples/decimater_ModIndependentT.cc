using namespace OpenMesh;

typedef TriMesh_ArrayKernelT<>               		Mesh;
typedef Decimater::DecimaterT<Mesh>          		Decimater;
typedef Decimater::ModIndependentT<Mesh>::Handle 	HModIndependent;
typedef Decimater::ModNormalDeviationT<Mesh>::Handle 	HModNormalDeviation;

Mesh        mesh;             // a mesh object
Decimater   decimater(mesh);  // a decimater object, connected to a mesh

HModIndependent hModIndependent;  				// use a independence module

//note that ModIndependent only supports binary mode and has to be used in combination with a non-binary module
HModNormalDeviation hModNormalDeviation;			// use a non-binary module primarily
decimater.add(hModNormalDeviation); 				// register the module at the decimater
decimater.module(hModNormalDeviation).set_binary(false); 	// choose non-binary mode

decimater.initialize();
decimater.decimate();
// after decimation: remove decimated elements from the mesh
mesh.garbage_collection();

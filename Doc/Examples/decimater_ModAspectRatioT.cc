using namespace OpenMesh;

typedef TriMesh_ArrayKernelT<>               		Mesh;
typedef Decimater::DecimaterT<Mesh>          		Decimater;
typedef Decimater::ModAspectRatioT<Mesh>::Handle 	HModAspectRatio;

Mesh        mesh;             // a mesh object
Decimater   decimater(mesh);  // a decimater object, connected to a mesh

HModAspectRatio hModAspectRatio;					// use an aspect ratio module
decimater.add(hModAspectRatio);						// register the aspect ratio module
decimater.module(hModAspectRatio).set_binary(false); 			// if the aspect ratio module is not the primary module set binary to true;
decimater.module(hModAspectRatio).set_aspect_ratio(3.0);   		// prevents collapsses which result in a smaller aspect ratio than the chosen minimum

decimater.initialize();
decimater.decimate();
// after decimation: remove decimated elements from the mesh
mesh.garbage_collection();

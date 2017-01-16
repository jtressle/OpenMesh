using namespace OpenMesh;

typedef TriMesh_ArrayKernelT<>               		Mesh;
typedef Decimater::DecimaterT<Mesh>          		Decimater;
typedef Decimater::ModNormalDeviationT<Mesh>::Handle 	HModNormalDeviation;
typedef Decimater::ModAspectRatioT<Mesh>::Handle 	HModAspectRatio;
typedef Decimater::ModHausdorffT<Mesh>::Handle 		HModHausdorff;


Mesh        mesh;             // a mesh object
Decimater   decimater(mesh);  // a decimater object, connected to a mesh

HModNormalDeviation hModNormalDeviation;				// use a normal deviation module primarily
decimater.add(hModNormalDeviation); 					// register deviation module at the decimater
decimater.module(hModNormalDeviation).set_binary(false); 		// exact one module must be non-binary
decimater.module(hModNormalDeviation).set_normal_deviation(15.0);	// set max angle between normals in degrees
std::cout << decimater.module(hModNormalDeviation).name() << std::endl; // module access

HModAspectRatio hModAspectRatio;					// use an aspect ratio module
decimater.add(hModAspectRatio);						// register the second module
decimater.module(hModAspectRatio).set_binary(true); 			// further modules must be binary
decimater.module(hModAspectRatio).set_aspect_ratio(3.0);   		// prevents collapsses which result in a smaller aspect ratio than the chosen minimum

HModHausdorff hModHausdorff;  						// use a hausdorff distance module
decimater.add(hModHausdorff); 						// register the third module
//note that ModHausdorff only supports binary mode
decimater.module(hModHausdorff).set_tolerance(10.0);			// set max Hausdorff distance tollerance

decimater.initialize();
decimater.decimate();
// after decimation: remove decimated elements from the mesh
mesh.garbage_collection();

using namespace OpenMesh;

typedef TriMesh_ArrayKernelT<>               		Mesh;
typedef Decimater::DecimaterT<Mesh>          		Decimater;
typedef Decimater::ModRoundnessT<Mesh>::Handle 		HModRoundness;

Mesh        mesh;             // a mesh object
Decimater   decimater(mesh);  // a decimater object, connected to a mesh

HModRoundness hModRoundness;						// use a roundness module
decimater.add(hModRoundness);						// register the roundness module

//in non-binary mode set OR unset minimal roundness
decimater.module(hModRoundness).set_min_roundness(0.5, false);		// choose a minimal roundness value between 0 and 1.0 and set binary mode to false
decimater.module(hModRoundness).unset_min_roundness();			// or unset minimal roundness

//in binary mode choose a minimal roundness OR angle
decimater.module(hModRoundness).set_min_roundness(0.5, true);		// choose a minimal roundness value between 0 and 1.0 and set binary mode to true
decimater.module(hModRoundness).set_min_angle(15.0, true);		// or choose a minimal angle in degrees. second argument gets ignored

decimater.initialize();
decimater.decimate();
// after decimation: remove decimated elements from the mesh
mesh.garbage_collection();

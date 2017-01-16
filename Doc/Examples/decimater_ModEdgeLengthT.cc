using namespace OpenMesh;

typedef TriMesh_ArrayKernelT<>               		Mesh;
typedef Decimater::DecimaterT<Mesh>          		Decimater;
typedef Decimater::ModEdgeLengthT<Mesh>::Handle 	HModEdgeLength;

Mesh        mesh;             // a mesh object
Decimater   decimater(mesh);  // a decimater object, connected to a mesh

HModEdgeLength hModEdgeLength;				// use an edge length module
decimater.add(hModEdgeLength);				// register the edge length module
decimater.module(hModEdgeLength).set_binary(false); 	// if the edge length module is not the primary module set binary to true;
decimater.module(hModEdgeLength).set_edge_length (4.0);	// choose the maximum length of edges to be collapsed

decimater.initialize();
decimater.decimate();
// after decimation: remove decimated elements from the mesh
mesh.garbage_collection();

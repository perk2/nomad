#include "demofw.h"

Model::Model(){
	/* The default constructor creates a test-triangle. */
	vertex_data = {	0.0f, 0.0f, -0.5f,
					0.5f, 0.0f, -0.5f,
					0.5f, 0.5f, -0.5f};
	vertex_indices = {0,1,2};
	num_indices = vertex_indices.size();
	bind_buffers();
}

Model::Model(const std::string& filename){
	import_collada(filename);
}

bool Model::import_collada(const std::string& filename){
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filename,
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices);

	if(!scene){
		std::cout << "Couldn't import the collada file." << std::endl;
		return false;
	}
	process_scene(scene);
	return true;
}

/*
 * Processes a scene returned by the asset importer. A scene contains the data
 * required to render a model.
 */
bool Model::process_scene(const aiScene* scene){
	assert(scene->HasMeshes());
	/* TODO: Remove. */
	std::cout << "Mesh0 contains:" << std::endl;
	std::cout << scene->mMeshes[0]->mNumVertices << " vertices." << std::endl;
	std::cout << scene->mMeshes[0]->mNumFaces << " faces." << std::endl;
	std::cout << "Prim.type>: " << scene->mMeshes[0]->mPrimitiveTypes  << std::endl;

	/*
	 * Process all the meshes in the scene:
	 * 1. Populate the vertex vector with all the vertices of the meshes.
	 * 2. Populate the index vector with all the mesh-indices.
	 * TODO: Can reduce the number of calls to push_back here.
	 */
	for(unsigned int i=0; i<scene->mNumMeshes;i++){
		assert(scene->mMeshes[i]->HasPositions());
		for(unsigned int j=0; j<scene->mMeshes[i]->mNumVertices;j++){
			/* mVertices is an array of 3D vectors. Store xyz in vertex_data */
			vertex_data.push_back(scene->mMeshes[i]->mVertices[j].x);
			vertex_data.push_back(scene->mMeshes[i]->mVertices[j].y);
			vertex_data.push_back(scene->mMeshes[i]->mVertices[j].z);
			/* Assume that all models have vertex normals */
			assert(scene->mMeshes[i]->HasNormals());
			vertex_normals.push_back(scene->mMeshes[i]->mNormals[j].x);
			vertex_normals.push_back(scene->mMeshes[i]->mNormals[j].y);
			vertex_normals.push_back(scene->mMeshes[i]->mNormals[j].z);
		}
		for(unsigned int j=0; j<scene->mMeshes[i]->mNumFaces;j++){
			/* Check that we only have triangles. */
			assert(scene->mMeshes[i]->mFaces[j].mNumIndices==3);
			num_indices = scene->mMeshes[i]->mNumFaces * 3;
			vertex_indices.push_back(scene->mMeshes[i]->mFaces[j].mIndices[0]);
			vertex_indices.push_back(scene->mMeshes[i]->mFaces[j].mIndices[1]);
			vertex_indices.push_back(scene->mMeshes[i]->mFaces[j].mIndices[2]);
		}
	}
	bind_buffers();

	return true;
}

void Model::bind_buffers(){
	GLuint buffers[3];
	glGenBuffers(3, buffers);
	vbo_handle = buffers[0];
	ebo_handle = buffers[1];
	vbo_normals_handle = buffers[2];

	/* Store vertex data */
	glBindBuffer(GL_ARRAY_BUFFER, vbo_handle);
	glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(GLfloat),
			vertex_data.data(),
			GL_STATIC_DRAW);
	/* Store indices */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_handle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			vertex_indices.size() * sizeof(GLushort),
			vertex_indices.data(), GL_STATIC_DRAW);
	/* Store vertex normals */
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normals_handle);
	glBufferData(GL_ARRAY_BUFFER, vertex_normals.size() * sizeof(GLfloat),
			vertex_normals.data(),
			GL_STATIC_DRAW);


	assert(glGetError()==GL_NO_ERROR);
}

void Model::render(){
	GLuint vposition_loc = glGetAttribLocation(program.handle, "vertex_position");
	glBindBuffer(GL_ARRAY_BUFFER, vbo_handle);
	glVertexAttribPointer(vposition_loc, 3, GL_FLOAT, GL_FALSE,0,0);
	glEnableVertexAttribArray(vposition_loc);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_normals_handle);
	GLuint vnormals_loc = glGetAttribLocation(program.handle, "vertex_normal");
	glEnableVertexAttribArray(vnormals_loc);
	glVertexAttribPointer(vnormals_loc, 3, GL_FLOAT, GL_FALSE,0,0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_handle);

	assert(glGetError()==GL_NO_ERROR);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, NULL);
}

void Model::print_vertex_data(){
	std::cout << "Printing vertex data:" << std::endl;
	std::cout << "Num vertices = " << vertex_data.size()/3 << std::endl;
	for(unsigned int i=0;i<vertex_data.size();i+=3){
		std::cout << "x:" << vertex_data[i] << " ";
		std::cout << "y:" << vertex_data[i+1]<< " ";
		std::cout << "z:" << vertex_data[i+2] << std::endl;
	}
}

void Model::print_vertex_indices(){
	std::cout << "Printing vertex indices:" << std::endl;
	std::cout << "Num indices = " << vertex_indices.size() << std::endl;
	for(unsigned int i=0;i<vertex_indices.size();++i){
		std::cout << "z:" << vertex_indices[i] << std::endl;
	}
}



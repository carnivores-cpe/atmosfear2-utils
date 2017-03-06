/******************************************************************
    @name	AtmosFEAR 2 Library
    @author Rexhunter99
    @date	6th March 2017
*/

#pragma once

#ifndef LIBAF2_MESH_H
#define LIBAF2_MESH_H

#include <cstdint>
#include <string>
#include <vector>


// Allow this header to be included separately from "af-files.h"
#ifndef LIBAF2_NAMESPACE
namespace libAF2 {
#endif // LIBAF2_NAMESPACE


class Mesh
{
public:

	enum FaceFlagsEnum
	{
		F_DOUBLESIDE		= 0x0001,
		F_DARKBACK			= 0x0002,
		F_OPACITY			= 0x0004,
		F_TRANSPARENCY		= 0x0008,
		F_MORTAL			= 0x0010,
		F_WEAKSPOT			= F_MORTAL,
		F_PHONG				= 0x0020,
		F_SPECULAR			= F_PHONG,
		F_ENVMAP			= 0x0040,
		F_NEEDVC			= 0x0080,
		F_UNUSED1			= 0x0100,
		F_UNUSED2			= 0x0200,
		F_UNUSED3			= 0x0400,
		F_UNUSED4			= 0x0800,
		F_UNUSED5			= 0x1000,
		F_UNUSED6			= 0x2000,
		F_UNUSED7			= 0x4000,
		F_DARK				= 0x8000,
		F_ALL				= 0xFFFF, // 65535
	};

	class Vertex
	{
	public:
		float		x, y, z;	// XYZ co-ordinates
		int16_t		bone;		// Bone index, -1 for no bone.
		int16_t		visible;	// This is used only by editor/viewer tools, the games ignore this.
	};

	class Triangle
	{
	public:
		uint32_t	i[3];		// Vertex indices
		float		uv[2][3];	// UV/ST Mapping values, these are normalised.
		uint16_t	flags;		// Flags defining how the triangle are handled and reacts
		uint16_t	dmask;		// Unknown TODO: Check Carnivores 2/IA source code.
		uint32_t	prev;		// The previous triangle that rendered, likely for transparency sorting
		uint32_t	next;		// The next triangle to render, likely for transparency sorting
		uint32_t	group;		// Assumed this is an editor feature, likely an index to a group of triangles.
		uint32_t	reserved[4];// Reserved/unused data
	};

	class Bone
	{
	public:
		std::string	name;		// Name of the bone.
		float		x, y, z;	// XYZ co-ordinates
		int16_t		parent;		// parent index, -1 means this bone is a root bone.
		int16_t		visible;	// This is used only by editor/viewer tools, the games ignore this.
	};

	/***************************************************************************
		Special type definitions to simplify the class
	*/
	typedef std::vector<Vertex>		vertex_vector;
	typedef std::vector<Triangle> 	triangle_vector;
	typedef std::vector<Bone>		bone_vector;
	typedef vertex_vector::iterator		vertex_iterator;
	typedef triangle_vector::iterator	triangle_iterator;
	typedef bone_vector::iterator		bone_iterator;


	/***************************************************************************
		Beginning of methods and operators
	*/

	Mesh();
	Mesh(const Mesh &mesh);
	~Mesh();

	Mesh& operator = (const Mesh& rhs);
	Mesh& operator + (const Mesh& rhs);


	const uint32_t	getVersion() const { return this->version; }

	/**********************************************************************************
		Get/set the internal name of the Mesh
	*/
	std::string	getName() const;
	void		setName( const std::string& name );


	/**********************************************************************************
		Get the number of elements that are stored internally and return it as a size_t
		data type.
	*/
	size_t	getVertexCount() const;
	size_t	getTriangleCount() const;
	size_t	getBoneCount() const;

	/**********************************************************************************
		Get the stored data as a raw blob, this copies the data to a pointer that the
		user must take ownership of. Basically, this just helps protect the internal
		vectors of elements from modification.
		If you need to edit the internal data, externally, I have supplied access to
		the vector iterators via functions below.
	*/
	size_t	getVerticesAsArray( intptr_t* );
	size_t	getIndexedVerticesAsArray( intptr_t* );
	size_t	getIndicesAsArray( intptr_t* );
	size_t	getBonesAsArray( intptr_t* );


	/***************************************************************************
		Copy the stored data into new vectors and return them here.  The
		returned vectors can be edited without affecting the internal elements.
	*/
	vertex_vector	getVertices( );
	triangle_vector	getTriangles( );
	bone_vector		getBones( );

	/**********************************************************************************
		Get the stored data as referenced vectors and return them here.  The returned
		vector references are directly linked to the internal elements.
	*/
	vertex_vector&		getVerticesRef( ) const;
	triangle_vector&	getTrianglesRef( ) const;
	bone_vector&		getBonesRef( ) const;


	/**********************************************************************************
		Provide a means to access the iterators for the internal vector of elements.
		This allows direct manipulation of values in the vectors.
		Warning! Changing values via the iterators changes them in the instance of the
		Mesh class that you are working with! For a 'safe' data set, use the
		get*AsArray() methods instead.
	*/
	vertex_iterator		vertexFirst() const;	// equivilent to std::vector::begin()
	vertex_iterator		vertexLast() const;	// equivilent to std::vector::end()
	triangle_iterator	triangleFirst() const;	// equivilent to std::vector::begin()
	triangle_iterator	triangleLast() const;	// equivilent to std::vector::end()
	bone_iterator		boneFirst() const;	// equivilent to std::vector::begin()
	bone_iterator		boneLast() const;	// equivilent to std::vector::end()


	/**********************************************************************************
		Add new elements to the internal vectors
	*/
	void addVertex( const Vertex& vertex );
	void addTriangle( const Triangle& triangle );
	void addBone( const Bone& bone );

private:

	/*****************************************************************/
	///	Private member variables
	const int				version = 2;
	std::string				m_name;
	vertex_vector			m_vertices;
	triangle_vector			m_triangles;
	bone_vector				m_bones;

};

#ifndef LIBAF2_NAMESPACE
};
#endif // LIBAF2_NAMESPACE

#endif // LIBAF2_MESH_H

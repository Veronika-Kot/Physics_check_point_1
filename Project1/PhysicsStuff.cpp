//PsysicsStuff.cpp
#include "globalStuff.h"
#include <glm/glm.hpp>
#include "sMOdelDrawInfo.h"
#include <algorithm>
#include <iostream>


typedef glm::vec3 Point;
typedef glm::vec3 Vector;

glm::vec3 thePoint;

bool isACloserThanB(glm::vec3 &A, glm::vec3 &B) {

	if (glm::length(thePoint - A) <= glm::length(thePoint - B))
	{
		return true;
	}

	return false;
}

Point ClosestPtPointTriangle(Point p, Point a, Point b, Point c)
{
	Vector ab = b - a;  //one edge
	Vector ac = c - a;
	Vector bc = c - b;

	// Compute parametric position s for projection P' of P on AB,
	// P' = A + s*AB, s = snom/(snom+sdenom)
	float snom = glm::dot(p - a, ab);
	float sdenom = glm::dot(p - b, a - b);
	// Compute parametric position t for projection P' of P on AC,
	// P' = A + t*AC, s = tnom/(tnom+tdenom)
	float tnom = glm::dot(p - a, ac);
    float tdenom = glm::dot(p - c, a - c);

	if (snom <= 0.0f && tnom <= 0.0f) return a; // Vertex region early out

	// Compute parametric position u for projection P' of P on BC,
	// P' = B + u*BC, u = unom/(unom+udenom)
	float unom = glm::dot(p - b, bc);
	float udenom = glm::dot(p - c, b - c);

	if (sdenom <= 0.0f && unom <= 0.0f) return b; // Vertex region early out
	if (tdenom <= 0.0f && udenom <= 0.0f) return c; // Vertex region early out


	// P is outside (or on) AB if the triple scalar product [N PA PB] <= 0
	Vector n = glm::cross(b - a, c - a);
	float vc = glm::dot(n, glm::cross(a - p, b - p));
	// If P outside AB and within feature region of AB,
	// return projection of P onto AB
	if (vc <= 0.0f && snom >= 0.0f && sdenom >= 0.0f)
		return a + snom / (snom + sdenom) * ab;

	// P is outside (or on) BC if the triple scalar product [N PB PC] <= 0
	float va = glm::dot(n, glm::cross(b - p, c - p));
	// If P outside BC and within feature region of BC,
	// return projection of P onto BC
	if (va <= 0.0f && unom >= 0.0f && udenom >= 0.0f)
		return b + unom / (unom + udenom) * bc;

	// P is outside (or on) CA if the triple scalar product [N PC PA] <= 0
	float vb = glm::dot(n, glm::cross(c - p, a - p));
	// If P outside CA and within feature region of CA,
	// return projection of P onto CA
	if (vb <= 0.0f && tnom >= 0.0f && tdenom >= 0.0f)
		return a + tnom / (tnom + tdenom) * ac;

	// P must project inside face region. Compute Q using barycentric coordinates
	float u = va / (va + vb + vc);
	float v = vb / (va + vb + vc);
	float w = 1.0f - u - v; // = vc / (va + vb + vc)
	return u * a + v * b + w * c;
}


//Pass in the terrsain
	//Pass in the location of the Bunny (the one I can Move)
	//"return" (by reference) as list points
	//-->Then I can draw points.
//void CalculateClosestPointOnMesh(sModelDrawInfo theMeshDrawInfo, glm::vec3 pointToTest, glm::vec3 &closestPointToModel)//std::vector<glm::vec3> &vecPoints)
void CalculateClosestPointOnMesh(sModelDrawInfo theMeshDrawInfo, glm::vec3 pointToTest, glm::vec3 &closestPointToModel, glm::vec3 theVertices[])
{
	std::vector<glm::vec3> vecPoints;
	///vecPoints.clear();
	///thePoint = pointToTest; //for sorting
	
	glm::vec3 vertices[3];

	for (unsigned int triIndex = 0; triIndex != theMeshDrawInfo.numberOfTriangles; triIndex++)
	{
		//sPlyTriangle theTriangle;
		sPlyTriangle CurTri = theMeshDrawInfo.pTriangles[triIndex];
		//...call the ClosestPointToTriangles
		//Need to to get the 3 vertices of the triangles
		sPlyVertex corner_1 = theMeshDrawInfo.pVerticesFromFile[CurTri.vertex_index_1];
		sPlyVertex corner_2 = theMeshDrawInfo.pVerticesFromFile[CurTri.vertex_index_2];
		sPlyVertex corner_3 = theMeshDrawInfo.pVerticesFromFile[CurTri.vertex_index_3];

		//convert this to glm
		glm::vec3 vert_1 = glm::vec3(corner_1.x, corner_1.y, corner_1.z);
		glm::vec3 vert_2 = glm::vec3(corner_2.x, corner_2.y, corner_2.z);
		glm::vec3 vert_3 = glm::vec3(corner_3.x, corner_3.y, corner_3.z);


		glm::vec3 closestPoint = ClosestPtPointTriangle(pointToTest, vert_1, vert_2, vert_3);


		if (vecPoints.size() == 0) {
			vecPoints.push_back(closestPoint);
			theVertices[0].x = theMeshDrawInfo.pVerticesFromFile[CurTri.vertex_index_1].x;
			theVertices[0].y = theMeshDrawInfo.pVerticesFromFile[CurTri.vertex_index_1].y;
			theVertices[0].z = theMeshDrawInfo.pVerticesFromFile[CurTri.vertex_index_1].z;
			theVertices[1].x = theMeshDrawInfo.pVerticesFromFile[CurTri.vertex_index_2].x;
			theVertices[1].y = theMeshDrawInfo.pVerticesFromFile[CurTri.vertex_index_2].y;
			theVertices[1].z = theMeshDrawInfo.pVerticesFromFile[CurTri.vertex_index_2].z;
			theVertices[2].x = theMeshDrawInfo.pVerticesFromFile[CurTri.vertex_index_3].x;
			theVertices[2].y = theMeshDrawInfo.pVerticesFromFile[CurTri.vertex_index_3].y;
			theVertices[2].z = theMeshDrawInfo.pVerticesFromFile[CurTri.vertex_index_3].z;
			///std::cout << vertices[2].x << " " << vertices[2].y << " " << vertices[2].z << std::endl;
			///std::cout << theVertices[2].x << " " << theVertices[2].y << " " << theVertices[2].z << std::endl;
		}
		else if (glm::length(pointToTest-vecPoints[0]) > glm::length(pointToTest - closestPoint)){
			vecPoints[0] = closestPoint;
			theVertices[0].x = theMeshDrawInfo.pVerticesFromFile[CurTri.vertex_index_1].x;
			theVertices[0].y = theMeshDrawInfo.pVerticesFromFile[CurTri.vertex_index_1].y;
			theVertices[0].z = theMeshDrawInfo.pVerticesFromFile[CurTri.vertex_index_1].z;
			theVertices[1].x = theMeshDrawInfo.pVerticesFromFile[CurTri.vertex_index_2].x;
			theVertices[1].y = theMeshDrawInfo.pVerticesFromFile[CurTri.vertex_index_2].y;
			theVertices[1].z = theMeshDrawInfo.pVerticesFromFile[CurTri.vertex_index_2].z;
			theVertices[2].x = theMeshDrawInfo.pVerticesFromFile[CurTri.vertex_index_3].x;
			theVertices[2].y = theMeshDrawInfo.pVerticesFromFile[CurTri.vertex_index_3].y;
			theVertices[2].z = theMeshDrawInfo.pVerticesFromFile[CurTri.vertex_index_3].z;
			///std::cout << theVertices[2].x << " " << theVertices[2].y << " " << theVertices[2].z << std::endl;
		}
		
	}

	//std::sort(vecPoints.begin(), vecPoints.end(), isACloserThanB); BAD performance 
	closestPointToModel = vecPoints[0];

	return;
}

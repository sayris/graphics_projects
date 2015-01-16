////////////////////////////////////////////////////////////////////////////////
// Filename: frustum.cxx
// Author: Naren Hazareesingh
////////////////////////////////////////////////////////////////////////////////
#include "frustum.hxx"


Frustum::Frustum()
{
}


Frustum::Frustum(const Frustum& other)
{
}


Frustum::~Frustum()
{
}


void Frustum::Construct(cs237::mat4f projection, cs237::mat4f model)
{
	float modl[16];
	float proj[16];
	float clip[16];
	float t;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int k = j+4*i;
			modl[k] = model[i][j];
			proj[k] = projection[i][j];
		}
	}
	
	
	
	/* Combine the two matrices (multiply projection by modelview) */
	clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
	clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
	clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
	clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];
	
	clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
	clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
	clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
	clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];
	
	clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
	clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
	clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
	clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];
	
	clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
	clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
	clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
	clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];
	
	/* Extract the numbers for the RIGHT plane */
	this->frustum[0][0] = clip[ 3] - clip[ 0];
	this->frustum[0][1] = clip[ 7] - clip[ 4];
	this->frustum[0][2] = clip[11] - clip[ 8];
	this->frustum[0][3] = clip[15] - clip[12];
	
	/* Normalize the result */
	t = sqrt( this->frustum[0][0] * this->frustum[0][0] + this->frustum[0][1] * this->frustum[0][1] + this->frustum[0][2] * this->frustum[0][2] );
	this->frustum[0][0] /= t;
	this->frustum[0][1] /= t;
	this->frustum[0][2] /= t;
	this->frustum[0][3] /= t;
	
	/* Extract the numbers for the LEFT plane */
	this->frustum[1][0] = clip[ 3] + clip[ 0];
	this->frustum[1][1] = clip[ 7] + clip[ 4];
	this->frustum[1][2] = clip[11] + clip[ 8];
	this->frustum[1][3] = clip[15] + clip[12];
	
	/* Normalize the result */
	t = sqrt( this->frustum[1][0] * this->frustum[1][0] + this->frustum[1][1] * this->frustum[1][1] + this->frustum[1][2] * this->frustum[1][2] );
	this->frustum[1][0] /= t;
	this->frustum[1][1] /= t;
	this->frustum[1][2] /= t;
	this->frustum[1][3] /= t;
	
	/* Extract the BOTTOM plane */
	this->frustum[2][0] = clip[ 3] + clip[ 1];
	this->frustum[2][1] = clip[ 7] + clip[ 5];
	this->frustum[2][2] = clip[11] + clip[ 9];
	this->frustum[2][3] = clip[15] + clip[13];
	
	/* Normalize the result */
	t = sqrt( this->frustum[2][0] * this->frustum[2][0] + this->frustum[2][1] * this->frustum[2][1] + this->frustum[2][2] * this->frustum[2][2] );
	this->frustum[2][0] /= t;
	this->frustum[2][1] /= t;
	this->frustum[2][2] /= t;
	this->frustum[2][3] /= t;
	
	/* Extract the TOP plane */
	this->frustum[3][0] = clip[ 3] - clip[ 1];
	this->frustum[3][1] = clip[ 7] - clip[ 5];
	this->frustum[3][2] = clip[11] - clip[ 9];
	this->frustum[3][3] = clip[15] - clip[13];
	
	/* Normalize the result */
	t = sqrt( this->frustum[3][0] * this->frustum[3][0] + this->frustum[3][1] * this->frustum[3][1] + this->frustum[3][2] * this->frustum[3][2] );
	this->frustum[3][0] /= t;
	this->frustum[3][1] /= t;
	this->frustum[3][2] /= t;
	this->frustum[3][3] /= t;
	
	/* Extract the FAR plane */
	this->frustum[4][0] = clip[ 3] - clip[ 2];
	this->frustum[4][1] = clip[ 7] - clip[ 6];
	this->frustum[4][2] = clip[11] - clip[10];
	this->frustum[4][3] = clip[15] - clip[14];
	
	/* Normalize the result */
	t = sqrt( this->frustum[4][0] * this->frustum[4][0] + this->frustum[4][1] * this->frustum[4][1] + this->frustum[4][2] * this->frustum[4][2] );
	this->frustum[4][0] /= t;
	this->frustum[4][1] /= t;
	this->frustum[4][2] /= t;
	this->frustum[4][3] /= t;
	
	/* Extract the NEAR plane */
	this->frustum[5][0] = clip[ 3] + clip[ 2];
	this->frustum[5][1] = clip[ 7] + clip[ 6];
	this->frustum[5][2] = clip[11] + clip[10];
	this->frustum[5][3] = clip[15] + clip[14];
	
	/* Normalize the result */
	t = sqrt( this->frustum[5][0] * this->frustum[5][0] + this->frustum[5][1] * this->frustum[5][1] + this->frustum[5][2] * this->frustum[5][2] );
	this->frustum[5][0] /= t;
	this->frustum[5][1] /= t;
	this->frustum[5][2] /= t;
	this->frustum[5][3] /= t;
	
	return;
}

void Frustum::Print() {
	printf("*********FRUSTUM**********\n");
	for (int i = 0; i < 6; i++) {
		printf("Plane %d:", i);
		for (int j = 0; j < 4; j++) {
			printf(" %f", this->frustum[i][j]);
		}
		printf("\n");
	}
}
bool Frustum::CheckPoint(cs237::vec3d pt) {
	for (int p = 0; p < 6; p++) {
		if( frustum[p][0] * pt.x + frustum[p][1] * pt.y + frustum[p][2] * pt.z + frustum[p][3] <= 0 ) {
			return false;
		}
	}
	return true;
}

bool Frustum::CheckAABB(cs237::AABBd box) {
	for (int p = 0; p < 6; p++) {
		for (int c = 0; c < 8; c++) {
			cs237::vec3d corner = box.corner(c);
			if (this->CheckPoint(corner)) return true;
		}
	}
	return false;
}
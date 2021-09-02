#pragma once
#include "Vector3.h"
#include "Vector4.h"
#include <memory>

struct Matrix4x4
{
	Matrix4x4() : Matrix4x4(1.0f) { }
	
	Matrix4x4(float diagonal)
	{
		memset(m_mat, 0, sizeof(float) * 16);
		m_mat[0][0] = diagonal;
		m_mat[1][1] = diagonal;
		m_mat[2][2] = diagonal;
		m_mat[3][3] = diagonal;
	}

	static Matrix4x4 identity()
	{
		return Matrix4x4(1.0f);
	}

	static Matrix4x4 translation(const Vector3& translation)
	{
		Matrix4x4 mat(1.0f);
		mat.m_mat[3][0] = translation.x;
		mat.m_mat[3][1] = translation.y;
		mat.m_mat[3][2] = translation.z;
		return mat;
	}

	static Matrix4x4 scale(const Vector3& scale)
	{
		Matrix4x4 mat(1.0f);
		mat.m_mat[0][0] = scale.x;
		mat.m_mat[1][1] = scale.y;
		mat.m_mat[2][2] = scale.z;
		return mat;
	}

	static Matrix4x4 rotationX(float x)
	{
		Matrix4x4 mat(1.0f);
		mat.m_mat[1][1] = cos(x);
		mat.m_mat[1][2] = sin(x);
		mat.m_mat[2][1] = -sin(x);
		mat.m_mat[2][2] = cos(x);
		return mat;
	}
	
	static Matrix4x4 rotationY(float x)
	{
		Matrix4x4 mat(1.0f);
		mat.m_mat[0][0] = cos(x);
		mat.m_mat[0][2] = -sin(x);
		mat.m_mat[2][0] = sin(x);
		mat.m_mat[2][2] = cos(x);
		return mat;
	}
	
	static Matrix4x4 rotationZ(float x)
	{
		Matrix4x4 mat(1.0f);
		mat.m_mat[0][0] = cos(x);
		mat.m_mat[0][1] = sin(x);
		mat.m_mat[1][0] = -sin(x);
		mat.m_mat[1][1] = cos(x);
		return mat;
	}

	static Matrix4x4 rotation(const Vector4& q)
	{
		Matrix4x4 mat(1.0f);
		float xSqr = q.x * q.x;
		float ySqr = q.y * q.y;
		float zSqr = q.z * q.z;
		mat.m_mat[0][0] = 1 - 2.0f * ySqr - 2.0f * zSqr;
		mat.m_mat[1][0] = 2.0f * (q.x * q.y - q.w * q.z);
		mat.m_mat[2][0] = 2.0f * (q.x * q.z + q.w * q.y);
		mat.m_mat[0][1] = 2.0f * (q.x * q.y + q.w * q.z);
		mat.m_mat[1][1] = 1 - 2.0f * xSqr - 2.0f * zSqr;
		mat.m_mat[2][1] = 2.0f * (q.y * q.z - q.w * q.x);
		mat.m_mat[0][2] = 2.0f * (q.x * q.z - q.w * q.y);
		mat.m_mat[1][2] = 2.0f * (q.y * q.z + q.w * q.x);
		mat.m_mat[2][2] = 1 - 2.0f * xSqr - 2.0f * ySqr;
		return mat;
	}

	static Matrix4x4 orthoLH(float width, float height, float near_plane, float far_plane)
	{
		Matrix4x4 mat(1.0f);
		mat.m_mat[0][0] = 2.0f / width;
		mat.m_mat[1][1] = 2.0f / height;
		mat.m_mat[2][2] = 1.0f / (far_plane - near_plane);
		mat.m_mat[3][2] = -(near_plane / (far_plane - near_plane));
		return mat;
	}

	static Matrix4x4 perspectiveFovLH(float fov, float aspect, float znear, float zfar)
	{
		float yScale = 1.0f / tan(fov / 2.0f);
		float xScale = yScale / aspect;
		Matrix4x4 mat(1.0f);
		mat.m_mat[0][0] = xScale;
		mat.m_mat[1][1] = yScale;
		mat.m_mat[2][2] = zfar / (zfar - znear);
		mat.m_mat[2][3] = 1.0f;
		mat.m_mat[3][2] = (-znear * zfar) / (zfar - znear);
		return mat;
	}

	void operator*=(const Matrix4x4& other)
	{
		Matrix4x4 out;
		
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				out.m_mat[i][j] =
					m_mat[i][0] * other.m_mat[0][j] +
					m_mat[i][1] * other.m_mat[1][j] +
					m_mat[i][2] * other.m_mat[2][j] +
					m_mat[i][3] * other.m_mat[3][j];
			}
		}

		memcpy(m_mat, out.m_mat, sizeof(float) * 16);
	}
	
	float getDeterminant()
	{
		Vector4 minor, v1, v2, v3;
		float det;

		v1 = Vector4(this->m_mat[0][0], this->m_mat[1][0], this->m_mat[2][0], this->m_mat[3][0]);
		v2 = Vector4(this->m_mat[0][1], this->m_mat[1][1], this->m_mat[2][1], this->m_mat[3][1]);
		v3 = Vector4(this->m_mat[0][2], this->m_mat[1][2], this->m_mat[2][2], this->m_mat[3][2]);

		minor.cross(v1, v2, v3);
		det = -(this->m_mat[0][3] * minor.x + this->m_mat[1][3] * minor.y + this->m_mat[2][3] * minor.z + this->m_mat[3][3] * minor.w);

		return det;
	}

	Vector3 getXDirection()
	{
		return Vector3(m_mat[0][0], m_mat[0][1], m_mat[0][2]);
	}
	
	Vector3 getYDirection()
	{
		return Vector3(m_mat[1][0], m_mat[1][1], m_mat[1][2]);
	}
	
	Vector3 getZDirection()
	{
		return Vector3(m_mat[2][0], m_mat[2][1], m_mat[2][2]);
	}

	Vector3 getTranslation()
	{
		return Vector3(m_mat[3][0], m_mat[3][1], m_mat[3][2]);
	}

	void inverse()
	{
		int a, i, j;
		Matrix4x4 out;
		Vector4 v, vec[3];
		float det = 0.0f;

		det = this->getDeterminant();
		if (!det)
			return;
		
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				if (j != i)
				{
					a = j;
					if (j > i) a = a - 1;
					vec[a].x = (this->m_mat[j][0]);
					vec[a].y = (this->m_mat[j][1]);
					vec[a].z = (this->m_mat[j][2]);
					vec[a].w = (this->m_mat[j][3]);
				}
			}
			v.cross(vec[0], vec[1], vec[2]);

			out.m_mat[0][i] = pow(-1.0f, i) * v.x / det;
			out.m_mat[1][i] = pow(-1.0f, i) * v.y / det;
			out.m_mat[2][i] = pow(-1.0f, i) * v.z / det;
			out.m_mat[3][i] = pow(-1.0f, i) * v.w / det;
		}

		memcpy(this, &out, sizeof(Matrix4x4));
	}

	float* getMatrix()
	{
		return *m_mat;
	}

	float m_mat[4][4];
};

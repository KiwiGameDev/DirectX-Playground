#pragma once
#include "Vector3.h"
#include <memory>

struct Matrix4x4
{
	Matrix4x4() : Matrix4x4(1.0f) { }
	
	Matrix4x4(float diagonal)
	{
		memset(m_matrix, 0, sizeof(float) * 16);
		m_matrix[0][0] = diagonal;
		m_matrix[1][1] = diagonal;
		m_matrix[2][2] = diagonal;
		m_matrix[3][3] = diagonal;
	}

	static Matrix4x4 identity()
	{
		return Matrix4x4(1.0f);
	}

	static Matrix4x4 translation(const Vector3& translation)
	{
		Matrix4x4 mat(1.0f);
		mat.m_matrix[3][0] = translation.x;
		mat.m_matrix[3][1] = translation.y;
		mat.m_matrix[3][2] = translation.z;
		return mat;
	}

	static Matrix4x4 scale(const Vector3& scale)
	{
		Matrix4x4 mat(1.0f);
		mat.m_matrix[0][0] = scale.x;
		mat.m_matrix[1][1] = scale.y;
		mat.m_matrix[2][2] = scale.z;
		return mat;
	}

	static Matrix4x4 rotationX(float x)
	{
		Matrix4x4 mat(1.0f);
		mat.m_matrix[1][1] = cos(x);
		mat.m_matrix[1][2] = sin(x);
		mat.m_matrix[2][1] = -sin(x);
		mat.m_matrix[2][2] = cos(x);
		return mat;
	}
	
	static Matrix4x4 rotationY(float x)
	{
		Matrix4x4 mat(1.0f);
		mat.m_matrix[0][0] = cos(x);
		mat.m_matrix[0][2] = -sin(x);
		mat.m_matrix[2][0] = sin(x);
		mat.m_matrix[2][2] = cos(x);
		return mat;
	}
	
	static Matrix4x4 rotationZ(float x)
	{
		Matrix4x4 mat(1.0f);
		mat.m_matrix[0][0] = cos(x);
		mat.m_matrix[0][1] = sin(x);
		mat.m_matrix[1][0] = -sin(x);
		mat.m_matrix[1][1] = cos(x);
		return mat;
	}

	static Matrix4x4 orthoLH(float width, float height, float near_plane, float far_plane)
	{
		Matrix4x4 mat(1.0f);
		mat.m_matrix[0][0] = 2.0f / width;
		mat.m_matrix[1][1] = 2.0f / height;
		mat.m_matrix[2][2] = 1.0f / (far_plane - near_plane);
		mat.m_matrix[3][2] = -(near_plane / (far_plane - near_plane));
		return mat;
	}

	void operator*=(const Matrix4x4& other)
	{
		Matrix4x4 out;
		
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				out.m_matrix[i][j] =
					m_matrix[i][0] * other.m_matrix[0][j] +
					m_matrix[i][1] * other.m_matrix[1][j] +
					m_matrix[i][2] * other.m_matrix[2][j] +
					m_matrix[i][3] * other.m_matrix[3][j];
			}
		}

		memcpy(m_matrix, out.m_matrix, sizeof(float) * 16);
	}

	float m_matrix[4][4];
};

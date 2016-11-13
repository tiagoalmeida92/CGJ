#pragma once
#include "vec.hpp"
#include <iostream>
#include <string>

using namespace std;

const float mThreshold = (float)1.0e-5;

struct Mat2 {
	static const int size = 2;
	static const int data_size = size * size;

	float data[data_size];

	Mat2();
	Mat2(float);
	Mat2(float, float, float, float);

	Mat2 operator + (const Mat2&);
	Mat2 operator - (const Mat2&);
	Mat2 operator * (float);
	Mat2 operator * (const Mat2&);
	Vec2 operator * (const Vec2&);

	Mat2 transpose();
	Mat2 inverse();
	float determinant();

	void operator +=(const Mat2 &);
	void operator -=(const Mat2 &);
	bool operator ==(const Mat2 &);
	void operator *=(const float);

	friend ostream& operator<< (ostream&, const Mat2&);
	friend istream& operator>> (istream&, Mat2&);
	Mat2 convert_opengl();

};

struct Mat3 {
	static const int size = 3;
	static const int data_size = 9;
	float data[data_size];

	Mat3();
	Mat3(float);
	Mat3(float, float, float, float, float, float, float, float, float);

	Mat3 operator + (const Mat3&);
	Mat3 operator - (const Mat3&);
	Mat3 operator * (float);
	Mat3 operator * (const Mat3&);
	Vec3 operator * (const Vec3&);

	Mat3 transpose();
	Mat3 inverse();
	void clean();
	float determinant();

	void operator +=(const Mat3 &);
	void operator -=(const Mat3 &);
	bool operator ==(const Mat3 &);
	void operator *=(const float);

	friend Mat3 operator * (float, const Mat3&);

	friend ostream& operator<< (ostream&, const Mat3&);
	friend istream& operator>> (istream&, Mat3&);
	Mat3 convert_opengl();

};


struct Mat4 {
	static const int size = 4;
	static const int data_size = 16;
	float data[data_size];

	Mat4();
	Mat4(float);
	Mat4(float, float, float, float, float, float, float, float, 
		float, float, float, float, float, float, float, float);

	Mat4 operator + (const Mat4&);
	Mat4 operator - (const Mat4&);
	
	Vec4 operator * (const Vec4&);

	const Mat4 transpose();
	void clean();

	void operator +=(const Mat4 &);
	void operator -=(const Mat4 &);
	bool operator ==(const Mat4 &);
	void operator *=(const float);

	friend Mat4 operator * (float, const Mat4&);
	friend Mat4 operator * (const Mat4&, const Mat4&);

	friend ostream& operator<< (ostream&, const Mat4&);
	friend istream& operator>> (istream&, Mat4&);
	float* convert_opengl();
};
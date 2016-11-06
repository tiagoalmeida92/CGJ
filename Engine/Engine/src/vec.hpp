#pragma once
#include <iostream>
#include <string>

using namespace std;

struct Vec2
{
	float x;
	float y;

	Vec2();
	Vec2(float, float);

	float magnitude();
	float quadrance();

	Vec2 operator + (const Vec2& vec);
	Vec2 operator - (const Vec2& vec);

	void operator += (const Vec2& vec);
	void operator -= (const Vec2& vec);
	void operator *= (float val);

	bool operator == (const Vec2& vec);
	bool operator != (const Vec2& vec);

	Vec2 Vec2::normalize();

	friend Vec2 operator * (Vec2 &v, float val);
	friend Vec2 operator * (float val, Vec2 &v);

	friend ostream& operator<< (ostream &out, const Vec2 &v);
	friend istream& operator>> (istream &out, Vec2 &v);


};


struct Vec3
{
	float x;
	float y;
	float z;

	Vec3();
	Vec3(float, float, float);

	float magnitude();
	float quadrance();
	float dot(const Vec3& vec);

	Vec3 cross(const Vec3& vec);
	Vec3 normalize();
	Vec3 operator + (const Vec3& vec);
	Vec3 operator - (const Vec3& vec);



	void operator += (const Vec3& vec);
	void operator -= (const Vec3& vec);
	void operator *= (float val);

	bool operator == (const Vec3& vec);
	bool operator != (const Vec3& vec);


	friend Vec3 operator * (Vec3 &v, float val);
	friend Vec3 operator * (float val, Vec3 &v);

	friend ostream& operator<< (ostream &out, const Vec3 &v);
	friend istream& operator>> (istream &out, Vec3 &v);


};

struct Vec4
{
	float x;
	float y;
	float z;
	float w;

	Vec4 normalize();
	
	bool operator == (const Vec4& vec);

	friend ostream& operator<< (ostream &out, const Vec4 &v);

};
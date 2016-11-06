#include "vec.hpp"

#define EPSILON 0.0001
#define CMP_FLOAT(a, b) (((a - EPSILON) < b) && (b <( a + EPSILON)))

/*
VECTOR 2
*/


Vec2::Vec2() {
	x = y = 0;
}

Vec2::Vec2(float x, float y)
{
	this->x = x;
	this->y = y;
}

float Vec2::magnitude() {
	return sqrt(x * x + y * y);
}

float Vec2::quadrance() {
	return x * x + y * y;
}

Vec2 Vec2::operator + (const Vec2& v) {
	Vec2 result;
	result.x = x + v.x;
	result.y = y + v.y;
	return result;
}

Vec2 Vec2::operator - (const Vec2& v) {
	Vec2 result;
	result.x = x - v.x;
	result.y = y - v.y;
	return result;
}


Vec2 operator * (Vec2& vec, float val) {
	Vec2 result;
	result.x = vec.x * val;
	result.y = vec.y * val;
	return result;
}

Vec2 operator * (float val, Vec2& vec) {
	return vec * val;
}

void Vec2::operator += (const Vec2& v) {
	x += v.x;
	y += v.y;
}


void Vec2::operator -= (const Vec2& v) {
	x -= v.x;
	y -= v.y;
}

void Vec2::operator *= (float val) {
	x *= val;
	y *= val;
}

bool Vec2::operator == (const Vec2& vec) {
	return CMP_FLOAT(x, vec.x) && CMP_FLOAT(y, vec.y);
}

bool Vec2::operator != (const Vec2& vec) {
	return !(*this == vec);
}

Vec2 Vec2::normalize() {
	float length = magnitude();
	Vec2 vec;
	vec.x = x / length;
	vec.y = y / length;
	return vec;
}

ostream& operator << (ostream& out, const Vec2 &v) {
	out << "Vec2(" << v.x << "," << v.y << ")";
	return out;
}

istream& operator>> (istream &in, Vec2 &v) {
	string line;
	getline(in, line, ',');
	float x = atof(line.c_str());
	getline(in, line);
	float y = atof(line.c_str());
	v.x = x;
	v.y = y;
	return in;
}

/*
VECTOR 3
*/

Vec3::Vec3() {
	x = y = z = 0;
}

Vec3::Vec3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

float Vec3::magnitude() {
	return sqrt(x * x + y * y + z * z);
}

float Vec3::quadrance() {
	return x * x + y * y + z * z;
}

float Vec3::dot(const Vec3 &vec) {
	return x * vec.x + y * vec.y + z * vec.z;
}

Vec3 Vec3::cross(const Vec3& v) {
	Vec3 result;
	result.x = y * v.z - z * v.y;
	result.y = z * v.x - x * v.z;
	result.z = x * v.y - y * v.x;
	return result;
}

Vec3 Vec3::operator + (const Vec3& v) {
	Vec3 result;
	result.x = x + v.x;
	result.y = y + v.y;
	result.z = z + v.z;
	return result;
}

Vec3 Vec3::operator - (const Vec3& v) {
	Vec3 result;
	result.x = x - v.x;
	result.y = y - v.y;
	result.z = z - v.z;
	return result;
}


Vec3 operator * (Vec3& vec, float val) {
	Vec3 result;
	result.x = vec.x * val;
	result.y = vec.y * val;
	result.z = vec.z * val;
	return result;
}

Vec3 operator * (float val, Vec3& vec) {
	return vec * val;
}

void Vec3::operator += (const Vec3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
}


void Vec3::operator -= (const Vec3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

void Vec3::operator *= (float val) {
	x *= val;
	y *= val;
	z *= val;
}

bool Vec3::operator == (const Vec3& vec) {
	return CMP_FLOAT(x, vec.x) && CMP_FLOAT(y, vec.y) && CMP_FLOAT(z, vec.z);
}

bool Vec3::operator != (const Vec3& vec) {
	return !(*this == vec);
}

Vec3 Vec3::normalize() {
	float length = magnitude();
	Vec3 vec;
	vec.x = x / length;
	vec.y = y / length;
	vec.z = z / length;
	return vec;
}

ostream& operator << (ostream& out, const Vec3 &v) {
	out << "Vec3(" << v.x << "," << v.y << "," << v.z << ")";
	return out;
}

istream& operator>> (istream &in, Vec3 &v) {
	string line;
	getline(in, line, ',');
	float x = atof(line.c_str());
	getline(in, line, ',');
	float y = atof(line.c_str());
	getline(in, line);
	float z = atof(line.c_str());
	v.x = x;
	v.y = y;
	v.z = z;
	return in;
}

//VEC 4

Vec4 Vec4::normalize()
{
	Vec4 vn;
	float s = 1 / (w * sqrt(x*x + y*y + z*z));
	vn.x = x * s;
	vn.y = y * s;
	vn.z = z * s;
	vn.w = 1.0f;
	return vn;
}

bool Vec4::operator == (const Vec4& vec) {
	return CMP_FLOAT(x, vec.x) && CMP_FLOAT(y, vec.y) && CMP_FLOAT(z, vec.z) && CMP_FLOAT(w, vec.w);
}



ostream& operator << (ostream& out, const Vec4 &v) {
	out << "Vec4(" << v.x << "," << v.y << "," << v.z << "," << v.w << ")";
	return out;
}
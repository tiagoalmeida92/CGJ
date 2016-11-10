#include "mat.hpp"


#define EPSILON 0.0001
#define CMP_FLOAT(a, b) (((a - EPSILON) < b) && (b <( a + EPSILON)))

/*
		mat2
*/

#pragma region mat2

Mat2::Mat2(){}

Mat2::Mat2(float val) {
	for (size_t i = 0; i < data_size; i++)
	{
		data[i] = val;
	}
}

Mat2::Mat2(float a, float b, float c, float d) {
	data[0] = a; data[1] = b;
	data[2] = c; data[3] = d;
}

bool Mat2::operator == (const Mat2 & mat) {
	for (size_t i = 0; i < data_size; i++)
	{
		if (!CMP_FLOAT(this->data[i],  mat.data[i])) {
			return false;
		}
	}
	return true;
}

Mat2 Mat2::operator + (const Mat2& mat) {
	Mat2 result;
	for (size_t i = 0; i < data_size; i++)
	{
		result.data[i] = this->data[i] + mat.data[i];
	}
	return result;
}

Mat2 Mat2::operator - (const Mat2& mat) {
	Mat2 result;
	for (size_t i = 0; i < data_size; i++)
	{
		result.data[i] = this->data[i] - mat.data[i];
	}
	return result;
}

Mat2 Mat2::operator*(float val)
{
	Mat2 result;
	for (size_t i = 0; i < data_size; i++)
	{
		result.data[i] = data[i] * val;
	}
	return result;
}

int getIdx(int row, int column, int size) {
	return row * size + column;
}

float dotProduct(const Mat2* mat, int row, const Mat2* other, int column, int size) {
	float result = 0;
	for (size_t i = 0; i < size; i++)
	{
		int idxA = getIdx(row, i, size);
		int idxB = getIdx(i, column, size);

		result += mat->data[idxA] * other->data[idxB];
	}
	return result;
}

Mat2 Mat2::operator*(const Mat2& mat)
{
	Mat2 result;
	int i = 0;
	for (size_t thisRow = 0; thisRow < size; thisRow++)
	{
		for (size_t otherColumn = 0; otherColumn < size; otherColumn++)
		{
			float dotResult = dotProduct(this, thisRow, &mat, otherColumn, size);
			result.data[i] = dotResult;
			i++;
		}
	}
	return result;
}

Vec2 Mat2::operator *(const Vec2& vec)
{
	Vec2 result;
	result.x = data[0] * vec.x + data[1] * vec.y;
	result.y = data[2] * vec.x + data[3] * vec.y;
	return result;
}

void Mat3::clean() {
	for (int i = 0; i < data_size; i++) {
		if (fabs(data[i]) < mThreshold) data[i] = 0.0f;
	}
}

Mat2 Mat2::transpose() {
	Mat2 result;
	int i = 0;

	//Iterate over the columns of the new matrix
	//and write the values from the original sequencially
	for (size_t column = 0; column < size; column++)
	{
		for (size_t row = 0; row < size; row++)
		{
			int idx = getIdx(row, column, size);
			result.data[idx] = this->data[i];
			i++;
		}
	}
	return result;
}

Mat2 Mat2::inverse() {
	float det = determinant();
	Mat2 result =  Mat2(data[3], -data[1],
						-data[2], data[0]) * (1 / det);
	return result;

}

float Mat2::determinant() {
	return data[0] * data[3] - data[1] * data[2];
}

void Mat2::operator += (const Mat2& mat) {
	for (size_t i = 0; i < data_size; i++)
	{
		this->data[i] += mat.data[i];
	}
}

void Mat2::operator -= (const Mat2& mat) {
	for (size_t i = 0; i < data_size; i++)
	{
		this->data[i] -= mat.data[i];
	}
}

void Mat2::operator*= (const float val) {
	for (size_t i = 0; i < data_size; i++)
	{
		this->data[i] *= val;
	}
}

Mat2 Mat2::convert_opengl() {
	return transpose();
}

ostream& operator << (ostream& out, const Mat2 &mat) {
	for (size_t i = 0; i < mat.data_size; i++)
	{
		if (i % mat.size == 0) {
			out << '\n';
		}
		out << mat.data[i] << " ";
		
	}
	out << '\n';
	return out;
}

istream& operator>> (istream &in, Mat2 &mat) {
	string line;
	for (size_t i = 0; i < mat.data_size; i++)
	{
		if (i != mat.data_size - 1) {
			getline(in, line, ' ');
		} 
		else {
			getline(in, line);
		}
		
		float val = atof(line.c_str());
		mat.data[i] = val;
	}
	return in;
}

#pragma endregion

/*
		mat3
*/

#pragma region mat3

Mat3::Mat3(){}

Mat3::Mat3(float val) {
	for (size_t i = 0; i < data_size; i++)
	{
		data[i] = val;
	}
}

Mat3::Mat3(float a, float b, float c, float d, float e, float f, float g, float h, float i) {
	data[0] = a; data[1] = b; data[2] = c;
	data[3] = d; data[4] = e; data[5] = f;
	data[6] = g; data[7] = h; data[8] = i;
}

bool Mat3::operator == (const Mat3 & mat) {
	for (size_t i = 0; i < data_size; i++)
	{
		if (!CMP_FLOAT(this->data[i], mat.data[i])) {
			return false;
		}
	}
	return true;
}

Mat3 Mat3::operator + (const Mat3& mat) {
	Mat3 result;
	for (size_t i = 0; i < data_size; i++)
	{
		result.data[i] = this->data[i] + mat.data[i];
	}
	return result;
}

Mat3 Mat3::operator - (const Mat3& mat) {
	Mat3 result;
	for (size_t i = 0; i < data_size; i++)
	{
		result.data[i] = this->data[i] - mat.data[i];
	}
	return result;
}

//TODO remove
Mat3 Mat3::operator*(float val)
{
	Mat3 result;
	for (size_t i = 0; i < data_size; i++)
	{
		result.data[i] = data[i] * val;
	}
	return result;
}

Mat3 operator* (float val, const Mat3& mat) {
	Mat3 result;
	for (size_t i = 0; i < mat.data_size; i++)
	{
		result.data[i] = mat.data[i] * val;
	}
	return result;
}

//TODO generico receber array
float dotProduct(const Mat3* mat, int row, const Mat3* other, int column, int size) {
	float result = 0;
	for (size_t i = 0; i < size; i++)
	{
		int idxA = getIdx(row, i, size);
		int idxB = getIdx(i, column, size);

		result += mat->data[idxA] * other->data[idxB];
	}
	return result;
}

Mat3 Mat3::operator*(const Mat3& mat)
{
	Mat3 result;
	int i = 0;
	for (size_t thisRow = 0; thisRow < size; thisRow++)
	{
		for (size_t otherColumn = 0; otherColumn < size; otherColumn++)
		{
			float dotResult = dotProduct(this, thisRow, &mat, otherColumn, size);
			result.data[i] = dotResult;
			i++;
		}
	}
	return result;
}


Vec3 Mat3::operator *(const Vec3& vec)
{
	Vec3 result;
	result.x = data[0] * vec.x + data[1] * vec.y + data[2] * vec.z;
	result.y = data[3] * vec.x + data[4] * vec.y + data[5] * vec.z;
	result.z = data[6] * vec.x + data[7] * vec.y + data[8] * vec.z;
	return result;
}


Mat3 Mat3::transpose() {
	Mat3 result;
	int i = 0;

	//Iterate over the columns of the new matrix
	//and write the values from the original sequencially
	for (size_t column = 0; column < size; column++)
	{
		for (size_t row = 0; row < size; row++)
		{
			int idx = getIdx(row, column, size);
			result.data[idx] = this->data[i];
			i++;
		}
	}
	return result;
}

//0 1 2
//3 4 5
//6 7 8

Mat3 Mat3::inverse() {
	float det = determinant();
	Mat3 result;
	
	result.data[0] = Mat2(data[4], data[5], data[7], data[8]).determinant();
	result.data[1] = Mat2(data[2], data[1], data[8], data[7]).determinant();
	result.data[2] = Mat2(data[1], data[2], data[4], data[5]).determinant();

	result.data[3] = Mat2(data[5], data[4], data[8], data[6]).determinant();
	result.data[4] = Mat2(data[0], data[2], data[6], data[8]).determinant();
	result.data[5] = Mat2(data[2], data[0], data[5], data[3]).determinant();

	result.data[6] = Mat2(data[3], data[4], data[6], data[7]).determinant();
	result.data[7] = Mat2(data[1], data[0], data[7], data[6]).determinant();
	result.data[8] = Mat2(data[0], data[1], data[3], data[4]).determinant();

	result *= (1 / det);

	return result;

}



float Mat3::determinant() {
	return data[0] * (data[4] * data[8] - data[5] * data[7])
		 - data[1] * (data[3] * data[8] - data[5] * data[6])
		 + data[2] * (data[3] * data[7] - data[4] * data[6]);
}

void Mat3::operator += (const Mat3& mat) {
	for (size_t i = 0; i < data_size; i++)
	{
		this->data[i] += mat.data[i];
	}
}

void Mat3::operator -= (const Mat3& mat) {
	for (size_t i = 0; i < data_size; i++)
	{
		this->data[i] -= mat.data[i];
	}
}

void Mat3::operator*= (const float val) {
	for (size_t i = 0; i < data_size; i++)
	{
		this->data[i] *= val;
	}
}

Mat3 Mat3::convert_opengl() {
	return transpose();
}

ostream& operator << (ostream& out, const Mat3 &mat) {
	for (size_t i = 0; i < mat.data_size; i++)
	{
		if (i % mat.size == 0) {
			out << '\n';
		}
		out << mat.data[i] << " ";

	}
	out << '\n';
	return out;
}

istream& operator>> (istream &in, Mat3 &mat) {
	string line;
	for (size_t i = 0; i < mat.data_size; i++)
	{
		if (i != mat.data_size - 1) {
			getline(in, line, ' ');
		}
		else {
			getline(in, line);
		}

		float val = atof(line.c_str());
		mat.data[i] = val;
	}
	return in;
}

#pragma endregion

/*
		mat4
*/

#pragma region mat4

Mat4::Mat4(){}

Mat4::Mat4(float a, float b, float c, float d, float e, float f, float g, float h, 
			float i,float j, float k, float l, float m, float n, float o, float p) {
	data[0] = a; data[1] = b; data[2] = c; data[3] = d; 
	data[4] = e; data[5] = f; data[6] = g; data[7] = h; 
	data[8] = i; data[9] = j; data[10] = k; data[11] = l;
	data[12] = m; data[13] = n; data[14] = o; data[15] = p;
}


Mat4::Mat4(float val) {
	for (size_t i = 0; i < data_size; i++)
	{
		data[i] = val;
	}
}

bool Mat4::operator == (const Mat4 & mat) {
	for (size_t i = 0; i < data_size; i++)
	{
		if (!CMP_FLOAT(this->data[i], mat.data[i])) {
			return false;
		}
	}
	return true;
}

Mat4 Mat4::operator + (const Mat4& mat) {
	Mat4 result;
	for (size_t i = 0; i < data_size; i++)
	{
		result.data[i] = this->data[i] + mat.data[i];
	}
	return result;
}

Mat4 Mat4::operator - (const Mat4& mat) {
	Mat4 result;
	for (size_t i = 0; i < data_size; i++)
	{
		result.data[i] = this->data[i] - mat.data[i];
	}
	return result;
}

Mat4 operator* (float val, const Mat4& mat) {
	Mat4 result;
	for (size_t i = 0; i < mat.data_size; i++)
	{
		result.data[i] = mat.data[i] * val;
	}
	return result;
}

//TODO generico receber array
float dotProduct(const Mat4* mat, int row, const Mat4* other, int column, int size) {
	float result = 0;
	for (size_t i = 0; i < size; i++)
	{
		int idxA = getIdx(row, i, size);
		int idxB = getIdx(i, column, size);

		result += mat->data[idxA] * other->data[idxB];
	}
	return result;
}

Mat4 operator*(const Mat4& lhs, const Mat4& rhs)
{
	Mat4 result;
	int i = 0;
	for (size_t thisRow = 0; thisRow < lhs.size; thisRow++)
	{
		for (size_t otherColumn = 0; otherColumn < lhs.size; otherColumn++)
		{
			float dotResult = dotProduct(&lhs, thisRow, &rhs, otherColumn, lhs.size);
			result.data[i] = dotResult;
			i++;
		}
	}
	return result;
}

Vec4 Mat4::operator *(const Vec4& vec)
{
	Vec4 result;
	result.x = data[0] * vec.x + data[1] * vec.y + data[2] * vec.z + data[3] * vec.w;
	result.y = data[4] * vec.x + data[5] * vec.y + data[6] * vec.z + data[7] * vec.w;
	result.z = data[8] * vec.x + data[9] * vec.y + data[10] * vec.z + data[11] * vec.w;
	result.w = data[12] * vec.x + data[13] * vec.y + data[14] * vec.z + data[15] * vec.w;

	return result;
}

const Mat4 Mat4::transpose() {
	Mat4 result;
	int i = 0;

	//Iterate over the columns of the new matrix
	//and write the values from the original sequencially
	for (size_t column = 0; column < size; column++)
	{
		for (size_t row = 0; row < size; row++)
		{
			int idx = getIdx(row, column, size);
			result.data[idx] = this->data[i];
			i++;
		}
	}
	return result;
}

void Mat4::clean() {
	for (int i = 0; i < data_size; i++) {
		if (fabs(data[i]) < mThreshold) data[i] = 0.0f;
	}
}


void Mat4::operator += (const Mat4& mat) {
	for (size_t i = 0; i < data_size; i++)
	{
		this->data[i] += mat.data[i];
	}
}

void Mat4::operator -= (const Mat4& mat) {
	for (size_t i = 0; i < data_size; i++)
	{
		this->data[i] -= mat.data[i];
	}
}

void Mat4::operator*= (const float val) {
	for (size_t i = 0; i < data_size; i++)
	{
		this->data[i] *= val;
	}
}


ostream& operator << (ostream& out, const Mat4 &mat) {
	for (size_t i = 0; i < mat.data_size; i++)
	{
		if (i % mat.size == 0) {
			out << '\n';
		}
		out << mat.data[i] << " ";

	}
	out << '\n';
	return out;
}

istream& operator>> (istream &in, Mat4 &mat) {
	string line;
	for (size_t i = 0; i < mat.data_size; i++)
	{
		if (i != mat.data_size - 1) {
			getline(in, line, ' ');
		}
		else {
			getline(in, line);
		}

		float val = atof(line.c_str());
		mat.data[i] = val;
	}
	return in;
}

float* Mat4::convert_opengl() {
	float* result = new float[data_size];
	Mat4 transposed = transpose();
	for (size_t i = 0; i < data_size; i++)
	{
		result[i] = transposed.data[i];
	}
	return result;
}

#pragma endregion
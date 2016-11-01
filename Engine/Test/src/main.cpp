#include "vec.hpp"
#include "mat.hpp"
#include "matrix_factory.hpp"
#include <iostream>
#include <assert.h>
#include <ctime>

using namespace std;

#define print(s) cout << #s" " << s << '\n'


void testVec2() {

	Vec2 v1(5.5f, -9.4f);
	Vec2 v2(-5.5f, 9.4f);
	Vec2 inputVec;

	Vec2 sum = v1 + v2;
	Vec2 dif = v1 - v2;
	bool equals = v1 == v2;
	Vec2 mul = 3 * v1;
	float magnitude = v1.magnitude();
	Vec2 normalized = v1.normalize();



	print(v1);
	print(v2);
	print(sum);
	print(dif);
	print(equals);
	print(mul);
	print(magnitude);
	print(normalized);
	print(normalized.magnitude());

	//asserts
	assert(sum == Vec2(0, 0));
	assert(dif == Vec2(11, -18.8));
	assert(equals == false);
	assert(mul == Vec2(16.5, -28.2));
	assert(v1 * 3 == Vec2(16.5, -28.2));
	assert(magnitude == 10.8908218f);
	assert(normalized == Vec2(0.505013f, -0.863113f));
	assert(normalized.magnitude() == 1);


	//cout << "\nInsert 2 decimals separated by comma (x,y) ";
	//cin >> inputVec;
	//print(inputVec);
}



void testVec3() {
	Vec3 v1(5.5f, -9.4f, 1.5f);
	Vec3 v2(-5.5f, 9.4f, -44.2f);


	Vec3 sum = v1 + v2;
	Vec3 dif = v1 - v2;
	bool equals = v1 == v2;
	Vec3 mul = v1 * 2;
	float magnitude = v1.magnitude();
	Vec3 normalized = v1.normalize();
	float dotProduct = v1.dot(v2);
	Vec3 crossProduct = v1.cross(v2);
	Vec3 inputVec;

	//asserts
	print(v1);
	print(v2);
	print(sum);
	print(dif);
	print(equals);
	print(mul);
	print(magnitude);
	print(normalized);
	print(dotProduct);
	print(crossProduct);


	assert(sum == Vec3(0, 0, -42.7));
	assert(dif == Vec3(11, -18.8, 45.7));
	assert(equals == false);
	assert(mul == Vec3(11, -18.8, 3));
	assert(3 * v1 == Vec3(16.5, -28.2, 4.5));
	assert(magnitude == 10.993634f);
	assert(normalized == Vec3(0.500289f, -0.855040f, 0.136442));
	assert(normalized.magnitude() == 1);
	assert(dotProduct == -184.91f);
	assert(crossProduct == Vec3(401.38, 234.85, 0));

	//cout << "\nInsert 3 decimals separated by comma (x,y,z) ";
	//cin >> inputVec;
	//print(inputVec);
}


//Comparison
//Sum
//diference
//multiplication mat x mat, mat x scalar and mat x vector
//transpose
//determinant
//inverse


void testMat2() {
	Mat2 mat(3.412f);
	Mat2 matInput;
	Mat2 matCopy = mat;
	Mat2 mat2(1.1f, 2.2f, 
			  3.3f, 4.4f);

	Mat2 product = mat * mat2;

	Vec2 productMatVec = mat2 * Vec2(2.0, 
									 3.0);

	print(mat);
	print(mat2);
	print(mat2.transpose());

	print(product);
	print(productMatVec);
	print(mat2.determinant());
	print(mat2.inverse());

	assert(mat == matCopy);
	assert(mat + mat == Mat2(6.824f));
	assert(mat - mat == Mat2(0));
	assert(mat2.transpose() == Mat2(1.1f, 3.3f,
									2.2f, 4.4f));
	//TODO assert product
	assert(productMatVec == Vec2(8.8f, 19.8f));
	assert(mat2.determinant() == -2.42f);

	



	//cin >> matInput;
	//cout << matInput;

}

void testMat3() {
	Mat3 mat(-9.9f, 2.2f, 3.3f,
		4.4f, 5.5f, 6.6f,
		7.7f, 8.8f, 9.9f);

	Mat3 mat2 { 1, 2, 3,
				0, 1, 4,
				5, 6, 0};

	print(mat);
	print(mat.transpose());
	print(mat.transpose().transpose());
	print(mat.determinant());
	print(mat2.inverse());
}

void testMat4() {
	Mat4 mat(-9.9f, 2.2f, 3.3f, 43.23f,
		4.4f, 5.5f, 6.6f, 321.42f,
		7.7f, 8.8f, 9.9f, 492.31f,
		3.1f, 31.3f, 14.3, 3.15f);

	Mat4 mat2 { 1, 2, 3, 4,
		0, 1, 4, 5,
		5, 6, 0, 24,
		31.3f, 33.3f, 31.3f, 3.14f };

	print(mat);
	print(mat.transpose());
}

void testMatFactory() {
	Mat3 identity = MatrixFactory::identity3();
	Mat3 scalingMat = MatrixFactory::scaling3(Vec3(0.25f, 0.5f, 0.75f));
	Mat4 translateMatrix = MatrixFactory::translate(Vec3(10, -4, 2));
	Mat3 rotation = MatrixFactory::rotate3(Vec3(1, 0, 0), 3.14f / 2);

	print(identity);
	print(scalingMat);
	print(translateMatrix);
	print(rotation);
}

#define PI 3.14159265358979f

void testRodrigues() {
	float angle = PI / 2;
	Vec3 axis[3] = {
		Vec3(1, 0, 0),
		Vec3(0, 2, 0),
		Vec3(0, 0, 3)
	};
	Mat3 identity = MatrixFactory::identity3(); //todo
	Mat3 rotationMatrices[3];
	for (size_t i = 0; i < 3; i++)
	{
		Vec3 normalized = axis[i].normalize();
		Mat3 a = MatrixFactory::dual(normalized);
		
		rotationMatrices[i] = identity 
							+ (a * sin(angle))
						+ (a * a) * (1 - cos(angle));
		print(rotationMatrices[i]);
		print(i);
	}
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			Vec3 result = rotationMatrices[j] * axis[i];
			cout << result << " ";
		}
		cout << '\n';
	}
}

int main() {
	cout << "TESTES VEC2\n\n";
	testVec2();
	cout << "\nTESTES VEC3\n\n";
	testVec3();
	cout << "\nTESTES MAT2\n\n";
	testMat2();
	cout << "\nTESTES MAT3\n\n";
	testMat3();
	cout << "\nTESTES MAT4\n\n";
	testMat4();
	cout << "\nTESTES MAT FACTORY\n\n";
	testMatFactory();
	//testRodrigues();

	//Wait one last enter key before closing
	getchar();
	return 0;

}
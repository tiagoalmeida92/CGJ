#include "utils.hpp"

int getIdx(int row, int column, int size) {
	return row * size + column;
}

void swap(float arr[], int idxA, int idxB) {
	float copy = arr[idxA];
	arr[idxA] = arr[idxB];
	arr[idxB] = copy;
}
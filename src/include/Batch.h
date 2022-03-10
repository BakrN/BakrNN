#pragma once 
#include <initializer_list>
#include <stdint.h>
#include "Core.h"
struct BNN_LIB BatchDimensions {
	uint32_t rows; 
	uint32_t cols; 
	uint32_t z; 
	uint32_t channels; 
	BatchDimensions() : rows{ 0 }, cols{ 0 }, z{ 0 }, channels{ 0 } {};
	uint32_t GetCount() { return rows * cols * z * channels; }; 
	uint32_t& operator[](uint8_t index) {
		if (index == 0) {
			return rows; 
		}
		if (index == 1) {
			return cols; 
		}
		if (index == 2) {
			return z; 
		}
		if (index == 3) {
			return channels; 
		}

	}
};
class BNN_LIB Batch {
private:
	BatchDimensions m_Dimensions;  // rows cols z #channels
	Batch* m_Parent; 
	float* m_Data; 

public:
	Batch(const std::initializer_list<uint32_t>& dimensions);
	Batch(const std::initializer_list<uint32_t>& dimensions, float* data); 
	Batch(Batch* parent, const std::initializer_list<uint32_t>& dimensions, float* data); 
	~Batch(); 
	void Normalize(); // batch normalization 
	BatchDimensions GetDimensions() {return m_Dimensions;} ;
	float* GetRef() { return m_Data; }; 

	Batch operator [](uint32_t v1); 
};

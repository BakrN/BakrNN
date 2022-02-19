#pragma once 
#include <initializer_list>

class Batch {
private:
	uint32_t m_Dimensions[4]; // rows cols z #channels
	Batch* m_Parent; 
	float* m_Data; 
	float m_Bias;
	uint32_t count;
public:
	Batch(const std::initializer_list<uint32_t>& dimensions);
	Batch(const std::initializer_list<uint32_t>& dimensions, float* data); 
	Batch(Batch* parent, const std::initializer_list<uint32_t>& dimensions, float* data); 
	~Batch(); 
	void Normalize(); // batch normalization 
	uint32_t* GetDimensions() {return m_Dimensions;} ;
	float* GetRef() { return m_Data; }; 

	Batch operator [](uint32_t v1); 
};

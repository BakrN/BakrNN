#include "include\Batch.h"
#include <assert.h>
Batch::Batch(const std::initializer_list<uint32_t>& dimensions)
{
	for (int i = 0; i < dimensions.size(); i++) {
		assert(dimensions.begin()[i] > 0); m_Dimensions[i] = dimensions.begin()[i];
	}
	// allocate data memory ; 
	m_Data = new float[m_Dimensions.GetCount()] ; 
}

Batch::Batch(const std::initializer_list<uint32_t>& dimensions, float* data)
{

	for (int i = 0; i < dimensions.size(); i++) {
		assert(dimensions.begin()[i] > 0);  m_Dimensions[i] = dimensions.begin()[i];
	}
	m_Data = data; 
	m_Parent = nullptr; 

}

Batch::Batch(Batch* parent, const std::initializer_list<uint32_t>& dimensions, float* data)
{
	m_Parent = parent; 
	m_Data = data;
	for (int i = 0; i < dimensions.size(); i++) {
		assert(dimensions.begin()[i] > 0);  m_Dimensions[i] = dimensions.begin()[i];
	}
}

Batch::~Batch()
{
	delete[] m_Data; 
}

/// <summary>
/// This function normalizes the values in the Batch
/// </summary>
void Batch::Normalize()
{
	
}



Batch Batch::operator[](uint32_t v1) {
	if (m_Dimensions[0] == 1) {
		if (m_Dimensions[1] == 1)
		{
			if (m_Dimensions[2] == 1) {
				if (m_Dimensions[3] == 1) {
					//assert(); 
					//return this; 
					
				}
				assert(v1 < m_Dimensions[3] && v1 >= 0);
				float* data = m_Data + v1 * (m_Parent->GetDimensions()[0] * m_Parent->GetDimensions()[1] * m_Parent->GetDimensions()[2]);
				return Batch({1,1,1,1}, data);
			}
						// z access 
			assert(v1 < m_Dimensions[2] && v1 >= 0);
			float* data = m_Data + v1*(m_Parent->GetDimensions()[0] * m_Parent->GetDimensions()[1]);
				
			return Batch(this, { 1,1, 1, m_Dimensions[3] }, data);
		}
		// col access 
		assert(v1 < m_Dimensions[1] && v1 >= 0); 
		float* data = m_Data + v1; 
		
		return Batch(this, {1,1, m_Dimensions[2] , m_Dimensions[3]}, data);
	}
	// row access 
	assert(v1 < m_Dimensions[0] && v1 >= 0);
	float* data = m_Data + v1 * m_Dimensions[1];

	return Batch(this, { 1, m_Dimensions[1], m_Dimensions[2] , m_Dimensions[3] }, data);

}

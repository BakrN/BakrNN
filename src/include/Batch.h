#pragma once 
#include <initializer_list>
#include <stdint.h>
#include <assert>
struct  BatchDimensions {
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
template <typename T> 
class  Batch {
private:
	BatchDimensions m_Dimensions;  // rows cols z #channels
	Batch* m_Parent; 
	T* m_Data; 
public:
	Batch() { this->m_Data = NULL; this->m_Parent = NULL; m_Dimensions = BatchDimensions();  };
	Batch(const std::initializer_list<uint32_t>& dimensions);
	Batch(const std::initializer_list<uint32_t>& dimensions, T* data); 
	Batch(Batch* parent, const std::initializer_list<uint32_t>& dimensions, T* data); 
	~Batch(); 
	void Normalize(); // batch normalization 
	BatchDimensions GetDimensions() {return m_Dimensions;} ;
	T* GetRef() { return m_Data; }; 
	Batch operator =(const Batch& batch) {
		this->m_Data = batch.m_Data; 
		this->m_Parent = batch.m_Parent;  
		this->m_Dimensions = batch.m_Dimensions; 
	}
	void Concatenate(const Batch<T>& batch) {
		if (batch.GetRef() == NULL) {
			return;
		}
		assert( this->m_Dimensions.rows == batch.GetDimensions().rows && this->m_Dimensions.cols == batch.GetDimensions().cols ); 
		if (this->m_Data = NULL) {
			this->m_Data = batch.m_Data;
			this->m_Parent = batch.m_Parent;
			this->m_Dimensions = batch.m_Dimensions;
			return;
		}
		assert(decltype(this->m_Data) == decltype(batch.GetRef())); // check if both contain same data format 
		m_Dimensions.z += batch.GetDimensions().z; 
		m_Data = realloc(m_Data, sizeof(T) * (batch.GetDimensions().GetCount() + this->GetDimensions().GetCount()); 
		T* ptr = m_Data + this->GetDimensions().GetCount(); 
		memcpy(ptr, batch.GetRef(), sizeof(decltype(T)) * (batch.GetDimensions().GetCount())); 
	}

	Batch operator [](uint32_t v1); 
};


template <typename T> 

inline Batch<T>::Batch(const std::initializer_list<uint32_t>& dimensions)
{
	for (int i = 0; i < dimensions.size(); i++) {
		assert(dimensions.begin()[i] > 0); m_Dimensions[i] = dimensions.begin()[i];
	}
	// allocate data memory ; 
	m_Data = new T[m_Dimensions.GetCount()];
}
template <typename T> 
inline Batch<T>::Batch(const std::initializer_list<uint32_t>& dimensions, T* data)
{
	for (int i = 0; i < dimensions.size(); i++) {
		assert(dimensions.begin()[i] > 0);  m_Dimensions[i] = dimensions.begin()[i];
	}
	m_Data = data;
	m_Parent = nullptr;35
}
template <typename T> 
Batch<T>::Batch(Batch* parent, const std::initializer_list<uint32_t>& dimensions, T* data)
{
	m_Parent = parent;
	m_Data = data;
	for (int i = 0; i < dimensions.size(); i++) {
		assert(dimensions.begin()[i] > 0);  m_Dimensions[i] = dimensions.begin()[i];
	}
}
template <typename T> 
inline Batch<T> ::~Batch()
{
	delete[] m_Data;
}

/// <summary>
/// This function normalizes the values in the Batch
/// </summary>
/// 
template <typename T> 
inline void Batch<T>::Normalize()
{

}


template <typename T>
inline Batch<T> Batch<T>::operator[](uint32_t v1) {
	if (m_Dimensions[0] == 1) {
		if (m_Dimensions[1] == 1)
		{
			if (m_Dimensions[2] == 1) {
				if (m_Dimensions[3] == 1) {
					//assert(); 
					//return this; 

				}
				assert(v1 < m_Dimensions[3] && v1 >= 0);
				T* data = m_Data + v1 * (m_Parent->GetDimensions()[0] * m_Parent->GetDimensions()[1] * m_Parent->GetDimensions()[2]);
				return Batch({ 1,1,1,1 }, data);
			}
			// z access 
			assert(v1 < m_Dimensions[2] && v1 >= 0);
			T* data = m_Data + v1 * (m_Parent->GetDimensions()[0] * m_Parent->GetDimensions()[1]);

			return Batch(this, { 1,1, 1, m_Dimensions[3] }, data);
		}
		// col access 
		assert(v1 < m_Dimensions[1] && v1 >= 0);
		T* data = m_Data + v1;

		return Batch(this, { 1,1, m_Dimensions[2] , m_Dimensions[3] }, data);
	}
	// row access 
	assert(v1 < m_Dimensions[0] && v1 >= 0);
	T* data = m_Data + v1 * m_Dimensions[1];

	return Batch(this, { 1, m_Dimensions[1], m_Dimensions[2] , m_Dimensions[3] }, data);

}

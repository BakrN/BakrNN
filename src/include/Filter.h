#pragma once 

template <typename T>
class  Filter {
private:
	T* m_Weights; // void* due to difference in type and size
	int m_FilterDimensions[4]; // rows // cols // z // count

public:
	Filter(int dimensions[4]) { m_FilterDimensions = dimensions;  weights = new T[dimensions[0] * dimensions[1] * dimensions[2] * dimensions[3]]; }
	Filter() { m_FilterDimensions = { 0,0,0,0 } };
	~Filter() { if (m_Weights) { delete[] m_Weights } };
	void SetDimension(int dimensions[4]) {};
	T* GetWeights() { return Ref; }
};
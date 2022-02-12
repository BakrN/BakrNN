#pragma once 
#define USE_OPENCL
#ifdef USE_OPENCL
#include <opencl>
#endif 
#include <string> 
#include "Layer.h"
template <typename T>
class NeuralNetwork {

private:
	std::vector<Layer> m_Layers;
	std::vector<T> m_Output;
	std::vector<T> m_Input;
	int m_InputDimension[3];
public:
	NeuralNetwork();
	~NeuralNetwork();

	void  ReadFromTorch(const std::string& ConfigFile, const std::string& weights);
	void AppendLayer(LayerType type, LayerInfo info, NodeActivation activation = RELU);
	void SetInput(void* input_data, int dimensions);

	void Forward();
	std::vector<T>& GetOutput() { return m_Output; };

};


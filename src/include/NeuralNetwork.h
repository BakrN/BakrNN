#pragma once 
#include <string> 
#include "Layer.h"
class NeuralNetwork {

private:
	std::vector<Layer> m_Layers;
	std::shared_ptr<Batch> m_Output;
	std::shared_ptr<Batch> m_Input;
public:
	NeuralNetwork();
	~NeuralNetwork();
	
	void ReadFromTorch(const std::string& ConfigFile, const std::string& weights);
	void AppendLayer(LayerType type, const LayerInfo& info);
	
	void SetInput(const std::shared_ptr<Batch>& input) { m_Input;  };
	
	void Forward();
	std::shared_ptr<Batch>& GetOutput() { return m_Output; };

};


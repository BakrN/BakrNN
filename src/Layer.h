#pragma once 
#include <vector>
class Batch {
private:
	uint32_t m_Dimensions[3]; // x y z 
	float* m_Data;
	float bias;
	uint32_t count;
public:
	Batch();
	~Batch();
	void Normalize(); // batch normalization 
	float* GetData() { return m_Data; };
};

enum LayerType { // Needed layer types for YOLO 
	CONVOLUTIONAL,
	MAXPOOL,
	FC,
	ROUTE,
	SHORTCUT
};
enum NodeActivation {
	RELU,
	SIGMOID,
	TANH,
	LINEAR
};
struct LayerInfo {
	int stride;
	int pad;
	int filter_count;  // for convolutional layers 
	int filter_size;
};

class Layer {
private:
	LayerType m_Type;
	LayerInfo m_Info;
	NodeActivation m_Activation;
	Batch* m_Input;
	Layer* m_Layer;
	friend class ConvLayer;
public:
	Layer(LayerType type, );
	Layer(LayerType type, NodeActivation function);
	~Layer();

	virtual void Propogate() = 0;
	virtual void SetInput(const Batch& input) = 0;
	LayerType GetLayerType() { return m_Type; };
};


class ConvLayer : Layer {
	// Inherited via Layer
	virtual void Propogate() override;
};

class MaxPoolLayer : Layer {
	virtual void Propogate() override;
};
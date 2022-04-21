#pragma once 
#include <vector>
#include <memory> 
#include <algorithm>
#include "Batch.h"	
 enum class  LayerType { // Needed layer types for YOLO 
	UNDEFINED, 
	CONVOLUTIONAL,
	MAXPOOL,
	FC,
	ROUTE,
	SHORTCUT, 
	UPSAMPLE, 
	YOLO 
};
 NodeActivation MapActivationFromString(const std::string& activation) {
	 std::for_each(activation.begin(), activation.end(), [](char& c) {
		 c = ::tolower(c);
		 });
	 switch (activation) {
	 case "sigmoid":
		 return NodeActivation::SIGMOID;
	 case "tanh":
		 return NodeActivation::TANH;
	 case "relu":
		 return NodeActivation::RELU;
	 case "linear":
		 return NodeActivation::LINEAR;
	 case "mish":
		 return NodeActivation::MISH;
	 case "leaky":
		 return NodeActivation::LEAKY;
	 case "softmax":
		 return NodeActivation::SOFTMAX;
	 default: 
		 return NodeActivation::RELU; 
	 }
 }
enum  NodeActivation : int {
	SIGMOID=0,
	TANH=1,
	RELU=2, 
	LINEAR=3, 
	MISH=4, 
	LEAKY=5 , 
	SOFTMAX
};


struct  ConvLayerInfo {
	int filter_count; 
	int stride; 
	int padding; 
	int size; // same size as input
	NodeActivation activation; 
};
struct  RouteLayerInfo {
	std::vector<int> RouteLayers; // list of all routing layers 
};
struct  ShortcutLayerInfo {
	int short_layer; 
	NodeActivation activation; 
};

struct  MaxpoolLayerInfo {
	int stride; 
	int size;

};
struct  UpsampleLayerInfo {
	int stride;
};
struct  YoloLayerInfo {
		std::vector<int> mask , 
		std::vector<int> anchors, 
		int classes = 80, 
		int num = 9,
		float jitter = .3,
		float ignore_thresh = .7,
		float truth_thresh = 1,
		float scale_x_y = 1.2,
		float iou_thresh = 0.213,
		float cls_normalizer = 1.0,
		float iou_normalizer = 0.07,
		iou_loss = ciou,
		nms_kind = greedynms,
		float beta_nms = 0.6
};
// this could be improved.. uses extra few kilobytes for N
union  LayerInfo {
	ConvLayerInfo ConvInfo;
	RouteLayerInfo RouteInfo; 
	ShortcutLayerInfo ShortcutInfo; 
	MaxpoolLayerInfo MaxpoolInfo;
	UpsampleLayerInfo UpsampleInfo; 
	YoloLayerInfo YoloInfo; 
};
template <typename T> 
class  Layer {

private:
	LayerType m_Type  = LayerType::UNDEFINED ;
	LayerInfo m_Info;
	std::shared_ptr<Batch<T>> m_Input;
	std::shared_ptr<Batch<T>> m_Output; 
	std::vector<T> m_Biases; 
	friend class ConvLayer;
	friend class MaxPoolLayer; 
	friend class RouteLayer; 
	friend class ShortcutLayer; 
	friend class Upsamplelayer; 
	friend class YoloLayer; 
public:
	Layer(LayerType type,const LayerInfo& info );
	Layer(LayerType type,const LayerInfo& info , const std::shared_ptr<Batch<T>>& input);
	Layer(LayerType type); 
	void SetInput(const std::shared_ptr<Batch>& input) { m_Input = input;  };
	void AddBias(T bias) {m_Biases.push_back(bias);}
	virtual void AllocateHostMemoryOutput() = 0 ; 
	virtual void Propogate() = 0;
	
	LayerType GetLayerType() { return m_Type; };
	auto GetOutput() { return m_Output;  }
};
template <typename T> 
inline Layer<T>::Layer(LayerType type, const LayerInfo& info)
{
	this->m_Type = type;
	this->m_Info = info;

}

template <typename T> 
inline Layer<T>::Layer(LayerType type, const LayerInfo& info, const std::shared_ptr<Batch<T>>& input)
{
	this->m_Type = type;
	this->m_Info = info;
	this->m_Input = input;

}

template <typename T> 
inline Layer<T> ::Layer(LayerType type)
{
	this->m_Type = type;
	this->m_Info = { 0 };
}



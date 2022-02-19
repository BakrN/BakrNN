#pragma once 
#include <vector>
#include <memory> 
#include "Batch.h"	

enum LayerType { // Needed layer types for YOLO 
	UNDEFINED, 
	CONVOLUTIONAL,
	MAXPOOL,
	FC,
	ROUTE,
	SHORTCUT, 
	UPSAMPLE, 
	YOLO 
};
enum NodeActivation {
	RELU,
	SIGMOID,
	TANH,
	LINEAR, 
	MISH, 
	LEAKY 
};
struct ConvLayerInfo {
	int filter_count; 
	int size; 
	int stride; 
	int padding; 
	NodeActivation activation; 
};
struct RouteLayerInfo {
	int* RouteLayers; 
	int RouteLayersCount; 

};
struct ShortcutLayerInfo {
	int short_layer; 
	NodeActivation activation; 
};

struct MaxpoolLayerInfo {
	int stride; 
	int size; 

};
struct UpsampleLayerInfo {
	int stride;
};
struct YoloLayerInfo {
/*	mask = 0, 1, 2
		anchors = 12, 16, 19, 36, 40, 28, 36, 75, 76, 55, 72, 146, 142, 110, 192, 243, 459, 401
		classes = 80
		num = 9
		jitter = .3
		ignore_thresh = .7
		truth_thresh = 1
		scale_x_y = 1.2
		iou_thresh = 0.213
		cls_normalizer = 1.0
		iou_normalizer = 0.07
		iou_loss = ciou
		nms_kind = greedynms
		beta_nms = 0.6*/
};
// this could be improved.. uses extra few kilobytes for NN 
union LayerInfo {
	ConvLayerInfo ConvInfo;
	RouteLayerInfo RouteInfo; 
	ShortcutLayerInfo ShortcutInfo; 
	MaxpoolLayerInfo MaxpoolInfo;
	UpsampleLayerInfo UpsampleInfo; 
	YoloLayerInfo YoloInfo; 
};
class Layer {
private:
	LayerType m_Type  = UNDEFINED ;
	LayerInfo m_Info; 
	std::shared_ptr<Batch> m_Input;
	std::shared_ptr<Batch> m_Output; 
	friend class ConvLayer;
	friend class MaxPoolLayer; 
	friend class RouteLayer; 
	friend class ShortcutLayer; 
	friend class Upsamplelayer; 
	friend class YoloLayer; 
public:
	Layer(LayerType type,const LayerInfo& info );
	Layer(LayerType type,const LayerInfo& info , const std::shared_ptr<Batch>& input);
	Layer(LayerType type); 

	~Layer();
	void SetInput(const std::shared_ptr<Batch>& input) { m_Input = input;  };
	

	virtual void Init() = 0 ; // allocate memory 
	virtual void Propogate() = 0;

	LayerType GetLayerType() { return m_Type; };
};


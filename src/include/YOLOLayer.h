#pragma once 

#include "Layer.h"
template <typename T>

class YOLOLayer : Layer<T> {
	YOLOLayer(YoloLayerInfo info) {
		this.m_Info = info;
		
	}
	virtual void AllocateHostMemoryOutput() override;
	virtual void Propogate() override;

};

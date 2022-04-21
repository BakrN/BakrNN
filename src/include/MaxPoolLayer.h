#pragma once 

#include "Layer.h"

template <typename T> 

class MaxPoolLayer : Layer<T> {
	MaxPoolLayer(MaxpoolLayerInfo info) {
		this.m_Info = info; 

	}
	virtual void AllocateHostMemoryOutput() override;
	virtual void Propogate() override;

};
#pragma once 

#include "Layer.h"
template <typename T>

class UpsampleLayers : Layer<T> {
	UpsampleLayer(UpsampleLayerInfo info) {
		this.m_Info = info;

	}
	virtual void AllocateHostMemoryOutput() override;
	virtual void Propogate() override;

};

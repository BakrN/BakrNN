#pragma once 

#include "Layer.h"
template <typename T>

class ShortcutLayer : Layer<T> {
	ShortcutLayer(ShortcutLayerInfo info) {
		this.m_Info = info;

	}
	virtual void AllocateHostMemoryOutput() override;
	virtual void Propogate() override;

};

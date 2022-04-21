#pragma once 

#include "Layer.h"
#include <initializer_list>
template <typename T>

class RouteLayer : Layer<T> {
	RouteLayer(RouteLayerInfo info) {
		this.m_Info = info;
		this.m_Input = std::make_shared<Batch<T>>();
		this.m_Output = m_Input; 
	} 
	virtual void AllocateHostMemoryOutput() override;
	virtual void Propogate() override;
	AddRouting(const std::initializer_list<Layer*>& layers) {
		for (auto it = layers.begin(); it != layers.end(); it++) {
			this->m_Input->Concatenate(it->GetOutput()); 
			
		}	
	}
};
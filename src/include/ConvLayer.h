#pragma once 
#include "Layer.h"
#include "OpenCLWrapper.h"
#include "Filter.h"
typedef struct __attribute__ ((packed)) {
    int i_width; 
    int i_height; 
    int i_z; 

    int filter_count ; 
    int filter_width; 
    int filter_height; 

    int o_width ;
    int o_height; 
    
    int stride; 
    
    float bias ; 
    int act_flag; 
} ConvolutionParams; 

template <typename T> 
class ConvLayer : Layer<T> {
private: 
	std::shared_ptr<Filter> m_Filters; 
public: 
	ConvLayer(ConvLayerInfo info) {
		this->m_Info = info;
	} 
	virtual void AllocateHostMemoryOutput() override;
	virtual void Propogate() override; 
	void AddFilters(const std::shared_ptr<Filter<T>>& filters) { m_Filters = filters;  };
};

template<typename T>
inline void ConvLayer<T>::AllocateHostMemoryOutput()
{
		
		m_Ouput = std::make_shared<Batch<T>>({ (this->m_Input->GetDimensions().rows - this.m_Filters[m_Fileter.size()-1].dimensions[0] + 2 * this->m_Info.ConvLayerInfo.padding) / this->m_Info.ConvLayerInfo.stride + 1 
		,(this->m_Input->GetDimensions().cols - this.m_Filters[m_Fileter.size() - 1].dimensions[1] + 2 * this->m_Info.ConvLayerInfo.padding) / this->m_Info.ConvLayerInfo.stride + 1
		,m_Input->GetDimensions().z  });
		
	 // allocate host memory for output 
}

template<typename T>
inline void ConvLayer<T>::Propogate()
{
	// create clBuffer 
	
	// create kernel if not there and use and pass args for every layers 
	
	// run conv layer 


}

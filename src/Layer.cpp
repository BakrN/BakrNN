#include "include/Layer.h"
Layer::Layer(LayerType type, const LayerInfo& info)
{
	this->m_Type = type; 
	this->m_Info = info; 

}

Layer::Layer(LayerType type, const LayerInfo& info, const std::shared_ptr<Batch>& input)
{
	this->m_Type = type;
	this->m_Info = info;
	this->m_Input = input; 

}

Layer::Layer(LayerType type)
{
	this->m_Type = type; 
	this->m_Info = {0}; 
}

#pragma once 
#include <string> 
#include "Layer.h"
#include "Core.h"
#include <fstream>

template <typename T> 
class  NeuralNetwork {

private:
	std::vector<Layer> m_Layers;
	std::shared_ptr<Batch<T>> m_Output;
	std::shared_ptr<Batch<T>> m_Input;
LayerInfo ReadFromDarknet(const std::ifstream& config, LayerType type); 
public:
	NeuralNetwork();
	NeuralNetwork(const std::string& config_file, const std::string& weights_file,enum CONFIGFILE_TYPE type ); 
	~NeuralNetwork();
	
	void ReadNNArchitecture(const std::string& ConfigFile, const std::string& WeightsFile);

	void AppendLayer(LayerType type, const LayerInfo& info);
	
	void SetInput(const std::shared_ptr<Batch<T>>& input) { m_Input;  };
	
	void Forward();
	std::shared_ptr<Batch<T>>& GetOutput() { return m_Output; };

};

template<typename T>
inline LayerInfo NeuralNetwork<T>::ReadFromDarknet(const std::ifstream& config, LayerType type)
{
	LayerInfo info; 
	std::string line;
	std::getline(config, line); 
	while (!line.empty()) {
		if (type == LayerType::CONVOLUTIONAL) {
			// rememeber to add batch normalization
			int pos = line.find("="); 
			
			if (line.find("filters") != std::string::npos) {
				info.ConvInfo.filter_count = std::stoi(line.substr(pos+1));
			}
			else if (line.find("size") != std::string::npos) {
				info.ConvInfo.size = std::stoi(line.substr(pos + 1));
			}
			else if (line.find("stride") != std::string::npos) {
				info.ConvInfo.stride = std::stoi(line.substr(pos + 1));
			}
			else if (line.find("pad") != std::string::npos) {
				info.ConvInfo.padding = std::stoi(line.substr(pos + 1)); // same size as input
			}
			else if (line.find("activation") != std::string::npos) {
				info.ConvInfo.activation = MapActivationFromString(line.substr(pos + 1));
			}
			
		}
		else if (type == LayerType::MAXPOOL) {
			int pos = line.find("=");
			if (line.find("size") != std::string::npos) {
				info.MaxpoolInfo.size = std::stoi(line.substr(pos + 1));
			}
			else if (line.find("stride") != std::string::npos) {
				info.MaxpoolInfo.stride = std::stoi(line.substr(pos + 1));
			}
		}
		else if (type == LayerType::ROUTE) {
			if (line.find("layers") != std::string::npos) {
				size_t prev = line.find("="); 
				size_t comma_place  = line.find(","); 
				while (comma_place != std::string::npos) {
					info.RouteInfo.RouteLayers.push_back(std::stoi(line.substr(prev+1, comma_place -prev-1  ))); 
					prev = comma_place; 
					comma_place  = line.find(",", comma_place+1); 
				}
				info.RouteInfo.RouteLayers.push_back(std::stoi(line.substr(prev + 1))); 
			}
		}
		else if (type == LayerType::SHORTCUT) {

		}
		else if (type == LayerType::UPSAMPLE) {

		}
		else if (type == LayerType::YOLO) {
	
		}
		std::getline(config, line); 
	}
	return;
}

template<typename T>
inline NeuralNetwork<T>::NeuralNetwork(const std::string& config_file, const std::string& weights_file, enum CONFIGFILE_TYPE type )
{
	switch (type) {
	case PYTORCH: 
		ReadNNArchitecture(config_file, weights_file); 
	case TENSORFLOW: 
		// Not implemented yet 
	default: 
	}
}

template<typename T>
inline void ReadNNArchitecture(const std::string& ConfigFile, const std::string& WeightsFile)
{
	std::ifstream config(ConfigFile); 
	for (std::string line; std::getline(config, line); ) {
		if (line.find("[convolutional]") != std::string::npos) {
			// convolutional layers add batch normalization
			this->AppendLayer(LayerType::CONVOLUTIONAL, ReadFromDarknet(config, LayerType::CONVOLUTIONAL));
		}
		else if (line.find("[maxpool]") != std::string::npos) {
			this->AppendLayer(LayerType::MAXPOOL, ReadFromDarknet(config, LayerType::MAXPOOL)); 
		}
		else if (line.find("[route]") != std::string::npos) {
		}
		else if (line.find("[upsample]") != std::string::npos) {
		}
		else if (line.find("[shortcut]") != std::string::npos) {
		}
		else if (line.find("[yolo]") != std::string::npos) {
		}
	}
	std::ifstream weights_file; 
	for (std::string line; std::getline(config, line); ) {

	}
}

template<typename T>
inline void NeuralNetwork<T>::AppendLayer(LayerType type, const LayerInfo& info)
{
	switch (type) {
	case CONVOLUTIONAL: 
		m_Layers.push_back(ConvLayer<T>(info.ConvInfo)); 
	case MAXPOOL: 

	case FC:

	case ROUTE:

	case SHORTCUT:

	case UPSAMPLE:

	case YOLO: 

	}
}

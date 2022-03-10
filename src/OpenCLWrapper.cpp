#include "include\OpenCLWrapper.h"
#include "include\OpenCLWrapper.h"
#include "include\OpenCLWrapper.h"
#include "include\OpenCLWrapper.h"
void OpenCLProgram::FetchPlatforms() {

	if (cl::Platform::get(&m_Platforms) != CL_SUCCESS) {
		// error; 
	}
	if (m_Platforms.size() == 0) {
		// didn't find opencl platforms ; 
	}
}

void OpenCLProgram::FetchDevices()
{
	// for now only taking devices of first platform of all types 
	for (auto& platform : m_Platforms){
		platform.getDevices(CL_DEVICE_TYPE_ALL, &m_Devices);; 
	}
}

void OpenCLProgram::CreateContextsAndQueues()
{
	for (int i = 0; i < m_Devices.size(); i++) {
		m_Contexts.push_back(cl::Context(m_Devices[i]));
		m_CommandQueues.push_back(cl::CommandQueue(m_Contexts[i]));
	}
}

void OpenCLProgram::Init()
{
	FetchPlatforms();
	FetchDevices(); 
	CreateContextsAndQueues(); 
	
}
void OpenCLProgram::AddBuffer(const std::string& buffer_name, const std::vector<float>& data, const cl::Context& context, int work_dimension[3], bool READ_ONLY)
{
	if (m_Buffers.find(buffer_name) == m_Buffers.end()) {
	
		m_Buffers[buffer_name] = std::move(clBuffer(clBuffer(work_dimension, data, context, READ_ONLY))); 
		return; 
	}
	// buffer with name already exists 

}

void OpenCLProgram::CreateKernels(const std::string& file_path)
{

}


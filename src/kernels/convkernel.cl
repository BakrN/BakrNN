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

/**
    Activation Functions
 */

float sigmoid(float z){return 1.0/(1.0+exp(-z));}

/*****************************/
__kernel  void convolve(__global float *input,ConvolutionParams conv_params, __global float *output, __global float* filter){
         
         const int x_in=get_global_id(0);//output col
         const int y_in=get_global_id(1);//output row
          
		const int filter_count=get_global_id(2);//filter count

        
         
         float sum=0;
         

            for (int i=0;i<conv_params.filter_width;i++ ){
                for (int j=0;j<conv_params.filter_height;j++){
                    for (int k = 0 ; k < conv_params.i_z; k++ )
                    sum += input[x_in*conv_params.stride + i + (y_in*conv_params.stride+ j) *  + k*conv_params.i_width*conv_params.i_height] * filter[i+j*conv_params.i_width+ k*conv_params.i_width*conv_params.i_height ] ; 
                }
        }

        output[x_in + y_in*conv_params.o_width + filter_count*conv_params.o_width*conv_params.o_height] = sum ; 
        
		
 }
         
    __kernel void apply_activation(__global float* input, int i_width, int i_height, float bias, int act_flag) { 
         const int x_in=get_global_id(0);//cols
         const int y_in=get_global_id(1);//rows

		  const int z=get_global_id(2);//
          float sum = input[x_in+y_in*i_width +z*i_width*i_height] + bias; 
		 switch(act_flag){
			case 0: input[x_in+y_in*i_width +z*i_width*i_height] = sigmoid(sum);break;
			case 1: input[x_in+y_in*i_width +z*i_width*i_height] = tanh(sum);break;
            case 3: input[x_in+y_in*i_width +z*i_width*i_height] = max(sum, 0.0f);break; // linear  - to be implemented 
            case 4: input[x_in+y_in*i_width +z*i_width*i_height] = max(sum, 0.0f);break; // mish  - to be implemented 
            case 5: input[x_in+y_in*i_width +z*i_width*i_height] = max(sum, 0.0f);break; // leaky - to be implemented 
            case 6:  input[x_in+y_in*i_width +z*i_width*i_height] = max(sum, 0.0f);break; // softmax - to be implemented 
			default: input[x_in+y_in*i_width +z*i_width*i_height] = max(sum, 0.0f);break; // relu 
		}
    }
 
  __kernel void max_pool( __global float* input,__global float* output,int width, int height, int stride , int pool_dim){
 
       const int x=get_global_id(0); // output rows 
       const int y=get_global_id(1); // output cols 
       const int z=get_global_id(2); // output z 
        float value = 0.0f ; 
        for ( int i = 0 ; i < pool_dim ; i ++) { 
            for ( int j = 0 ; j  < pool_dim; j++ ) {
               value = max( input[x*stride+i  +(y*stride + j)*width +z*width*height]  , value) ; 
            }
        }
                                               
        output[x+y*width +z*width*height] = value;  
         
 
 }
 
 
 __kernel void rotatemat( __global float* source,__global float* destin,int dim){
	
	const int xIn=get_global_id(0);
    const int yIn=get_global_id(1);

	destin[xIn+dim*yIn]=source[(dim-xIn)+dim*(dim-yIn)];
 
 }
 
 
__kernel batch_normalize(__global input, int x_dim, int y_dim, int z_dim , int channels, float gamma , float beta ) { 
    
}
__kernel add_inputs(__global input1, __global input2,  __global output, int act_flag) { 
    const int x = get_global_id(0) ; 
    output[x] = input1[x]+ input2[x] ; 
  
}
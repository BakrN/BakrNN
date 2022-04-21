typedef struct  { 
    void* weights; 
    int dimensions[3] ; 
}Filter ; 


__kernel  void convolve(__global float *image, __global Filter* filters, __global float * featMap,int filterWidth,int inWidth,int featmapdim){
         
         const int xIn=get_global_id(0);//cols
         const int yIn=get_global_id(1);//rows
          
		  const int z=get_global_id(2);//filters

         
         float sum=0;
         for (int r=0;r<filterWidth;r++){
             for (int c=0;c<filterWidth;c++){
                 
                 sum+= filters[z].weights[c*filterWidth +r]*image[(xIn+c)+inWidth*(yIn+r)];
                //sum+= filters[z].weights[(filterWidth-c)+ filterWidth*(filterWidth-r)]*image[(xIn+c)+inWidth *(yIn+r)];
                 
                 
            }
        }
        
		sum +=filters[z].bias;
		 switch(actflag){
			case 0: featMap[(xIn+yIn*featmapdim +z*featmapdim*featmapdim)] =sigmoid(sum);break;
			case 1: featMap[(xIn+yIn*featmapdim +z*featmapdim*featmapdim)]=mtanh(sum);break;
			case 2: featMap[(xIn+yIn*featmapdim +z*featmapdim*featmapdim)]=relu(sum);break;
		}
 }
         

 
  __kernel void max_pool( __global float* prevfeatMap,__global float* poolMap,__global int* indexes,int Width,int pooldim){
 
 const int xIn=get_global_id(0);
 const int yIn=get_global_id(1);
 
  const int z=get_global_id(2);

 

     float max=0;
	 int index=0;
         for (int r=0;r<2;r++){
             for (int c=0;c<2;c++){
                 
               
                 if(prevfeatMap[(yIn+c)*Width*z +(xIn+r)]>max){
                       max=prevfeatMap[(yIn+c)*Width*z +(xIn+r)];
					   index=c*2+r;
					   }
						
                 
                 }
             }
             
             poolMap[(xIn+yIn*pooldim +z*pooldim*pooldim)]=max;
			 indexes[(xIn+yIn*pooldim +z*pooldim*pooldim)]=index;
    
                                               
         
         
 
 }
 
 

 __kernel void rotatemat( __global float* source,__global float* destin,int dim){
	
	const int xIn=get_global_id(0);
    const int yIn=get_global_id(1);

	destin[xIn+dim*yIn]=source[(dim-xIn)+dim*(dim-yIn)];
 
 }
 
 
 

   
__kernel void cnntoFcnn(__global float* poolMap,__global Node* nodes,int inputsize,int mapindex){


        const int xIn=get_global_id(0);
         const int yIn=get_global_id(1);
         
         const int z=get_global_id(2);
     
                 
        nodes[(xIn+yIn*inputsize +z*inputsize*inputsize)].output =poolMap[(xIn+yIn*inputsize +z*inputsize*inputsize)];
                   

 }


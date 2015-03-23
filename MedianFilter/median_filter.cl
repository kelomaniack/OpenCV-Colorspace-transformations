                                                                                                    
__constant sampler_t image_sampler = CLK_NORMALIZED_COORDS_FALSE                                      
                                     | CLK_ADDRESS_CLAMP_TO_EDGE                                      
                                     | CLK_FILTER_NEAREST;                                            
                                                                                                      
__kernel void median_filter_kernel(__read_only image2d_t in_image,                                    
                            __write_only image2d_t out_image,                                         
                            int windowSize)                                                            
{                                                                                                      
    unsigned int x = get_global_id(0);                                                                 
    unsigned int y = get_global_id(1);                                                                 
    int halfWindow = windowSize/2;                                                                     
    float4 pixelValue;                                                                                 
    int i, j, ifilter, jfilter;                                                                        
    float oldPixels[9]; float tmp;                                                                     
    int index =0;                                                                                      
    //Load the window in oldPixels                                                                     
    for(i=-halfWindow, ifilter=0; i<=halfWindow; i++, ifilter++){                                      
       for(j=-halfWindow, jfilter=0; j<=halfWindow; j++, jfilter++){                                   
           pixelValue = read_imagef(in_image, image_sampler, (int2)(x+i, y+j));                        
           oldPixels[index] = pixelValue.x;                                                            
           index++;                                                                                    
       }                                                                                               
    }                                                                                                  
                                                                                                       
    // Find the rank-th element                                                                        
    int totalNumber = windowSize*windowSize;                                                           
    int rank = totalNumber/2 +1;                                                                       
    for(int i=0; i< rank;++i)                                                                          
    {                                                                                                  
        for(int j=0; j< totalNumber-1 ; ++j)                                                           
        {                                                                                              
            if(oldPixels[j] > oldPixels[j+1])                                                          
            {                                                                                          
                tmp = oldPixels[j];                                                                    
                oldPixels[j] = oldPixels[j+1];                                                         
                oldPixels[j+1] = tmp;                                                                  
            }                                                                                          
        }                                                                                              
        totalNumber--;                                                                                 
    }                                                                                                  
                                                                                                       
    //median is oldPixels[rank], update                                                                
    pixelValue.x = oldPixels[rank];                                                                    
                                                                                                       
                                                                                                       
    write_imagef(out_image, (int2)(x, y), pixelValue);                                                 
};
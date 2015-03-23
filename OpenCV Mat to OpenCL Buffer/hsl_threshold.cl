const sampler_t smp = CLK_NORMALIZED_COORDS_FALSE | //Natural coordinates
						CLK_ADDRESS_CLAMP | //Clamp to zeros
						CLK_FILTER_NEAREST; //Don't interpolate

__kernel void hsl_threshold(__read_only image2d_t img1,
	__write_only image2d_t img2) 
{

	int2 coord = (int2)(get_global_id(0), get_global_id(1));
	float4 val = read_imagef(img1, smp, coord);

	val.w = 0;
	write_imagef(img2, coord, val);
}
#define wgs 16
//Work group size
#define cas3(a, b)                              
do {

	float4 x = a;                           
	int4 c = a> b;                          
	a.s012 = select(b, a, c).s012;          
	b.s012 = select(x, b, c).s012;          
} while (0)

__kernel void median3x3_rgb(read_only image2d_t src, write_only image2d_t dst) {
	int gx = get_global_id(0), gy = get_global_id(1);
	int lx = get_local_id(0), ly = get_local_id(1);
	const sampler_t smp = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;

	if ((gx >= get_image_width(dst)) | (gy >= get_image_height(dst)))
		return;

	__local float4 la[wgs + 2][wgs + 2];

	la[lx + 1][ly + 1] = read_imagef(src, smp, (int2)(gx, gy));

	if (lx == 0){
		la[lx][ly + 1] = read_imagef(src, smp, (int2)(gx - 1, gy));
		if (ly == 0)
			la[lx + 1][ly] = read_imagef(src, smp, (int2)(gx - 1, gy));
		if (ly == wgs)
			la[lx + 1][ly + 2] = read_imagef(src, smp, (int2)(gx + 1, gy));
	}
	else if (lx == wgs){
		la[lx + 2][ly + 1] = read_imagef(src, smp, (int2)(gx + 1, gy));
		if (ly == 0)
			la[lx + 1][ly] = read_imagef(src, smp, (int2)(gx - 1, gy));
		if (ly == wgs)
			la[lx + 1][ly + 2] = read_imagef(src, smp, (int2)(gx + 1, gy));
	}
	else if (ly == 0)
		la[lx + 1][ly] = read_imagef(src, smp, (int2)(gx - 1, gy));
	else if (ly == wgs)
		la[lx + 1][ly + 2] = read_imagef(src, smp, (int2)(gx + 1, gy));


	barrier(CLK_LOCAL_MEM_FENCE); //----------------------- mem barrier

	float4 s0 = la[lx - 1][ly - 1];
	float4 s1 = la[lx][ly - 1];
	float4 s2 = la[lx + 1][ly - 1];
	float4 s3 = la[lx - 1][ly];
	float4 s4 = la[lx][ly];
	float4 s5 = la[lx + 1][ly];
	float4 s6 = la[lx - 1][ly + 1];
	float4 s7 = la[lx][ly + 1];
	float4 s8 = la[lx + 1][ly + 1];
	//Starting the sorting this area works fine don't worry about it

	// stage0
	cas3(s1, s2);
	cas3(s4, s5);
	cas3(s7, s8);

	// 1
	cas3(s0, s1);
	cas3(s3, s4);
	cas3(s6, s7);

	// 2
	cas3(s1, s2);
	cas3(s4, s5);
	cas3(s7, s8);

	// 3/4
	cas3(s3, s6);
	cas3(s4, s7);
	cas3(s5, s8);
	cas3(s0, s3);

	cas3(s1, s4);
	cas3(s2, s5);
	cas3(s3, s6);

	cas3(s4, s7);
	cas3(s1, s3);

	cas3(s2, s6);
	cas3(s2, s3);
	cas3(s4, s6);

	cas3(s3, s4);

	write_imagef(dst, (int2)(gx, gy), s4);
};
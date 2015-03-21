/*
Copyright (c) 2011 Michael Zucchi

This file is part of socles, an OpenCL image processing library.

socles is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

socles is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with socles.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
Median filters
*/

// 84uS
#define cas(a, b)                               
do {

	float x = a;                    
	int c = a > b;                  
	a = c ? b : a;                  
	b = c ? x : b;                  
} while (0)

#define cas2(a, b)                              
do {

	float4 x = a;                           
	int4 c = a> b;                          
	a.s01 = select(b, a, c).s01;           
	b.s01 = select(x, b, c).s01;            
} while (0)

// 175
#define cas3(a, b)                              
do {

	float4 x = a;                           
	int4 c = a> b;                          
	a.s012 = select(b, a, c).s012;          
	b.s012 = select(x, b, c).s012;          
} while (0)

// 238
#define cas4(a, b)                              
do {

	float4 x = a;                   
	int4 c = a > b;                 
	a = select(b, a, c);            
	b = select(x, b, c);            
} while (0)


// 3x3 median filter
// uses a sorting network to sort entirely in registers with no branches
// sorting network from knuth volume 3, S5.3.4
__kernel void median3x3_r(__read_only image2d_t src, write_only image2d_t dst) {
	int gx = get_global_id(0), gy = get_global_id(1);
	const sampler_t smp = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;

	if ((gx >= get_image_width(dst)) | (gy >= get_image_height(dst)))
		return;

	float s0 = read_imagef(src, smp, (int2) { gx - 1, gy - 1 }).s0;
	float s1 = read_imagef(src, smp, (int2) { gx, gy - 1 }).s0;
	float s2 = read_imagef(src, smp, (int2) { gx + 1, gy - 1 }).s0;
	float s3 = read_imagef(src, smp, (int2) { gx - 1, gy }).s0;
	float s4 = read_imagef(src, smp, (int2) { gx, gy }).s0;
	float s5 = read_imagef(src, smp, (int2) { gx + 1, gy }).s0;
	float s6 = read_imagef(src, smp, (int2) { gx - 1, gy + 1 }).s0;
	float s7 = read_imagef(src, smp, (int2) { gx, gy + 1 }).s0;
	float s8 = read_imagef(src, smp, (int2) { gx + 1, gy + 1 }).s0;

	// stage0
	cas(s1, s2);
	cas(s4, s5);
	cas(s7, s8);

	// 1
	cas(s0, s1);
	cas(s3, s4);
	cas(s6, s7);

	// 2
	cas(s1, s2);
	cas(s4, s5);
	cas(s7, s8);

	// 3/4
	cas(s3, s6);
	cas(s4, s7);
	cas(s5, s8);
	cas(s0, s3);

	cas(s1, s4);
	cas(s2, s5);
	cas(s3, s6);

	//      cas(s5, s8); // not needed for median
	cas(s4, s7);
	cas(s1, s3);

	cas(s2, s6);
	//      cas(s5, s7); // not needed for median
	cas(s2, s3);
	cas(s4, s6);

	cas(s3, s4);
	//      cas(s5, s6); // not needed for median

	write_imagef(dst, (int2)(gx, gy), (float4)s4);
};

// rgb version
__kernel void median3x3_rgb(__read_only image2d_t src, write_only image2d_t dst) {
	int gx = get_global_id(0), gy = get_global_id(1);
	const sampler_t smp = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;

	if ((gx >= get_image_width(dst)) | (gy >= get_image_height(dst)))
		return;

	float4 s0 = read_imagef(src, smp, (int2) { gx - 1, gy - 1 });
	float4 s1 = read_imagef(src, smp, (int2) { gx, gy - 1 });
	float4 s2 = read_imagef(src, smp, (int2) { gx + 1, gy - 1 });
	float4 s3 = read_imagef(src, smp, (int2) { gx - 1, gy });
	float4 s4 = read_imagef(src, smp, (int2) { gx, gy });
	float4 s5 = read_imagef(src, smp, (int2) { gx + 1, gy });
	float4 s6 = read_imagef(src, smp, (int2) { gx - 1, gy + 1 });
	float4 s7 = read_imagef(src, smp, (int2) { gx, gy + 1 });
	float4 s8 = read_imagef(src, smp, (int2) { gx + 1, gy + 1 });

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

	//      cas3(s5, s8); // not needed for median
	cas3(s4, s7);
	cas3(s1, s3);

	cas3(s2, s6);
	//      cas3(s5, s7); // not needed for median
	cas3(s2, s3);
	cas3(s4, s6);

	cas3(s3, s4);
	//      cas3(s5, s6); // not needed for median

	write_imagef(dst, (int2)(gx, gy), s4);
};
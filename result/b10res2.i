// ROSHAN PATEL
// CS415: COMPILERS PROJECT 1
// 	 LOCAL REGISTER ALLOCATION
// Output ILOC instructions for report2.i
// Total physical register = 10
// Allocator type: b

	loadI	1020	=> r0
	loadI	1024	=> r2	
	loadI	1028	=> r3	
	load	r2	=> r2	
	load	r3	=> r3	
	loadI	4	=> r4	
	loadI	1024	=> r5	
	store	r2	=> r5	
	add	r2, r3		=> r2	
	add	r5, r4		=> r5	
	store	r2	=> r5	
	add	r2, r3		=> r2	
	add	r5, r4		=> r5	
	store	r2	=> r5	
	add	r2, r3		=> r2	
	add	r5, r4		=> r5	
	store	r2	=> r5	
	add	r2, r3		=> r2	
	add	r5, r4		=> r5	
	store	r2	=> r5	
	add	r2, r3		=> r2	
	add	r5, r4		=> r5	
	store	r2	=> r5	
	add	r2, r3		=> r2	
	add	r5, r4		=> r5	
	store	r2	=> r5	
	add	r2, r3		=> r2	
	add	r5, r4		=> r5	
	store	r2	=> r5	
	add	r2, r3		=> r2	
	add	r5, r4		=> r5	
	store	r2	=> r5	
	add	r2, r3		=> r2	
	add	r5, r4		=> r5	
	store	r2	=> r5	
	add	r2, r3		=> r3	
	add	r5, r4		=> r5	
	store	r3	=> r5	
	loadI	1024	=> r5	
	load	r5	=> r2	
	add	r5, r4		=> r5	
	load	r5	=> r3	
	add	r5, r4		=> r4	
	load	r4	=> r5	
	add	r2, r3		=> r3	
	load	r4	=> r4	
	add	r4, r4		=> r4	
	mult	r3, r4		=> r4	
	loadI	2048	=> r2	
	store	r4	=> r2	
	output	2048


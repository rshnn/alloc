// ROSHAN PATEL
// CS415: COMPILERS PROJECT 1
// 	 LOCAL REGISTER ALLOCATION
// Output ILOC instructions for report2.i
// Total physical register = 15
// Allocator type: t

	loadI	1020	=> r0
	loadI	1024	=> r3
	loadI	1028	=> r4
	load	r3	=> r3
	load	r4	=> r4
	loadI	4	=> r5
	loadI	1024	=> r6
	store	r3	=> r6
	add	r3 ,r4	=> r3
	add	r6 ,r5	=> r6
	store	r3	=> r6
	add	r3 ,r4	=> r3
	add	r6 ,r5	=> r6
	store	r3	=> r6
	add	r3 ,r4	=> r3
	add	r6 ,r5	=> r6
	store	r3	=> r6
	add	r3 ,r4	=> r3
	add	r6 ,r5	=> r6
	store	r3	=> r6
	add	r3 ,r4	=> r3
	add	r6 ,r5	=> r6
	store	r3	=> r6
	add	r3 ,r4	=> r3
	add	r6 ,r5	=> r6
	store	r3	=> r6
	add	r3 ,r4	=> r3
	add	r6 ,r5	=> r6
	store	r3	=> r6
	add	r3 ,r4	=> r3
	add	r6 ,r5	=> r6
	store	r3	=> r6
	add	r3 ,r4	=> r3
	add	r6 ,r5	=> r6
	store	r3	=> r6
	add	r3 ,r4	=> r3
	add	r6 ,r5	=> r4
	store	r3	=> r4
	loadI	1024	=> r3
	load	r3	=> r4
	add	r3 ,r5	=> r3
	load	r3	=> r6
	add	r3 ,r5	=> r3
	load	r3	=> r5
	add	r4 ,r6	=> r5
	load	r3	=> r4
	add	r4 ,r4	=> r3
	mult	r5 ,r3	=> r3
	loadI	2048	=> r4
	store	r3	=> r4
	output	2048

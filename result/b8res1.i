// ROSHAN PATEL
// CS415: COMPILERS PROJECT 1
// 	 LOCAL REGISTER ALLOCATION
// Output ILOC instructions for report1.i
// Total physical register = 8
// Allocator type: b

	loadI	1020	=> r0
	loadI	4000	=> r2	
	loadI	4	=> r3	
	loadI	1	=> r4	
	load	r2	=> r5	
	add	r2, r3		=> r2	
	load	r2	=> r6	
	add	r2, r3		=> r2	
	load	r2	=> r7	
	add	r2, r3		=> r2	
	loadI	0	=> r1
	add	r0, r1	=> r1
	store	r7	=> r1
	load	r2	=> r7	
	add	r2, r3		=> r2	
	loadI	-4	=> r1
	add	r0, r1	=> r1
	store	r7	=> r1
	load	r2	=> r7	
	add	r2, r3		=> r2	
	loadI	-8	=> r1
	add	r0, r1	=> r1
	store	r7	=> r1
	load	r2	=> r7	
	add	r2, r3		=> r2	
	loadI	-12	=> r1
	add	r0, r1	=> r1
	store	r7	=> r1
	load	r2	=> r7	
	add	r2, r3		=> r2	
	loadI	-16	=> r1
	add	r0, r1	=> r1
	store	r7	=> r1
	load	r2	=> r7	
	add	r2, r3		=> r2	
	loadI	-20	=> r1
	add	r0, r1	=> r1
	store	r7	=> r1
	load	r2	=> r7	
	add	r2, r3		=> r2	
	loadI	-24	=> r1
	add	r0, r1	=> r1
	store	r7	=> r1
	load	r2	=> r7	
	add	r2, r3		=> r3	
	loadI	0	=> r2	
	loadI	-28	=> r1
	add	r0, r1	=> r1
	store	r2	=> r1
	add	r5, r6		=> r2	
	mult	r4, r6		=> r4	
	sub	r7, r5		=> r5	
	loadI	-32	=> r1
	add	r0, r1	=> r1
	store	r3	=> r1
	loadI	0	=> r1
	add	r0, r1	=> r1
	load	r1	=> r3
	add	r2, r3		=> r2	
	mult	r4, r3		=> r4	
	loadI	-36	=> r1
	add	r0, r1	=> r1
	store	r6	=> r1
	loadI	-24	=> r1
	add	r0, r1	=> r1
	load	r1	=> r6
	add	r5, r6		=> r5	
	loadI	-40	=> r1
	add	r0, r1	=> r1
	store	r7	=> r1
	loadI	-4	=> r1
	add	r0, r1	=> r1
	load	r1	=> r7
	add	r2, r7		=> r2	
	mult	r4, r7		=> r4	
	loadI	0	=> r1
	add	r0, r1	=> r1
	store	r3	=> r1
	loadI	-20	=> r1
	add	r0, r1	=> r1
	load	r1	=> r3
	sub	r5, r3		=> r5	
	loadI	-24	=> r1
	add	r0, r1	=> r1
	store	r6	=> r1
	loadI	-8	=> r1
	add	r0, r1	=> r1
	load	r1	=> r6
	add	r2, r6		=> r2	
	mult	r4, r6		=> r4	
	loadI	-4	=> r1
	add	r0, r1	=> r1
	store	r7	=> r1
	loadI	-16	=> r1
	add	r0, r1	=> r1
	load	r1	=> r7
	add	r5, r7		=> r5	
	loadI	-20	=> r1
	add	r0, r1	=> r1
	store	r3	=> r1
	loadI	-12	=> r1
	add	r0, r1	=> r1
	load	r1	=> r3
	add	r2, r3		=> r2	
	mult	r4, r3		=> r4	
	sub	r5, r3		=> r3	
	add	r2, r7		=> r2	
	mult	r4, r7		=> r7	
	add	r5, r6		=> r6	
	loadI	16	=> r4	
	rshift	r7, r4		=> r4	
	loadI	-20	=> r1
	add	r0, r1	=> r1
	load	r1	=> r5
	add	r2, r5		=> r2	
	mult	r4, r5		=> r5	
	loadI	-4	=> r1
	add	r0, r1	=> r1
	load	r1	=> r4
	sub	r6, r4		=> r4	
	loadI	-24	=> r1
	add	r0, r1	=> r1
	load	r1	=> r6
	add	r2, r6		=> r2	
	mult	r5, r6		=> r6	
	loadI	0	=> r1
	add	r0, r1	=> r1
	load	r1	=> r5
	add	r4, r5		=> r5	
	loadI	-40	=> r1
	add	r0, r1	=> r1
	load	r1	=> r4
	add	r2, r4		=> r2	
	mult	r6, r4		=> r4	
	loadI	-36	=> r1
	add	r0, r1	=> r1
	load	r1	=> r6
	sub	r5, r6		=> r6	
	loadI	1024	=> r5	
	store	r2	=> r5	
	loadI	1028	=> r5	
	store	r4	=> r5	
	loadI	1032	=> r5	
	store	r6	=> r5	
	output	1024

	output	1028

	output	1032


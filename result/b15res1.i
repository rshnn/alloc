// ROSHAN PATEL
// CS415: COMPILERS PROJECT 1
// 	 LOCAL REGISTER ALLOCATION
// Output ILOC instructions for report1.i
// Total physical register = 15
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
	load	r2	=> r8	
	add	r2, r3		=> r2	
	load	r2	=> r9	
	add	r2, r3		=> r2	
	load	r2	=> r10	
	add	r2, r3		=> r2	
	load	r2	=> r11	
	add	r2, r3		=> r2	
	load	r2	=> r12	
	add	r2, r3		=> r2	
	load	r2	=> r13	
	add	r2, r3		=> r2	
	load	r2	=> r14	
	add	r2, r3		=> r3	
	loadI	0	=> r2	
	loadI	0	=> r1
	add	r0, r1	=> r1
	store	r2	=> r1
	add	r5, r6		=> r2	
	mult	r4, r6		=> r4	
	sub	r14, r5		=> r5	
	add	r2, r7		=> r2	
	mult	r4, r7		=> r4	
	add	r5, r13		=> r5	
	add	r2, r8		=> r2	
	mult	r4, r8		=> r4	
	sub	r5, r12		=> r5	
	add	r2, r9		=> r2	
	mult	r4, r9		=> r4	
	add	r5, r11		=> r5	
	add	r2, r10		=> r2	
	mult	r4, r10		=> r4	
	sub	r5, r10		=> r10	
	add	r2, r11		=> r2	
	mult	r4, r11		=> r11	
	add	r5, r9		=> r9	
	loadI	16	=> r4	
	rshift	r11, r4		=> r4	
	add	r2, r12		=> r2	
	mult	r4, r12		=> r12	
	sub	r9, r8		=> r8	
	add	r2, r13		=> r2	
	mult	r12, r13		=> r13	
	add	r8, r7		=> r7	
	add	r2, r14		=> r2	
	mult	r13, r14		=> r14	
	sub	r7, r6		=> r6	
	loadI	1024	=> r4	
	store	r2	=> r4	
	loadI	1028	=> r4	
	store	r14	=> r4	
	loadI	1032	=> r4	
	store	r6	=> r4	
	output	1024

	output	1028

	output	1032


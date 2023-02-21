(define (problem transport-long-path)
	(:domain transport)
	
(:objects
	p_0_0 p_1_0 p_0_1 p_1_1 - place
	b1 - box
	t1 - truck
)

(:init
	(connected p_0_0 p_1_0)
	(connected p_1_0 p_0_0)
	(connected p_0_0 p_0_1)
	(connected p_0_1 p_0_0)
	(connected p_0_1 p_1_1)
	(connected p_1_1 p_0_1)
	(connected p_1_0 p_1_1)
	(connected p_1_1 p_1_0)
	(at t1 p_0_0)
	(at b1 p_0_0)
	(empty t1)
)

(:goal
	(at b1 p_1_1)
)
)

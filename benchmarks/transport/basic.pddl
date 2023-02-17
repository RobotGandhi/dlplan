(define (problem transport-basic)
	(:domain transport)
	
(:objects
	p1 p2 - place
	b1 - box
	t1 - truck
)

(:init
	(connected p1 p2)
	(connected p2 p1)
	(at t1 p1)
	(at b1 p1)
	(empty t1)
)

(:goal
	(at b1 p2)
)
)

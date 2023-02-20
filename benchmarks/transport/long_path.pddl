(define (problem transport-long-path)
	(:domain transport)
	
(:objects
	p1 p2 p3 p4 - place
	b1 - box
	t1 - truck
)

(:init
	(connected p1 p2)
	(connected p2 p1)
	(connected p2 p3)
	(connected p3 p2)
	(connected p3 p4)
	(connected p4 p3)
	(at t1 p1)
	(at b1 p1)
	(empty t1)
)

(:goal
	(at b1 p4)
)
)

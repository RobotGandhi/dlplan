(define (domain transport)
(:requirements :typing)
(:types	place - object
		movable - object
		box - movable
		truck - movable
)

(:predicates	(at ?x1 - movable ?x2 - place)
		(in ?x1 - box ?x2 - truck)
		(connected ?x1 - place ?x2 - place)
		(empty ?x1 - truck)
)

(:action pick-up-box
:parameters (?t - truck ?b - box ?x - place)
:precondition (and (at ?b ?x) (at ?t ?x) (empty ?t))
:effect (and
(not (at ?b ?x))
(not (empty ?t))
(in ?b ?t))
)


(:action drop-box
:parameters (?t - truck ?b - box ?x - place)
:precondition (and (at ?t ?x) (in ?b ?t))
:effect (and
(empty ?t)
(not (in ?b ?t))
(at ?b ?x))
)


(:action move
:parameters (?t - truck ?from - place ?to - place)
:precondition (and (connected ?from ?to) (at ?t ?from))
:effect (and
(not (at ?t ?from))
(at ?t ?to))
)

)

module training/training

go 1.17

replace training/sort => ../sort

require (
	training/fibonacci v0.0.0-00010101000000-000000000000
	training/sort v0.0.0-00010101000000-000000000000
	training/tree v0.0.0-00010101000000-000000000000
)

replace training/tree => ../tree

replace training/fibonacci => ../fibonacci

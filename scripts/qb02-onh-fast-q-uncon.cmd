# N-queens with one-hot encoding.  N = 2
# Preconstrain method: None
and preconstrain
var r-0.c-0 r-0.c-1 r-1.c-0 r-1.c-1
# Row Constraints
# Row 0
and n_t0
or okr.0
ite okr.0 r-0.c-0 n_t0 okr.0
and n_t0 n_t0 !r-0.c-0
ite okr.0 r-0.c-1 n_t0 okr.0
and n_t0 n_t0 !r-0.c-1
delete n_t0
# Row 1
and n_t1
or okr.1
ite okr.1 r-1.c-0 n_t1 okr.1
and n_t1 n_t1 !r-1.c-0
ite okr.1 r-1.c-1 n_t1 okr.1
and n_t1 n_t1 !r-1.c-1
delete n_t1
and okR okr.0 okr.1
delete okr.0 okr.1
# Column Constraints
# Column 0
and n_t2
or okc.0
ite okc.0 r-0.c-0 n_t2 okc.0
and n_t2 n_t2 !r-0.c-0
ite okc.0 r-1.c-0 n_t2 okc.0
and n_t2 n_t2 !r-1.c-0
delete n_t2
and okc.0 okc.0 preconstrain
# Column 1
and n_t3
or okc.1
ite okc.1 r-0.c-1 n_t3 okc.1
and n_t3 n_t3 !r-0.c-1
ite okc.1 r-1.c-1 n_t3 okc.1
and n_t3 n_t3 !r-1.c-1
delete n_t3
and okc.1 okc.1 preconstrain
and okC okc.0 okc.1
delete okc.0 okc.1
# Combine row and column
and okRC okR okC
delete okC
delete okR
# Diagonal Constraints
# Diagonal -1
and n_t4
or n_t5
ite n_t5 r-1.c-0 n_t4 n_t5
and n_t4 n_t4 !r-1.c-0
or okd.0 n_t4 n_t5
delete n_t4
delete n_t5
and okd.0 okd.0 preconstrain
# Diagonal 0
and n_t6
or n_t7
ite n_t7 r-0.c-0 n_t6 n_t7
and n_t6 n_t6 !r-0.c-0
ite n_t7 r-1.c-1 n_t6 n_t7
and n_t6 n_t6 !r-1.c-1
or okd.1 n_t6 n_t7
delete n_t7
delete n_t6
and okd.1 okd.1 preconstrain
# Diagonal 1
and n_t8
or n_t9
ite n_t9 r-0.c-1 n_t8 n_t9
and n_t8 n_t8 !r-0.c-1
or okd.2 n_t8 n_t9
delete n_t8
delete n_t9
and okd.2 okd.2 preconstrain
and okD okd.0 okd.1 okd.2
delete okd.0 okd.1 okd.2
# Add diagonal to row & column
and okRCD okRC okD
delete okRC
delete okD
# Off-diagonal Constraints
# Off-diagonal -1
and n_t10
or n_t11
ite n_t11 r-0.c-0 n_t10 n_t11
and n_t10 n_t10 !r-0.c-0
or oko.0 n_t10 n_t11
delete n_t10
delete n_t11
and oko.0 oko.0 preconstrain
# Off-diagonal 0
and n_t12
or n_t13
ite n_t13 r-1.c-0 n_t12 n_t13
and n_t12 n_t12 !r-1.c-0
ite n_t13 r-0.c-1 n_t12 n_t13
and n_t12 n_t12 !r-0.c-1
or oko.1 n_t12 n_t13
delete n_t13
delete n_t12
and oko.1 oko.1 preconstrain
# Off-diagonal 1
and n_t14
or n_t15
ite n_t15 r-1.c-1 n_t14 n_t15
and n_t14 n_t14 !r-1.c-1
or oko.2 n_t14 n_t15
delete n_t14
delete n_t15
and oko.2 oko.2 preconstrain
and okO oko.0 oko.1 oko.2
delete oko.0 oko.1 oko.2
# Add off diagonal to row, column, and diagonal
and ok okRCD okO
delete okO
delete okRCD
delete preconstrain
# BDD generation completed
time
info ok
# Model counting
count ok
status
# Flush state
flush
# Exit
quit

# N-queens with one-hot encoding.  N = 8
# Preconstrain method: None
and preconstrain
var r-0.c-0 r-0.c-1 r-0.c-2 r-0.c-3 r-0.c-4 r-0.c-5 r-0.c-6 r-0.c-7 r-1.c-0 r-1.c-1 r-1.c-2 r-1.c-3 r-1.c-4 r-1.c-5 r-1.c-6 r-1.c-7 r-2.c-0 r-2.c-1 r-2.c-2 r-2.c-3 r-2.c-4 r-2.c-5 r-2.c-6 r-2.c-7 r-3.c-0 r-3.c-1 r-3.c-2 r-3.c-3 r-3.c-4 r-3.c-5 r-3.c-6 r-3.c-7 r-4.c-0 r-4.c-1 r-4.c-2 r-4.c-3 r-4.c-4 r-4.c-5 r-4.c-6 r-4.c-7 r-5.c-0 r-5.c-1 r-5.c-2 r-5.c-3 r-5.c-4 r-5.c-5 r-5.c-6 r-5.c-7 r-6.c-0 r-6.c-1 r-6.c-2 r-6.c-3 r-6.c-4 r-6.c-5 r-6.c-6 r-6.c-7 r-7.c-0 r-7.c-1 r-7.c-2 r-7.c-3 r-7.c-4 r-7.c-5 r-7.c-6 r-7.c-7
# Row Constraints
# Row 0
and n_t0
or okr.0
ite okr.0 r-0.c-0 n_t0 okr.0
and n_t0 n_t0 !r-0.c-0
ite okr.0 r-0.c-1 n_t0 okr.0
and n_t0 n_t0 !r-0.c-1
ite okr.0 r-0.c-2 n_t0 okr.0
and n_t0 n_t0 !r-0.c-2
ite okr.0 r-0.c-3 n_t0 okr.0
and n_t0 n_t0 !r-0.c-3
ite okr.0 r-0.c-4 n_t0 okr.0
and n_t0 n_t0 !r-0.c-4
ite okr.0 r-0.c-5 n_t0 okr.0
and n_t0 n_t0 !r-0.c-5
ite okr.0 r-0.c-6 n_t0 okr.0
and n_t0 n_t0 !r-0.c-6
ite okr.0 r-0.c-7 n_t0 okr.0
and n_t0 n_t0 !r-0.c-7
delete n_t0
# Row 1
and n_t1
or okr.1
ite okr.1 r-1.c-0 n_t1 okr.1
and n_t1 n_t1 !r-1.c-0
ite okr.1 r-1.c-1 n_t1 okr.1
and n_t1 n_t1 !r-1.c-1
ite okr.1 r-1.c-2 n_t1 okr.1
and n_t1 n_t1 !r-1.c-2
ite okr.1 r-1.c-3 n_t1 okr.1
and n_t1 n_t1 !r-1.c-3
ite okr.1 r-1.c-4 n_t1 okr.1
and n_t1 n_t1 !r-1.c-4
ite okr.1 r-1.c-5 n_t1 okr.1
and n_t1 n_t1 !r-1.c-5
ite okr.1 r-1.c-6 n_t1 okr.1
and n_t1 n_t1 !r-1.c-6
ite okr.1 r-1.c-7 n_t1 okr.1
and n_t1 n_t1 !r-1.c-7
delete n_t1
# Row 2
and n_t2
or okr.2
ite okr.2 r-2.c-0 n_t2 okr.2
and n_t2 n_t2 !r-2.c-0
ite okr.2 r-2.c-1 n_t2 okr.2
and n_t2 n_t2 !r-2.c-1
ite okr.2 r-2.c-2 n_t2 okr.2
and n_t2 n_t2 !r-2.c-2
ite okr.2 r-2.c-3 n_t2 okr.2
and n_t2 n_t2 !r-2.c-3
ite okr.2 r-2.c-4 n_t2 okr.2
and n_t2 n_t2 !r-2.c-4
ite okr.2 r-2.c-5 n_t2 okr.2
and n_t2 n_t2 !r-2.c-5
ite okr.2 r-2.c-6 n_t2 okr.2
and n_t2 n_t2 !r-2.c-6
ite okr.2 r-2.c-7 n_t2 okr.2
and n_t2 n_t2 !r-2.c-7
delete n_t2
# Row 3
and n_t3
or okr.3
ite okr.3 r-3.c-0 n_t3 okr.3
and n_t3 n_t3 !r-3.c-0
ite okr.3 r-3.c-1 n_t3 okr.3
and n_t3 n_t3 !r-3.c-1
ite okr.3 r-3.c-2 n_t3 okr.3
and n_t3 n_t3 !r-3.c-2
ite okr.3 r-3.c-3 n_t3 okr.3
and n_t3 n_t3 !r-3.c-3
ite okr.3 r-3.c-4 n_t3 okr.3
and n_t3 n_t3 !r-3.c-4
ite okr.3 r-3.c-5 n_t3 okr.3
and n_t3 n_t3 !r-3.c-5
ite okr.3 r-3.c-6 n_t3 okr.3
and n_t3 n_t3 !r-3.c-6
ite okr.3 r-3.c-7 n_t3 okr.3
and n_t3 n_t3 !r-3.c-7
delete n_t3
# Row 4
and n_t4
or okr.4
ite okr.4 r-4.c-0 n_t4 okr.4
and n_t4 n_t4 !r-4.c-0
ite okr.4 r-4.c-1 n_t4 okr.4
and n_t4 n_t4 !r-4.c-1
ite okr.4 r-4.c-2 n_t4 okr.4
and n_t4 n_t4 !r-4.c-2
ite okr.4 r-4.c-3 n_t4 okr.4
and n_t4 n_t4 !r-4.c-3
ite okr.4 r-4.c-4 n_t4 okr.4
and n_t4 n_t4 !r-4.c-4
ite okr.4 r-4.c-5 n_t4 okr.4
and n_t4 n_t4 !r-4.c-5
ite okr.4 r-4.c-6 n_t4 okr.4
and n_t4 n_t4 !r-4.c-6
ite okr.4 r-4.c-7 n_t4 okr.4
and n_t4 n_t4 !r-4.c-7
delete n_t4
# Row 5
and n_t5
or okr.5
ite okr.5 r-5.c-0 n_t5 okr.5
and n_t5 n_t5 !r-5.c-0
ite okr.5 r-5.c-1 n_t5 okr.5
and n_t5 n_t5 !r-5.c-1
ite okr.5 r-5.c-2 n_t5 okr.5
and n_t5 n_t5 !r-5.c-2
ite okr.5 r-5.c-3 n_t5 okr.5
and n_t5 n_t5 !r-5.c-3
ite okr.5 r-5.c-4 n_t5 okr.5
and n_t5 n_t5 !r-5.c-4
ite okr.5 r-5.c-5 n_t5 okr.5
and n_t5 n_t5 !r-5.c-5
ite okr.5 r-5.c-6 n_t5 okr.5
and n_t5 n_t5 !r-5.c-6
ite okr.5 r-5.c-7 n_t5 okr.5
and n_t5 n_t5 !r-5.c-7
delete n_t5
# Row 6
and n_t6
or okr.6
ite okr.6 r-6.c-0 n_t6 okr.6
and n_t6 n_t6 !r-6.c-0
ite okr.6 r-6.c-1 n_t6 okr.6
and n_t6 n_t6 !r-6.c-1
ite okr.6 r-6.c-2 n_t6 okr.6
and n_t6 n_t6 !r-6.c-2
ite okr.6 r-6.c-3 n_t6 okr.6
and n_t6 n_t6 !r-6.c-3
ite okr.6 r-6.c-4 n_t6 okr.6
and n_t6 n_t6 !r-6.c-4
ite okr.6 r-6.c-5 n_t6 okr.6
and n_t6 n_t6 !r-6.c-5
ite okr.6 r-6.c-6 n_t6 okr.6
and n_t6 n_t6 !r-6.c-6
ite okr.6 r-6.c-7 n_t6 okr.6
and n_t6 n_t6 !r-6.c-7
delete n_t6
# Row 7
and n_t7
or okr.7
ite okr.7 r-7.c-0 n_t7 okr.7
and n_t7 n_t7 !r-7.c-0
ite okr.7 r-7.c-1 n_t7 okr.7
and n_t7 n_t7 !r-7.c-1
ite okr.7 r-7.c-2 n_t7 okr.7
and n_t7 n_t7 !r-7.c-2
ite okr.7 r-7.c-3 n_t7 okr.7
and n_t7 n_t7 !r-7.c-3
ite okr.7 r-7.c-4 n_t7 okr.7
and n_t7 n_t7 !r-7.c-4
ite okr.7 r-7.c-5 n_t7 okr.7
and n_t7 n_t7 !r-7.c-5
ite okr.7 r-7.c-6 n_t7 okr.7
and n_t7 n_t7 !r-7.c-6
ite okr.7 r-7.c-7 n_t7 okr.7
and n_t7 n_t7 !r-7.c-7
delete n_t7
and okR okr.0 okr.1 okr.2 okr.3 okr.4 okr.5 okr.6 okr.7
delete okr.0 okr.1 okr.2 okr.3 okr.4 okr.5 okr.6 okr.7
# Column Constraints
# Column 0
and n_t8
or okc.0
ite okc.0 r-0.c-0 n_t8 okc.0
and n_t8 n_t8 !r-0.c-0
ite okc.0 r-1.c-0 n_t8 okc.0
and n_t8 n_t8 !r-1.c-0
ite okc.0 r-2.c-0 n_t8 okc.0
and n_t8 n_t8 !r-2.c-0
ite okc.0 r-3.c-0 n_t8 okc.0
and n_t8 n_t8 !r-3.c-0
ite okc.0 r-4.c-0 n_t8 okc.0
and n_t8 n_t8 !r-4.c-0
ite okc.0 r-5.c-0 n_t8 okc.0
and n_t8 n_t8 !r-5.c-0
ite okc.0 r-6.c-0 n_t8 okc.0
and n_t8 n_t8 !r-6.c-0
ite okc.0 r-7.c-0 n_t8 okc.0
and n_t8 n_t8 !r-7.c-0
delete n_t8
and okc.0 okc.0 preconstrain
# Column 1
and n_t9
or okc.1
ite okc.1 r-0.c-1 n_t9 okc.1
and n_t9 n_t9 !r-0.c-1
ite okc.1 r-1.c-1 n_t9 okc.1
and n_t9 n_t9 !r-1.c-1
ite okc.1 r-2.c-1 n_t9 okc.1
and n_t9 n_t9 !r-2.c-1
ite okc.1 r-3.c-1 n_t9 okc.1
and n_t9 n_t9 !r-3.c-1
ite okc.1 r-4.c-1 n_t9 okc.1
and n_t9 n_t9 !r-4.c-1
ite okc.1 r-5.c-1 n_t9 okc.1
and n_t9 n_t9 !r-5.c-1
ite okc.1 r-6.c-1 n_t9 okc.1
and n_t9 n_t9 !r-6.c-1
ite okc.1 r-7.c-1 n_t9 okc.1
and n_t9 n_t9 !r-7.c-1
delete n_t9
and okc.1 okc.1 preconstrain
# Column 2
and n_t10
or okc.2
ite okc.2 r-0.c-2 n_t10 okc.2
and n_t10 n_t10 !r-0.c-2
ite okc.2 r-1.c-2 n_t10 okc.2
and n_t10 n_t10 !r-1.c-2
ite okc.2 r-2.c-2 n_t10 okc.2
and n_t10 n_t10 !r-2.c-2
ite okc.2 r-3.c-2 n_t10 okc.2
and n_t10 n_t10 !r-3.c-2
ite okc.2 r-4.c-2 n_t10 okc.2
and n_t10 n_t10 !r-4.c-2
ite okc.2 r-5.c-2 n_t10 okc.2
and n_t10 n_t10 !r-5.c-2
ite okc.2 r-6.c-2 n_t10 okc.2
and n_t10 n_t10 !r-6.c-2
ite okc.2 r-7.c-2 n_t10 okc.2
and n_t10 n_t10 !r-7.c-2
delete n_t10
and okc.2 okc.2 preconstrain
# Column 3
and n_t11
or okc.3
ite okc.3 r-0.c-3 n_t11 okc.3
and n_t11 n_t11 !r-0.c-3
ite okc.3 r-1.c-3 n_t11 okc.3
and n_t11 n_t11 !r-1.c-3
ite okc.3 r-2.c-3 n_t11 okc.3
and n_t11 n_t11 !r-2.c-3
ite okc.3 r-3.c-3 n_t11 okc.3
and n_t11 n_t11 !r-3.c-3
ite okc.3 r-4.c-3 n_t11 okc.3
and n_t11 n_t11 !r-4.c-3
ite okc.3 r-5.c-3 n_t11 okc.3
and n_t11 n_t11 !r-5.c-3
ite okc.3 r-6.c-3 n_t11 okc.3
and n_t11 n_t11 !r-6.c-3
ite okc.3 r-7.c-3 n_t11 okc.3
and n_t11 n_t11 !r-7.c-3
delete n_t11
and okc.3 okc.3 preconstrain
# Column 4
and n_t12
or okc.4
ite okc.4 r-0.c-4 n_t12 okc.4
and n_t12 n_t12 !r-0.c-4
ite okc.4 r-1.c-4 n_t12 okc.4
and n_t12 n_t12 !r-1.c-4
ite okc.4 r-2.c-4 n_t12 okc.4
and n_t12 n_t12 !r-2.c-4
ite okc.4 r-3.c-4 n_t12 okc.4
and n_t12 n_t12 !r-3.c-4
ite okc.4 r-4.c-4 n_t12 okc.4
and n_t12 n_t12 !r-4.c-4
ite okc.4 r-5.c-4 n_t12 okc.4
and n_t12 n_t12 !r-5.c-4
ite okc.4 r-6.c-4 n_t12 okc.4
and n_t12 n_t12 !r-6.c-4
ite okc.4 r-7.c-4 n_t12 okc.4
and n_t12 n_t12 !r-7.c-4
delete n_t12
and okc.4 okc.4 preconstrain
# Column 5
and n_t13
or okc.5
ite okc.5 r-0.c-5 n_t13 okc.5
and n_t13 n_t13 !r-0.c-5
ite okc.5 r-1.c-5 n_t13 okc.5
and n_t13 n_t13 !r-1.c-5
ite okc.5 r-2.c-5 n_t13 okc.5
and n_t13 n_t13 !r-2.c-5
ite okc.5 r-3.c-5 n_t13 okc.5
and n_t13 n_t13 !r-3.c-5
ite okc.5 r-4.c-5 n_t13 okc.5
and n_t13 n_t13 !r-4.c-5
ite okc.5 r-5.c-5 n_t13 okc.5
and n_t13 n_t13 !r-5.c-5
ite okc.5 r-6.c-5 n_t13 okc.5
and n_t13 n_t13 !r-6.c-5
ite okc.5 r-7.c-5 n_t13 okc.5
and n_t13 n_t13 !r-7.c-5
delete n_t13
and okc.5 okc.5 preconstrain
# Column 6
and n_t14
or okc.6
ite okc.6 r-0.c-6 n_t14 okc.6
and n_t14 n_t14 !r-0.c-6
ite okc.6 r-1.c-6 n_t14 okc.6
and n_t14 n_t14 !r-1.c-6
ite okc.6 r-2.c-6 n_t14 okc.6
and n_t14 n_t14 !r-2.c-6
ite okc.6 r-3.c-6 n_t14 okc.6
and n_t14 n_t14 !r-3.c-6
ite okc.6 r-4.c-6 n_t14 okc.6
and n_t14 n_t14 !r-4.c-6
ite okc.6 r-5.c-6 n_t14 okc.6
and n_t14 n_t14 !r-5.c-6
ite okc.6 r-6.c-6 n_t14 okc.6
and n_t14 n_t14 !r-6.c-6
ite okc.6 r-7.c-6 n_t14 okc.6
and n_t14 n_t14 !r-7.c-6
delete n_t14
and okc.6 okc.6 preconstrain
# Column 7
and n_t15
or okc.7
ite okc.7 r-0.c-7 n_t15 okc.7
and n_t15 n_t15 !r-0.c-7
ite okc.7 r-1.c-7 n_t15 okc.7
and n_t15 n_t15 !r-1.c-7
ite okc.7 r-2.c-7 n_t15 okc.7
and n_t15 n_t15 !r-2.c-7
ite okc.7 r-3.c-7 n_t15 okc.7
and n_t15 n_t15 !r-3.c-7
ite okc.7 r-4.c-7 n_t15 okc.7
and n_t15 n_t15 !r-4.c-7
ite okc.7 r-5.c-7 n_t15 okc.7
and n_t15 n_t15 !r-5.c-7
ite okc.7 r-6.c-7 n_t15 okc.7
and n_t15 n_t15 !r-6.c-7
ite okc.7 r-7.c-7 n_t15 okc.7
and n_t15 n_t15 !r-7.c-7
delete n_t15
and okc.7 okc.7 preconstrain
and okC okc.0 okc.1 okc.2 okc.3 okc.4 okc.5 okc.6 okc.7
delete okc.0 okc.1 okc.2 okc.3 okc.4 okc.5 okc.6 okc.7
# Combine row and column
and okRC okR okC
delete okR
delete okC
# Diagonal Constraints
# Diagonal -7
and n_t16
or n_t17
ite n_t17 r-7.c-0 n_t16 n_t17
and n_t16 n_t16 !r-7.c-0
or okd.0 n_t16 n_t17
delete n_t17
delete n_t16
and okd.0 okd.0 preconstrain
# Diagonal -6
and n_t18
or n_t19
ite n_t19 r-6.c-0 n_t18 n_t19
and n_t18 n_t18 !r-6.c-0
ite n_t19 r-7.c-1 n_t18 n_t19
and n_t18 n_t18 !r-7.c-1
or okd.1 n_t18 n_t19
delete n_t18
delete n_t19
and okd.1 okd.1 preconstrain
# Diagonal -5
and n_t20
or n_t21
ite n_t21 r-5.c-0 n_t20 n_t21
and n_t20 n_t20 !r-5.c-0
ite n_t21 r-6.c-1 n_t20 n_t21
and n_t20 n_t20 !r-6.c-1
ite n_t21 r-7.c-2 n_t20 n_t21
and n_t20 n_t20 !r-7.c-2
or okd.2 n_t20 n_t21
delete n_t21
delete n_t20
and okd.2 okd.2 preconstrain
# Diagonal -4
and n_t22
or n_t23
ite n_t23 r-4.c-0 n_t22 n_t23
and n_t22 n_t22 !r-4.c-0
ite n_t23 r-5.c-1 n_t22 n_t23
and n_t22 n_t22 !r-5.c-1
ite n_t23 r-6.c-2 n_t22 n_t23
and n_t22 n_t22 !r-6.c-2
ite n_t23 r-7.c-3 n_t22 n_t23
and n_t22 n_t22 !r-7.c-3
or okd.3 n_t22 n_t23
delete n_t22
delete n_t23
and okd.3 okd.3 preconstrain
# Diagonal -3
and n_t24
or n_t25
ite n_t25 r-3.c-0 n_t24 n_t25
and n_t24 n_t24 !r-3.c-0
ite n_t25 r-4.c-1 n_t24 n_t25
and n_t24 n_t24 !r-4.c-1
ite n_t25 r-5.c-2 n_t24 n_t25
and n_t24 n_t24 !r-5.c-2
ite n_t25 r-6.c-3 n_t24 n_t25
and n_t24 n_t24 !r-6.c-3
ite n_t25 r-7.c-4 n_t24 n_t25
and n_t24 n_t24 !r-7.c-4
or okd.4 n_t24 n_t25
delete n_t25
delete n_t24
and okd.4 okd.4 preconstrain
# Diagonal -2
and n_t26
or n_t27
ite n_t27 r-2.c-0 n_t26 n_t27
and n_t26 n_t26 !r-2.c-0
ite n_t27 r-3.c-1 n_t26 n_t27
and n_t26 n_t26 !r-3.c-1
ite n_t27 r-4.c-2 n_t26 n_t27
and n_t26 n_t26 !r-4.c-2
ite n_t27 r-5.c-3 n_t26 n_t27
and n_t26 n_t26 !r-5.c-3
ite n_t27 r-6.c-4 n_t26 n_t27
and n_t26 n_t26 !r-6.c-4
ite n_t27 r-7.c-5 n_t26 n_t27
and n_t26 n_t26 !r-7.c-5
or okd.5 n_t26 n_t27
delete n_t26
delete n_t27
and okd.5 okd.5 preconstrain
# Diagonal -1
and n_t28
or n_t29
ite n_t29 r-1.c-0 n_t28 n_t29
and n_t28 n_t28 !r-1.c-0
ite n_t29 r-2.c-1 n_t28 n_t29
and n_t28 n_t28 !r-2.c-1
ite n_t29 r-3.c-2 n_t28 n_t29
and n_t28 n_t28 !r-3.c-2
ite n_t29 r-4.c-3 n_t28 n_t29
and n_t28 n_t28 !r-4.c-3
ite n_t29 r-5.c-4 n_t28 n_t29
and n_t28 n_t28 !r-5.c-4
ite n_t29 r-6.c-5 n_t28 n_t29
and n_t28 n_t28 !r-6.c-5
ite n_t29 r-7.c-6 n_t28 n_t29
and n_t28 n_t28 !r-7.c-6
or okd.6 n_t28 n_t29
delete n_t29
delete n_t28
and okd.6 okd.6 preconstrain
# Diagonal 0
and n_t30
or n_t31
ite n_t31 r-0.c-0 n_t30 n_t31
and n_t30 n_t30 !r-0.c-0
ite n_t31 r-1.c-1 n_t30 n_t31
and n_t30 n_t30 !r-1.c-1
ite n_t31 r-2.c-2 n_t30 n_t31
and n_t30 n_t30 !r-2.c-2
ite n_t31 r-3.c-3 n_t30 n_t31
and n_t30 n_t30 !r-3.c-3
ite n_t31 r-4.c-4 n_t30 n_t31
and n_t30 n_t30 !r-4.c-4
ite n_t31 r-5.c-5 n_t30 n_t31
and n_t30 n_t30 !r-5.c-5
ite n_t31 r-6.c-6 n_t30 n_t31
and n_t30 n_t30 !r-6.c-6
ite n_t31 r-7.c-7 n_t30 n_t31
and n_t30 n_t30 !r-7.c-7
or okd.7 n_t30 n_t31
delete n_t30
delete n_t31
and okd.7 okd.7 preconstrain
# Diagonal 1
and n_t32
or n_t33
ite n_t33 r-0.c-1 n_t32 n_t33
and n_t32 n_t32 !r-0.c-1
ite n_t33 r-1.c-2 n_t32 n_t33
and n_t32 n_t32 !r-1.c-2
ite n_t33 r-2.c-3 n_t32 n_t33
and n_t32 n_t32 !r-2.c-3
ite n_t33 r-3.c-4 n_t32 n_t33
and n_t32 n_t32 !r-3.c-4
ite n_t33 r-4.c-5 n_t32 n_t33
and n_t32 n_t32 !r-4.c-5
ite n_t33 r-5.c-6 n_t32 n_t33
and n_t32 n_t32 !r-5.c-6
ite n_t33 r-6.c-7 n_t32 n_t33
and n_t32 n_t32 !r-6.c-7
or okd.8 n_t32 n_t33
delete n_t33
delete n_t32
and okd.8 okd.8 preconstrain
# Diagonal 2
and n_t34
or n_t35
ite n_t35 r-0.c-2 n_t34 n_t35
and n_t34 n_t34 !r-0.c-2
ite n_t35 r-1.c-3 n_t34 n_t35
and n_t34 n_t34 !r-1.c-3
ite n_t35 r-2.c-4 n_t34 n_t35
and n_t34 n_t34 !r-2.c-4
ite n_t35 r-3.c-5 n_t34 n_t35
and n_t34 n_t34 !r-3.c-5
ite n_t35 r-4.c-6 n_t34 n_t35
and n_t34 n_t34 !r-4.c-6
ite n_t35 r-5.c-7 n_t34 n_t35
and n_t34 n_t34 !r-5.c-7
or okd.9 n_t34 n_t35
delete n_t34
delete n_t35
and okd.9 okd.9 preconstrain
# Diagonal 3
and n_t36
or n_t37
ite n_t37 r-0.c-3 n_t36 n_t37
and n_t36 n_t36 !r-0.c-3
ite n_t37 r-1.c-4 n_t36 n_t37
and n_t36 n_t36 !r-1.c-4
ite n_t37 r-2.c-5 n_t36 n_t37
and n_t36 n_t36 !r-2.c-5
ite n_t37 r-3.c-6 n_t36 n_t37
and n_t36 n_t36 !r-3.c-6
ite n_t37 r-4.c-7 n_t36 n_t37
and n_t36 n_t36 !r-4.c-7
or okd.10 n_t36 n_t37
delete n_t37
delete n_t36
and okd.10 okd.10 preconstrain
# Diagonal 4
and n_t38
or n_t39
ite n_t39 r-0.c-4 n_t38 n_t39
and n_t38 n_t38 !r-0.c-4
ite n_t39 r-1.c-5 n_t38 n_t39
and n_t38 n_t38 !r-1.c-5
ite n_t39 r-2.c-6 n_t38 n_t39
and n_t38 n_t38 !r-2.c-6
ite n_t39 r-3.c-7 n_t38 n_t39
and n_t38 n_t38 !r-3.c-7
or okd.11 n_t38 n_t39
delete n_t38
delete n_t39
and okd.11 okd.11 preconstrain
# Diagonal 5
and n_t40
or n_t41
ite n_t41 r-0.c-5 n_t40 n_t41
and n_t40 n_t40 !r-0.c-5
ite n_t41 r-1.c-6 n_t40 n_t41
and n_t40 n_t40 !r-1.c-6
ite n_t41 r-2.c-7 n_t40 n_t41
and n_t40 n_t40 !r-2.c-7
or okd.12 n_t40 n_t41
delete n_t41
delete n_t40
and okd.12 okd.12 preconstrain
# Diagonal 6
and n_t42
or n_t43
ite n_t43 r-0.c-6 n_t42 n_t43
and n_t42 n_t42 !r-0.c-6
ite n_t43 r-1.c-7 n_t42 n_t43
and n_t42 n_t42 !r-1.c-7
or okd.13 n_t42 n_t43
delete n_t42
delete n_t43
and okd.13 okd.13 preconstrain
# Diagonal 7
and n_t44
or n_t45
ite n_t45 r-0.c-7 n_t44 n_t45
and n_t44 n_t44 !r-0.c-7
or okd.14 n_t44 n_t45
delete n_t45
delete n_t44
and okd.14 okd.14 preconstrain
and okD okd.0 okd.1 okd.2 okd.3 okd.4 okd.5 okd.6 okd.7 okd.8 okd.9 okd.10 okd.11 okd.12 okd.13 okd.14
delete okd.0 okd.1 okd.2 okd.3 okd.4 okd.5 okd.6 okd.7 okd.8 okd.9 okd.10 okd.11 okd.12 okd.13 okd.14
# Add diagonal to row & column
and okRCD okRC okD
delete okD
delete okRC
# Off-diagonal Constraints
# Off-diagonal -7
and n_t46
or n_t47
ite n_t47 r-0.c-0 n_t46 n_t47
and n_t46 n_t46 !r-0.c-0
or oko.0 n_t46 n_t47
delete n_t46
delete n_t47
and oko.0 oko.0 preconstrain
# Off-diagonal -6
and n_t48
or n_t49
ite n_t49 r-1.c-0 n_t48 n_t49
and n_t48 n_t48 !r-1.c-0
ite n_t49 r-0.c-1 n_t48 n_t49
and n_t48 n_t48 !r-0.c-1
or oko.1 n_t48 n_t49
delete n_t49
delete n_t48
and oko.1 oko.1 preconstrain
# Off-diagonal -5
and n_t50
or n_t51
ite n_t51 r-2.c-0 n_t50 n_t51
and n_t50 n_t50 !r-2.c-0
ite n_t51 r-1.c-1 n_t50 n_t51
and n_t50 n_t50 !r-1.c-1
ite n_t51 r-0.c-2 n_t50 n_t51
and n_t50 n_t50 !r-0.c-2
or oko.2 n_t50 n_t51
delete n_t50
delete n_t51
and oko.2 oko.2 preconstrain
# Off-diagonal -4
and n_t52
or n_t53
ite n_t53 r-3.c-0 n_t52 n_t53
and n_t52 n_t52 !r-3.c-0
ite n_t53 r-2.c-1 n_t52 n_t53
and n_t52 n_t52 !r-2.c-1
ite n_t53 r-1.c-2 n_t52 n_t53
and n_t52 n_t52 !r-1.c-2
ite n_t53 r-0.c-3 n_t52 n_t53
and n_t52 n_t52 !r-0.c-3
or oko.3 n_t52 n_t53
delete n_t53
delete n_t52
and oko.3 oko.3 preconstrain
# Off-diagonal -3
and n_t54
or n_t55
ite n_t55 r-4.c-0 n_t54 n_t55
and n_t54 n_t54 !r-4.c-0
ite n_t55 r-3.c-1 n_t54 n_t55
and n_t54 n_t54 !r-3.c-1
ite n_t55 r-2.c-2 n_t54 n_t55
and n_t54 n_t54 !r-2.c-2
ite n_t55 r-1.c-3 n_t54 n_t55
and n_t54 n_t54 !r-1.c-3
ite n_t55 r-0.c-4 n_t54 n_t55
and n_t54 n_t54 !r-0.c-4
or oko.4 n_t54 n_t55
delete n_t54
delete n_t55
and oko.4 oko.4 preconstrain
# Off-diagonal -2
and n_t56
or n_t57
ite n_t57 r-5.c-0 n_t56 n_t57
and n_t56 n_t56 !r-5.c-0
ite n_t57 r-4.c-1 n_t56 n_t57
and n_t56 n_t56 !r-4.c-1
ite n_t57 r-3.c-2 n_t56 n_t57
and n_t56 n_t56 !r-3.c-2
ite n_t57 r-2.c-3 n_t56 n_t57
and n_t56 n_t56 !r-2.c-3
ite n_t57 r-1.c-4 n_t56 n_t57
and n_t56 n_t56 !r-1.c-4
ite n_t57 r-0.c-5 n_t56 n_t57
and n_t56 n_t56 !r-0.c-5
or oko.5 n_t56 n_t57
delete n_t57
delete n_t56
and oko.5 oko.5 preconstrain
# Off-diagonal -1
and n_t58
or n_t59
ite n_t59 r-6.c-0 n_t58 n_t59
and n_t58 n_t58 !r-6.c-0
ite n_t59 r-5.c-1 n_t58 n_t59
and n_t58 n_t58 !r-5.c-1
ite n_t59 r-4.c-2 n_t58 n_t59
and n_t58 n_t58 !r-4.c-2
ite n_t59 r-3.c-3 n_t58 n_t59
and n_t58 n_t58 !r-3.c-3
ite n_t59 r-2.c-4 n_t58 n_t59
and n_t58 n_t58 !r-2.c-4
ite n_t59 r-1.c-5 n_t58 n_t59
and n_t58 n_t58 !r-1.c-5
ite n_t59 r-0.c-6 n_t58 n_t59
and n_t58 n_t58 !r-0.c-6
or oko.6 n_t58 n_t59
delete n_t58
delete n_t59
and oko.6 oko.6 preconstrain
# Off-diagonal 0
and n_t60
or n_t61
ite n_t61 r-7.c-0 n_t60 n_t61
and n_t60 n_t60 !r-7.c-0
ite n_t61 r-6.c-1 n_t60 n_t61
and n_t60 n_t60 !r-6.c-1
ite n_t61 r-5.c-2 n_t60 n_t61
and n_t60 n_t60 !r-5.c-2
ite n_t61 r-4.c-3 n_t60 n_t61
and n_t60 n_t60 !r-4.c-3
ite n_t61 r-3.c-4 n_t60 n_t61
and n_t60 n_t60 !r-3.c-4
ite n_t61 r-2.c-5 n_t60 n_t61
and n_t60 n_t60 !r-2.c-5
ite n_t61 r-1.c-6 n_t60 n_t61
and n_t60 n_t60 !r-1.c-6
ite n_t61 r-0.c-7 n_t60 n_t61
and n_t60 n_t60 !r-0.c-7
or oko.7 n_t60 n_t61
delete n_t61
delete n_t60
and oko.7 oko.7 preconstrain
# Off-diagonal 1
and n_t62
or n_t63
ite n_t63 r-7.c-1 n_t62 n_t63
and n_t62 n_t62 !r-7.c-1
ite n_t63 r-6.c-2 n_t62 n_t63
and n_t62 n_t62 !r-6.c-2
ite n_t63 r-5.c-3 n_t62 n_t63
and n_t62 n_t62 !r-5.c-3
ite n_t63 r-4.c-4 n_t62 n_t63
and n_t62 n_t62 !r-4.c-4
ite n_t63 r-3.c-5 n_t62 n_t63
and n_t62 n_t62 !r-3.c-5
ite n_t63 r-2.c-6 n_t62 n_t63
and n_t62 n_t62 !r-2.c-6
ite n_t63 r-1.c-7 n_t62 n_t63
and n_t62 n_t62 !r-1.c-7
or oko.8 n_t62 n_t63
delete n_t62
delete n_t63
and oko.8 oko.8 preconstrain
# Off-diagonal 2
and n_t64
or n_t65
ite n_t65 r-7.c-2 n_t64 n_t65
and n_t64 n_t64 !r-7.c-2
ite n_t65 r-6.c-3 n_t64 n_t65
and n_t64 n_t64 !r-6.c-3
ite n_t65 r-5.c-4 n_t64 n_t65
and n_t64 n_t64 !r-5.c-4
ite n_t65 r-4.c-5 n_t64 n_t65
and n_t64 n_t64 !r-4.c-5
ite n_t65 r-3.c-6 n_t64 n_t65
and n_t64 n_t64 !r-3.c-6
ite n_t65 r-2.c-7 n_t64 n_t65
and n_t64 n_t64 !r-2.c-7
or oko.9 n_t64 n_t65
delete n_t65
delete n_t64
and oko.9 oko.9 preconstrain
# Off-diagonal 3
and n_t66
or n_t67
ite n_t67 r-7.c-3 n_t66 n_t67
and n_t66 n_t66 !r-7.c-3
ite n_t67 r-6.c-4 n_t66 n_t67
and n_t66 n_t66 !r-6.c-4
ite n_t67 r-5.c-5 n_t66 n_t67
and n_t66 n_t66 !r-5.c-5
ite n_t67 r-4.c-6 n_t66 n_t67
and n_t66 n_t66 !r-4.c-6
ite n_t67 r-3.c-7 n_t66 n_t67
and n_t66 n_t66 !r-3.c-7
or oko.10 n_t66 n_t67
delete n_t66
delete n_t67
and oko.10 oko.10 preconstrain
# Off-diagonal 4
and n_t68
or n_t69
ite n_t69 r-7.c-4 n_t68 n_t69
and n_t68 n_t68 !r-7.c-4
ite n_t69 r-6.c-5 n_t68 n_t69
and n_t68 n_t68 !r-6.c-5
ite n_t69 r-5.c-6 n_t68 n_t69
and n_t68 n_t68 !r-5.c-6
ite n_t69 r-4.c-7 n_t68 n_t69
and n_t68 n_t68 !r-4.c-7
or oko.11 n_t68 n_t69
delete n_t69
delete n_t68
and oko.11 oko.11 preconstrain
# Off-diagonal 5
and n_t70
or n_t71
ite n_t71 r-7.c-5 n_t70 n_t71
and n_t70 n_t70 !r-7.c-5
ite n_t71 r-6.c-6 n_t70 n_t71
and n_t70 n_t70 !r-6.c-6
ite n_t71 r-5.c-7 n_t70 n_t71
and n_t70 n_t70 !r-5.c-7
or oko.12 n_t70 n_t71
delete n_t70
delete n_t71
and oko.12 oko.12 preconstrain
# Off-diagonal 6
and n_t72
or n_t73
ite n_t73 r-7.c-6 n_t72 n_t73
and n_t72 n_t72 !r-7.c-6
ite n_t73 r-6.c-7 n_t72 n_t73
and n_t72 n_t72 !r-6.c-7
or oko.13 n_t72 n_t73
delete n_t73
delete n_t72
and oko.13 oko.13 preconstrain
# Off-diagonal 7
and n_t74
or n_t75
ite n_t75 r-7.c-7 n_t74 n_t75
and n_t74 n_t74 !r-7.c-7
or oko.14 n_t74 n_t75
delete n_t74
delete n_t75
and oko.14 oko.14 preconstrain
and okO oko.0 oko.1 oko.2 oko.3 oko.4 oko.5 oko.6 oko.7 oko.8 oko.9 oko.10 oko.11 oko.12 oko.13 oko.14
delete oko.0 oko.1 oko.2 oko.3 oko.4 oko.5 oko.6 oko.7 oko.8 oko.9 oko.10 oko.11 oko.12 oko.13 oko.14
# Add off diagonal to row, column, and diagonal
and ok okRCD okO
delete okRCD
delete okO
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

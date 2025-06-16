def matrix_inv(A,B,n):
		
	det,eps = 1,1e-13
	perm=[i for i in range(n)]


	# 1. Triangulate matrix
	for i in range(n):
		
		# find max value
		for w in range(i+1,n):
			if ( abs( A[ perm[w] ][i] ) > abs( A[ perm[i] ][i] ) ):
				perm[w],perm[i]=perm[i],perm[w]
				det *= -1
		
		e=A[perm[i]][i]

		if ( abs(e) < eps ):
			return 0

		det *= e

		# gauss
		for w in range(i+1,n):
			factor = A[ perm[w] ][i] / e
			for k in range(i,n):
				A[ perm[w] ][k] -= A[ perm[i] ][k]*factor
				B[ perm[w] ][k] -= B[ perm[i] ][k]*factor
			

		#A to unitary progress
		factor=e
		for k in range(i,n):
			A[ perm[i] ][k] /= factor
			B[ perm[i] ][k] /= factor
    
	pass

	# 2. Reverse gauss
	for i in range(n-1,-1,-1):		
		e=A[perm[i]][i]; #always 1


		#gauss
		for w in range(i-1,-1,-1):
			factor = A[ perm[w] ][i] / e # /1
			for k in range(i,-1,-1):
				A[ perm[w] ][k] -= A[ perm[i] ][k]*factor
				B[ perm[w] ][k] -= B[ perm[i] ][k]*factor

	pass


A=[[1,2,-1],[2,1,0],[-1,1,2]]
B=[[1,0,0],[0,1,0],[0,0,1]]
matrix_inv(A,B,3)
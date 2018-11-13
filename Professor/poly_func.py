"""
Helper functions to construct polynomial functions
used to model the dependence of tuning parameters on 
experimental observables.
"""
import numpy as np

def fullfact(levels):
    """Get Full Factorization based on levels
    @levels: a list that contains the number of factors for each dimension.
    """
    lines = np.prod(levels)
    n_paras = len(levels)
    H = np.zeros((lines, n_paras))
    
    range_repeat = np.prod(levels)
    level_repeat = 1
    for i in range(n_paras):
        range_repeat /= levels[i]
        lvl = []
        for j in range(levels[i]):
            lvl += [j]*level_repeat

        rng = lvl*int(range_repeat)
        level_repeat *= levels[i]
        H[:, i] = rng
    return H


def factorial(n_paras, order):
    """
    return a Matrix in which each row a list of 
    powers, sum of which equals the @order.
    """
    H = fullfact([order+1]*n_paras)
        
    R = []
    for irow in range(H.shape[0]):
        row = H[irow, :]
        if np.sum(row) == order:
            R.append(row)
    return np.array(R)


def all_facts(n_paras, order):
    """
    return a Matrix which each row a list of 
    powers, sum of which equals an order that is
    less than or equals to @order.
    """
    if order < 1:
        return [0]*n_paras
    
    R = [[0]*n_paras]
    for io in range(1, order+1):
        tR = factorial(n_paras, io)
        R = np.append(R, tR, axis=0)
    
    return R


def multiply(dp, pfacts):
    """
    Get polynomial factors for each 
    parameter variations @dp. 
    """
    if len(dp) != pfacts.shape[1]:
        print("ERROR in multiply")
        return
    
    R = []
    for ir in range(pfacts.shape[0]):
        val = 1.
        for ip,power in zip(dp, pfacts[ir]):
            val *= ip**power
        R.append(val)
    return R


def getMatrixP(in_matrix, order):
    """
    @in_matrix: n_scans x n_parameter_variations
    @order: order of polynomial function that models
    the dependence of generator parameters on observables.
    """
    pfacts = all_facts(in_matrix.shape[0], order)
    R = []
    for iscan in range(in_matrix.shape[1]):
        dp = in_matrix[:, iscan]
        R.append(multiply(dp, pfacts))
        
    return np.array(R) 


def chi2_objective(P, data, error):
    """The objective function to be minized"""
    nscans = P.shape[0]
    def objective(X):
        return sum( ((np.matmul(P, X) - data)/error)**2 )/nscans
    
    return objective


def get_N_coeffienct(n_x, order):
    """
    return number of coefficients given the number 
    of parameters to tune and the polynomial orders.
    """
    N = 1
    for io in range(1, order+1):
        multi = 1
        for j in range(io):
            multi *= n_x + j
        multi /= math.factorial(io)
        N+= multi
    return N
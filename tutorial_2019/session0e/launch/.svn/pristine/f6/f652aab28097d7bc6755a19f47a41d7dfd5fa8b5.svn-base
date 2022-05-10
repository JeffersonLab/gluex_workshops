#!/usr/bin/env python
#
# This script is a convenience for printing the count of each
# problem type in a given workflow. Run like this:
#
#   ./problem_summary.py recon_2018-01_ver02_batch04
#
#

import os
import sys
import subprocess


if len(sys.argv) < 2:
	print('\nUsage:\n    problem_summary.py workflow\n\n')
	print('workflow is the swif2 workflow. See a list with swif2 list\n')
	sys.exit(0)

workflow = sys.argv[1]

# Get list of problem jobs
lines = subprocess.Popen(['swif2', 'status', '-workflow', workflow], stdout=subprocess.PIPE).communicate()[0]
#print(lines)
for line in lines.split('\n'):
	toks = line.split()
	if len(toks)<3: continue
	key = toks[0]
	vals = toks[2:]
	if key == 'jobs' : Njobs = vals[0]
	if key == 'dispatched'    : Ndispatched = vals[0]
	if key == 'succeeded'     : Nsucceeded  = vals[0]
	if key == 'problems'      : Nproblems   = vals[0]
	if key == 'problem_types' : problems    = vals[0].split(',')


# Find count of each problem type
Nprobs = {}
lines =  subprocess.Popen(['swif2', 'status', '-workflow', workflow, '-problems'], stdout=subprocess.PIPE).communicate()[0]
for line in lines.split('\n'):
	for problem in problems:
		if problem in line:
			if not problem in Nprobs.keys(): Nprobs[problem] = 0
			Nprobs[problem] += 1

# Print the summary
print('=======================================================')
print('   workflow: ' + workflow)
print('      Njobs: ' + str(Njobs))
print('Ndispatched: ' + str(Ndispatched))
print('  Nproblems: ' + str(Nproblems))
for problem,cnt in Nprobs.items():
	print('         %4d - %s' % (cnt, problem))
print('=======================================================')
print('')
print('Retry command:\n')

cmd = ['swif2', 'retry-jobs', '-workflow', workflow, '-problems'] + problems
print( ' '.join(cmd) )
print('')

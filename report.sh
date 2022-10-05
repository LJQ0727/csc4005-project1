#!/bin/bash
#SBATCH --job-name=parallel_job_jiaqi # Job name
#SBATCH --nodes=4                    # Run all processes on a single node	
#SBATCH --ntasks=20                  # number of processes = 4
#SBATCH --cpus-per-task=4            # Number of CPU cores per process
#SBATCH --mem=600mb                  # Total memory limit
#SBATCH --time=00:09:00              # Time limit hrs:min:sec
#SBATCH --partition=Project            # Partition name: Project or Debug (Debug is default)
echo sequential ------------
make sequential
make test
echo parallel -----------
make num_procs=1 parallel
make test
make num_procs=2 parallel
make test
make num_procs=3 parallel
make test
make num_procs=4 parallel
make test
make num_procs=5 parallel
make test
make num_procs=10 parallel
make test
make num_procs=15 parallel
make test
make num_procs=20 parallel
make test


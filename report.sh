#!/bin/bash
#SBATCH --job-name=parallel_job_jiaqi # Job name
#SBATCH --nodes=2                    # Run all processes on a single node	
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
make num_procs=4 parallel
make test
make num_procs=6 parallel
make test
make num_procs=8 parallel
make test
make num_procs=10 parallel
make test
make num_procs=12 parallel
make test
make num_procs=14 parallel
make test
make num_procs=16 parallel
make test
make num_procs=18 parallel
make test
make num_procs=20 parallel
make test
make num_procs=22 parallel
make test
make num_procs=24 parallel
make test


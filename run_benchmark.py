import statistics
import subprocess
import sys
import json
import argparse
import socket
import math
import os
import shutil
import time


def get_program_name(program, delimiter="_"):
    job_name_arr = program.split(" ")
    program_name = os.path.basename(job_name_arr[0])
    del job_name_arr[0]
    for i in range(len(job_name_arr)):
        if os.path.exists(job_name_arr[i]):
            job_name_arr[i] = os.path.basename(job_name_arr[i])
    args_string = "_".join(job_name_arr)
    if args_string != "":
        program_name = program_name + delimiter + args_string

    return program_name


def run_on_lcc3(program, job_name):
    output_file = "{}.log".format(job_name)
    job_script = """#!/bin/bash
# Execute job in the partition "lva" unless you have special requirements.
#SBATCH --partition=lva
# Name your job to be able to identify it later
#SBATCH --job-name {job_name}
# Redirect output stream to this file
#SBATCH --output={output_file}
# Maximum number of tasks (=processes) to start in total
#SBATCH --ntasks=1
# Maximum number of tasks (=processes) to start per node
#SBATCH --ntasks-per-node=1
# Enforce exclusive node allocation, do not share with other jobs
#SBATCH --exclusive

{program}""".format(job_name=job_name, output_file=output_file, program=program)

    job_file = "{}.sh".format(job_name)
    f = open(job_file, "w")
    f.write(job_script)
    f.close()

    lcc3_benchmark = "sbatch -W {}".format(os.path.abspath(job_file))
    process = subprocess.run(lcc3_benchmark, shell=True,
                             stdout=subprocess.DEVNULL)

    while not os.path.exists(output_file):
        time.sleep(1)

    res_array = open(output_file, "r").readlines()[-2]
    res_list = open(output_file, "r").readlines()[-1]

    os.remove(output_file)
    os.remove(job_file)

    return res_array, res_list


def measure_program(program, job_name, quiet_mode):
    program_parts = program.split(" ")
    executable = program_parts[0]
    arguments = program_parts[1:]
    program_benchmark = [executable] + arguments

    if socket.gethostname() == "login.lcc3.uibk.ac.at":
        return run_on_lcc3(program, job_name)
    else:
        proc = subprocess.run(
            program_benchmark, encoding='utf-8', stdout=subprocess.PIPE)
        
        print('------------')
        print(program)
        print(proc.stdout.strip())
        print('------------')
        
        return proc.stdout


def main():
    # Default values
    output_file = "benchmark_result.json"
    lcc3 = False
    quiet_mode = False
    # Command line args
    parser = argparse.ArgumentParser(
        description="Script for benchmarking programs.")
    parser.add_argument("-o", help="Output file path")
    parser.add_argument("-q", action="store_true", help="Quiet mode")
    parser.add_argument("program", nargs='+',
                        help="Program to benchmark with arguments")
    args = parser.parse_args()

    program = ' '.join(args.program)
    # Check if program is given
    if not program:
        print("Usage: {} [options] program [program_args...]".format(
            sys.argv[0]))
        sys.exit(1)

    print("Benchmarking command: '{}' {}...".format(
        program, " on LCC3" if lcc3 else ""))

    program_name = get_program_name(program)
    program_name_pretty = get_program_name(program, " ")

    if args.o:
        output_file = args.o
    else:
        output_file = "benchmark_results_{}.json".format(program_name)
    if args.q:
        quiet_mode = True

    insert_ratios = [0, 1, 10, 50]
    element_sizes = [1, 2, 3]
    number_of_elements = [10, 1000, 100000, 10000000]

    output_string = "collectionName; elementSize; collectionSize; Read/Writes; Insert/Deletes; TotalOperations/Second\n"

    for n in number_of_elements:
        for element_size in element_sizes:
            for insert_ratio in insert_ratios:
                
                if n>1000 and element_size >= 3:
                    continue

                read_ratio = 100 - insert_ratio
                program_with_args = program + \
                    " {} {} {} {} {}".format(
                        element_size, n, read_ratio, insert_ratio, 4)

                output_file_name = output_file.replace('.json', "_es_{}_n{}_rw{}_id{}".format(
                    element_size, n, read_ratio, insert_ratio))

                result = measure_program(
                    program_with_args, output_file_name, quiet_mode)

                
                string_current_run=""
                
                for line in result.strip().split(os.linesep):
                    
                    data_structure_name, total_operations = line.split(';')
                    string_current_run = string_current_run + "{};{};{};{};{};{}\n".format(data_structure_name.strip(
                    ), element_size, n, read_ratio, insert_ratio, total_operations.strip())
                    

                    
                output_string = output_string + string_current_run

    with open("DataStructureOutput.csv", 'w') as f:
        f.write(output_string)


if __name__ == "__main__":
    main()

import sys
from Simulator import Simulator


def main():
    if len(sys.argv) <= 5:
        scheduler = int(sys.argv[1])
        the_lambda = int(sys.argv[2])
        average_service_time = float(sys.argv[3])
        if len(sys.argv) == 5:
            quantum = float(sys.argv[4])
        else:
            quantum = 0

    end_condition = 10000

    sim = Simulator(scheduler, the_lambda, average_service_time, quantum, end_condition)
    sim.run()
    sim.generateReport()

if __name__ == "__main__":
    main()







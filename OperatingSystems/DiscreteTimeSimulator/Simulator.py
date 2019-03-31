import math
import random

class CPU:
    def __init__(self):
        self.clock = 0
        self.busy = False
        self.process_being_worked_on = Process()

class Process:
    def __init__(self):
        self.arrival_time = 0
        self.service_time = 0
        self.start_time = 0
        self.end_time = 0
        self.response_ratio = 0
        self.remaining_service_time = 0

class Event:
    def __init__(self):
        self.time = 0
        self.type = ""
        self.process = Process()

class Simulator:
    def __init__(self, scheduler, the_lambda, avg_service_time, quantum, end_condition):
        self.cpu = CPU()
        self.scheduler = scheduler
        self.the_lambda = the_lambda
        self.avg_service_time = avg_service_time
        self.quantum = quantum
        self.end_condition = end_condition
        self.number_completed_processes = 0
        self.total_turnaround_time = 0
        self.total_service_times = 0
        self.current_number_in_ready_queue = []

        self.ready_queue = []
        self.event_queue = []

        first_process = self.generateNewProcess()
        self.event_queue.append(self.generateEvent(first_process, "ARR", first_process.arrival_time))

    def FCFS(self):
        while self.number_completed_processes < self.end_condition:
            #sort the event queue so that the next occuring event appears
            self.event_queue.sort(key=lambda x: x.time)

            #remove the next event from the event queue (get the event)

            event = self.event_queue.pop(0)

            #set the clock so that it is at the time of the occuring event
            self.cpu.clock = event.time

            #determine what type of event has just been pulled from the event queue
            #The instance that the event is an arrival
            if event.type is "ARR":
                #if the arrival happens, the cpu is not busy
                if self.cpu.busy is False:
                    #make the cpu busy and and
                    #CHANGE the event from an arrival to a departure
                    self.cpu.busy = True
                    event.type = "DEP"
                    event.process.end_time = self.cpu.clock + event.process.service_time
                    event.time = event.process.end_time
                    self.event_queue.append(event)
                #if the arrival happens, the cpu is not busy, and the ready queue is not empty
                elif self.cpu.busy is True:
                    self.ready_queue.append(event.process)
                else:
                    print("something is wrong")

                #create a new process
                new_process = self.generateNewProcess()
                #attach it to an arrival event
                self.event_queue.append(self.generateEvent(new_process, "ARR", new_process.arrival_time))


            #else the event type is a DEPARTURE event
            elif event.type is "DEP":
                self.total_service_times += event.process.service_time
                self.current_number_in_ready_queue.append(len(self.ready_queue))

                #number of completed process goes up
                self.number_completed_processes += 1
                self.total_turnaround_time += (self.cpu.clock - event.process.arrival_time)

                #if ready queue is empty, cpu goes idle and event is deleted
                if len(self.ready_queue) is 0:
                    self.cpu.busy = False
                #if ready queue is not empty, the next process is pulled and a departure event is created
                #that process will have its start time and end time set here
                else:
                    process_departing = self.ready_queue.pop(0)
                    process_departing.start_time = self.cpu.clock
                    process_departing.end_time = process_departing.start_time + process_departing.service_time
                    self.event_queue.append(self.generateEvent(process_departing, "DEP", process_departing.end_time))
            else:
                print("Invalid Event Type")

    def SRTF(self):
        while self.number_completed_processes < self.end_condition:
            #sort the event queue so that the next occuring event appears
            self.event_queue.sort(key=lambda x: x.time)

            #remove the next event from the event queue (get the event)

            event = self.event_queue.pop(0)

            #set the clock so that it is at the time of the occuring event
            self.cpu.clock = event.time

            #determine what type of event has just been pulled from the event queue
            #The instance that the event is an arrival
            if event.type is "ARR":
                """
                (time process has been waiting + process's remaining service time)/process's remaining service time
                """
                #if the arrival happens, the cpu is not busy
                if self.cpu.busy is False:
                    #make the cpu busy and and change the event from an arrival to a departure (schedule the departure)
                    self.cpu.busy = True
                    event.type = "DEP"
                    event.process.end_time = self.cpu.clock + event.process.remaining_service_time
                    event.time = event.process.end_time
                    #add back to event queue
                    self.event_queue.append(event)
                    self.cpu.process_being_worked_on = event.process

                #if the arrival happens, the cpu is busy
                elif self.cpu.busy is True:
                    #add the process
                    self.ready_queue.append(event.process)
                    #calculate the shortest remaining time and act accordingly
                    self.shortestRemainingTimeCalculation()
                else:
                    print("something is wrong")

                #create a new process and attach it to an arrival event
                new_process = self.generateNewProcess()
                self.event_queue.append(self.generateEvent(new_process, "ARR", new_process.arrival_time))
            #else the event type is a DEPARTURE event
            elif event.type is "DEP":
                self.total_service_times += event.process.service_time
                self.current_number_in_ready_queue.append(len(self.ready_queue))

                #number of completed process goes up
                self.number_completed_processes += 1
                self.total_turnaround_time += (self.cpu.clock - event.process.arrival_time)
                self.cpu.process_being_worked_on = None
                #if ready queue is empty, cpu goes idle and event is deleted
                if len(self.ready_queue) is 0:
                    self.cpu.busy = False
                #if ready queue is not empty, the next SRT process is pulled and a departure event is created
                #that process will have its start time and end time set here
                else:
                    #sort everything in the ready queue by remaining service time
                    self.ready_queue.sort(key=lambda x: x.remaining_service_time)
                    #remove the SRT process from the ready queue, adjust its end time, put it in the CPU
                    process_departing = self.ready_queue.pop(0)
                    process_departing.start_time = self.cpu.clock
                    process_departing.end_time = self.cpu.clock + process_departing.remaining_service_time
                    self.cpu.process_being_worked_on = process_departing
                    #create a departure event for the process and add it to the event queue
                    self.event_queue.append(self.generateEvent(process_departing, "DEP", process_departing.end_time))

            else:
                print("Invalid Event Type")

    def HRRN(self):
        while self.number_completed_processes < self.end_condition:
            #sort the event queue so that the next occuring event appears
            self.event_queue.sort(key=lambda x: x.time)

            #remove the next event from the event queue (get the event)

            event = self.event_queue.pop(0)

            #set the clock so that it is at the time of the occuring event
            self.cpu.clock = event.time

            #determine what type of event has just been pulled from the event queue
            #The instance that the event is an arrival
            if event.type is "ARR":
                """
                (time process has been waiting + process's service time)/process's service time
                """
                #if the arrival happens, the cpu is not busy, and the ready queue is empty
                if self.cpu.busy is False:
                    #make the cpu busy and and change the event from an arrival to a departure (schedule the departure)
                    self.cpu.busy = True
                    event.type = "DEP"
                    event.process.end_time = self.cpu.clock + event.process.service_time
                    event.time = event.process.end_time
                    #add back to event queue
                    self.event_queue.append(event)
                    self.current_number_in_ready_queue.append(len(self.ready_queue))

                #if the arrival happens, the cpu is not busy, and the ready queue is not empty
                elif self.cpu.busy is True:

                    self.ready_queue.append(event.process)
                else:
                    print("something is wrong")

                #create a new process and attach it to an arrival event
                new_process = self.generateNewProcess()
                self.event_queue.append(self.generateEvent(new_process, "ARR", new_process.arrival_time))
            #else the event type is a DEPARTURE event
            elif event.type is "DEP":
                self.total_service_times += event.process.service_time

                self.current_number_in_ready_queue.append(len(self.ready_queue))
                #number of completed process goes up
                self.number_completed_processes += 1
                self.total_turnaround_time += (self.cpu.clock - event.process.arrival_time)
                #if ready queue is empty, cpu goes idle and event is deleted
                if len(self.ready_queue) is 0:
                    self.cpu.busy = False

                #if ready queue is not empty, the next process is pulled and a departure event is created
                #that process will have its start time and end time set here
                else:
                    self.ratioCalculation()
                    self.ready_queue.sort(key=lambda x: x.response_ratio, reverse=True)

                    process_departing = self.ready_queue.pop(0)
                    process_departing.start_time = self.cpu.clock
                    process_departing.end_time = process_departing.start_time + process_departing.service_time
                    self.event_queue.append(self.generateEvent(process_departing, "DEP", process_departing.end_time))

            else:
                print("Invalid Event Type")

    def RR(self):
        while self.number_completed_processes < self.end_condition:
            #sort the event queue so that the next occuring event appears
            self.event_queue.sort(key=lambda x: x.time)

            #remove the next event from the event queue (get the event)

            event = self.event_queue.pop(0)

            #set the clock so that it is at the time of the occuring event
            self.cpu.clock = event.time
            event.process.start_time = self.cpu.clock
            #determine what type of event has just been pulled from the event queue
            #The instance that the event is an arrival
            if event.type is "ARR":
                #If Cpu is idle
                    # create a departure event
                    # add to event queue
                    # set the end to time to clock + remaining service time
                if self.cpu.busy is False:
                    self.cpu.busy = True
                    if event.process.remaining_service_time > self.quantum:
                        event.type = "SWAP"
                        event.time = self.cpu.clock + self.quantum
                        event.process.remaining_service_time -= self.quantum
                    else:
                        event.type = "DEP"
                        event.process.end_time = self.cpu.clock + event.process.remaining_service_time
                        event.time = event.process.end_time
                    self.cpu.process_being_worked_on = event.process
                    self.event_queue.append(event)
                #else if CPU is not idle and ready queue is empty
                else:
                    self.ready_queue.append(event.process)
                        #gets put back into ready queue
                        #process in cpu's new remaining service time calculated

                # create a new process and attach it to an arrival event
                new_process = self.generateNewProcess()
                self.event_queue.append(self.generateEvent(new_process, "ARR", new_process.arrival_time))
            elif event.type is "SWAP":
                if len(self.ready_queue) is 0:
                    if event.process.remaining_service_time > self.quantum:
                        event.process.remaining_service_time -= self.quantum
                        event.time += self.quantum
                        self.event_queue.append(event)
                    else:
                        event.type = "DEP"
                        event.time = event.process.remaining_service_time + self.cpu.clock
                        self.event_queue.append(event)

                else:
                    #pull process from cpu and process from front of ready queue
                    self.ready_queue.append(self.cpu.process_being_worked_on)
                    self.cpu.process_being_worked_on = None
                    process_from_front_of_ready_queue = self.ready_queue.pop(0)
                    #If process that is coming out of ready queue has a remaining service time that is less than the quantum
                    if process_from_front_of_ready_queue.remaining_service_time < self.quantum:
                        # schedule a departure event
                        self.cpu.process_being_worked_on = process_from_front_of_ready_queue
                        self.cpu.process_being_worked_on.start_time = self.cpu.clock
                        self.cpu.process_being_worked_on.end_time = self.cpu.clock + self.cpu.process_being_worked_on.remaining_service_time
                        self.event_queue.append(self.generateEvent(self.cpu.process_being_worked_on, "DEP",
                                                                   self.cpu.process_being_worked_on.end_time))
                #else
                    else:
                        # schedule another swap
                        self.cpu.process_being_worked_on = process_from_front_of_ready_queue
                        self.cpu.process_being_worked_on.remaining_service_time -= self.quantum
                        self.cpu.process_being_worked_on.start_time = self.cpu.clock
                        self.event_queue.append(self.generateEvent(self.cpu.process_being_worked_on, "SWAP", (self.cpu.clock + self.quantum)))
            elif event.type is "DEP":
                self.total_service_times += event.process.service_time
                self.current_number_in_ready_queue.append(len(self.ready_queue))
                self.total_turnaround_time += (self.cpu.clock - event.process.arrival_time)
                self.number_completed_processes += 1
                #If ready queue is empty
                if len(self.ready_queue) is 0:
                    self.cpu.busy = False
                    #this would mean no more processes
                #else if the ready queue is not empty
                else:
                    # take the next process from the ready queue
                    next_process = self.ready_queue.pop(0)
                    # if the remaining service time is less than quantum
                    if next_process.remaining_service_time < self.quantum:
                        # schedule a departure event
                        self.cpu.process_being_worked_on = next_process
                        self.cpu.process_being_worked_on.end_time = self.cpu.clock + event.process.remaining_service_time
                        self.cpu.process_being_worked_on.start_time = self.cpu.clock
                        self.event_queue.append(self.generateEvent(self.cpu.process_being_worked_on, "DEP", self.cpu.process_being_worked_on.end_time))
                    # else
                    else:
                        # schedule another swap
                        self.cpu.process_being_worked_on = next_process
                        self.cpu.process_being_worked_on.remaining_service_time -= self.quantum
                        self.cpu.process_being_worked_on.start_time = self.cpu.clock
                        self.event_queue.append(self.generateEvent(self.cpu.process_being_worked_on, "SWAP", (self.cpu.clock + self.quantum)))
            else:
                print("Invalid Event Type")

    def generateNewProcess(self):
        new_process = Process()
        new_process.arrival_time = self.cpu.clock + (math.log(1 - float(random.uniform(0, 1))) / (-self.the_lambda))
        new_process.service_time = math.log(1 - float(random.uniform(0, 1))) / (-(1/ self.avg_service_time))
        new_process.start_time = 0
        new_process.remaining_service_time = new_process.service_time
        new_process.end_time = new_process.arrival_time + new_process.service_time

        return new_process

    def generateEvent(self, process, type, time):
        new_event = Event()
        new_event.time = time
        new_event.type = type
        new_event.process = process

        return new_event

    def run(self):
        if self.scheduler is 1:
            self.FCFS()
        elif self.scheduler is 2:
            self.SRTF()
        elif self.scheduler is 3:
            self.HRRN()
        elif self.scheduler is 4:
            self.RR()
        else:
            print("Invalid input.")

    def generateReport(self):
        if self.scheduler is 1:
            scheduler_label = "FCFS"
        elif self.scheduler is 2:
            scheduler_label = "SRTF"
        elif self.scheduler is 3:
            scheduler_label = "HRRN"
        elif self.scheduler is 4:
            scheduler_label = "RR"
        else:
            print("Invalid input.")

        avg_turn_around_time = round((self.total_turnaround_time / self.end_condition), 3)
        throughput = round((self.end_condition / self.cpu.clock), 3)
        cpu_utilization = round(self.total_service_times / self.cpu.clock, 3)
        sum_of_average_number_in_ready_queue = 0
        for i in range(len(self.current_number_in_ready_queue)):
            sum_of_average_number_in_ready_queue += self.current_number_in_ready_queue[i]
        avg_num_processes_in_readyQ = round(sum_of_average_number_in_ready_queue/self.end_condition, 3)

        if self.the_lambda is 1:
            with open("results.txt", "a+") as results_file:
                results_file.write("---------\t------\t--------------\t-----------------\t----------\t--------\t--------------\t-------\n")
                results_file.write("Scheduler\tLambda\tAvgServiceTime\tAvgTurnaroundTime\tThroughput\tCPU Util\tAvg#ProcReadyQ\tQuantum\n")
                results_file.write("---------\t------\t--------------\t-----------------\t----------\t--------\t--------------\t-------\n")
                results_file.write('{:9}'.format(scheduler_label) + str("\t"))
                results_file.write('{:>6}'.format(str(self.the_lambda)) + str("\t"))
                results_file.write('{:>14}'.format(str(self.avg_service_time)) + str("\t"))
                results_file.write('{:>17}'.format(str(avg_turn_around_time)) + str("\t"))
                results_file.write('{:>10}'.format(str(throughput)) + str("\t"))
                results_file.write('{:>8}'.format(str(cpu_utilization)) + str("\t"))
                results_file.write('{:>14}'.format(str(avg_num_processes_in_readyQ)) + str("\t"))
                results_file.write('{:>7}'.format(str(self.quantum)) + str("\n"))
                results_file.close()

        else:
            with open("results.txt", "a+") as results_file:
                results_file.write('{:9}'.format(scheduler_label) + str("\t"))
                results_file.write('{:>6}'.format(str(self.the_lambda)) + str("\t"))
                results_file.write('{:>14}'.format(str(self.avg_service_time)) + str("\t"))
                results_file.write('{:>17}'.format(str(avg_turn_around_time)) + str("\t"))
                results_file.write('{:>10}'.format(str(throughput)) + str("\t"))
                results_file.write('{:>8}'.format(str(cpu_utilization)) + str("\t"))
                results_file.write('{:>14}'.format(str(avg_num_processes_in_readyQ)) + str("\t"))
                results_file.write('{:>7}'.format(str(self.quantum)) + str("\n"))
                results_file.close()

    def ratioCalculation(self):
        for i in range(len(self.ready_queue)):
            self.ready_queue[i].response_ratio = (self.cpu.clock - self.ready_queue[i].service_time)/self.ready_queue[i].service_time

    def shortestRemainingTimeCalculation(self):
        #sort everything in the ready queue by its remaining service time
        self.ready_queue.sort(key=lambda x: x.remaining_service_time)

        #get the new remaining time of the process currently in the cpu
        self.cpu.process_being_worked_on.remaining_service_time = self.cpu.process_being_worked_on.end_time - self.cpu.clock

        if self.cpu.process_being_worked_on.remaining_service_time > self.ready_queue[0].remaining_service_time:
            # find the event that holds the same process at the one in the CPU
            for i in range(len(self.event_queue)):
                #do it by the arrival time because they are gaurenteed to match (THIS IS THE ONLY VARIABLE THAT STAYS CONSTANT)
                #remaining service times at this point are different
                if self.event_queue[i].process.arrival_time is self.cpu.process_being_worked_on.arrival_time:
                    # remove that event entirely
                    self.event_queue.pop(i)

            #Take the process from the CPU and put it in the ready queue
            # NOTHING NEEDS TO CHANGE HERE IN REGARDS TO THE PROCESS
            # ITS REMAINING SERVICE TIME HAS ALREADY BEEN ADJUSTED
            # THE END TIME WILL BE READJUSTED WHEN ITS DEPARTURE EVENT IS CREATED
            self.ready_queue.append(self.cpu.process_being_worked_on)

            #create a new departure event for the process that is replacing the one in the CPU
            new_event = Event()
            #set the event to current clock (when the swap is happening) + remaining service time
            new_event.time = self.cpu.clock + self.ready_queue[0].remaining_service_time
            new_event.process = self.ready_queue.pop(0)
            new_event.process.end_time = new_event.time
            new_event.type = "DEP"
            self.cpu.process_being_worked_on = new_event.process
            self.event_queue.append(new_event)

            #At this point the one being worked on is back in the ready queue and its corresponding departure event is deleted
            # the SRT from ready queue has been placed into the CPU and a departure event has been created for it



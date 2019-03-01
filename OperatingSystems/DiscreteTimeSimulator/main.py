from collections import deque
from CPU import State

clock = 0.0
arrival_times = []
service_times = []
departure_times = []
state = State()
end_condition = 10000

ready_queue = deque()
event_queue = deque()

#read in arrival and service times from files
with open("/Users/zacharygolla/DiscreteTimeSimulator/files/lambda_value_1_file.txt", "r") as f:
  for line in f:
    arrival_times.append(int(line.strip()))

with open("/Users/zacharygolla/DiscreteTimeSimulator/files/average_service_time_value_.06_file.txt", "r") as f:
  for line in f:
    service_times.append(float(line.strip()))

""" 
for index in arrival_times:
    departure_times.append(arrival_times[index] + service_times[index])

end_condition = 10000
"""

ready_queue = deque()






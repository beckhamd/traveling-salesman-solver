import math, sys, time

def run_tsp(graph):
	# Make a copy of the graph in order to store the unvisited cities
	unvisited_cities = dict(graph)

	# Start at the first city and remove it from the unvisited cities
	current_city = list(unvisited_cities.keys())[0]
	del unvisited_cities[current_city]

	# Add the first city to the tour
	tour = [current_city]

	# Visit every city in order to get the tour
	while len(unvisited_cities):
		min_distance = float('inf')

		# Check all of the unvisited cities which neighbor the current city
		for neighbor_city in unvisited_cities:
			# Get the distance from the current city to the neighboring city
			neighbor_distance = round(math.hypot(graph[current_city][0] - unvisited_cities[neighbor_city][0],
				graph[current_city][1] - unvisited_cities[neighbor_city][1]))

			# Store information about the neighboring city if it is closer than the last checked neighboring city
			if neighbor_distance < min_distance:
				closest_city = neighbor_city
				min_distance = neighbor_distance

		# Update the current city and remove the closest city from the unvisited cities
		current_city = closest_city
		del unvisited_cities[closest_city]

		# Add the nearest neighboring city to the tour
		tour.append(closest_city)

	return tour

def run_2opt(graph, tour):
	# Make a copy of the tour
	best_tour = tour[:]

	# Set the improvement value
	improvement = True

	# Continuously run 2-opt on the tour until no improvement can be made
	while improvement:
		# Reset the improvement value
		improvement = False

		# Run 2-opt on every possible combination of cities
		for i in range(1, len(best_tour)):
			# Reset the best difference and endpoints
			best_delta = 0
			best_endpoints = [0, 0]

			for j in range(i + 1, len(best_tour) - 1):
				# Calculate the original endpoint distances and swapped endpoint distances
				delta = round((math.hypot(graph[best_tour[i - 1]][0] - graph[best_tour[j]][0], graph[best_tour[i - 1]][1] - graph[best_tour[j]][1])
					+ math.hypot(graph[best_tour[i]][0] - graph[best_tour[j + 1]][0], graph[best_tour[i]][1] - graph[best_tour[j + 1]][1])
					- math.hypot(graph[best_tour[i - 1]][0] - graph[best_tour[i]][0], graph[best_tour[i - 1]][1] - graph[best_tour[i]][1])
					- math.hypot(graph[best_tour[j]][0] - graph[best_tour[j + 1]][0], graph[best_tour[j]][1] - graph[best_tour[j + 1]][1])))

				# Store information about the endpoints if swapping them results in a shorter tour length
				if delta < best_delta:
					best_delta = delta
					best_endpoints = [i, j]

					# Indicate that an improvement has been made
					improvement = True

			# Swap the endpoints if necessary
			if best_delta < 0:
				best_tour[best_endpoints[0]:best_endpoints[1] + 1] = best_tour[best_endpoints[0]:best_endpoints[1] + 1][::-1]

				# Start 2-opt again
				break

	return best_tour

def get_tour_length(graph, tour):
	tour_length = 0

	# Calculate the tour length and round it to the nearest integer
	for i in range(len(tour)):
		tour_length += int(round(math.hypot(graph[tour[i - 1]][0] - graph[tour[i]][0], graph[tour[i - 1]][1] - graph[tour[i]][1])))

	return tour_length

def main(instance_file):
	# Record the start time
	start_time = time.time()

	graph = {}

	# Read the file and store the information in the graph
	with open(instance_file) as f:
		for line in f:
			id, x, y = (int(i) for i in line.split())
			graph[id] = x, y

	# Run the TSP algorithm on the graph to get a tour
	tour = run_tsp(graph)

	# Run the 2-opt algorithm on the tour
	best_tour = run_2opt(graph, tour)

	# Calculate the new tour length
	best_tour_length = get_tour_length(graph, best_tour)

	# Write the results to the file
	with open("{}.tour".format(instance_file), "w") as f:
		f.write("{}\n".format(best_tour_length))
		for i in best_tour:
			f.write("{}\n".format(i))

	# Print the end time
	print "Running time: {}".format(time.time() - start_time)

if __name__ == '__main__':
	main(sys.argv[1])

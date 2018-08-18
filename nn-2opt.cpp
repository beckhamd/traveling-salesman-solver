#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <vector>

std::vector<int> nearestNeighbor(std::map<int, std::pair<int, int>> &cities, std::vector<std::vector<int>> &distanceMatrix)
{
	// Start at the first city
	int currentCity = (*cities.begin()).first;
	std::vector<int> tour;

	// Remove the first city from the unvisited cities and add it to the tour
	cities.erase(currentCity);
	tour.push_back(currentCity);

	// Visit every city in order to get the tour
	while (!cities.empty())
	{
		int closestCity = 0;
		int minimumDistance = std::numeric_limits<int>::max();

		// Check all of the unvisited cities which neighbor the current city
		for (std::map<int, std::pair<int, int>>::iterator i = cities.begin(); i != cities.end(); ++i)
		{
			// Get the distance from the current city to the neighboring city
			int distance = distanceMatrix[i->first][currentCity];

			// Store information about the neighboring city if it is closer than the last checked neighboring city
			if (distance < minimumDistance)
			{
				closestCity = i->first;
				minimumDistance = distance;
			}
		}

		// Remove the closest city from the unvisited cities and add it to the tour
		cities.erase(closestCity);
		tour.push_back(closestCity);

		// Make the current city the closest city
		currentCity = closestCity;
	}

	return tour;
}

void twoOpt(std::vector<int> &tour, std::vector<std::vector<int>> &distanceMatrix)
{
	bool improvement = true;

	// Continuously run 2-opt on the tour until no improvement can be made
	while (improvement)
	{
		improvement = false;

		// Run 2-opt on every possible combination of cities
		for (int i = 0; i < tour.size(); ++i)
		{
			int bestDelta = 0;
			std::pair<int, int> bestEndpoints;

			for (int j = i + 1; j < tour.size(); ++j)
			{
				// Skip adjacent cities
				if (tour.size() - j - i == 1)
				{
					continue;
				}

				// Calculate the difference between the original endpoint distances and swapped endpoint distances
				int delta = distanceMatrix[tour[((i - 1) + tour.size()) % tour.size()]][tour[j]]
					+ distanceMatrix[tour[i]][tour[(j + 1) % tour.size()]]
					- distanceMatrix[tour[((i - 1) + tour.size()) % tour.size()]][tour[i]]
					- distanceMatrix[tour[j]][tour[(j + 1) % tour.size()]];

				// Store information about the endpoints if swapping them results in a shorter tour length
				if (delta < bestDelta)
				{
					bestDelta = delta;
					bestEndpoints = std::make_pair(i, j);
					improvement = true;
				}
			}

			if (bestDelta < 0)
			{
				int m = bestEndpoints.first, n = bestEndpoints.second;

				// Swap the endpoints
				while (n > m)
				{
					int cityId = tour[m];
					tour[m++] = tour[n];
					tour[n--] = cityId;
				}

				// Start 2-opt again
				break;
			}
		}
	}
}

std::vector<std::vector<int>> buildDistanceMatrix(std::map<int, std::pair<int, int>> &cities)
{
	std::vector<std::vector<int>> distanceMatrix(cities.size());

	// Calculate and store the distances between each city
	for (std::map<int, std::pair<int, int>>::iterator i = cities.begin(); i != cities.end(); ++i)
	{
		distanceMatrix[i->first] = std::vector<int>(cities.size());

		for (std::map<int, std::pair<int, int>>::iterator j = cities.begin(); j != cities.end(); ++j)
		{
			distanceMatrix[i->first][j->first] = std::lround(std::hypot(i->second.first - j->second.first, i->second.second - j->second.second));
		}
	}

	return distanceMatrix;
}

int calculateTourLength(std::vector<int> &tour, std::vector<std::vector<int>> &distanceMatrix)
{
	int tourLength = 0;

	// Sum the distances between each city
	for (int i = 0; i < tour.size(); ++i)
	{
		tourLength += distanceMatrix[tour[i]][tour[((i + 1) + tour.size()) % tour.size()]];
	}

	return tourLength;
}

std::map<int, std::pair<int, int>> readFile(std::string name)
{
	std::map<int, std::pair<int, int>> cities;
	std::ifstream file(name);

	if (!file)
	{
		std::cout << "Error opening " << name << " for reading" << std::endl;
		exit(1);
	}
	else
	{
		int cityId = 0, cityX = 0, cityY = 0;

		// Read the numbers from the file and store them
		while (file >> cityId >> cityX >> cityY)
		{
			cities.insert(std::make_pair(cityId, std::make_pair(cityX, cityY)));
		}
	}

	return cities;
}

void writeFile(std::string name, std::vector<int> &tour, std::vector<std::vector<int>> &distanceMatrix)
{
	std::ofstream file(name);

	if (!file)
	{
		std::cout << "Error opening " << name << " for writing" << std::endl;
		exit(1);
	}
	else
	{
		// Write the tour length to the file
		file << calculateTourLength(tour, distanceMatrix) << "\n";

		// Write the cities in the tour to the file
		for (std::vector<int>::iterator i = tour.begin(); i != tour.end(); ++i)
		{
			file << (*i) << "\n";
		}
	}
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Invalid number of arguments" << std::endl;
		exit(1);
	}
	else
	{
		// Record the start time
		std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();

		// Read the file to get the cities
		std::map<int, std::pair<int, int>> cities = readFile(std::string(argv[1]));

		// Calculate the distance matrix for the tour
		std::vector<std::vector<int>> distanceMatrix = buildDistanceMatrix(cities);

		// Run the nearest neighbor algorithm to get a tour
		std::vector<int> tour = nearestNeighbor(cities, distanceMatrix);

		// Run the 2-opt algorithm to optimize the tour
		twoOpt(tour, distanceMatrix);

		// Write the tour information to the file
		writeFile(std::string(argv[1]).append(".tour"), tour, distanceMatrix);

		// Record the end time and calculate the duration in seconds
		std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();
		double duration = std::chrono::duration<double>(endTime - startTime).count();

		// Output the running time
		std::cout << "Running time: " << duration << " seconds" << std::endl;
	}

	return 0;
}

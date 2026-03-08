#include <iostream>
#include <string>
#include <vector>
#include <random>

int main() {
    const std::string target = "methinksitislikeaweasel";
    const std::size_t length = target.size();
    const double mutation_prob = 0.05;
    const int population_size = 100;

    std::random_device rd;
    std::default_random_engine gen(rd());
    std::uniform_int_distribution<int> letter_dist(0, 25);
    std::uniform_real_distribution<double> prob_dist(0.0, 1.0);

    std::string current;
    for (std::size_t i = 0; i < length; ++i) {
        current += static_cast<char>('a' + letter_dist(gen));
    }

    int iteration = 0;
    while (true) {
        int current_errors = 0;
        for (std::size_t i = 0; i < length; ++i) {
            if (current[i] != target[i]) ++current_errors;
        }
        std::cout << "Iteration " << iteration << ": " << current << " (errors: " << current_errors << ")\n";

        std::vector<std::string> population(population_size);
        std::vector<int> errors(population_size);

        int min_error = length + 1;
        int best_index = -1;

        for (int p = 0; p < population_size; ++p) {
            std::string offspring = current;
            for (std::size_t i = 0; i < length; ++i) {
                if (prob_dist(gen) < mutation_prob) {
                    offspring[i] = static_cast<char>('a' + letter_dist(gen));
                }
            }
            population[p] = offspring;

            int err = 0;
            for (std::size_t i = 0; i < length; ++i) {
                if (offspring[i] != target[i]) ++err;
            }
            errors[p] = err;

            if (err < min_error) {
                min_error = err;
                best_index = p;
            }
        }

        if (min_error == 0) {
            std::cout << "Found target at iteration " << iteration << ": " << population[best_index] << "\n";
            break;
        }

        current = population[best_index];
        ++iteration;
    }

    return 0;
}
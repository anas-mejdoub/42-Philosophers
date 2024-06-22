# Philosophers Project

## Description

The "Philosophers" project is a classic synchronization problem that illustrates the challenges of concurrent programming. The project simulates a scenario where philosophers alternately eat, think, and sleep, using threads and mutexes to manage shared resources (forks). The goal is to avoid deadlocks and ensure that no philosopher starves.

## Features

- **Thread-based Simulation**: Each philosopher is represented by a thread.
- **Mutexes for Resource Management**: Mutexes are used to manage access to the forks to prevent race conditions.
- **Time Management**: Philosophers follow a cycle of eating, sleeping, and thinking based on specified time intervals.
- **Death Detection**: The simulation stops if a philosopher dies of starvation.

## Requirements

- **Number of Philosophers**: The number of philosophers and forks.
- **Time to Die**: The maximum time (in milliseconds) a philosopher can go without eating before they die.
- **Time to Eat**: The time (in milliseconds) it takes for a philosopher to eat.
- **Time to Sleep**: The time (in milliseconds) a philosopher spends sleeping.
- **Optional**: The number of times each philosopher must eat before the simulation ends.

## How to Run

1. **Clone the Repository**:
    ```sh
    git clone https://github.com/anas-mejdoub/42-Cursus-Philosophers.git
    cd 42-Cursus-Philosophers
    ```

2. **inside the philo directory compile The Mandatory Part**:
    ```sh
    make
    ```

3. **Run the Simulation**:
    ```sh
    ./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
    ```

    For example:
    ```sh
    ./philo 5 800 200 200
    ```

## Project Structure

- **philo/**: The Mandatory Part Source Files
- **philo_bonus/**: The Bonus Part Source Files
- **Makefile**: Compilation instructions

## Example Output

timestamp_in_ms X has taken a fork
timestamp_in_ms X is eating
timestamp_in_ms X is sleeping
timestamp_in_ms X is thinking
timestamp_in_ms X died

Replace `timestamp_in_ms` with the current timestamp in milliseconds and `X` with the philosopher number.

## Additional Notes

- The project handles synchronization issues to prevent data races and deadlocks.
- Philosophers should avoid dying to ensure the simulation runs smoothly.

## Bonus Part

The bonus part of the project includes the use of processes and semaphores to manage the philosophers and forks.

### How to Run the Bonus Part

1. **inside the philo_bonus directory compile the Bonus Part**:
    ```sh
    make
    ```

2. **Run the Bonus Simulation**:
    ```sh
    ./philo_bonus number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
    ```
   For example:
    ```sh
    ./philo_bonus 5 800 200 200
    ```

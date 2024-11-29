import pandas as pd
import matplotlib.pyplot as plt

# File paths
pc_csv = "performance_data/performance_producers_consumers.csv"
rw_csv = "performance_data/performance_readers_writers.csv"
philo_csv = "performance_data/performance_philosophers.csv"

# Load data
pc_data = pd.read_csv(pc_csv)
rw_data = pd.read_csv(rw_csv)
philo_data = pd.read_csv(philo_csv)

def plot_performance(data, title, output_file, color="blue"):
    """
    Plots the execution time vs. number of threads with mean and std deviation.
    
    Args:
        data (pd.DataFrame): The input data with columns 'Threads', 'Run', and 'Time (s)'.
        title (str): Title of the graph.
        output_file (str): File path to save the plot.
    """
    # Group by the number of threads and calculate mean and std deviation
    grouped = data.groupby("Threads")["Time (s)"]
    means = grouped.mean()
    stds = grouped.std()

    # Plot the results
    plt.figure(figsize=(10, 6))
    plt.errorbar(means.index, means, yerr=stds, fmt='-o', color=color, capsize=7, label="Execution time with standard deviation")
    
    # Graph settings
    plt.title(title)
    plt.xlabel("Number of Threads")
    plt.ylabel("Execution Time [s]")
    plt.ylim(0)  # y-axis starts at 0
    plt.xticks(means.index)  # Ensure x-axis matches thread counts
    plt.grid(True, linestyle='--', alpha=0.5)
    plt.legend()
    
    # Show the plot
    plt.savefig('plots/' + output_file)
    plt.show()

# Plot for Producers/Consumers
plot_performance(pc_data, "Performance Producers/Consumers depending on the numbers of Threads", "producers_consumers_performance.png", color="slateblue")

# Plot for Readers/Writers
plot_performance(rw_data, "Performance Readers/Writers depending on the numbers of Threads", "readers_writers_performance.png", color="mediumseagreen")

# Plot for Philosophers
plot_performance(philo_data, "Performance Philosophers depending on the numbers of Threads", "philosophers_performance.png", color="red")
